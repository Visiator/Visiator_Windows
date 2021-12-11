#include "stdafx.h"

#include "INDICATOR.h"
#include "tools.h"
#include "SERVICE.h"
#include "APPLICATION_ATTRIBUTES.h"
#include "TOTAL_CONTROL.h"
#include "CLIPBOARD.h"
//#include "mem.h"

extern APPLICATION_ATTRIBUTES app_attributes;
extern INDICATOR *indicator;
extern bool GLOBAL_STOP;
//extern ALL_THREAD_LIST thread_list;
extern TOTAL_CONTROL *total_control;
extern CLIPBOARD clipboard;

bool CHECK_DESKTOP();

//wchar_t VisiatorIndicatorMutex[] = { L"Global\\vim" };

NOTIFY_ICON::NOTIFY_ICON() {

}
NOTIFY_ICON::~NOTIFY_ICON() {

}

DWORD last_set_desktop_ok = 0;

void NOTIFY_ICON::set(HWND hw, int icon_idx, unsigned long long service_id) {
	if (icon_load == false) {
		icons[0]  = LoadIcon(app_attributes.hInstance, MAKEINTRESOURCE(103)); // DestroyIcon(hIcon)
		icons[1]  = LoadIcon(app_attributes.hInstance, MAKEINTRESOURCE(104)); // DestroyIcon(hIcon)
		icons[2]  = LoadIcon(app_attributes.hInstance, MAKEINTRESOURCE(105)); // DestroyIcon(hIcon)
		icons[3]  = LoadIcon(app_attributes.hInstance, MAKEINTRESOURCE(106)); // stop DestroyIcon(hIcon)
		icon_load = true;
	}
	if (icon_create == false) {
		//send_udp2("icon_create == false");
		if (CHECK_DESKTOP() == false) {
			// TODO 2019 12
			// CHECK_DESKTOP_fail++;
		}
		else {
			// CHECK_DESKTOP_fail = 0;
			last_set_desktop_ok = GetTickCount();
		}
		add(hw);
	}
	if (icon_create == true) {

		if (icon_idx == last_set_icon_idx && service_id == last_set_service_id) return;

		last_set_icon_idx = icon_idx;
		last_set_service_id = service_id;

		char iids[50], infos[50];
		zero_unsigned_char((unsigned char *)iids, 50);

		generate_ID_to_text(iids, service_id);
		sprintf_s(infos, "NOTIFY_ICON::set() VISIATOR %s", iids);
		sudp(infos);


		wchar_t iid[50], info[50];
		zero_wchar_t(iid, 50);

		if (service_id != 0) {
			generate_ID_to_text(iid, service_id);
			my_strcpy(info, L"VISIATOR ");
			my_strcat(info, iid);
			//my_strcpy(icon.szTip, info);
		}
		else {
			my_strcpy(info, L"VISIATOR");
		}

		sudp(infos);

		//void generate_ID_to_text(wchar_t *txt_, unsigned long long local_id_);

		icon.hIcon = icons[icon_idx];
		icon.uFlags = NIF_TIP | NIF_ICON;
		my_strcpy(icon.szTip, info);
		Shell_NotifyIcon(NIM_MODIFY, &icon);
	};
}

void NOTIFY_ICON::del() {
	if (icon_create == true) {
		Shell_NotifyIcon(NIM_DELETE, &icon);
		icon_create = false;
	};
}

void NOTIFY_ICON::add(HWND hw) {
	int s1, s2;
	s1 = NOTIFYICONDATA_V3_SIZE;
	s2 = sizeof(NOTIFYICONDATA);

	zero_unsigned_char((unsigned char *)&icon, sizeof(NOTIFYICONDATA));

	icon.cbSize = NOTIFYICONDATA_V3_SIZE; // XP
	icon.hWnd = hw;
	icon.uVersion = 3; // XP  NOTIFYICON_VERSION;
	icon.uCallbackMessage = WM_USER;
	icon.hIcon = icons[1];
	icon.uFlags = NIF_TIP | NIF_ICON;
	my_strcpy(icon.szTip, L"VISIATOR");

	BOOL r;
	//DWORD err;
	r = Shell_NotifyIcon(NIM_ADD, &icon);
	if (r == TRUE) {
		//send_udp("========== Shell_NotifyIcon() ADD TRUE ");
		icon_create = true;
	}
	else {
	
		//err = GetLastError();
		//if (err == ERROR_FILE_NOT_FOUND) send_udp2("ERROR_FILE_NOT_FOUND");
		//if (err == ERROR_NO_TOKEN) send_udp2("ERROR_NO_TOKEN");
		
		//char ss[300];
		//sprintf_ s(ss, 250, "========== Shell_NotifyIcon() ADD FALSE (%d) ", err);
		//send_udp(ss);

	}

}
//****************************************************************

INDICATOR::INDICATOR()
{
}

INDICATOR::~INDICATOR()
{
}

int ttk = 0;

LRESULT CALLBACK WndProc_INDICATOR(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		// Стандартная обработка сообщений
	case WM_CREATE:	 return indicator->WM_CREATE_(window, message, wParam, lParam);
	case WM_TIMER: 	 indicator->WM_TIMER_(window);		break;

	case WM_DRAWCLIPBOARD: return indicator->WM_DRAWCLIPBOARD_(window, message, wParam, lParam);
	case WM_CHANGECBCHAIN: return indicator->WM_CHANGECBCHAIN_(window, message, wParam, lParam);

		

	case WM_DESTROY: 
		// 2021 my_ChangeClipboardChain( window, indicator->Next_Clipboard_Viewer );
		PostQuitMessage(0); break;

	default:
		return DefWindowProc(window, message, wParam, lParam);
	}
	return 0;
}
void INDICATOR::RUN(HINSTANCE instance) {

	app_attributes.is_indicator = true;

	if (lock_mutex(&mutex, L"Global\\vim") == false) {
		sudp("mutex lock is fail");
		return;
	}
	sudp("mutex lock ok");

	////////////////////////////////////////////////////////////
	int timeout = 15;
	while (CHECK_DESKTOP() == false) {
		timeout--;
		if (timeout == 0) {
			sudp("change DESKTOP is fail. GLOBAL_STOP = true");
			set_GLOBAL_STOP_true(); // GLOBAL_STOP = true;
			return;
		}
	}
	sudp("change DESKTOP is OK");
	last_set_desktop_ok = GetTickCount();


	////////////////////////////////////////////////////////////

	// Регистрация класса окна
	WNDCLASSEX main = { 0 };
	main.cbSize = sizeof(WNDCLASSEX);
	main.hInstance = instance;
	main.lpszClassName = TEXT("VisiatorIndicator");
	main.lpfnWndProc = WndProc_INDICATOR;
	RegisterClassEx(&main);

	// Создание главного окна
	app_attributes.indicator_hwnd = CreateWindowEx(0, TEXT("VisiatorIndicator"), NULL, 0, 0, 0, 0, 0, NULL, NULL, instance, NULL);

	//notify_icon.load_icons_from_resource(app_attributes.indicator_hwnd);

	

	SetTimer(app_attributes.indicator_hwnd, 0, 500, NULL);

	MSG message;
	while (GetMessage(&message, NULL, 0, 0))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	KillTimer(app_attributes.indicator_hwnd, 0);

	// Удаление значка
	//Shell_NotifyIcon(NIM_DELETE, &icon);
	notify_icon.del();

}

#define ICON_stop 3
#define ICON_client_connected 2
#define ICON_ready 1
#define ICON_disable 0


/*
void INDICATOR::set_icon(int icon_idx, unsigned long long service_id ) {

	if (icon_idx == last_set_icon_idx && service_id == last_set_service_id) return;

	last_set_icon_idx = icon_idx;
	last_set_service_id = service_id;

	wchar_t iid[50], info[50];
	zero(iid, 50);
	if (service_id != 0) {
		generate_ID_to_text(iid, service_id);
		my_strcpy(info, L"VISIATOR ");
		my_strcat(info, iid);
		//my_strcpy(icon.szTip, info);
	}
	else {
		my_strcpy(info, L"VISIATOR");
	}

	//void generate_ID_to_text(wchar_t *txt_, unsigned long long local_id_);
	
	icon.hIcon = icons[icon_idx];
	icon.uFlags = NIF_TIP | NIF_ICON;
	my_strcpy(icon.szTip, info);
	Shell_NotifyIcon(NIM_MODIFY, &icon);
}
*/

int srv_detect_fail = 0;

CLIPBOARD clipboard22;
DWORD last_t22 = 0;

void INDICATOR::WM_TIMER_(HWND hw) {


	

	if (last_t22 + 3000 < GetTickCount()) {
		last_t22 = GetTickCount();
		//clipboard22.test(GetForegroundWindow());

	};

	//SERVICE_INFO service_info;
	//bool r;

	total_control->send_udp_INDICATOR();

	

	if(last_read_service_info_INDICATOR == 0) last_read_service_info_INDICATOR = GetTickCount();

	notify_icon.set(hw, need_set_notify_icon, need_set_notify_service_id);

	if (is_need_sync_clipboards == true && is_sync_clipboards == false) {
		is_sync_clipboards = true;
		sudp("is_sync_clipboards = true");

		// 2021 Next_Clipboard_Viewer = my_SetClipboardViewer(hw, &clipboard);

	}
	if (is_need_sync_clipboards == false && is_sync_clipboards == true) {
		is_sync_clipboards = false;
		sudp("is_sync_clipboards = false");
		// 2021 my_ChangeClipboardChain(app_attributes.viewer_window_hwnd, Next_Clipboard_Viewer);
	}

	/*
	zero((char *)&service_info, 128);
	service_info.who_is_asked = IT_IS_indicator;
	
	if (clipboard_is_changed) {
		service_info.INDICATOR_detect_clipboard_is_changed = 1;
	}
	else {
		service_info.INDICATOR_detect_clipboard_is_changed = 0;
	}


	r = read_service_info_nonblocking((unsigned char *)&service_info);

	//HANDLE pipe_handle_;
	//r = read_service_info(&service_info, &pipe_handle_);
	 

	if (r == false) { // сервис не запущен?
		
		srv_detect_fail++;

		if (srv_detect_fail == 4) {
			//set_icon(ICON_stop, service_info.service_id);
		
			notify_icon.set(hw, ICON_stop, 0);
		};
	}
	else {
		if (service_info.is_sync_clipboards == 1 && is_sync_clipboards == false) {
			is_sync_clipboards = true;
			send_udp2("+");
			
			Next_Clipboard_Viewer = my_SetClipboardViewer(hw);

		}
		if (service_info.is_sync_clipboards != 1 && is_sync_clipboards == true) {
			is_sync_clipboards = false;
			send_udp2("-");
			my_ChangeClipboardChain(app_attributes.viewer_window_hwnd, Next_Clipboard_Viewer);
		}

		clipboard_is_changed = false;
		srv_detect_fail = 0;
		last_read_service_info = GetTickCount();
		
		total_control.INDICATOR_WM_TIMER__srv_detect_ok++;

		

		if (service_info.is_client_connected > 0) {
			notify_icon.set(hw, ICON_client_connected, service_info.service_id);
		}
		else {
			if (service_info.is_ready_to_client_connect > 0) {
				notify_icon.set(hw, ICON_ready, service_info.service_id);
			}
			else {
				notify_icon.set(hw, ICON_disable, service_info.service_id);
			};
		}
		
	}
	*/
	

	

	if (last_read_service_info_INDICATOR + 5000 < GetTickCount()) {
		sudp("SERVICE is stop? indicator need stop");
		set_GLOBAL_STOP_true(); // GLOBAL_STOP = true;
		PostQuitMessage(0);
	}

	
}

LRESULT INDICATOR::WM_CREATE_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {

	

	CHECK_DESKTOP();
	

	start_PIPE_SLAVE_thread();

	return TRUE;
};

LRESULT INDICATOR::WM_DRAWCLIPBOARD_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {

	

	sudp("WM_DRAWCLIPBOARD +++");

	if (clipboard.last_set_clipboard > 0 && clipboard.last_set_clipboard + 1500 > GetTickCount()) {
		sudp("it is my set!");

	}
	else {
		clipboard_is_changed = true;
	}

	//send_Clipboard_to_partner();
	if (Next_Clipboard_Viewer != 0 && Next_Clipboard_Viewer != hw) {
		if (IsWindow(Next_Clipboard_Viewer)) {
			PostMessage(Next_Clipboard_Viewer, msg, wp, lp);
		};
	};
	return 0;
}

LRESULT INDICATOR::WM_CHANGECBCHAIN_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {

	sudp("WM_CHANGECBCHAIN ");

	if (Next_Clipboard_Viewer == (HWND)wp)
		Next_Clipboard_Viewer = (HWND)lp;
	else if (IsWindow(Next_Clipboard_Viewer))
		PostMessage(Next_Clipboard_Viewer, msg, wp, lp);

	return 0;
}

void INDICATOR::set_notify_icon(int v, unsigned long long service_id_) {
	need_set_notify_icon = v;
	need_set_notify_service_id = service_id_;
}

void INDICATOR::PIPE_SLAVE_EXECUTE() {
	
	//*************************************************************************************
	/*

	Подключаемся к PIPE который создал SERVICE

	Входим в цикл
	{
		read
		write
		// если где то ошибка, выходим из программы
	};

	*/
	//*************************************************************************************

	DWORD err;
	
	char *r_buf_1 = NULL;
	r_buf_1 = new char[ sizeof_MASTER_AGENT_PACKET_HEADER ];

	char *w_buf_1 = NULL;
	w_buf_1 = new char[ sizeof_MASTER_AGENT_PACKET_HEADER ];

	//char *w_buf_2 = NULL;
	//w_buf_2 = new_char(300, 323);


	MASTER_AGENT_PACKET_HEADER *packet_recv;
	packet_recv = (MASTER_AGENT_PACKET_HEADER *)r_buf_1;

	MASTER_AGENT_PACKET_HEADER *packet_send;
	packet_send = (MASTER_AGENT_PACKET_HEADER *)w_buf_1;

	is_run_indicator_PIPE_SLAVE = true;
	//sudp("INDICATOR - connect... $visiator_indicator$");
	indicator_pipe_handle_SLAVE = CreateFile(L"\\\\.\\pipe\\$visiator_indicator$", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

	if (indicator_pipe_handle_SLAVE == INVALID_HANDLE_VALUE) {

		err = GetLastError();

		sudp("info open pipe_error $visiator_indicator$");

		if (err == ERROR_ACCESS_DENIED) { sudp("ERROR_ACCESS_DENIED"); };
		if (err == ERROR_PIPE_BUSY) { sudp("ERROR_PIPE_BUSY"); };
		if (err == ERROR_FILE_NOT_FOUND) { sudp("ERROR_FILE_NOT_FOUND"); };
		indicator_pipe_handle_SLAVE = 0;

		sudp("INDICATOR::PIPE_SLAVE_EXECUTE() open pipe error ! GLOBAL_STOP = true");

		delete[] r_buf_1; 
		r_buf_1 = nullptr;

		set_GLOBAL_STOP_true(); // GLOBAL_STOP = true;
		is_run_indicator_PIPE_SLAVE = false;
		return;
	}

	//sudp("INDICATOR - connect OK $visiator_indicator$");

	bool x;
	DWORD r, w;


	while (GLOBAL_STOP == false) {
		
		zero_unsigned_char((unsigned char *)r_buf_1, sizeof_MASTER_AGENT_PACKET_HEADER);

		PIPE_SLAVE_Timeout = GetTickCount();

		//sudp("INDICATOR - read() 128 ... $visiator_indicator$");

		r = 0;
		x = read_pipe( indicator_pipe_handle_SLAVE, r_buf_1, sizeof_MASTER_AGENT_PACKET_HEADER, &r, &read_SLAVE_pipe_indicator_TIMEOUT, "i02");
		if (x == false || r != 128) {
			
			sudp("err dsfgsdfh");

			delete[] r_buf_1; 
			r_buf_1 = nullptr;

			is_run_indicator_PIPE_SLAVE = false;

			set_GLOBAL_STOP_true(); // GLOBAL_STOP = true;

			return;
		}
		
		last_read_service_info_INDICATOR = GetTickCount(); // <--- $$$


		bool flag;

		flag = false;

		if (flag == false && packet_recv->packet_size == sizeof_MASTER_AGENT_PACKET_HEADER) {
			if (flag == false && packet_recv->packet_type == packet_type_SERVICE_INFO) {
				flag = true;
				//send_udp2("detect packet_type_SERVICE_INFO");

				SERVICE_INFO *s;
				s = (SERVICE_INFO *)&(packet_recv->reserv[0]);

				if (s->type == 1286011) {
					//send_udp2("+++++++11", s->is_ready_to_client_connect);

					SET_is_sync_clipboards(s->is_sync_clipboards);

					if (s->is_client_connected > 0) {
						set_notify_icon(ICON_client_connected, s->service_id);
					}
					else {
						if (s->is_ready_to_client_connect > 0) {
							set_notify_icon(ICON_ready, s->service_id);
						}
						else {
							set_notify_icon(ICON_disable, s->service_id);
						};
					}




				}

				zero_unsigned_char((unsigned char *)w_buf_1, sizeof_MASTER_AGENT_PACKET_HEADER);

				//zero(w_buf_2, 300);

				packet_send->packet_size = 128;

				//SERVICE_INFO *s;
				s = (SERVICE_INFO *)&(packet_send->reserv[0]);

				if (clipboard_is_changed) {
					clipboard_is_changed = false;
					packet_send->reserv[0] = 1;
					
				}
				else {
					packet_send->reserv[0] = 0;
				}

				PIPE_SLAVE_Timeout = GetTickCount();

				//send_udp2("w1");

				x = write_pipe(indicator_pipe_handle_SLAVE, w_buf_1, sizeof_MASTER_AGENT_PACKET_HEADER, &w, &read_SLAVE_pipe_indicator_TIMEOUT);

				//send_udp2("w2");
			}
		};

		if (flag == false && packet_recv->packet_size == sizeof_MASTER_AGENT_PACKET_HEADER) {
			if (flag == false && packet_recv->packet_type == packet_type_REQUEST_CLIPBOARD) {
				flag = true;
				//send_udp2("detect packet_type_REQUEST_CLIPBOARD");

				try
				{
					// 2021 clipboard._get();
					// 2021 check_clipboard_buf_by_lighthouse(clipboard.buf, clipboard.buf_size);

					//send_udp2("INDICATOR::PIPE_SLAVE_EXECUTE() clipboard.buf_size = ", clipboard.buf_size);
				}
				catch (...) {
					crash_log("catch(...) INDICATOR::PIPE_SLAVE_EXECUTE() ");
				}

			

				zero_unsigned_char((unsigned char *)w_buf_1, sizeof_MASTER_AGENT_PACKET_HEADER);

				//zero(w_buf_2, 300);

				packet_send->packet_size = 128 + clipboard.buf_size;

				PIPE_SLAVE_Timeout = GetTickCount();



				x = write_pipe(indicator_pipe_handle_SLAVE, w_buf_1, sizeof_MASTER_AGENT_PACKET_HEADER, &w, &read_SLAVE_pipe_indicator_TIMEOUT);

				if (clipboard.buf_size > 0) {

					//send_udp2("write_pipe()... clipboard.buf_size =", clipboard.buf_size);

					x = write_pipe( indicator_pipe_handle_SLAVE, clipboard.buf, clipboard.buf_size, &w, &read_SLAVE_pipe_indicator_TIMEOUT);

					//send_udp2("write_pipe()... w =", w);
				};

			};

			if (flag == false && packet_recv->packet_size == sizeof_MASTER_AGENT_PACKET_HEADER) {
				if (flag == false && packet_recv->packet_type == packet_type_SET_CLIPBOARD) {
					flag = true;
					//send_udp2("detect packet_type_SET_CLIPBOARD");

					unsigned char *q;
					unsigned int *ii, b_size;

					ii = (unsigned int *)&packet_recv->reserv[0];

					b_size = *ii;

					//send_udp2("detect packet_type_SET_CLIPBOARD body_size = ", b_size);

					if (b_size > 0) {

						q = new unsigned char[b_size+10];

						//send_udp2("packet_type_SET_CLIPBOARD read2 b_size = ", b_size);

						r = 0;
						x = read_pipe(indicator_pipe_handle_SLAVE, q, b_size, &r, &read_SLAVE_pipe_indicator_TIMEOUT, "i03");
						if (x == false || r != b_size) {
							//send_udp2("read2 clipboard fail");
						}
						else {

							//send_udp2("read2 clipboard ok ", r);
							// 2021 clipboard.paste_from_buffer(q, b_size);
						}

						delete[] q;
						q = nullptr;
					};
				};
			};
		};

		

		
		

		

		PIPE_SLAVE_Timeout = 0;

		//***********************************************************************************************
		// analiz
		

		if (flag == false && packet_recv->packet_size == 128 && packet_recv->packet_type == packet_type_REQUEST_SCREEN_one_byte) {
			/*
			flag = true;
			//send_udp("DETECT packet_type_REQUEST_SCREEN_one_byte");

			last_detect_MASTER = GetTickCount();

			total_control.AGENT_PIPE_SLAVE_EXECUTE_2_screen++;

			PIPE_SLAVE_EXECUTE_2_____FINAL_SCREEN_one_byte();
			*/
		}
		
		
		/*if (flag == false) {
			send_udp("PIPE_SLAVE_EXECUTE() не опознанный запрос  GLOBAL_STOP = true;");
			delete_(&r_buf_1); //delete_ w_buf_2; delete_ w_buf_3; if (w_buf_4 != NULL) delete_ w_buf_4;
			is_run_indicator_PIPE_SLAVE = false;
			set_GLOBAL_STOP_true(); // GLOBAL_STOP = true;
			return;
		}*/



		Sleep(1);
	};

	delete[] r_buf_1;
	r_buf_1 = nullptr;

	is_run_indicator_PIPE_SLAVE = false;
}


void INDICATOR::start_PIPE_SLAVE_thread() {
	//HANDLE h;	DWORD id;
	//h = my_Create_Thread(0, 0, (LPTHREAD_START_ROUTINE)indicator_PIPE_SLAVE_executionThread, 0, 0, &id, "INDICATOR::start_PIPE_SLAVE_thread()");
	SLAVE_EXECUTE_thread = app_attributes.tgroup.create_thread(boost::bind(&INDICATOR::PIPE_SLAVE_EXECUTE, this));
}

void INDICATOR::INDICATOR_PIPE_CONTROL_THREAD_EXECUTE() {

	is_run_CONTROL = true;

	boost::posix_time::milliseconds SleepTime(500);

	while (GLOBAL_STOP == false) {

		if (PIPE_SLAVE_Timeout != 0 &&
			PIPE_SLAVE_Timeout + 5000 < GetTickCount()) {
			// Timeout!

			sudp("PIPE_SLAVE_Timeout! GLOBAL_STOP = true");
			set_GLOBAL_STOP_true(); // GLOBAL_STOP = true;
			

		}

		boost::this_thread::sleep(SleepTime);
	}

	is_run_CONTROL = false;

}
void INDICATOR::start_INDICATOR_PIPE_CONTROL_THREAD() {
	//HANDLE h;	DWORD id;
	//h = my_Create_Thread(0, 0, (LPTHREAD_START_ROUTINE)INDICATOR_PIPE_CONTROL_THREAD_executionThread, 0, 0, &id, "INDICATOR::start_INDICATOR_PIPE_CONTROL_THREAD()");

	EXECUTE_CONTROL_thread = app_attributes.tgroup.create_thread(boost::bind(&INDICATOR::INDICATOR_PIPE_CONTROL_THREAD_EXECUTE, this));

}

void INDICATOR::SET_is_sync_clipboards(unsigned int v) {
	
	if (v == 1 && is_need_sync_clipboards == false) {
		is_need_sync_clipboards = true;
		sudp("is_need_sync_clipboards = true");
	}
	if (v == 0 && is_need_sync_clipboards == true) {
		is_need_sync_clipboards = false;
		sudp("is_need_sync_clipboards = false");
	}
}
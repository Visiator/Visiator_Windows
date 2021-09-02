#include "DESKTOP.h"
#include "APPLICATION_ATTRIBUTES.h"
#include "GUI.h"
#include "GUI_low_level.h"

#define _window_w 250
#define _window_h 344


extern APPLICATION_ATTRIBUTES app_attributes;
extern bool GLOBAL_STOP;
extern DESKTOP *desktop;

LRESULT CALLBACK MainWinProcDesktop(HWND hw, UINT msg, WPARAM wp, LPARAM lp);

void DESKTOP::init_gui() {
	GUI_Element *q;
	q = gui->add_element( GUI_Element_Type_panel , 50, 10, 80, 30, 0 );
	q->color = 0xff00ff;
}

void DESKTOP::calc_start_size(int &x, int &y, int &w, int &h) {
	w = _window_w;
	h = _window_h;

	int display_w, display_h;
	display_w = GetSystemMetrics(SM_CXSCREEN);
	display_h = GetSystemMetrics(SM_CYSCREEN);

	x = display_w / 2 - w / 2;
	y = display_h / 2 - h / 2 - h;


}


void DESKTOP::RUN() {

	app_attributes.is_desktop = true;

	init_gui();


	HICON hi_sm = NULL;
	hi_sm = LoadIcon(app_attributes.hInstance, MAKEINTRESOURCE(107));


	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_BYTEALIGNCLIENT | CS_BYTEALIGNWINDOW;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = MainWinProcDesktop;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = app_attributes.hInstance;
	wc.hIcon = hi_sm; //  LoadIcon(app_attributes.hInstance, MAKEINTRESOURCE(1055));
	wc.hIconSm = LoadIcon(app_attributes.hInstance, MAKEINTRESOURCE(108));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(6);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = L"VISIATOR DESKTOP";

	if (!RegisterClassEx(&wc)) {
		fatal_error("RegisterClass error");
		return;
	};

	int x, y, w, h;

	int scr_w, scr_h;

	get_screen_size(&scr_w, &scr_h);



	x = 400; y = 50; w = 200; h = 600;
	calc_start_size(x, y, w, h);

	load_onetime_screenposition_desktop(&x, &y);

	if (x < 0) x = 0;
	if (y < 0) y = 0;
	if (x + 100 > scr_w) x = scr_w - 100;
	if (y + 100 > scr_h) y = scr_h - 100;

	app_attributes.desktop_window_hwnd = CreateWindow(L"VISIATOR DESKTOP", L"VISIATOR", WS_POPUP, x, y, w, h, NULL, NULL, app_attributes.hInstance, NULL);
	//app_attributes.desktop_window_hwnd = CreateWindowEx( 0, L"VISIATOR VIEWER", L"VISIATOR", WS_OVERLAPPEDWINDOW, x, y, w, h, NULL, NULL, app_attributes.hInstance, NULL);

	if (!app_attributes.desktop_window_hwnd) {
		fatal_error("CreateWindow error");
		return;
	};

	ShowWindow(app_attributes.desktop_window_hwnd, SW_SHOWNORMAL);
	UpdateWindow(app_attributes.desktop_window_hwnd);

	int vv;
	MSG msg;
	DWORD tik;
	tik = GetTickCount() + 100;
	while (GLOBAL_STOP == false) {
		//timer();
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message != 0xf
				&& msg.message != 0x31f
				&& msg.message != 0x2a3
				&& msg.message != 0x113
				&& msg.message != 0x200
				) {
				if (msg.message == WM_SYSKEYDOWN) {
					vv = msg.wParam;
				};
			};
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			Sleep(1);
		}
	};

	KillTimer(app_attributes.desktop_window_hwnd, 1);

	GLOBAL_STOP = true;

	//while (net_client_session != nullptr && net_client_session->EXECUTE_thread_is_run == true) { Sleep(10); }

	while (EXECUTE_net_server_session_pool_thread_is_run == true) { Sleep(10); }
	while (EXECUTE_encrypt_out_pass_thread_is_run == true) { Sleep(10); }
}


DESKTOP::DESKTOP() {
	gui = new GUI();
}


DESKTOP::~DESKTOP() {

}

LRESULT CALLBACK MainWinProcDesktop(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
	//int w_param;

	//int ww;

	//TRACKMOUSEEVENT tme;
	//MINMAXINFO *pInfo;
	//POINT Min, Max;
	RECT rr;
	//RECT WindowRect, ClientRect

	switch (msg) {

	case WM_CREATE:				return desktop->WM_CREATE_(hw, msg, wp, lp);
	case WM_TIMER:				return desktop->WM_TIMER_(hw, msg, wp, lp);
	case WM_GETMINMAXINFO:		return desktop->WM_GETMINMAXINFO_(hw, msg, wp, lp);
	case WM_CAPTURECHANGED:		return desktop->WM_CAPTURECHANGED_(hw, msg, wp, lp);
	case WM_SYSCOMMAND:			return desktop->WM_SYSCOMMAND_(hw, msg, wp, lp);
	case WM_SETFOCUS:			return desktop->WM_SETFOCUS_(hw, msg, wp, lp);
	case WM_KILLFOCUS:			return desktop->WM_KILLFOCUS_(hw, msg, wp, lp);
	case WM_ACTIVATE:			return desktop->WM_ACTIVATE_(hw, msg, wp, lp);
	case WM_ACTIVATEAPP:		return desktop->WM_ACTIVATEAPP_(hw, msg, wp, lp);
	case WM_WINDOWPOSCHANGED:	return desktop->WM_WINDOWPOSCHANGED_(hw, msg, wp, lp);
	case WM_WINDOWPOSCHANGING:	return desktop->WM_WINDOWPOSCHANGING_(hw, msg, wp, lp);
	case WM_NCACTIVATE:			return desktop->WM_NCACTIVATE_(hw, msg, wp, lp);
	case WM_MOUSEACTIVATE:		return desktop->WM_MOUSEACTIVATE_(hw, msg, wp, lp);
	case WM_SETCURSOR:			return desktop->WM_SETCURSOR_(hw, msg, wp, lp);
	case WM_NCMOUSEMOVE:		return desktop->WM_NCMOUSEMOVE_(hw, msg, wp, lp);
	case WM_MOUSEMOVE:			return desktop->WM_MOUSEMOVE_(hw, msg, wp, lp);
	case WM_MOUSELEAVE:			return desktop->WM_MOUSELEAVE_(hw, msg, wp, lp);
	case WM_NCMOUSELEAVE:		return desktop->WM_NCMOUSELEAVE_(hw, msg, wp, lp);
	case WM_NCLBUTTONDOWN:		return desktop->WM_NCLBUTTONDOWN_(hw, msg, wp, lp);
	case WM_LBUTTONDOWN:		return desktop->WM_LBUTTONDOWN_(hw, msg, wp, lp);
	case WM_NCLBUTTONUP:		return desktop->WM_NCLBUTTONUP_(hw, msg, wp, lp);
	case WM_LBUTTONUP:			return desktop->WM_LBUTTONUP_(hw, msg, wp, lp);
	case WM_RBUTTONDOWN:		return desktop->WM_RBUTTONDOWN_(hw, msg, wp, lp);
	case WM_RBUTTONUP:			return desktop->WM_RBUTTONUP_(hw, msg, wp, lp);
	case WM_ERASEBKGND:			return desktop->WM_ERASEBKGND_(hw, msg, wp, lp);
	case WM_PAINT:				return desktop->WM_PAINT_(hw, msg, wp, lp);

	case WM_DRAWCLIPBOARD:				return desktop->WM_DRAWCLIPBOARD_(hw, msg, wp, lp);
	case WM_CHANGECBCHAIN:				return desktop->WM_CHANGECBCHAIN_(hw, msg, wp, lp);

	case WM_DESTROY:

		//my_ChangeClipboardChain(hw, desktop->Next_Clipboard_Viewer);

		GetWindowRect(hw, &rr);
		/*2019 app_attributes.set_last_screenposition(&rr);
		if (app_attributes.hwnd_window_moved == 1) {
			app_attributes.hwnd_window_moved = 0;
			save_onetime_screenposition(app_attributes.hwnd_window_x, app_attributes.hwnd_window_y);
		}*/
		GLOBAL_STOP = true;

		PostQuitMessage(0);
		return TRUE;
	case WM_CLOSE:				PostMessage(hw, WM_DESTROY, 0, 0);		return TRUE;
	case WM_MOVE:				return desktop->WM_MOVE_(hw, msg, wp, lp);
	case WM_VSCROLL:			return desktop->WM_VSCROLL_(hw, msg, wp, lp);
	case WM_HSCROLL:			return desktop->WM_HSCROLL_(hw, msg, wp, lp);
	case WM_SIZING:				return desktop->WM_SIZING_(hw, msg, wp, lp);
	case WM_SIZE:				return desktop->WM_SIZE_(hw, msg, wp, lp);
	case WM_MOUSEWHEEL:			return desktop->WM_MOUSEWHEEL_(hw, msg, wp, lp);
	case WM_CHAR:				return desktop->WM_CHAR_(hw, msg, wp, lp);
	case WM_KEYDOWN:			return desktop->WM_KEYDOWN_(hw, msg, wp, lp);
	case WM_KEYUP:				return desktop->WM_KEYUP_(hw, msg, wp, lp);
	case WM_SYSKEYDOWN:			return desktop->WM_SYSKEYDOWN_(hw, msg, wp, lp);
	case WM_SYSKEYUP:			return desktop->WM_SYSKEYUP_(hw, msg, wp, lp);
	case WM_NCHITTEST:			return desktop->WM_NCHITTEST_(hw, msg, wp, lp);// gui->get_WM_NCHITTEST(&rr, mmx, mmy);


	};

	return DefWindowProc(hw, msg, wp, lp);

};

LRESULT DESKTOP::WM_CREATE_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {

	if (gui == nullptr) {
		gui = new GUI();
	}

	if (low_level == nullptr) {
		low_level = new GUI_low_level(hw);
	}

	SetTimer(hw, 1, 1, NULL);


	/*** 2021
	start_EXECUTE_net_server_session_pool();
	***/
	return TRUE;
	//return DefWindowProc(hw, msg, wp, lp);
};

LRESULT DESKTOP::WM_KEYUP_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {

	return 0;
};
LRESULT DESKTOP::WM_SYSKEYDOWN_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {

	return 0;
};
LRESULT DESKTOP::WM_SYSKEYUP_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {

	return 0;
};
LRESULT DESKTOP::WM_NCHITTEST_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {

	/*** 2021
	int mmx, mmy;
	RECT rr;


	mmx = (int)LOWORD(lp);
	mmy = (int)HIWORD(lp);


	//char ss[500];


	if (GetWindowRect(hw, &rr) != TRUE) return 0;
	mmy -= (int)rr.top;
	mmx -= (int)rr.left;

	GUI_Element *e;

	e = gui->get_active_Element_mouse_over(mmx, mmy);
	if (e != NULL) {

		//sprintf_ s(ss, 450, "client %d %d ", mmx, mmy);
		//send_udp(ss);
		return HTCLIENT;
	};
	***/
	return HTCAPTION;


};



LRESULT DESKTOP::WM_CHAR_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {

	/*** 2021
	if (app_attributes.modal_process != 0) {
		return DefWindowProc(hw, msg, wp, lp);
	}

	char_(msg, wp, lp);
	***/
	return DefWindowProc(hw, msg, wp, lp);
};

LRESULT DESKTOP::WM_KEYDOWN_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {

	/*** 2021
	if (app_attributes.modal_process != 0) {
		return DefWindowProc(hw, msg, wp, lp);
	}

	char_keydown(msg, wp, lp);
	***/
	return 0;
};
LRESULT DESKTOP::WM_MOUSEWHEEL_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {


	return 0;
};
LRESULT DESKTOP::WM_SIZE_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {

	int mx = (int)LOWORD(lp);
	int my = (int)HIWORD(lp);
	low_level->change_size(hw, mx, my);



	return DefWindowProc(hw, msg, wp, lp);
};
LRESULT DESKTOP::WM_MOVE_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
	/*** 2021
	gui->end_edit_all(low_level);
	***/
	return DefWindowProc(hw, msg, wp, lp);
};
LRESULT DESKTOP::WM_VSCROLL_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
	return DefWindowProc(hw, msg, wp, lp);
};
LRESULT DESKTOP::WM_HSCROLL_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
	return DefWindowProc(hw, msg, wp, lp);
};
LRESULT DESKTOP::WM_SIZING_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
	return DefWindowProc(hw, msg, wp, lp);
};
LRESULT DESKTOP::WM_ERASEBKGND_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
	return 1;
};

LRESULT DESKTOP::WM_PAINT_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {

	gui->Paint(low_level);

	low_level->Paint();
	return 0;
	//return DefWindowProc(hw, msg, wp, lp);
};
LRESULT DESKTOP::WM_RBUTTONUP_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {

	return 0;
};
LRESULT DESKTOP::WM_RBUTTONDOWN_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {

	return 0;
};
LRESULT DESKTOP::WM_NCLBUTTONUP_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {

	return DefWindowProc(hw, msg, wp, lp);
};
LRESULT DESKTOP::WM_NCMOUSELEAVE_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {

	return 0;
};
LRESULT DESKTOP::WM_MOUSELEAVE_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {

	return 0;
};
LRESULT DESKTOP::WM_NCLBUTTONDOWN_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {


	return DefWindowProc(hw, msg, wp, lp);
};

LRESULT DESKTOP::WM_LBUTTONUP_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {

	/*** 2021
	if (app_attributes.modal_process != 0) {
		return DefWindowProc(hw, msg, wp, lp);
	}

	int mmx, mmy;


	mmx = (int)LOWORD(lp);
	mmy = (int)HIWORD(lp);

	GUI_Element *e = nullptr;

	int i;
	i = 0;
	while (i < gui->element_max_count && gui->element[i] != nullptr) {

		bool is_v;

		is_v = true;
		if (gui->element[i]->is_visible == false) is_v = false;
		if (gui->element[i]->parent != nullptr && gui->element[i]->parent->is_visible == false) is_v = false;
		if (gui->element[i]->parent != nullptr && gui->element[i]->parent->parent != nullptr && gui->element[i]->parent->parent->is_visible == false) is_v = false;

		if (is_v == true && gui->element[i]->is_passive == false && gui->element[i]->it_is_my(mmx, mmy)) {

			if (gui->element[i]->func__mouse_unpress != nullptr) {
				gui->element[i]->func__mouse_unpress();
			}
			//return gui->element[i];
		}

		i++;
	}
	***/
	return 0;
};

LRESULT DESKTOP::WM_LBUTTONDOWN_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {

	/*** 2021

	if (app_attributes.modal_process != 0) {
		return DefWindowProc(hw, msg, wp, lp);
	}

	int mmx, mmy;


	mmx = (int)LOWORD(lp);
	mmy = (int)HIWORD(lp);

	GUI_Element *e;

	int i;
	i = 0;
	while (i < gui->element_max_count && gui->element[i] != nullptr) {

		bool is_v;

		is_v = true;
		if (gui->element[i]->is_visible == false) is_v = false;
		if (gui->element[i]->parent != nullptr && gui->element[i]->parent->is_visible == false) is_v = false;
		if (gui->element[i]->parent != nullptr && gui->element[i]->parent->parent != nullptr && gui->element[i]->parent->parent->is_visible == false) is_v = false;

		if (is_v == true && gui->element[i]->it_is_my(mmx, mmy)) {
			if (gui->element[i]->type == GUI_ELEMENT_editfield) {

				if (mmx - gui->element[i]->x > 148) {
					if (gui->element[i]->is_pass_view == true) { gui->element[i]->is_pass_view = false; }
					else { gui->element[i]->is_pass_view = true; };
					low_level->invalidate();
				}
			};
		};

		if (is_v == true && gui->element[i]->is_passive == false && gui->element[i]->it_is_my(mmx, mmy)) {

			if (gui->element[i]->type == GUI_ELEMENT_editfield) {

				if (gui->element[i]->is_edit_process == false) {
					gui->end_edit_all(low_level);
					gui->element[i]->begin_edit(low_level);
					return DefWindowProc(hw, msg, wp, lp);
				}
				else {
					// нажали на элемент который уже нажат
					return DefWindowProc(hw, msg, wp, lp);
				}
			}

			if (gui->element[i]->func__mouse_press != nullptr) {
				gui->element[i]->func__mouse_press();
			}
			//return gui->element[i];
		}

		i++;
	}
	gui->end_edit_all(low_level);


	***/

	return DefWindowProc(hw, msg, wp, lp);
};
//////////////////////////////////
LRESULT DESKTOP::WM_SETFOCUS_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
	return DefWindowProc(hw, msg, wp, lp);
};
LRESULT DESKTOP::WM_KILLFOCUS_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
	return DefWindowProc(hw, msg, wp, lp);
};
LRESULT DESKTOP::WM_ACTIVATE_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
	return DefWindowProc(hw, msg, wp, lp);
};
LRESULT DESKTOP::WM_ACTIVATEAPP_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
	return DefWindowProc(hw, msg, wp, lp);
};
LRESULT DESKTOP::WM_WINDOWPOSCHANGED_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
	return DefWindowProc(hw, msg, wp, lp);
};
LRESULT DESKTOP::WM_WINDOWPOSCHANGING_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
	return DefWindowProc(hw, msg, wp, lp);
};
LRESULT DESKTOP::WM_NCACTIVATE_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
	return DefWindowProc(hw, msg, wp, lp);
};
LRESULT DESKTOP::WM_MOUSEACTIVATE_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
	return DefWindowProc(hw, msg, wp, lp);
};
LRESULT DESKTOP::WM_SETCURSOR_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
	return DefWindowProc(hw, msg, wp, lp);
};
LRESULT DESKTOP::WM_NCMOUSEMOVE_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {


	return DefWindowProc(hw, msg, wp, lp);
};
LRESULT DESKTOP::WM_MOUSEMOVE_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {


	return DefWindowProc(hw, msg, wp, lp);
};
LRESULT DESKTOP::WM_GETMINMAXINFO_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
	return DefWindowProc(hw, msg, wp, lp);
};
LRESULT DESKTOP::WM_CAPTURECHANGED_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
	return DefWindowProc(hw, msg, wp, lp);
};
LRESULT DESKTOP::WM_SYSCOMMAND_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {


	return DefWindowProc(hw, msg, wp, lp);
};

LRESULT DESKTOP::WM_TIMER_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {

	/*** 2021

	if (Panel_Top_auto != nullptr && Panel_Top_auto->is_visible) {
		if (last_check_pass_for_autorun + 5000 < GetTickCount()) {
			last_check_pass_for_autorun = GetTickCount();

			check_service_is_set_run_pass();

		}
	};

	if (last_check_service_info + 5000 < GetTickCount()) {

		last_check_service_info = GetTickCount();

		SERVICE_INFO service_info;
		bool r;
		wchar_t iidd[100];

		zero_char((char *)&service_info, 128);
		service_info.who_is_asked = IT_IS_desktop;

		//r = read_service_info_nonblocking((unsigned char *)&service_info);
		HANDLE pp;
		pp = 0;
		r = read_service_info((unsigned char *)&service_info, &pp);

		if (r == true) {
			last_update_service_info = GetTickCount();
		};
		if (r == false) {
			if (last_update_service_info + 6500 < GetTickCount()) {
				last_update_service_info = GetTickCount();
				service_info.service_id = 0;
				service_info.is_client_connected = 0;
				service_info.agent_need_stop = 0;
				service_info.heart_beat = 0;
				service_info.is_ready_to_client_connect = 0;
				r = true;
			};
		};
		if (r == true) {
			zero_wchar_t(iidd, 100);
			if (service_info.service_id != 0) generate_ID_to_text(iidd, service_info.service_id);

			if (my_strcmp(iidd, Editfield_AUTO_ID->text) != 0) {
				Editfield_AUTO_ID->set_text(iidd);
			}
			if (service_info.is_ready_to_client_connect == 0) {
				if (Editfield_AUTO_ID->text_color != 0x999999) {
					Editfield_AUTO_ID->text_color = 0x999999;
				}
			}
			else {
				if (service_info.is_client_connected == 0) {
					Editfield_AUTO_ID->text_color = 0x695f2f;
				}
				else {
					Editfield_AUTO_ID->text_color = 0x699f2f;
				}
			}

		}
	};


	if (app_attributes.modal_process == MODAL_PROCESS_show_message_box1_1) {
		if (message_box->is_visible == false) message_box->is_visible = true;
		message_box->cursor_position += 20; // это празрачность в данной контексте
		if (message_box->cursor_position > 255) message_box->cursor_position = 255;
		if (message_box->cursor_position >= 230) {
			app_attributes.modal_process = MODAL_PROCESS_show_message_box1_2;
			message_box->cursor_position = 4000;
		}
		invalidate();
	}
	if (app_attributes.modal_process == MODAL_PROCESS_show_message_box1_2) {
		if (message_box->is_visible == false) message_box->is_visible = true;
		message_box->cursor_position -= 20; // это празрачность в данной контексте
		if (message_box->cursor_position < 0) message_box->cursor_position = 0;
		if (message_box->cursor_position == 0) {
			app_attributes.modal_process = 0;
			message_box->is_visible = false;
		}
		invalidate();
	}


	***/

	return 0;
};

LRESULT DESKTOP::WM_DRAWCLIPBOARD_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {

	return 0;
}

LRESULT DESKTOP::WM_CHANGECBCHAIN_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {

	return 0;
}

void DESKTOP::char_keydown(int msg, int wp, int lp) {
	/*** 2021
	if (app_attributes.modal_process != 0) {
		return;
	}
	gui->char_keydown(low_level, msg, wp, lp);
	***/
}

void DESKTOP::char_keyup(int msg, int wp, int lp) {
	/*** 2021
	if (app_attributes.modal_process != 0) {
		return;
	}
	gui->char_keyup(low_level, msg, wp, lp);
	***/
}

void DESKTOP::char_(int msg, int wp, int lp) {
	/*** 2021
	if (app_attributes.modal_process != 0) {
		return;
	}
	gui->char_(low_level, msg, wp, lp);
	***/
}

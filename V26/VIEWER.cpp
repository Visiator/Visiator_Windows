#include "stdafx.h"

#include "VIEWER.h"
#include "APPLICATION_ATTRIBUTES.h"
#include "tools.h"
#include "GUI.h"
#include "GUI_low_level.h"
#include "VIEWER_small_top_panel.h"
#include "NET_SERVER_SESSION.h"


extern APPLICATION_ATTRIBUTES app_attributes;
extern bool GLOBAL_STOP;
extern VIEWER  *viewer;

#define MY_MENU_CTRL_ALT_DEL 123345
#define MY_MENU_LNG 123346
#define MY_MENU_STRETCH 123347
#define MY_MENU_NORMAL 123348
#define MY_MENU_FULLSCREEN 123349
#define MY_MENU_GET_CLIPBOARD 123350
#define MY_MENU_SET_CLIPBOARD 123351
#define MY_MENU_SEND_F1 123352
#define MY_MENU_SYNC_CLIPBOARD 123353
#define MY_MENU_TRANSFER_FILE 123354

LRESULT CALLBACK MainWinProcViewer(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
void _callback__arrived_screen(unsigned char *buf, int buf_size, unsigned int _FORMAT_VER);
void _callback__connect();
void _callback__disconnect();

void _callback__arrived_screen(unsigned char *buf, int buf_size, unsigned int _FORMAT_VER) {
	if (viewer != nullptr) viewer->callback__arrived_screen(buf, buf_size, _FORMAT_VER);
}

void _callback__connect() {
	if (viewer != nullptr) viewer->callback__connect();
}
void _callback__disconnect() {
	if (viewer != nullptr) viewer->callback__disconnect();
}


VIEWER::VIEWER() {

}


VIEWER::~VIEWER()
{
}

void VIEWER::init_gui_VIEWER() {
	//if (gui == nullptr) gui = new GUI();

	//VIS = gui->add_element(nullptr, GUI_ELEMENT_panel, 52, 17, 141, 26);

	gui_viewer = gui->add_element(GUI_Element_Type_viewer, 0, 0, 0, 0, 0);
	//gui_viewer->set_font(0);
	gui_viewer->screen_light_from_server_fullsize = screen_light_from_server_fullsize;
	gui_viewer->screen_light_from_server_resized = screen_light_from_server_resized;


	gui_viewer_small_top_panel = gui->add_element(GUI_Element_Type_viewer_small_top_panel, 200, 20, 60, 25, 0xffff00);
	gui_viewer_small_top_panel->is_visible = false;

	gui_viewer_transfer_dialog = gui->add_element(GUI_Element_Type_transfer_dialog, 200, 20, 60, 25, 0);
	
	file_transfer_dialog = new TRANSFER_DIALOG2(this);
	
	//gui_viewer_transfer_dialog->file_transfer_dialog_ptr = file_transfer_dialog;

	// 2021 file_transfer_dialog->set_size(low_level, 100, 50, 900, 500);

	gui_viewer_transfer_dialog->is_visible = false;

}

void VIEWER::calc_start_size(int &x, int &y, int &w, int &h) {
	int ww, hh;
	ww = GetSystemMetrics(SM_CXSCREEN);
	hh = GetSystemMetrics(SM_CYSCREEN);

	w = 300;
	h = 200;
	x = ww / 2 - w / 2;
	y = hh / 2 - h / 2;

}


void VIEWER::RUN_VIEWER(uint8_t *str_partner_id_, uint8_t *pass_encrypted_length32_, uint8_t *pass_no_encrypted_length32_) {
	app_attributes.is_viewer = true;

	load_all_standart_cursor();


	if (str_partner_id_ == NULL || str_partner_id_[0] == 0) {
		return;
	}
	if ((pass_encrypted_length32_ == NULL || pass_encrypted_length32_[0] == 0) && (pass_no_encrypted_length32_ == NULL || pass_no_encrypted_length32_[0] == 0) ) {
		return;
	}
	init_decode_color2();


	if (screen_light_from_server_fullsize == NULL)  screen_light_from_server_fullsize = new SCREEN_LIGHT();
	if (screen_light_from_server_resized  == NULL)  screen_light_from_server_resized = new SCREEN_LIGHT();
	if (screen_light_from_server_resized1 == NULL)  screen_light_from_server_resized1 = new SCREEN_LIGHT();
	if (screen_light_from_server_resized2 == NULL)  screen_light_from_server_resized2 = new SCREEN_LIGHT();

	if (last_use_resized == 2) { last_use_resized = 1; screen_light_from_server_resized = screen_light_from_server_resized1; }
	else { last_use_resized = 2;  screen_light_from_server_resized = screen_light_from_server_resized2; }

	partner_id = generate_ID(str_partner_id_);


	zero_unsigned_char(pass__encripted, 32);
	zero_unsigned_char(pass_no_encripted, 32);
	
	int i;

	i = 0;
	while (i < 32 && pass_encrypted_length32_[i] != 0) {
		pass__encripted[i] = pass_encrypted_length32_[i];
		i++;
	}

	i = 0;
	while (i < 16 && pass_no_encrypted_length32_[i] != 0) {
		pass_no_encripted[i] = pass_no_encrypted_length32_[i];
		i++;
	}

	HICON hi = nullptr;

	//init_gui_VIEWER();

	HICON hi_sm = NULL;
	hi_sm = LoadIcon(app_attributes.hInstance, MAKEINTRESOURCE(101));


	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_BYTEALIGNCLIENT | CS_BYTEALIGNWINDOW;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = MainWinProcViewer;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = app_attributes.hInstance;
	wc.hIcon = hi_sm; //  LoadIcon(app_attributes.hInstance, MAKEINTRESOURCE(1055));
	wc.hIconSm = LoadIcon(app_attributes.hInstance, MAKEINTRESOURCE(101));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(6);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = L"VISIATOR VIEWER";

	if (!RegisterClassEx(&wc)) {
		fatal_error("RegisterClass error");
		return;
	};

	int x, y, w, h;

	x = 400; y = 50; w = 200; h = 600;
	calc_start_size(x, y, w, h);

	app_attributes.viewer_window_hwnd = CreateWindow(L"VISIATOR VIEWER", L"VISIATOR", WS_OVERLAPPEDWINDOW, x, y, w, h, NULL, NULL, app_attributes.hInstance, NULL);
	//app_attributes.desktop_window_hwnd = CreateWindowEx( 0, L"VISIATOR VIEWER", L"VISIATOR", WS_OVERLAPPEDWINDOW, x, y, w, h, NULL, NULL, app_attributes.hInstance, NULL);

	if (!app_attributes.viewer_window_hwnd) {
		fatal_error("CreateWindow error");
		return;
	};
	//window_hwnd = app_attributes.viewer_window_hwnd;

	SetScrollRange(app_attributes.viewer_window_hwnd, SB_VERT, 0, 0, FALSE);
	SetScrollRange(app_attributes.viewer_window_hwnd, SB_HORZ, 0, 0, FALSE);


	h_system_menu = GetSystemMenu(app_attributes.viewer_window_hwnd, FALSE);

	AppendMenu(h_system_menu, MF_SEPARATOR, 0, L"");
	AppendMenu(h_system_menu, MF_STRING, MY_MENU_GET_CLIPBOARD, L"Get clipboard");
	AppendMenu(h_system_menu, MF_STRING, MY_MENU_SET_CLIPBOARD, L"Send clipboard");
	//AppendMenu(h_system_menu, MF_STRING| MF_CHECKED, MY_MENU_SYNC_CLIPBOARD, L"Sync clipboards");

	AppendMenu(h_system_menu, MF_SEPARATOR, 0, L"");
	AppendMenu(h_system_menu, MF_STRING, MY_MENU_FULLSCREEN, L"Full screen \tF1");
	AppendMenu(h_system_menu, MF_STRING, MY_MENU_SEND_F1, L"Send F1-key");
	AppendMenu(h_system_menu, MF_SEPARATOR, 0, L"");
	AppendMenu(h_system_menu, MF_STRING, MY_MENU_CTRL_ALT_DEL, L"Ctrl+Alt+Del");
	AppendMenu(h_system_menu, MF_STRING, MY_MENU_LNG, L"Change Lng");
	AppendMenu(h_system_menu, MF_SEPARATOR, 0, L"");
	AppendMenu(h_system_menu, MF_STRING, MY_MENU_TRANSFER_FILE, L"Transfer files");

	//AppendMenu(GetSystemMenu(app_attributes.desktop_window_hwnd, FALSE), MF_STRING, MY_MENU_STRETCH, L"Stretch");
	//AppendMenu(GetSystemMenu(app_attributes.desktop_window_hwnd, FALSE), MF_STRING, MY_MENU_NORMAL, L"Normal");
	//AppendMenu(GetSystemMenu(app_attributes.desktop_window_hwnd, FALSE), MF_STRING, MY_MENU_FULLSCREEN, L"Full screen");



	//HWND hh;
	//hh = CreateSimpleToolbar(app_attributes.desktop_window_hwnd, app_attributes.hInstance);

	ShowWindow(app_attributes.viewer_window_hwnd, SW_SHOWNORMAL);
	UpdateWindow(app_attributes.viewer_window_hwnd);

	// 2021 09 re_set_clipboard_view(true);


	//SetForegroundWindow(app_attributes.desktop_window_hwnd);

	//********************************************************************
	//**
	//while (app_attributes.LoadDesktopParameters() == false && !GLOBAL_STOP) {
	//	send_udp("LoadServiceParameters() err");
	//	my_Sleep(1000);
	//}



	//2019 EXECUTE_thread_id = 0;
	//2019 my_Create_Thread(NULL, NULL, DESKTOP_thread, NULL, NULL, &EXECUTE_thread_id, "DESKTOP_thread");




	//**
	//********************************************************************
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

	KillTimer(app_attributes.viewer_window_hwnd, 1);

	set_GLOBAL_STOP_true(); // GLOBAL_STOP = true;

	// 2021 09 while (net_client_session != NULL && net_client_session->EXECUTE_thread_is_run == true) { Sleep(10); }

	// 2021 09 while (is_ALL_THREAD_IS_STOP() == false) { Sleep(10); }


}

LRESULT CALLBACK MainWinProcViewer(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
	//int w_param;

	//int ww;

	//TRACKMOUSEEVENT tme;
	//MINMAXINFO *pInfo;
	//POINT Min, Max;
	RECT rr;
	//RECT ClientRect, WindowRect;

	switch (msg) {

	case WM_CREATE:				return viewer->WM_CREATE_(hw, msg, wp, lp);
	case WM_TIMER:				return viewer->WM_TIMER_(hw, msg, wp, lp);
	case WM_GETMINMAXINFO:		return viewer->WM_GETMINMAXINFO_(hw, msg, wp, lp);
	case WM_CAPTURECHANGED:		return viewer->WM_CAPTURECHANGED_(hw, msg, wp, lp);
	case WM_SYSCOMMAND:			return viewer->WM_SYSCOMMAND_(hw, msg, wp, lp);
	case WM_SETFOCUS:			return viewer->WM_SETFOCUS_(hw, msg, wp, lp);
	case WM_KILLFOCUS:			return viewer->WM_KILLFOCUS_(hw, msg, wp, lp);
	case WM_ACTIVATE:			return viewer->WM_ACTIVATE_(hw, msg, wp, lp);
	case WM_ACTIVATEAPP:		return viewer->WM_ACTIVATEAPP_(hw, msg, wp, lp);
	case WM_WINDOWPOSCHANGED:	return viewer->WM_WINDOWPOSCHANGED_(hw, msg, wp, lp);
	case WM_WINDOWPOSCHANGING:	return viewer->WM_WINDOWPOSCHANGING_(hw, msg, wp, lp);
	case WM_NCACTIVATE:			return viewer->WM_NCACTIVATE_(hw, msg, wp, lp);
	case WM_MOUSEACTIVATE:		return viewer->WM_MOUSEACTIVATE_(hw, msg, wp, lp);
	case WM_SETCURSOR:			return viewer->WM_SETCURSOR_(hw, msg, wp, lp);
	case WM_NCMOUSEMOVE:		return viewer->WM_NCMOUSEMOVE_(hw, msg, wp, lp);
	case WM_MOUSEMOVE:			return viewer->WM_MOUSEMOVE_(hw, msg, wp, lp);
	case WM_MOUSELEAVE:			return viewer->WM_MOUSELEAVE_(hw, msg, wp, lp);
	case WM_NCLBUTTONDOWN:		return viewer->WM_NCLBUTTONDOWN_(hw, msg, wp, lp);
	case WM_LBUTTONDOWN:		return viewer->WM_LBUTTONDOWN_(hw, msg, wp, lp);
	case WM_NCLBUTTONUP:		return viewer->WM_NCLBUTTONUP_(hw, msg, wp, lp);
	case WM_LBUTTONUP:			return viewer->WM_LBUTTONUP_(hw, msg, wp, lp);
	case WM_RBUTTONDOWN:		return viewer->WM_RBUTTONDOWN_(hw, msg, wp, lp);
	case WM_RBUTTONUP:			return viewer->WM_RBUTTONUP_(hw, msg, wp, lp);
	case WM_ERASEBKGND:			return viewer->WM_ERASEBKGND_(hw, msg, wp, lp);
	case WM_PAINT:				return viewer->WM_PAINT_(hw, msg, wp, lp);
	case WM_DESTROY:

		// 2021 09	my_ChangeClipboardChain(app_attributes.viewer_window_hwnd, viewer->Next_Clipboard_Viewer);

		GetWindowRect(hw, &rr);

		// 2021 09	if (viewer->is_sync_clipboards == true)
		// 2021 09	{
		// 2021 09		viewer->is_sync_clipboards = false;
		// 2021 09		viewer->re_set_clipboard_view(viewer->is_sync_clipboards);
		// 2021 09	};

		/*2019 app_attributes.set_last_screenposition(&rr);
		if (app_attributes.hwnd_window_moved == 1) {
			app_attributes.hwnd_window_moved = 0;
			save_onetime_screenposition(app_attributes.hwnd_window_x, app_attributes.hwnd_window_y);
		}*/
		set_GLOBAL_STOP_true(); // GLOBAL_STOP = true;

		PostQuitMessage(0);
		return TRUE;

	case WM_CLOSE:				PostMessage(hw, WM_DESTROY, 0, 0);		return TRUE;
	case WM_MOVE:				return viewer->WM_MOVE_(hw, msg, wp, lp);
	case WM_VSCROLL:			return viewer->WM_VSCROLL_(hw, msg, wp, lp);
	case WM_HSCROLL:			return viewer->WM_HSCROLL_(hw, msg, wp, lp);
	case WM_SIZING:				return viewer->WM_SIZING_(hw, msg, wp, lp);
	case WM_SIZE:				return viewer->WM_SIZE_(hw, msg, wp, lp);
	case WM_MOUSEWHEEL:			return viewer->WM_MOUSEWHEEL_(hw, msg, wp, lp);
	case WM_CHAR:				return viewer->WM_CHAR_(hw, msg, wp, lp);
	case WM_KEYDOWN:			return viewer->WM_KEYDOWN_(hw, msg, wp, lp);
	case WM_KEYUP:				return viewer->WM_KEYUP_(hw, msg, wp, lp);
	case WM_SYSKEYDOWN:			return viewer->WM_SYSKEYDOWN_(hw, msg, wp, lp);
	case WM_SYSKEYUP:			return viewer->WM_SYSKEYUP_(hw, msg, wp, lp);
	case WM_NCHITTEST:			return viewer->WM_NCHITTEST_(hw, msg, wp, lp);// gui->get_WM_NCHITTEST(&rr, mmx, mmy);


	case WM_DRAWCLIPBOARD:		return viewer->WM_DRAWCLIPBOARD_(hw, msg, wp, lp);
	case WM_CHANGECBCHAIN:		return viewer->WM_CHANGECBCHAIN_(hw, msg, wp, lp);

	};

	return DefWindowProc(hw, msg, wp, lp);

};



LRESULT VIEWER::WM_DRAWCLIPBOARD_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {

	/* 2021 09
	send_udp("WM_DRAWCLIPBOARD +++");

	if (clipboard.last_set_clipboard > 0 && clipboard.last_set_clipboard + 1500 > GetTickCount()) {
		send_udp("it is my set!");

	}
	else {
		send_Clipboard_to_partner();
	}



	if (IsWindow(Next_Clipboard_Viewer))
		PostMessage(Next_Clipboard_Viewer, msg, wp, lp);
	*/
	return 0;
}

LRESULT VIEWER::WM_CHANGECBCHAIN_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
	/* 2021 09
	send_udp("WM_CHANGECBCHAIN ");

	if (Next_Clipboard_Viewer == (HWND)wp)
		Next_Clipboard_Viewer = (HWND)lp;
	else if (IsWindow(Next_Clipboard_Viewer))
		PostMessage(Next_Clipboard_Viewer, msg, wp, lp);
	*/
	return 0;
}

LRESULT VIEWER::WM_CREATE_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {

	if (gui == nullptr) { gui = new GUI(hw); }

	init_gui_VIEWER();

	window_hwnd = hw;

	DWORD style;
	style = (DWORD)GetWindowLong(hw, GWL_EXSTYLE);
	SetWindowLong(hw, GWL_EXSTYLE, style | WS_EX_COMPOSITED);


	SetTimer(hw, 1, 10, NULL);

	//start_EXECUTE();
	//start_ASYNC_LOAD_EXECUTE();

	if (small_top_panel == nullptr) { small_top_panel = new VIEWER_small_top_panel(this); }

	thread_EXECUTE = app_attributes.tgroup.create_thread(boost::bind(&VIEWER::EXECUTE, this));

	if (net_client_session == nullptr) {
		net_client_session = new NET_CLIENT_SESSION();

		net_client_session->parent_low_level = gui->low_level;

		net_client_session->parent_func__arrived_screen = &_callback__arrived_screen;
		net_client_session->parent_func__connect = &_callback__connect;
		net_client_session->parent_func__disconnect = &_callback__disconnect;
		
		// net_client_session->set_partner_pass_and_id(partner_id, pass_encripted);
		net_client_session->thread_EXECUTE = app_attributes.tgroup.create_thread(boost::bind(&NET_CLIENT_SESSION::EXECUTE, net_client_session));

		//net_client_session->start_EXECUTE(low_level, partner_id, prepare_pass);
	}

	return TRUE;
	//return DefWindowProc(hw, msg, wp, lp);
};

void VIEWER::change_view_mode(int p) {



	step1_size_h = 0; step1_size_w = 0;
	step2_size_h = 0; step21_size_w = 0; step22_size_x = 0; step22_size_w = 0;
	step3_size_h = 0; step3_size_w = 0;
	delta_x = 0; delta_y = 0;



	if (p == VIEW_MODE_STRETCH) {
		//view_mode = VIEW_MODE_STRETCH;

		SetWindowLong(window_hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);//Устанавливаем стили окнного режима
		SetWindowLong(window_hwnd, GWL_EXSTYLE, 0L);
		SetWindowPlacement(window_hwnd, &wpc); // Загружаем парметры предыдущего оконного режима
		ShowWindow(window_hwnd, SW_SHOWDEFAULT); // Показываем обычное окно

		view_mode = VIEW_MODE_STRETCH;

		return;
	}
	if (p == VIEW_MODE_FULLSCREEN) {

		view_mode = VIEW_MODE_FULLSCREEN;

		GetWindowPlacement(window_hwnd, &wpc);//Сохраняем параметры оконного режима
		SetWindowLong(window_hwnd, GWL_STYLE, WS_POPUP);//Устанавливаем новые стили
		SetWindowLong(window_hwnd, GWL_EXSTYLE, WS_EX_TOPMOST);
		ShowWindow(window_hwnd, SW_SHOWMAXIMIZED);//Окно во весь экран

		return;
	}
	if (p == -1) {
		if (view_mode == VIEW_MODE_STRETCH) {
			change_view_mode(VIEW_MODE_FULLSCREEN);
			return;
		}
		if (view_mode == VIEW_MODE_FULLSCREEN) {
			change_view_mode(VIEW_MODE_STRETCH);
			return;
		}
	}

	change_view_mode(VIEW_MODE_STRETCH);

}

void VIEWER::char_keydown(int msg, int wp, int lp) {
	if (net_client_session != NULL) net_client_session->char_keydown(msg, wp, lp);
}

void VIEWER::char_keyup(int msg, int wp, int lp) {
	if (net_client_session != NULL) net_client_session->char_keyup(msg, wp, lp);
}

void VIEWER::send_CtrlAltDel() {
	if (net_client_session != NULL) net_client_session->send_CtrlAltDel();
}
void VIEWER::send_Change_LNG() {
	if (net_client_session != NULL) net_client_session->send_Change_LNG();
}

LRESULT VIEWER::WM_SETFOCUS_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
	return DefWindowProc(hw, msg, wp, lp);
};
LRESULT VIEWER::WM_KILLFOCUS_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
	return DefWindowProc(hw, msg, wp, lp);
};
LRESULT VIEWER::WM_ACTIVATE_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
	return DefWindowProc(hw, msg, wp, lp);
};
LRESULT VIEWER::WM_ACTIVATEAPP_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
	return DefWindowProc(hw, msg, wp, lp);
};
LRESULT VIEWER::WM_WINDOWPOSCHANGED_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
	return DefWindowProc(hw, msg, wp, lp);
};
LRESULT VIEWER::WM_WINDOWPOSCHANGING_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
	return DefWindowProc(hw, msg, wp, lp);
};
LRESULT VIEWER::WM_NCACTIVATE_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
	return DefWindowProc(hw, msg, wp, lp);
};
LRESULT VIEWER::WM_MOUSEACTIVATE_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
	return DefWindowProc(hw, msg, wp, lp);
};

LRESULT VIEWER::WM_MOVE_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {

	int x, y;
	x = (int)(short)LOWORD(lp);   // позиция по горизонтали
	y = (int)(short)HIWORD(lp);   // позиция по вертикали

	GetWindowRect(hw, &last_WM_MOVE);

	return 0;
	//return DefWindowProc(hw, msg, wp, lp);
};
LRESULT VIEWER::WM_VSCROLL_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
	return DefWindowProc(hw, msg, wp, lp);
};
LRESULT VIEWER::WM_HSCROLL_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
	return DefWindowProc(hw, msg, wp, lp);
};
LRESULT VIEWER::WM_SIZING_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
	RECT *NewWindRect, WindowRect, ClientRect;

	int dw, dh, ww;

	if (view_mode == VIEW_MODE_NOCONNECT) {
		NewWindRect = (RECT *)lp;
		NewWindRect->left = last_WM_MOVE.left;
		NewWindRect->top = last_WM_MOVE.top;
		NewWindRect->right = last_WM_MOVE.right;
		NewWindRect->bottom = last_WM_MOVE.bottom;
		return TRUE;
	}

	if (view_mode == VIEW_MODE_STRETCH) {
		NewWindRect = (RECT *)lp;

		GetWindowRect(hw, &WindowRect);
		GetClientRect(hw, &ClientRect);

		dw = (WindowRect.right - WindowRect.left) - (ClientRect.right - ClientRect.left);
		dh = (WindowRect.bottom - WindowRect.top) - (ClientRect.bottom - ClientRect.top);

		SCREEN_LIGHT *fullsizescreen;
		fullsizescreen = screen_light_from_server_fullsize;

		if ((NewWindRect->right - NewWindRect->left - dw > fullsizescreen->w) || (NewWindRect->bottom - NewWindRect->top - dh > fullsizescreen->h))
		{
			switch (wp) {

			case 4: // top + left
				NewWindRect->left = NewWindRect->right - dw - fullsizescreen->w;
				NewWindRect->top = NewWindRect->bottom - dh - fullsizescreen->h;
				break;
			case 1: // left
			case 7: // bottom + left
				NewWindRect->left = NewWindRect->right - dw - fullsizescreen->w;
				NewWindRect->bottom = fullsizescreen->h + NewWindRect->top + dh;
				break;
			case 2: // right
			case 6: // bottom
			case 8: // bottom + right	
				NewWindRect->right = fullsizescreen->w + NewWindRect->left + dw;
				NewWindRect->bottom = fullsizescreen->h + NewWindRect->top + dh;
				break;
			case 3: // top
			case 5: // top+right
				NewWindRect->top = NewWindRect->bottom - dh - fullsizescreen->h;
				NewWindRect->right = fullsizescreen->w + NewWindRect->left + dw;
				break;
			default:
				NewWindRect->right = fullsizescreen->w + NewWindRect->left + dw;
				NewWindRect->bottom = fullsizescreen->h + NewWindRect->top + dh;
				break;

			};
		}
		switch (wp) {
		case 1: // left
			ww = (int)((NewWindRect->right - NewWindRect->left - dw) / fullsizescreen->aspect);
			NewWindRect->bottom = NewWindRect->top + ww + dh;
			break;
		case 2: // right
			ww = (int)((NewWindRect->right - NewWindRect->left - dw) / fullsizescreen->aspect);
			NewWindRect->bottom = NewWindRect->top + ww + dh;
			break;
		case 5: // top+right
			ww = (int)((NewWindRect->right - NewWindRect->left - dw) / fullsizescreen->aspect);
			NewWindRect->top = NewWindRect->bottom - ww - dh;
			break;
		case 4: // top + left
			ww = (int)((NewWindRect->right - NewWindRect->left - dw) / fullsizescreen->aspect);
			NewWindRect->top = NewWindRect->bottom - ww - dh;
			break;
		case 3: // top
		case 6: // bottom
			ww = (int)((NewWindRect->bottom - NewWindRect->top - dh) * fullsizescreen->aspect);
			NewWindRect->right = NewWindRect->left + ww + dw;
			break;
		case 7: // bottom + left
			ww = (int)((NewWindRect->right - NewWindRect->left - dw) / fullsizescreen->aspect);
			NewWindRect->bottom = NewWindRect->top + ww + dh;
			break;
		case 8: // bottom + right
			ww = (int)((NewWindRect->right - NewWindRect->left - dw) / fullsizescreen->aspect);
			NewWindRect->bottom = NewWindRect->top + ww + dh;
			break;
		default:
			break;
		}



	};
	return TRUE;
	//return DefWindowProc(hw, msg, wp, lp);
};

LRESULT VIEWER::WM_TIMER_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {



	//POINT p;

	//GetCursorPos(&p);

	if (last_check_fokus_last + 500 < GetTickCount()) {

		HWND ff;
		ff = GetForegroundWindow();
		if (ff == app_attributes.viewer_window_hwnd) {
			if (last_detect_window_in_focus == false) { // фокус появился
				last_detect_window_in_focus = true;
			}
		}
		else {
			if (last_detect_window_in_focus == true) { // фокус потерян
				last_detect_window_in_focus = false;
				if (net_client_session != NULL) net_client_session->need_send_ALL_KEY_UP_event = true;
			}
		}
	}

	if (view_mode == VIEW_MODE_FULLSCREEN && gui->low_level->window_h > 0) {
		if ((old_m_y >= 0 && old_m_y < 5) ||
			(old_m_y >= gui->low_level->window_h - 5 && old_m_y < gui->low_level->window_h) ||
			(old_m_x >= 0 && old_m_x < 5) ||
			(old_m_x >= gui->low_level->window_w - 5 && old_m_x < gui->low_level->window_w)
			)
		{
			recalc_delta_xy_for_FULLSCREEN(old_m_x, old_m_y);
		}
	}

	if (

		(old_m_y >= 0 && old_m_y < 10) &&
		(old_m_x >= gui->low_level->window_w / 6 && old_m_x <= (gui->low_level->window_w / 6) * 5)

		)
	{
		is_novis_timer = 0;
		is_vis_timer++;

		if (is_vis_timer == 20 && small_top_panel != NULL && small_top_panel->is_visible_ == false) {
			small_top_panel->is_visible_ = true;
		}
	}
	else {
		if (small_top_panel->is_pin_fixed == false) {
			is_vis_timer = 0;
			is_novis_timer++;
			if (is_novis_timer == 110 && small_top_panel != NULL && small_top_panel->is_visible_ == true) {
				small_top_panel->is_visible_ = false;
			}
		};
	}

	return 0;
};

LRESULT VIEWER::WM_SYSKEYDOWN_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
	//AnsiString a;
	//a = "SYSKEYDOWN ";

	//send_udp();
	char_keydown(msg, wp, lp);
	return 0;
};
LRESULT VIEWER::WM_SYSKEYUP_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
	char_keyup(msg, wp, lp);
	return 0;
};
LRESULT VIEWER::WM_NCHITTEST_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
	return DefWindowProc(hw, msg, wp, lp);
};

void VIEWER::recalc_delta_xy_for_FULLSCREEN(int mx, int my) {
	int dx_max, dy_max;

	if (my >= 0 && my < 5 && last_change_delta_xy < GetTickCount()) {

		last_change_delta_xy = GetTickCount() + 20;

		dy_max = gui->low_level->window_h - screen_light_from_server_fullsize->h;

		if (delta_y < 0) {
			if (delta_y + 10 < 0) { delta_y += 10; }
			else { delta_y = 0; };
			gui->low_level->invalidate();
		}
	}

	if (old_m_y >= gui->low_level->window_h - 5 && old_m_y < gui->low_level->window_h && last_change_delta_xy < GetTickCount()) {
		last_change_delta_xy = GetTickCount() + 20;

		dy_max = gui->low_level->window_h - screen_light_from_server_fullsize->h;
		if (delta_y > dy_max) {
			if (delta_y - 10 > dy_max) { delta_y -= 10; }
			else { delta_y = dy_max + 1; };
			gui->low_level->invalidate();
		}
	}

	if (mx >= 0 && mx < 5 && last_change_delta_xy < GetTickCount()) {

		last_change_delta_xy = GetTickCount() + 20;

		dx_max = gui->low_level->window_w - screen_light_from_server_fullsize->w;
		if (delta_x < 0) {
			if (delta_x + 10 < 0) { delta_x += 10; }
			else { delta_x = 0; };
			gui->invalidate();
		}
	}

	if (old_m_x >= gui->low_level->window_w - 5 && old_m_x < gui->low_level->window_w && last_change_delta_xy < GetTickCount()) {
		last_change_delta_xy = GetTickCount() + 20;

		dx_max = gui->low_level->window_w - screen_light_from_server_fullsize->w;


		if (delta_x > dx_max) {
			if (delta_x - 10 > dx_max) { delta_x -= 10; }
			else { delta_x = dx_max + 1; };
			gui->invalidate();
		}

	}
}

LRESULT VIEWER::WM_GETMINMAXINFO_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {

	if (view_mode == VIEW_MODE_NOCONNECT) {

		MINMAXINFO *lpmmi;
		lpmmi = (LPMINMAXINFO)lp;
		int ww, hh;
		get_screen_size( &ww, &hh);

		lpmmi->ptMaxPosition.x = ww / 2 - 300 / 2;
		lpmmi->ptMaxPosition.y = hh / 2 - 200 / 2;

		lpmmi->ptMaxSize.x = 300;
		lpmmi->ptMaxSize.y = 200;
	};
	return DefWindowProc(hw, msg, wp, lp);
}

LRESULT VIEWER::WM_SIZE_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {

	RECT ClientRect;

	GetClientRect(window_hwnd, &ClientRect);

	int mx = (int)LOWORD(lp);
	int my = (int)HIWORD(lp);
	gui->low_level->change_size(hw, mx, my);

	if (ClientRect.right - ClientRect.left > 0) {
		aspect_w = (float)server_screen_w / (float)(ClientRect.right - ClientRect.left);
	}
	else {
		aspect_w = 0;
	}
	if (ClientRect.bottom - ClientRect.top > 0) {
		aspect_h = (float)server_screen_h / (float)(ClientRect.bottom - ClientRect.top);
	}
	else {
		aspect_h = 0;
	}


	int x, y;

	x = (int)LOWORD(lp);
	y = (int)HIWORD(lp);

	small_top_panel->resize(x, y);


	//gui->low_level->set_window_size(x, y); // lock ok
	// 2021 09 if (file_transfer_dialog != nullptr) { file_transfer_dialog->AutoArrangeSizes(low_level); };

	if (wp == SIZE_MAXIMIZED) {
		return 0;
	}

	/*
	if (wp == SIZE_MAXIMIZED && view_mode != VIEW_MODE_FULLSCREEN) {
		change_view_mode(VIEW_MODE_FULLSCREEN);
	}
	*/

	return DefWindowProc(hw, msg, wp, lp);
};

LRESULT VIEWER::WM_SETCURSOR_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {

	//char ss[300];
	DWORD nHittest, wMouseMsg;

	nHittest = LOWORD(lp);  //код местоположения курсора (hit-test code) 
	wMouseMsg = HIWORD(lp);

	//sprintf_ s(ss, 290, "[%d  %d]", nHittest, wMouseMsg);
	//send_udp2(ss);

	if (nHittest == 1) {
		if (file_transfer_dialog_IS_ACTIVE()) {
			// 2021 09 SetCursor((HCURSOR)decode_cursor_id(file_transfer_dialog->need_cursor)); return 0;
		}
		else {
			SetCursor((HCURSOR)decode_cursor_id(need_cursor)); return 0;
		};
	}

	return DefWindowProc(hw, msg, wp, lp);

};

bool VIEWER::file_transfer_dialog_IS_ACTIVE() {
	//if (gui_viewer_transfer_dialog == NULL || file_transfer_dialog == NULL) return false;
	//if (gui_viewer_transfer_dialog->is_visible == true) return true;

	return false;
}

LRESULT VIEWER::WM_SYSCOMMAND_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {

	if ((int)wp == MY_MENU_GET_CLIPBOARD) {
		// 2021 09 viewer->request_Clipboard_from_partner();
	}
	if ((int)wp == MY_MENU_SET_CLIPBOARD) {
		// 2021 09 viewer->send_Clipboard_to_partner();
	}
	if ((int)wp == MY_MENU_SYNC_CLIPBOARD) {

		/* 2021 09 
		if (is_sync_clipboards == true) {

			ModifyMenu(h_system_menu, MY_MENU_GET_CLIPBOARD, MF_STRING, MY_MENU_GET_CLIPBOARD, L"Get clipboard");
			ModifyMenu(h_system_menu, MY_MENU_SET_CLIPBOARD, MF_STRING, MY_MENU_SET_CLIPBOARD, L"Send clipboard");
			ModifyMenu(h_system_menu, MY_MENU_SYNC_CLIPBOARD, 0, MY_MENU_SYNC_CLIPBOARD, L"Sync clipboards");
			is_sync_clipboards = false;
			re_set_clipboard_view(is_sync_clipboards);

		}
		else {

			ModifyMenu(h_system_menu, MY_MENU_GET_CLIPBOARD, MF_STRING | MF_GRAYED, MY_MENU_GET_CLIPBOARD, L"Get clipboard");
			ModifyMenu(h_system_menu, MY_MENU_SET_CLIPBOARD, MF_STRING | MF_GRAYED, MY_MENU_SET_CLIPBOARD, L"Send clipboard");
			ModifyMenu(h_system_menu, MY_MENU_SYNC_CLIPBOARD, MF_STRING | MF_CHECKED, MY_MENU_SYNC_CLIPBOARD, L"Sync clipboards");
			is_sync_clipboards = true;
			re_set_clipboard_view(is_sync_clipboards);
		};
		*/
	}


	if ((int)wp == MY_MENU_CTRL_ALT_DEL) {
		viewer->send_CtrlAltDel();
	}

	if ((int)wp == MY_MENU_LNG) {
		viewer->send_Change_LNG();
	}

	if ((int)wp == MY_MENU_FULLSCREEN) {
		viewer->change_view_mode(2);
	}
	if ((int)wp == MY_MENU_SEND_F1) {
		viewer->char_keydown(0x100, 0x70, 0x3b0001); // press F1
		viewer->char_keyup(0x101, 0x70, 0x3b0001); // unpress F1
	}


	if ((int)wp == MY_MENU_STRETCH) {
		viewer->change_view_mode(VIEW_MODE_STRETCH);
	}
	if ((int)wp == MY_MENU_TRANSFER_FILE) {
		viewer->transfer_dialog_use();
	}


	/*if ((int)wp == MY_MENU_NORMAL) {
		viewer->change_view_mode(0);
	}*/

	return DefWindowProc(hw, msg, wp, lp);
};

LRESULT VIEWER::WM_RBUTTONDOWN_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
	int mx, my;

	mx = (int)LOWORD(lp);
	my = (int)HIWORD(lp);


	if (file_transfer_dialog_IS_ACTIVE()) {
		// 2021 09 if (view_mode == VIEW_MODE_STRETCH)    file_transfer_dialog->mouse_right_button_down(low_level, (int)(mx * aspect_w), (int)(my * aspect_h));
		// 2021 09 if (view_mode == VIEW_MODE_FULLSCREEN) file_transfer_dialog->mouse_right_button_down(low_level, mx - step21_size_w - delta_x, my - step1_size_h - delta_y);
		return 0;
	}

	if (view_mode == VIEW_MODE_STRETCH) {
		if (net_client_session != NULL) {
			net_client_session->mouse_right_button_down((int)(mx * aspect_w), (int)(my * aspect_h));
		}
	}

	if (view_mode == VIEW_MODE_FULLSCREEN) {
		if (net_client_session != NULL) {
			net_client_session->mouse_right_button_down(mx - step21_size_w - delta_x, my - step1_size_h - delta_y);
		}
	}

	return 0;
};
LRESULT VIEWER::WM_RBUTTONUP_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
	int mx, my;

	mx = (int)LOWORD(lp);
	my = (int)HIWORD(lp);

	if (file_transfer_dialog_IS_ACTIVE()) {
		// 2021 09 if (view_mode == VIEW_MODE_STRETCH)    file_transfer_dialog->mouse_right_button_up(low_level, (int)(mx * aspect_w), (int)(my * aspect_h));
		// 2021 09 if (view_mode == VIEW_MODE_FULLSCREEN) file_transfer_dialog->mouse_right_button_up(low_level, mx - step21_size_w - delta_x, my - step1_size_h - delta_y);
		return 0;
	}

	if (view_mode == VIEW_MODE_STRETCH) {
		if (net_client_session != NULL) {
			net_client_session->mouse_right_button_up((int)(mx * aspect_w), (int)(my * aspect_h));
		}
	}

	if (view_mode == VIEW_MODE_FULLSCREEN) {
		if (net_client_session != NULL) {
			net_client_session->mouse_right_button_up(mx - step21_size_w - delta_x, my - step1_size_h - delta_y);
		}
	}

	return 0;
};
LRESULT VIEWER::WM_PAINT_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
	/*
	char ss[300];
	ss[0] = 0;
	if (net_client_session != nullptr) {
		if (net_client_session->recv_responce_screen + 1000 < GetTickCount()) {
			net_client_session->get_statistic(ss, 290);
		};
	}
	else {
		//sprintf__s_c(ss, 290, "null", "");
	}*/

	/*
	if (view_mode == VIEW_MODE_NOCONNECT) {

		gui->low_level->fill_color(0xeeeeee);

		gui->Paint();

		gui->low_level->Paint();

		InvalidateRect(hw, NULL, FALSE);
		return 0;
	}*/

	gui->low_level->fill_color(0xeeeeee);

	gui->Paint();
	
	/* 2021 09
	gui->low_level->paint_text(0, 50 - 1, 50 - 1, 500, 30, ww, 0, 0, -1);
	gui->low_level->paint_text(0, 50 + 1, 50 - 1, 500, 30, ww, 0, 0, -1);
	gui->low_level->paint_text(0, 50 - 1, 50 + 1, 500, 30, ww, 0, 0, -1);
	gui->low_level->paint_text(0, 50 + 1, 50 + 1, 500, 30, ww, 0, 0, -1);

	gui->low_level->paint_text(0, 50, 50, 500, 30, ww, 0x99ff00, 0, -1);
	*/
	gui->low_level->Paint();

	//gui->invalidate();

	//InvalidateRect(hw, NULL, FALSE);

	return 0;
	//return DefWindowProc(hw, msg, wp, lp);
};
LRESULT VIEWER::WM_NCMOUSEMOVE_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
	return DefWindowProc(hw, msg, wp, lp);
};
LRESULT VIEWER::WM_NCLBUTTONDOWN_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
	return DefWindowProc(hw, msg, wp, lp);
};
LRESULT VIEWER::WM_NCLBUTTONUP_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
	int mx, my;

	mx = (int)LOWORD(lp);
	my = (int)HIWORD(lp);

	if (small_top_panel->l_mouse_up(gui->low_level, mx, my) == true) return 0;

	if (file_transfer_dialog_IS_ACTIVE()) {
		// 2021 09 if (view_mode == VIEW_MODE_STRETCH)    file_transfer_dialog->mouse_left_button_up(low_level, (int)(mx * aspect_w), (int)(my * aspect_h));
		// 2021 09 if (view_mode == VIEW_MODE_FULLSCREEN) file_transfer_dialog->mouse_left_button_up(low_level, mx - step21_size_w - delta_x, my - step1_size_h - delta_y);
		return 0;
	}

	return DefWindowProc(hw, msg, wp, lp);
};
LRESULT VIEWER::WM_MOUSEMOVE_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {

	int mx, my;

	mx = (int)LOWORD(lp);
	my = (int)HIWORD(lp);

	if (small_top_panel->is_move == true) {
		small_top_panel->panel_x = mx + small_top_panel->move_delta_mx;
		small_top_panel->panel_y = my + small_top_panel->move_delta_my;

		if (small_top_panel->panel_x < 0) small_top_panel->panel_x = 0;
		if (small_top_panel->panel_y < 0) small_top_panel->panel_y = 0;

		if (small_top_panel->panel_x + small_top_panel->panel_w >= gui->low_level->window_w) small_top_panel->panel_x = gui->low_level->window_w - small_top_panel->panel_w - 1;
		if (small_top_panel->panel_y + 26 >= gui->low_level->window_h) small_top_panel->panel_y = gui->low_level->window_h - 26 - 1;

		gui->low_level->invalidate();
		return 0;
	}

	if (small_top_panel->mouse_over(gui->low_level, mx, my) == true) {
		return 0;
	}


	if (old_m_x != mx || old_m_y != my) {
		old_m_x = mx;
		old_m_y = my;

		if (file_transfer_dialog_IS_ACTIVE()) {
			//if (view_mode == VIEW_MODE_STRETCH)    file_transfer_dialog->mouse_over(low_level, mx, my);// ((int)(mx * aspect_w), (int)(my * aspect_h));
			//if (view_mode == VIEW_MODE_FULLSCREEN) file_transfer_dialog->mouse_over(low_level, mx, my); // (mx - step21_size_w - delta_x, my - step1_size_h - delta_y);
			gui->low_level->invalidate();
			return 0;
		}


		if (view_mode == VIEW_MODE_STRETCH) {
			if (net_client_session != NULL) {
				net_client_session->mouse_move((int)(mx * aspect_w), (int)(my * aspect_h));
			}
		}

		if (view_mode == VIEW_MODE_FULLSCREEN) {
			if (net_client_session != NULL) {
				net_client_session->mouse_move(mx - step21_size_w - delta_x, my - step1_size_h - delta_y);
			}
		}

	};

	return 0;
};
LRESULT VIEWER::WM_MOUSEWHEEL_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {

	int mx, my;

	mx = (int)LOWORD(lp);
	my = (int)HIWORD(lp);

	POINT pp;
	pp.x = mx;
	pp.y = my;

	ScreenToClient(hw, &pp);


	if (file_transfer_dialog_IS_ACTIVE()) {
		if ((wp & 0x80000000) == 0x80000000) {
			// 20021 09 file_transfer_dialog->mouse_wheel(low_level, 1, pp.x, pp.y);
		}
		else {
			// 20021 09 file_transfer_dialog->mouse_wheel(low_level, 2, pp.x, pp.y);
		}
		return 0;
	};


	if (net_client_session == NULL) return 0;
	if ((wp & 0x80000000) == 0x80000000) {
		net_client_session->mouse_whell(WM_MOUSEWHEEL__, 1, 0);
	}
	else {
		net_client_session->mouse_whell(WM_MOUSEWHEEL__, 2, 0);
	}

	return 0;
};
LRESULT VIEWER::WM_MOUSELEAVE_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {

	// 2021 09 if (file_transfer_dialog_IS_ACTIVE()) { file_transfer_dialog->mouse_leave(low_level); return 0; }

	return DefWindowProc(hw, msg, wp, lp);
};
LRESULT VIEWER::WM_LBUTTONDOWN_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {

	int mx, my;

	mx = (int)LOWORD(lp);
	my = (int)HIWORD(lp);

	if (small_top_panel->l_mouse_down(gui->low_level, mx, my) == true) return 0;

	/* 2021 09
	if (file_transfer_dialog_IS_ACTIVE()) {
		if (view_mode == VIEW_MODE_STRETCH) { 
			file_transfer_dialog->mouse_left_button_down(low_level, mx, my);
			set_track(hw);
			return 0;
		};
		if (view_mode == VIEW_MODE_FULLSCREEN) {
			file_transfer_dialog->mouse_left_button_down(low_level, mx, my);
			set_track(hw);
			return 0;
		};

		file_transfer_dialog->Close_btn_Click(low_level);

		set_track(hw);
		return 0;
	} 
	*/

	if (view_mode == VIEW_MODE_STRETCH) {
		if (net_client_session != NULL) {
			net_client_session->mouse_left_button_down((int)(mx * aspect_w), (int)(my * aspect_h));
		}
	}

	if (view_mode == VIEW_MODE_FULLSCREEN) {
		if (net_client_session != NULL) {
			net_client_session->mouse_left_button_down(mx - step21_size_w - delta_x, my - step1_size_h - delta_y);
		}
	}

	return 0;
};
LRESULT VIEWER::WM_LBUTTONUP_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
	int mx, my;

	mx = (int)LOWORD(lp);
	my = (int)HIWORD(lp);

	if (small_top_panel->l_mouse_up(gui->low_level, mx, my) == true) return 0;

	/* 2021 09
	if (file_transfer_dialog_IS_ACTIVE()) {
		if (view_mode == VIEW_MODE_STRETCH)    file_transfer_dialog->mouse_left_button_up(low_level, mx, my);
		if (view_mode == VIEW_MODE_FULLSCREEN) file_transfer_dialog->mouse_left_button_up(low_level, mx, my);
		return 0;
	}
	*/

	if (view_mode == VIEW_MODE_STRETCH) {
		if (net_client_session != NULL) {
			net_client_session->mouse_left_button_up((int)(mx * aspect_w), (int)(my * aspect_h));
		}
	}

	if (view_mode == VIEW_MODE_FULLSCREEN) {
		if (net_client_session != NULL) {
			net_client_session->mouse_left_button_up(mx - step21_size_w - delta_x, my - step1_size_h - delta_y);
		}
	}

	return 0;
};
LRESULT VIEWER::WM_KEYUP_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
	char_keyup(msg, wp, lp);
	return 0;
};
LRESULT VIEWER::WM_KEYDOWN_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {

	//send_udp("WM_KEYDOWN_");

	
	SHORT ctrl_key_state, shift_state, alt_state;

	if (viewer->prepare_pass_tik <= 20) {
		return DefWindowProc(hw, msg, wp, lp);
	}

	if (wp == VK_F1) {
		ctrl_key_state = GetKeyState(VK_CONTROL) & 0x8000;
		shift_state = GetKeyState(VK_SHIFT) & 0x8000;
		alt_state = GetKeyState(VK_MENU) & 0x8000;

		if (ctrl_key_state == 0 && shift_state == 0 && alt_state == 0) {
			if (view_mode == VIEW_MODE_STRETCH) { change_view_mode(VIEW_MODE_FULLSCREEN); }
			else {
				if (view_mode == VIEW_MODE_FULLSCREEN) change_view_mode(VIEW_MODE_STRETCH);
			};
			return 0;
		};
	};

	/* 2021 09 if (file_transfer_dialog_IS_ACTIVE()) {
		file_transfer_dialog->char_keydown(low_level, msg, wp, lp);
		return 0;
	}; */
	

	char_keydown(msg, wp, lp);

	return 0;
};

LRESULT VIEWER::WM_ERASEBKGND_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
	return 1;
};
LRESULT VIEWER::WM_CHAR_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
	return DefWindowProc(hw, msg, wp, lp);
};
LRESULT VIEWER::WM_CAPTURECHANGED_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
	return DefWindowProc(hw, msg, wp, lp);
};

void VIEWER::EXECUTE() {

	EXECUTE_is_run = true;

	

	boost::posix_time::milliseconds SleepTime(10);

	if (pass_no_encripted[0] == 0) {
		net_client_session->set_partner_pass_and_id(partner_id, pass__encripted);
		EXECUTE_is_run = false;
		return;
	};
	prepare_pass_tik = 0;
	

	//char ss[100];

	//sprintf_s(ss, 90, "pass_no_encripted %02X-%02X-%02X-%02X-%02X", pass_no_encripted[0], pass_no_encripted[1], pass_no_encripted[2], pass_no_encripted[3], pass_no_encripted[4] );

	//sudp(ss);

	while (GLOBAL_STOP == false) {

		if (prepare_pass_tik >= 0 && prepare_pass_tik < 20) {

			prepare_pass_tik++;
			gui->invalidate();
			

			for (int i = 0; i < 5000; i++) {
				sha3.hash_32_byte(pass_no_encripted, pass_no_encripted);
			};

			if (prepare_pass_tik == 20) {
				prepare_pass_tik++;
				net_client_session->set_partner_pass_and_id(partner_id, pass_no_encripted);
				EXECUTE_is_run = false;
				return;
			};

			//Sleep(333);
		}
		else {

			boost::this_thread::sleep(SleepTime);
		};
	}

	EXECUTE_is_run = false;
}

void VIEWER::callback__connect() {
	connect_ok = true;
	//set_view_mode(VIEW_MODE_STRETCH);
	change_view_mode(VIEW_MODE_STRETCH);
}
void VIEWER::callback__disconnect() {
	connect_ok = false;
	//set_view_mode(VIEW_MODE_NOCONNECT);
	change_view_mode(VIEW_MODE_NOCONNECT);
	/* 2021 09
	if (file_transfer_dialog->get_visible() == true) {
		file_transfer_dialog->set_visible(low_level, false);
	}
	*/
}

void resize_screen(GUI_low_level *low_level, SCREEN_LIGHT *src, SCREEN_LIGHT *dest, int new_window_w, int new_window_h) {

	int dest_w, dest_h;

	float left_dogon, dogon_right;

	int dogon, j_max;

	float koef_w, koef_h;


	if (new_window_w > src->w) new_window_w = src->w;
	if (new_window_h > src->h) new_window_h = src->h;

	if (low_level->window_w == 0) {
		koef_w = 1;
		koef_h = 1;
	}
	else {
		koef_w = (float)src->w / (float)new_window_w;
		koef_h = (float)src->h / (float)new_window_h;
	};

	//if (koef_w > 1) koef_w = 1;
	//if (koef_h > 1) koef_h = 1;


	left_dogon = 0.0;
	dogon = (int)koef_w;
	dogon_right = koef_w - (float)dogon;

	dest_w = (int)(src->w / koef_w);
	dest_h = (int)(src->h / koef_h);



	dest->lock_FULL(6000);
	//src->lock(7000);

	if (dest->buf_size < src->w * src->h) {
		dest->set_new_size(src->w, src->h);
	};

	if (dest_w > dest->w) {

		dest_w = dest->w;
	}

	unsigned int *d, *s;
	d = dest->get_buf();
	s = src->get_buf();
	int x, y, xx, yy;

	float count;

	yy = src->h;
	y = 0;

	int r, g, b, j;

	j_max = src->w * src->h;

	while (y < yy) {

		left_dogon = 0.0;
		dogon = (int)koef_w;
		dogon_right = koef_w - (float)dogon;

		d = dest->get_buf();
		s = src->get_buf();

		d += y * dest->w;
		s += y * src->w;

		x = 0;
		xx = dest_w;
		while (x < xx) {
			r = 0;
			g = 0;
			b = 0;

			j = (int)((float)x * koef_w);

			if (left_dogon != 0) {
				if (j < j_max) {
					r = (int)((float)(s[j] & 0x0000ff) * left_dogon);
					g = (int)((float)((s[j] & 0x00ff00) >> 8) * left_dogon);
					b = (int)((float)((s[j] & 0xff0000) >> 16) * left_dogon);
				}
				j++;
			};

			for (int i = 0; i < dogon; i++) {
				if (j < j_max) {
					r += (s[j] & 0x0000ff);
					g += ((s[j] & 0x00ff00) >> 8);
					b += ((s[j] & 0xff0000) >> 16);
				};
				j++;
			};

			if (dogon_right > 0) {
				if (j < j_max) {
					r += (int)((float)(s[j] & 0x0000ff) * dogon_right);
					g += (int)((float)((s[j] & 0x00ff00) >> 8) * dogon_right);
					b += (int)((float)((s[j] & 0xff0000) >> 16) * dogon_right);
				};
			};
			r = (int)(r / koef_w);

			if (r > 255) {
				r = 255;
			}


			g = (int)(g / koef_w);
			if (g > 255) {
				g = 255;
			}
			g = g << 8;

			//b = b >> 16;
			b = (int)(b / koef_w);
			if (b > 255) {
				b = 255;
			}
			b = b << 16;

			count = left_dogon;

			count += dogon;

			count += dogon_right;

			left_dogon = 1 - dogon_right;
			dogon = (int)(koef_w - left_dogon);
			dogon_right = koef_w - (float)dogon - left_dogon;

			*d = r | g | b;
			d++;

			x++;
		}

		y++;
	}

	x = 0;
	while (x < dest_w) {

		left_dogon = 0.0;
		dogon = (int)koef_h;
		dogon_right = koef_h - (float)dogon;

		d = dest->get_buf();
		s = dest->get_buf();

		d += x;
		s += x;

		j_max = dest->w * dest->h;

		y = 0;
		while (y < dest_h) {

			r = 0;
			g = 0;
			b = 0;

			j = (int)((float)y * koef_h) * dest->w;

			if (left_dogon != 0) {
				if (j < j_max) {
					r = (int)((float)(s[j] & 0x0000ff) * left_dogon);
					g = (int)((float)((s[j] & 0x00ff00) >> 8) * left_dogon);
					b = (int)((float)((s[j] & 0xff0000) >> 16) * left_dogon);
				}
				else {
					r = 255;
					g = 0;
					b = 0;
				}
				j += dest->w;
			};

			for (int i = 0; i < dogon; i++) {
				if (j < j_max) {
					r += (s[j] & 0x0000ff);
					g += ((s[j] & 0x00ff00) >> 8);
					b += ((s[j] & 0xff0000) >> 16);
				}
				else {
					r = 255;
					g = 0;
					b = 0;
				};
				j += dest->w;
			};

			if (dogon_right > 0) {
				if (j < j_max) {
					r += (int)((float)(s[j] & 0x0000ff) * dogon_right);
					g += (int)((float)((s[j] & 0x00ff00) >> 8) * dogon_right);
					b += (int)((float)((s[j] & 0xff0000) >> 16) * dogon_right);
				}
				else {
					r = 255;
					g = 0;
					b = 0;
				};
			};

			r = (int)(r / koef_h);
			g = (int)(g / koef_h);
			g = g << 8;
			b = (int)(b / koef_h);
			b = b << 16;

			left_dogon = 1 - dogon_right;
			dogon = (int)(koef_h - left_dogon);
			dogon_right = koef_h - (float)dogon - left_dogon;

			*d = r | g | b;
			d += dest->w;

			y++;
		}

		x++;
	}


	if (dest->resized_w != new_window_w || dest->resized_h != new_window_h) {
		dest->resized_w = new_window_w;
		dest->resized_h = new_window_h;
	};

	dest->mouse_x = src->mouse_x;
	dest->mouse_y = src->mouse_y;
	dest->keyboard_location = src->keyboard_location;
	dest->mouse_cursor_type_id = src->mouse_cursor_type_id;
	dest->_itis_user_move_mouse_ = src->_itis_user_move_mouse_;
	dest->screen_id = src->screen_id;
	dest->old_screen_id = src->old_screen_id;

	dest->unlock_FULL(6001);
	//src->unlock(7001);
}


void VIEWER::callback__arrived_screen(unsigned char *buf, int buf_size, unsigned int _FORMAT_VER) {


	int display_w, display_h;
	int flag = 0;
	int dw, dh;
	float aspect;
	RECT WindowRect, ClientRect;

	bool flag1 = false, flag2 = false, flag3 = false;

	if (screen_light_from_server_fullsize->w == 0) flag1 = true;

	if (_FORMAT_VER == PACKET_TYPE_responce_screen_ver11) {
		ENCODED_SCREEN_8bit_header *hdr8;
		
		hdr8 = (ENCODED_SCREEN_8bit_header *)buf;

		if (last_cursor != hdr8->mouse_cursor_type_id) {
			last_cursor = hdr8->mouse_cursor_type_id;
			need_cursor = hdr8->mouse_cursor_type_id;
			PostMessage(window_hwnd, WM_SETCURSOR, 0, 0);
		}

		if (hdr8->w != screen_light_from_server_fullsize->w || hdr8->h != screen_light_from_server_fullsize->h) {

			screen_light_from_server_fullsize->lock_FULL(4000);
			screen_light_from_server_fullsize->set_new_size(hdr8->w, hdr8->h);

			flag3 = true;
		};

		if (hdr8->w != screen_light_from_server_fullsize->w || hdr8->h != screen_light_from_server_fullsize->h) {
			flag3 = true;
		};

		decode_screen_8bit_first(buf, buf_size, screen_light_from_server_fullsize);

	};

	if (_FORMAT_VER == PACKET_TYPE_responce_screen_ver22) {
		ENCODED_SCREEN_12bit_header *hdr12;

		hdr12 = (ENCODED_SCREEN_12bit_header *)buf;

		if (last_cursor != hdr12->mouse_cursor_type_id) {
			last_cursor = hdr12->mouse_cursor_type_id;
			need_cursor = hdr12->mouse_cursor_type_id;
			PostMessage(window_hwnd, WM_SETCURSOR, 0, 0);
		}

		if (hdr12->w != screen_light_from_server_fullsize->w || hdr12->h != screen_light_from_server_fullsize->h) {

			screen_light_from_server_fullsize->lock_FULL(4000);
			screen_light_from_server_fullsize->set_new_size(hdr12->w, hdr12->h);

			flag3 = true;
		};

		if (hdr12->w != screen_light_from_server_fullsize->w || hdr12->h != screen_light_from_server_fullsize->h) {
			flag3 = true;
		};

		decode_screen_12bit_first(buf, buf_size, screen_light_from_server_fullsize);

	};

	if (_FORMAT_VER == PACKET_TYPE_responce_screen_ver33) {
		//sudp("detect arrived PACKET_TYPE_responce_screen_ver33");
		ENCODED_SCREEN_8bit_header *hdr8s;

		hdr8s = (ENCODED_SCREEN_8bit_header *)buf;

		if (last_cursor != hdr8s->mouse_cursor_type_id) {
			last_cursor = hdr8s->mouse_cursor_type_id;
			need_cursor = hdr8s->mouse_cursor_type_id;
			PostMessage(window_hwnd, WM_SETCURSOR, 0, 0);
		}

		if (hdr8s->w != screen_light_from_server_fullsize->w || hdr8s->h != screen_light_from_server_fullsize->h) {

			screen_light_from_server_fullsize->lock_FULL(4000);
			screen_light_from_server_fullsize->set_new_size(hdr8s->w, hdr8s->h);

			flag3 = true;
		};

		if (hdr8s->w != screen_light_from_server_fullsize->w || hdr8s->h != screen_light_from_server_fullsize->h) {
			flag3 = true;
		};

		decode_screen_8bit_second(buf, buf_size, screen_light_from_server_fullsize);
	};

	server_screen_w = screen_light_from_server_fullsize->w;
	server_screen_h = screen_light_from_server_fullsize->h;

	if (view_mode == VIEW_MODE_STRETCH) {

		if (screen_light_from_server_fullsize->w < gui->low_level->window_w || screen_light_from_server_fullsize->h < gui->low_level->window_h) flag2 = true;

		if (flag1 == true || flag2 == true || flag3 == true) {
			// размер экрана на сервере уменьшился и теперь нужно уменьшить размер окна

			get_display_size_pixel(&display_w, &display_h);

			GetWindowRect(window_hwnd, &WindowRect);
			GetClientRect(window_hwnd, &ClientRect);

			dw = (WindowRect.right - WindowRect.left) - (ClientRect.right - ClientRect.left);
			dh = (WindowRect.bottom - WindowRect.top) - (ClientRect.bottom - ClientRect.top);

			if (ClientRect.right - ClientRect.left > 0) {
				aspect_w = (float)screen_light_from_server_fullsize->w / (float)(ClientRect.right - ClientRect.left);
			}
			else {
				aspect_w = 0;
			}
			if (ClientRect.bottom - ClientRect.top > 0) {
				aspect_h = (float)screen_light_from_server_fullsize->h / (float)(ClientRect.bottom - ClientRect.top);
			}
			else {
				aspect_h = 0;
			}
			// расчитаем размер по высоте + ширине. если не подойдет, то потом расчитаем по ширине+высоте

			int ww, hh;

			ww = screen_light_from_server_fullsize->w + dw;
			hh = screen_light_from_server_fullsize->h + dh;

			if (ww > display_w || hh > display_h) {

				aspect = (float)screen_light_from_server_fullsize->h / (float)screen_light_from_server_fullsize->w;

				ww = (int)((float)display_w / (float)1.1);
				hh = (int)((float)ww * aspect);

				if (hh > display_h) {
					hh = (int)((float)display_h / (float)1.1);
					ww = (int)((float)hh / aspect);
				}

				ww += dw;
				hh += dh;



			}


			WindowRect.left = (display_w - ww) / 2;
			WindowRect.right = WindowRect.left + ww;

			WindowRect.top = (display_h - hh) / 2;
			WindowRect.bottom = WindowRect.top + hh;

			if (screen_light_from_server_fullsize->is_lock_FULL) screen_light_from_server_fullsize->unlock_FULL(4001);

			MoveWindow(window_hwnd, WindowRect.left, WindowRect.top, ww, hh, TRUE);
			return;
		};


	}
	
	if (last_use_resized == 2) {

		resize_screen(gui->low_level, screen_light_from_server_fullsize, screen_light_from_server_resized1, gui->low_level->window_w, gui->low_level->window_h);

		last_use_resized = 1;

		screen_light_from_server_resized = screen_light_from_server_resized1;
	}
	else {

		resize_screen(gui->low_level, screen_light_from_server_fullsize, screen_light_from_server_resized2, gui->low_level->window_w, gui->low_level->window_h);

		last_use_resized = 2;

		screen_light_from_server_resized = screen_light_from_server_resized2;
	}

	if (screen_light_from_server_fullsize->is_lock_FULL) {
		screen_light_from_server_fullsize->unlock_FULL(4004);
	};

	gui->low_level->invalidate();
}

void VIEWER::request_FILE_LIST_from_partner(TRANSFER_DIALOG2_DirsFiles_TREE_element *e) {
	/* 2021 09
	if (e == nullptr) return;
	wchar_t w[5100];

	zero_wchar_t(w, 5100);

	e->get_folder_full_name(w);

	if (net_client_session != NULL) {

		//net_client_session->need_request_FilesList(folder_name);
		net_client_session->need_request_FilesList(w);


	}
	*/
}

void VIEWER::request_FILE_LIST_from_partner_RESPONCE_1(unsigned char *buf, int buf_size) {
	/* 2021 09
	if (file_transfer_dialog != nullptr &&
		file_transfer_dialog->Dest_DirsFiles != nullptr &&
		file_transfer_dialog->Dest_DirsFiles->Tree != nullptr
		)
	{
		file_transfer_dialog->Dest_DirsFiles->Tree->file_list_from_partner_RESPONCE(low_level, buf, buf_size);
	}
	*/
}

void VIEWER::request_FILE_LIST_from_partner_RESPONCE_2(unsigned char *buf, int buf_size) {
	// 2021 09 file_transfer_dialog->transfer_PARTNER_to_MY->request_folder_content_RESPONCE(buf, buf_size);
};


void VIEWER::transfer_dialog_use() {
	if (gui_viewer_transfer_dialog == NULL) return;
	if (gui_viewer_transfer_dialog->is_visible == false) {
		transfer_dialog_SHOW();
	}
	else {
		transfer_dialog_HIDE();
	}

}

void VIEWER::transfer_dialog_SHOW() {
	if (gui_viewer_transfer_dialog == nullptr) return;
	gui_viewer_transfer_dialog->is_visible = true;
	file_transfer_dialog->set_visible(gui->low_level, true);

	ModifyMenu(h_system_menu, MY_MENU_TRANSFER_FILE, MF_STRING | MF_CHECKED, MY_MENU_TRANSFER_FILE, L"Transfer files");

	gui->low_level->invalidate();
}

void VIEWER::transfer_dialog_HIDE() {
	if (gui_viewer_transfer_dialog == NULL) return;
	gui_viewer_transfer_dialog->is_visible = false;
	file_transfer_dialog->set_visible(gui->low_level, false);

	ModifyMenu(h_system_menu, MY_MENU_TRANSFER_FILE, 0, MY_MENU_TRANSFER_FILE, L"Transfer files");

	gui->low_level->invalidate();
}

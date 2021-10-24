#pragma once

#define _WINSOCKAPI_ 
#include <windows.h>
#undef _WINSOCKAPI_
#include <winsock2.h>
#include <stdlib.h>
#include <iphlpapi.h>
#include <stdio.h>
#undef _WINSOCKAPI_

#include "tools.h"
#include "GUI.h"
#include "GUI_element.h"
#include "NET_CLIENT_SESSION.h"
#include "TRANSFER_DIALOG2.h"
#include "TRANSFER_DIALOG2_DirsFiles_TREE_element.h"

#define VIEW_MODE_NOCONNECT 0
#define VIEW_MODE_STRETCH 1
#define VIEW_MODE_FULLSCREEN 2

class VIEWER_small_top_panel;

class VIEWER 
{
public:

	uint64_t partner_id = 0;
	unsigned char  pass__encripted[32];
	unsigned char  pass_no_encripted[32];

	GUI *gui = nullptr;
	GUI_Element *gui_viewer = nullptr
		, *gui_viewer_small_top_panel = nullptr
		, *gui_viewer_transfer_dialog = nullptr;
	TRANSFER_DIALOG2 *file_transfer_dialog = nullptr;

	void transfer_dialog_use();
	void transfer_dialog_SHOW();
	void transfer_dialog_HIDE();

	bool connect_ok = false;

	void init_gui_VIEWER();

	int step1_size_h = 0, step1_size_w = 0;
	int step2_size_h = 0, step21_size_w = 0, step22_size_x = 0, step22_size_w = 0;
	int step3_size_h = 0, step3_size_w = 0;
	int delta_x = 0, delta_y = 0;
	WINDOWPLACEMENT wpc;
	RECT last_WM_MOVE;
	DWORD last_check_fokus_last = 0;

	float aspect_w = 0, aspect_h = 0;
	int server_screen_w = 0, server_screen_h = 0;

	HMENU h_system_menu = 0;
	HWND window_hwnd = 0;
	VIEWER_small_top_panel *small_top_panel = nullptr;

	MY_SHA3 sha3;
	int prepare_pass_tik = 99;
	NET_CLIENT_SESSION *net_client_session = nullptr;

	//boost::thread* thread_EXECUTE = nullptr;
	//bool EXECUTE_is_run = false;
	//void EXECUTE();

	int view_mode = VIEW_MODE_NOCONNECT;
	//void set_view_mode(int new_mode);
	void change_view_mode(int p);

	DWORD last_change_delta_xy = 0;
	void  recalc_delta_xy_for_FULLSCREEN(int mx, int my);
	int old_m_x = -1, old_m_y = -1;
	int is_vis_timer = 0;
	int is_novis_timer = 0;
	unsigned long last_cursor = 0, last_set_cursor = 0, need_cursor = 0;

	void char_keydown(int msg, int wp, int lp);
	void char_keyup(int msg, int wp, int lp);

	void send_CtrlAltDel();
	void send_Change_LNG();

	bool file_transfer_dialog_IS_ACTIVE();
	//########################################################################

	void request_FILE_LIST_from_partner(TRANSFER_DIALOG2_DirsFiles_TREE_element *e);
	void request_FILE_LIST_from_partner_RESPONCE_1(unsigned char *buf, int buf_size);
	void request_FILE_LIST_from_partner_RESPONCE_2(unsigned char *buf, int buf_size);

	//########################################################################

	bool is_sync_clipboards = true;
	void callback__arrived_screen(unsigned char *buf, int buf_size, unsigned int _FORMAT_VER);
	void callback__connect();
	void callback__disconnect();

	SCREEN_LIGHT *screen_light_from_server_fullsize = nullptr; // скрин с сервера полный
	SCREEN_LIGHT *screen_light_from_server_resized  = nullptr; // скрин с сервера уменьшенный до текущего размера окна
	SCREEN_LIGHT *screen_light_from_server_resized1 = nullptr; // скрин с сервера уменьшенный до текущего размера окна
	SCREEN_LIGHT *screen_light_from_server_resized2 = nullptr; // скрин с сервера уменьшенный до текущего размера окна
	int last_use_resized = 2;

	bool last_detect_window_in_focus = false; // нужно для того, чтобы определить момент потери фокуса главным окном

	LRESULT WM_CREATE_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
	LRESULT WM_TIMER_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
	LRESULT WM_GETMINMAXINFO_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
	LRESULT WM_CAPTURECHANGED_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
	LRESULT WM_SYSCOMMAND_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
	LRESULT WM_SETFOCUS_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
	LRESULT WM_KILLFOCUS_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
	LRESULT WM_ACTIVATE_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
	LRESULT WM_ACTIVATEAPP_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
	LRESULT WM_WINDOWPOSCHANGED_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
	LRESULT WM_WINDOWPOSCHANGING_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
	LRESULT WM_NCACTIVATE_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
	LRESULT WM_MOUSEACTIVATE_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);

	LRESULT WM_SETCURSOR_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);

	LRESULT WM_NCMOUSEMOVE_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);

	LRESULT WM_MOUSEMOVE_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);

	LRESULT WM_MOUSELEAVE_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
	LRESULT WM_NCLBUTTONDOWN_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);

	LRESULT WM_LBUTTONDOWN_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);

	LRESULT WM_NCLBUTTONUP_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
	LRESULT WM_LBUTTONUP_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
	LRESULT WM_RBUTTONDOWN_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
	LRESULT WM_RBUTTONUP_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
	LRESULT WM_ERASEBKGND_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);

	LRESULT WM_PAINT_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);

	LRESULT WM_MOVE_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
	LRESULT WM_VSCROLL_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
	LRESULT WM_HSCROLL_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);

	LRESULT WM_SIZING_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
	LRESULT WM_SIZE_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
	

	LRESULT WM_MOUSEWHEEL_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
	LRESULT WM_CHAR_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);

	LRESULT WM_KEYDOWN_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);

	LRESULT WM_KEYUP_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
	LRESULT WM_SYSKEYDOWN_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
	LRESULT WM_SYSKEYUP_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
	LRESULT WM_NCHITTEST_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);

	LRESULT WM_DRAWCLIPBOARD_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
	LRESULT WM_CHANGECBCHAIN_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);

	
	boost::thread* EXECUTE_thread = nullptr;
	bool EXECUTE_thread_is_run = false;
	void EXECUTE();
	void start_EXECUTE();

	boost::thread* ASYNC_LOAD_EXECUTE_thread = nullptr;
	bool ASYNC_LOAD_thread_is_run = false;
	void ASYNC_LOAD_EXECUTE();
	void start_ASYNC_LOAD_EXECUTE();

	


	void calc_start_size(int &x, int &y, int &w, int &h);

	void RUN_VIEWER(uint8_t *partner_id, uint8_t *pass_encrypted, uint8_t *pass_no_encripted);

	TRACKMOUSEEVENT tme;
	void set_track(HWND hw);

	VIEWER();
	~VIEWER();
};




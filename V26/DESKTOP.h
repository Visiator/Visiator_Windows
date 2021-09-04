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
#include "GUI_low_level.h"

class DESKTOP
{
public:

	GUI *gui = nullptr;
	GUI_Element *btn_close = nullptr
		, *btn_change_to_incomming = nullptr
		, *btn_change_to_outcomming = nullptr
		, *btn_change_to_autorun = nullptr
		, *panel_incoming = nullptr
		, *panel_outgoing = nullptr
		, *panel_autorun = nullptr
		, *btn_outgoing_connect = nullptr
		, *btn_save_pass = nullptr
		, *btn_www = nullptr;


	void RUN();
	void init_gui();
	void calc_start_size(int &x, int &y, int &w, int &h);

	// thread для net server session pool
	bool  EXECUTE_net_server_session_pool_thread_is_run = false;
	void  EXECUTE_net_server_session_pool();
	DWORD EXECUTE_net_server_session_pool_thread_id = 0;
	void  start_EXECUTE_net_server_session_pool();

	// thread для encrypt out pass
	bool  EXECUTE_encrypt_out_pass_thread_is_run = false;
	void  EXECUTE_encrypt_out_pass();
	DWORD EXECUTE_encrypt_out_pass_thread_id = 0;
	void  start_EXECUTE_encrypt_out_pass();
	bool  need_encrypt_out_pass = false;

	// thread для encrypt autostart pass
	bool  EXECUTE_encrypt_autostart_pass_thread_is_run = false;
	void  EXECUTE_encrypt_autostart_pass();
	DWORD EXECUTE_encrypt_autostart_pass_thread_id = 0;
	void  start_EXECUTE_encrypt_autostart_pass();
	bool  need_encrypt_autostart_pass = false;

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

	LRESULT WM_NCMOUSELEAVE_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
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


	void char_keydown(int msg, int wp, int lp);
	void char_keyup(int msg, int wp, int lp);

	void char_(int msg, int wp, int lp);

	TRACKMOUSEEVENT tme;
	bool mouse_track_is_set = false;
	void set_track(HWND hw);

	DESKTOP();
	~DESKTOP();

};


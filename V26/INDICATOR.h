#pragma once

#define WIN32_LEAN_AND_MEAN
#define WIN32_WINNT 0x0501


#define _WINSOCKAPI_ 
#include <windows.h>
#undef _WINSOCKAPI_
#include <winsock2.h>
#include <stdlib.h>
#include <iphlpapi.h>
#include <stdio.h>
#undef _WINSOCKAPI_

#include <boost/thread.hpp>
#include <boost/bind.hpp>

#include <shellapi.h>

//#include "CLIPBOARD.h"

struct NOTIFYICONDATAW_ {
	DWORD cbSize;
	HWND hWnd;
	UINT uID;
	UINT uFlags;
	UINT uCallbackMessage;
	HICON hIcon;
	WCHAR  szTip[128];
	DWORD dwState;
	DWORD dwStateMask;
	WCHAR  szInfo[256];
	union {
		UINT  uTimeout;
		UINT  uVersion;  // used with NIM_SETVERSION, values 0, 3 and 4
	} DUMMYUNIONNAME;
	WCHAR  szInfoTitle[64];
	DWORD dwInfoFlags;
	GUID guidItem;

};

class NOTIFY_ICON
{
public:
	unsigned long long last_set_service_id = 1;
	int last_set_icon_idx = -1;

	bool icon_create = false, icon_load = false;
	NOTIFYICONDATA icon;
	HICON icons[4];
	void add(HWND hw);
	void set(HWND hw, int icon_type, unsigned long long service_id);
	void del();
	
	
	NOTIFY_ICON();
	~NOTIFY_ICON();
};

class INDICATOR
{
public:

	

	void set_notify_icon(int v, unsigned long long service_id);
	int need_set_notify_icon = 0;
	unsigned long long need_set_notify_service_id = 1;

	DWORD read_SLAVE_pipe_indicator_TIMEOUT = 0;
	
	bool is_need_sync_clipboards = false; 
	bool is_sync_clipboards = false;

	NOTIFY_ICON notify_icon;
	bool clipboard_is_changed = false;

	HANDLE mutex;
	bool lock_mutex(wchar_t *mutex_name);

	DWORD last_read_service_info_INDICATOR = 0;



	HANDLE INDICATOR_pipe_handle_read_info = 0;  
	//DWORD INDICATOR_pipe_handle_read_info_TIMEOUT = 0;
	bool is_run_PIPE_CONTROL = false;
	//void PIPE_CONTROL_EXECUTE();

	//void start_tread_PIPE_CONTROL();
	
	bool is_pipe_info_CONNECT = false;
	//void pipe_info_CONNECT();

	LRESULT WM_CREATE_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);

	void    WM_TIMER_(HWND hw);

	HWND Next_Clipboard_Viewer = 0;
	LRESULT WM_DRAWCLIPBOARD_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
	LRESULT WM_CHANGECBCHAIN_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);

	void RUN(HINSTANCE instance);
	
	void SET_is_sync_clipboards(unsigned int v);

	bool is_run_CONTROL = false;
	bool is_run_indicator_PIPE_SLAVE = false;
	HANDLE indicator_pipe_handle_SLAVE = 0;
	DWORD PIPE_SLAVE_Timeout = 0;

	boost::thread *SLAVE_EXECUTE_thread = nullptr;
	void PIPE_SLAVE_EXECUTE();
	void start_PIPE_SLAVE_thread();

	// control
	boost::thread* EXECUTE_CONTROL_thread = nullptr;
	void INDICATOR_PIPE_CONTROL_THREAD_EXECUTE();
	void start_INDICATOR_PIPE_CONTROL_THREAD();

	//void set_icon(int icon_idx, unsigned long long service_id);

	INDICATOR();
	~INDICATOR();
};


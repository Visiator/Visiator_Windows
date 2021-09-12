#pragma once

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


#include "tools.h"

class APPLICATION_ATTRIBUTES
{
public:
	int VER = 0;
	boost::thread_group tgroup;
	bool is_desktop = false;
	bool is_viewer  = false;
	bool is_service = false;
	bool im_is_admin = false;
	int  is_32_or_64_bit_system = 0;
	
	wchar_t *my_exe_file_name = nullptr;
	unsigned char startup_parametr_PASS_ENCR[32];
	void _SetProcessDPIAware();

	PROXY_LIST *proxy_list = nullptr;

	HWND  desktop_window_hwnd = 0;
	HWND  viewer_window_hwnd = 0;

	HINSTANCE hInstance = 0;
	DWORD global_my_proc_id = 0;
	void get_programm_version();
	void getRealWindowsVersion();
	void set_startup_paramert_pr(wchar_t *p);

	void get_all_parametrs(HINSTANCE hInstance_);

	int  OsMajorVersion = 0, OsMinorVersion = 0;

	APPLICATION_ATTRIBUTES();
};


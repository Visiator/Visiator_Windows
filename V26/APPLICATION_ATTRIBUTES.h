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
#include <boost/bind/bind.hpp>


#include "tools.h"
#include "GUI_Element.h"

// message box
#define MODAL_PROCESS_show_message_box1_1  108
#define MODAL_PROCESS_show_message_box1_2  109



class APPLICATION_ATTRIBUTES
{
public:
	int VER = 0;
	boost::thread_group tgroup;
	bool is_desktop   = false;
	bool is_viewer    = false;
	bool is_service   = false;
	bool is_agent     = false;
	bool is_indicator = false;

	bool im_is_admin = false;
	int  is_32_or_64_bit_system = 0;
	int  modal_process = 0;

	wchar_t *my_exe_file_name = nullptr;
	unsigned char startup_parametr_PASS_ENCR[32];
	void _SetProcessDPIAware();

	
	

	unsigned long long desktop_public_id = 0, desktop_private_id = 0, service_public_id = 0, service_private_id = 0;
	GUI_Element *gui_desktop_public_id = nullptr, *gui_service_public_id = nullptr, *gui_service_connection_status = nullptr, *gui_service_VISIATOR_status = nullptr;

	void set_service_public_id(unsigned long long val);
	void set_service_private_id(unsigned long long val);

	PROXY_LIST *proxy_list = nullptr;

	HWND  desktop_window_hwnd = 0;
	HWND  viewer_window_hwnd = 0;
	HWND  indicator_hwnd = 0;

	HINSTANCE hInstance = 0;
	DWORD global_my_proc_id = 0;
	DWORD agent_process_id = 0;
	DWORD indicator_process_id = 0;

	void get_programm_version();
	void getRealWindowsVersion();
	void set_startup_paramert_pr(wchar_t *p);

	void get_all_parametrs(HINSTANCE hInstance_);
	bool startup_parametr_ReadOnly = false;
	void set_startup_paramert_ro(wchar_t *p);

	int  OsMajorVersion = 0, OsMinorVersion = 0;

	bool WINDOWS_is_XP();

	APPLICATION_ATTRIBUTES();
};


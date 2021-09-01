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

class APPLICATION_ATTRIBUTES
{
public:

	bool is_desktop = false;

	HWND  desktop_window_hwnd = 0;

	HINSTANCE hInstance = 0;
	DWORD global_my_proc_id = 0;


	void get_all_parametrs(HINSTANCE hInstance_);

};


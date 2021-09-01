#pragma once

#define _WINSOCKAPI_ 
#include <windows.h>
#undef _WINSOCKAPI_
#include <winsock2.h>
#include <stdlib.h>
#include <iphlpapi.h>
#include <stdio.h>
#undef _WINSOCKAPI_


class GUI_low_level
{
private:
	int buf_size = 0;
	BITMAPINFO *bmInfo = NULL;

	bool is_lock_FULL = false;
	bool is_lock_READ = false;
	bool is_lock_WRITE = false;


public:
	unsigned int *buf = nullptr;

	HWND hw = 0;
	int window_w = 0, window_h = 0;

	void lock_FULL();
	void lock_READ();
	void lock_WRITE();

	void unlock_FULL();
	void unlock_READ();
	void unlock_WRITE();

	void fill_color(unsigned int col);

	void change_size(HWND hw, int w_, int h_);
	void Paint();

	GUI_low_level(HWND hw_);
};


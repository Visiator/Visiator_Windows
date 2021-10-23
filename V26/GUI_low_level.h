#pragma once

//#include "stdint.h"

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

	void set_pixx(int x, int y, unsigned int color);
	unsigned int get_pix(int x, int y);
	void line_v(int x, int y, int h, unsigned int color);
	void line_h(int x, int y, int w, unsigned int color);
	void fill_color(unsigned int col);

	void rectangle(int x, int y, int w, int h, unsigned int color);

	void fill_rectangle(int x, int y, int w, int h, unsigned int color, int transparent);
	void paint_tri_1_close(int x, int y, unsigned int color);
	void paint_tri_1_open(int x, int y, unsigned int color);

	void change_size(HWND hw, int w_, int h_);
	void invalidate();
	void Paint();

	bool paint_text_rect(int font_idx, int x, int y, int w, int h, wchar_t *text, unsigned int color);
	int  text_height(int font_idx);
	int  text_width(int font_idx, wchar_t *text);
	void paint_tre_up(int x, int y, int w, unsigned int color);
	void paint_tre_down(int x, int y, int w, unsigned int color);

	void paint_text(int font_idx, int x, int y, int w, int h, wchar_t *text, unsigned int color, int aligment, int edit_cursor_position);
	void paint_text(int font_idx, int x, int y, int w, int h, char *text, unsigned int color, int aligment, int edit_cursor_position);

	void paint_close_krest(int x, int y, int w, unsigned int color);

	GUI_low_level(HWND hw_);
};


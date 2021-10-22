#pragma once

#include <map>
//#include "stdint.h"

#include "GUI_low_level.h"

class BUKVA
{
public:
	uint32_t idx = 0;
	unsigned char *buf;
	int buf_size = 0;
	int w, h;
	
	int paintAAA(GUI_low_level *low_level, int x, int y, uint32_t color, bool show_cursor);
	int paint_dummy(GUI_low_level *low_level, int x, int y, uint32_t color);
	int paint2(GUI_low_level *low_level, int x, int y, unsigned int color, bool show_cursor = false);


	BUKVA();
	BUKVA(int idx_, int w_, int h_, uint8_t *buf_);
	BUKVA::BUKVA(int idx_, int w_, int h_, uint8_t *buf_, int v2);
};

class FONT
{
public:

	std::map <int, BUKVA> bukva;

	int  text_width(const char *txt);
	int  text_width(const wchar_t *txt);
	int  text_height();
	void paintAAA(GUI_low_level *low_level, int x, int y, const wchar_t *txt, uint32_t color_, int cursor_position, bool is_pass);
	
	void paint_dummy(GUI_low_level *low_level, int x, int y, const wchar_t *txt, uint32_t color_);

	int  paint_text_in_width(GUI_low_level *low_level, int x, int y, int max_w, wchar_t *text, unsigned int color);

	void load_from_buffer(unsigned char *buf, int buf_size);
	int  load_dummy(unsigned char *buf, int buf_size);

	FONT();
};


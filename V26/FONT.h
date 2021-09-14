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
	BUKVA();
	BUKVA(int idx_, int w_, int h_, uint8_t *buf_);
};

class FONT
{
public:

	std::map <int, BUKVA> bukva;
	int  text_width(const wchar_t *txt);
	int  text_height();
	void paintAAA(GUI_low_level *low_level, int x, int y, const wchar_t *txt, uint32_t color_, int cursor_position, bool is_pass);

	int  paint_text_in_width(GUI_low_level *low_level, int x, int y, int max_w, wchar_t *text, unsigned int color);

	void load_from_buffer(uint8_t *buf, int buf_size);

	FONT();
};


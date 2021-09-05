#pragma once

#include <map>
//#include "stdint.h"

#include "GUI_low_level.h"

class BUKVA
{
public:
	uint32_t idx = 0;
	uint8_t *buf;
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

	void paintAAA(GUI_low_level *low_level, int x, int y, const wchar_t *txt, uint32_t color_, int cursor_position);

	void load_from_buffer(uint8_t *buf, int buf_size);

	FONT();
};


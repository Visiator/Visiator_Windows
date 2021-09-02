#pragma once

#include "stdint.h"



#include "GUI_low_level.h"

#define GUI_Element_Type_panel 1

class GUI_Element
{
public:
	int  type = 0;
	int  x = 0, y = 0, w = 0, h = 0;
	uint32_t color = 0;
	bool is_passive = true;

	void clean();
	void Paint(GUI_low_level *low_level);

	GUI_Element(int type_, int x_, int y_, int w_, int h_, int BMP_id);
	~GUI_Element();
};


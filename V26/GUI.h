#pragma once

#include <list>

#include "GUI_low_level.h"
#include "GUI_Element.h"

class GUI
{
public:
	std::list<GUI_Element> elements;

	GUI_Element *add_element(int type_, int x_, int y_, int w_, int h_, int BMP_id);

	void Paint(GUI_low_level *low_level);

	GUI();
};


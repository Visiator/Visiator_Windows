#pragma once

#include <list>

#include "GUI_low_level.h"
#include "GUI_Element.h"
#include "FONT.h"

class GUI
{
public:
	
	HWND hw = 0;
	GUI_low_level *low_level = nullptr;
	void change_size(HWND hw_, int w_, int h_);

	std::list<GUI_Element> elements;

	GUI_Element *get_active_Element_mouse_over(int mx, int my);

	void mouse_move(int mx, int my);

	void left_button_mouse_down(int mx, int my);
	void left_button_mouse_up(int mx, int my);

	GUI_Element *add_element(int type_, int x_, int y_, int w_, int h_, uint32_t color_);

	void char_keydown(int msg, int wp, int lp);
	void char_keyup(int msg, int wp, int lp);


	void close_application();
	void invalidate();
	void Paint();

	GUI(HWND hw_);
};


#pragma once

#include "stdint.h"



#include "GUI_low_level.h"
#include "TEXTURA.h"

#define GUI_Element_Type_panel 1
#define GUI_Element_Type_button 2

class GUI_Element
{
public:
	int  type = 0;
	int  x = 0, y = 0, w = 0, h = 0;
	uint32_t color = 0;

	GUI_Element *parent = nullptr;

	bool is_visible = true;
	bool is_active = false;
	bool is_mouse_hover = false;
	bool is_mouse_pressed = false;

	void set_mouse_hover(bool val);
	void set_mouse_pressed(bool val);

	void(*func__mouse_press)(int mx, int my) = nullptr;
	void(*func__mouse_unpress)(int mx, int my) = nullptr;


	TEXTURA *textura_normal  = nullptr;
	TEXTURA *textura_hover   = nullptr;
	TEXTURA *textura_pressed = nullptr;


	void clean();
	void Paint(GUI_low_level *low_level);

	void load_BMP_from_resource(int id_normal, int id_hover, int id_pressed);

	GUI_Element(int type_, int x_, int y_, int w_, int h_, uint32_t color_);
	~GUI_Element();
};


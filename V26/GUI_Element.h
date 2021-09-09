#pragma once

#include <string>



#include "GUI_low_level.h"
#include "TEXTURA.h"

#define GUI_Element_Type_panel 1
#define GUI_Element_Type_button 2
#define GUI_Element_Type_edit 3
#define GUI_Element_Type_indicator 4

class PASS_EYE {
public: 
	bool is_eye_open = false;
	TEXTURA *eye = nullptr, *eye_open = nullptr;
	PASS_EYE();
};

class GUI_Element
{
public:
	std::wstring name;
	int  type = 0;
	int  x = 0, y = 0, w = 0, h = 0;
	uint32_t color = 0;

	int cursor_position = 0;
	std::wstring text;
	void set_text(wchar_t *txt_);

	GUI_Element *parent = nullptr;

	
	bool is_visible = true;
	bool is_active = false;
	bool is_mouse_hover = false;
	bool is_mouse_pressed = false;
	bool is_edit_begin = false;
	bool is_password = false;
	


	void edit_begin();
	void edit_end();

	void set_mouse_hover(bool val);
	void set_mouse_pressed(bool val);

	void(*func__mouse_press)(int mx, int my) = nullptr;
	void(*func__mouse_unpress)(int mx, int my) = nullptr;


	TEXTURA *textura_normal  = nullptr;
	TEXTURA *textura_hover   = nullptr;
	TEXTURA *textura_pressed = nullptr;
	PASS_EYE pass_eye;
	

	void char_keydown(int msg, int wp, int lp, wchar_t *wbuffer);
	void char_keyup(int msg, int wp, int lp);


	void clean();
	void Paint(GUI_low_level *low_level);

	void load_BMP_from_resource(int id_normal, int id_hover, int id_pressed);

	GUI_Element(int type_, int x_, int y_, int w_, int h_, uint32_t color_);
	~GUI_Element();
};


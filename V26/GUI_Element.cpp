

#include "GUI_Element.h"
#include "GUI_low_level.h"
#include "FONT.h"

extern FONT *font[10];

GUI_Element::GUI_Element(int type_, int x_, int y_, int w_, int h_,  uint32_t color_) {
	clean();
	type = type_;
	x = x_;
	y = y_;
	w = w_;
	h = h_;
	color = color_;
	is_active = false;
	if (type_ == GUI_Element_Type_button) {
		is_active = true;
	}
}

GUI_Element::~GUI_Element() {

}


void GUI_Element::set_mouse_hover(bool val) {
	if (val == true) {
		if (is_mouse_hover != true) {
			is_mouse_hover = true;
		}
	}
	else {
		if (is_mouse_hover != false) {
			is_mouse_hover = false;
		}
	}
}

void GUI_Element::set_mouse_pressed(bool val) {
	if (val == true) {
		if (is_mouse_pressed != true) {
			is_mouse_pressed = true;
		}
	}
	else {
		if (is_mouse_pressed != false) {
			is_mouse_pressed = false;
		}
	}
}


void GUI_Element::clean() {
	type = 0;
	x = 0;
	y = 0;
	w = 0; 
	h = 0;
	color = 0;
	is_visible = true;
	is_active = false;
	is_mouse_hover = false;
	is_mouse_pressed = false;
}

void GUI_Element::Paint(GUI_low_level *low_level) {

	int cur_pos;

	if (is_visible == false) return;
	if (parent != nullptr && parent->is_visible == false) return;

	low_level->rectangle(x, y, w, h, color);
	if (is_mouse_pressed == true && textura_pressed != nullptr) {
		textura_pressed->Paint(low_level, x, y);
		return;
	}
	else {
		if (is_mouse_hover == true && textura_hover != nullptr) {
			textura_hover->Paint(low_level, x, y);
			return;
		}
		else {
			if (textura_normal != nullptr) {
				textura_normal->Paint(low_level, x, y);
			}
		};
	};
	if (type == GUI_Element_Type_edit) {
		if (text.empty() == false) {	
			if (is_active == true && is_edit_begin == true) {
				cur_pos = cursor_position;
			}
			else {
				cur_pos = -1;
			}
			font[1]->paintAAA(low_level, x, y, text.c_str(), 0x000000, cur_pos);
		};
	}
}

void GUI_Element::load_BMP_from_resource(int id_normal, int id_hover, int id_pressed) {
	if (id_normal != 0 && textura_normal == nullptr) {
		textura_normal = new TEXTURA(id_normal);
	}
	if (id_hover != 0 && textura_hover == nullptr) {
		textura_hover = new TEXTURA(id_hover);
	}
	if (id_pressed != 0 && textura_pressed == nullptr) {
		textura_pressed = new TEXTURA(id_pressed);
	}
}

void GUI_Element::set_text(wchar_t *txt_) {
	text.clear();
	text = txt_;
}

void GUI_Element::edit_begin() {
	if (is_edit_begin == true) return;
	is_edit_begin = true;
	cursor_position = text.length();
}

void GUI_Element::edit_end() {
	if (is_edit_begin == false) return;
	is_edit_begin = false;
}

void GUI_Element::char_keydown(int msg, int wp, int lp) {

}

void GUI_Element::char_keyup(int msg, int wp, int lp) {

}

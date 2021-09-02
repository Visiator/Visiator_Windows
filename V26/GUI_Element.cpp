#include "GUI_Element.h"
#include "GUI_low_level.h"

GUI_Element::GUI_Element(int type_, int x_, int y_, int w_, int h_,  int BMP_id) {
	clean();
	type = type_;
	x = x_;
	y = y_;
	w = w_;
	h = h_;
}

GUI_Element::~GUI_Element() {

}


void GUI_Element::clean() {
	type = 0;
	x = 0;
	y = 0;
	w = 0; 
	h = 0;
	color = 0;
	is_passive = true;
}

void GUI_Element::Paint(GUI_low_level *low_level) {
	low_level->rectangle(x, y, w, h, color);
}
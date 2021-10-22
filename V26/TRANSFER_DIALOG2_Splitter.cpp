#include "stdafx.h"

#include "GUI_low_level.h"
#include "TRANSFER_DIALOG2.h"
#include "TRANSFER_DIALOG2_Splitter.h"

TRANSFER_DIALOG2_Splitter::TRANSFER_DIALOG2_Splitter(TRANSFER_DIALOG2 *parent_) {
	
	parent = parent_;
}
TRANSFER_DIALOG2_Splitter::~TRANSFER_DIALOG2_Splitter() {

}
void TRANSFER_DIALOG2_Splitter::set_size(int x_, int y_, int w_, int h_) {
	x = x_;
	y = y_;
	w = w_;
	h = h_;
	if (w < 0) w = 0;
	if (h < 0) h = 0;
}

void TRANSFER_DIALOG2_Splitter::Paint(GUI_low_level *low_level) {
	//low_level->fill_rectangle(x + parent->get_global_x(), y + parent->get_global_y(), w, h, TrDialog_Splitter_bgColor, TrDialog_Splitter_bgColor_alpha);
}

bool TRANSFER_DIALOG2_Splitter::its_me(int local_mx, int local_my) {
	if (local_mx >= x && local_mx < x + w && local_my >= y && local_my < y + h) {
		return true;
	}
	return false;
}
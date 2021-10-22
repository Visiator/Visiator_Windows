#include "stdafx.h"

#include "TRANSFER_DIALOG2_DirsFiles_Label.h"
#include "GUI_low_level.h"
#include "MODAL_DIALOG.h"
#include "FONT.h"
#include "tools.h"


extern FONT *font[10];
//extern FONT *fontAAA[10];


TRANSFER_DIALOG2_DirsFiles_Label::TRANSFER_DIALOG2_DirsFiles_Label(MODAL_DIALOG *parent_) {
	
	parent = parent_;
	//font = low_level->font[4];
	is_visible = true;
}

TRANSFER_DIALOG2_DirsFiles_Label::~TRANSFER_DIALOG2_DirsFiles_Label() {

}

void TRANSFER_DIALOG2_DirsFiles_Label::set_size(int x_, int y_, int w_, int h_) {
	x = x_;
	y = y_;
	w = w_;
	h = h_;
	if (w < 0) w = 0;
	if (h < 0) h = 0;
}

void TRANSFER_DIALOG2_DirsFiles_Label::Paint(GUI_low_level *low_level) {

	if (is_visible == false) return;

	//low_level->fill_rectangle(x + parent->get_global_x(), y + parent->get_global_y(), w, h, 0xff, 0x7f);
	if (label != nullptr) {
		// 2021 
		low_level->paint_text_rect(font_idx, x + parent->get_global_x(), y + parent->get_global_y(), w, h, label, label_color);
	}
}

void TRANSFER_DIALOG2_DirsFiles_Label::Paint_right(GUI_low_level *low_level) {

	if (is_visible == false) return;

	//low_level->fill_rectangle(x + parent->get_global_x(), y + parent->get_global_y(), w, h, 0xff, 0xaf);
	if (label != nullptr) {
		int ww, px = 0, py = 0;
		ww = low_level->text_width(font_idx, label);
		if (parent != nullptr) {
			px = parent->get_global_x();
			py = parent->get_global_y();
		}
		// 2021 font->paint_text_rect(x + parent->get_global_x() + w - ww, y + parent->get_global_y(), w, h, label, label_color);
		low_level->paint_text_rect(font_idx, x + px + w - ww , y + py, w, h, label, 0);
	}
}

void TRANSFER_DIALOG2_DirsFiles_Label::Paint_center(GUI_low_level *low_level) {

	if (is_visible == false) return;

	//low_level->fill_rectangle(x + parent->get_global_x(), y + parent->get_global_y(), w, h, 0xff, 0xcf);
	if (label != nullptr) {
		int ww, px = 0, py = 0;
		ww = low_level->text_width(font_idx, label);
		if (parent != nullptr) {
			px = parent->get_global_x();
			py = parent->get_global_y();
		}
		// 2021 
		// font->paint_text_rect(x + parent->get_global_x() + w/2 - ww/2, y + parent->get_global_y(), w, h, label, label_color);
		low_level->paint_text_rect(font_idx, x + px + w/2 - ww/2, y + py, w, h, label, label_color);
	}
}


void TRANSFER_DIALOG2_DirsFiles_Label::lock() {
	do
	{
		if (is_locked == false) {
			enter_crit(51);
			if (is_locked == false) {
				is_locked = true;
				leave_crit(51);
				return;
			}
			leave_crit(51);
		}
		else {
			::Sleep(0);
		}
	} while (true);
}

void TRANSFER_DIALOG2_DirsFiles_Label::unlock() {
	if (is_locked == true) {
		is_locked = false;
	}
}


void TRANSFER_DIALOG2_DirsFiles_Label::set_label(GUI_low_level *low_level, wchar_t *val) {
	lock();

	if (label_len < my_strlen(val)) {
		if (label != nullptr) delete[] label;
		label_len = my_strlen(val);
		label = new wchar_t[label_len + 1];
	}
	
	my_strcpy(label, val);

	unlock();

	if(low_level != nullptr) low_level->invalidate();
}

int TRANSFER_DIALOG2_DirsFiles_Label::text_width(GUI_low_level *low_level) {
	if (low_level == nullptr) return 0;
	if (font_idx < 0 || font_idx >= 10) return 0;
	if (font[font_idx] == nullptr) return 0;
	return font[font_idx]->text_width(label);
}

bool TRANSFER_DIALOG2_DirsFiles_Label::its_me(int m_x, int m_y) {
	if (is_visible == false) return false;

	if (m_x >= x && m_x < x + w && m_y >= y && m_y < y + h) {
		return true;
	}
	return false;
}
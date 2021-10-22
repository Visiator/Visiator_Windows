#include "stdafx.h"

#include "GUI_low_level.h"
#include "TRANSFER_DIALOG2_DirsFiles_Button.h"
#include "TRANSFER_DIALOG2_DirsFiles_Toolbar.h"
#include "MODAL_DIALOG.h"
#include "tools.h"

TRANSFER_DIALOG2_DirsFiles_Checkbox::TRANSFER_DIALOG2_DirsFiles_Checkbox(MODAL_DIALOG *parent_, wchar_t *label_) {
	
	parent_modal_dialog = parent_;
	label = new wchar_t[my_strlen(label_) + 1];
	my_strcpy(label, label_);
}

TRANSFER_DIALOG2_DirsFiles_Checkbox::~TRANSFER_DIALOG2_DirsFiles_Checkbox() {

}

void TRANSFER_DIALOG2_DirsFiles_Checkbox::mouse_left_button_down(GUI_low_level *low_level, int mx, int my) {
	if (its_me(mx, my)) {
		set_is_pressed(low_level, !is_pressed);
	}
}

void TRANSFER_DIALOG2_DirsFiles_Checkbox::mouse_left_button_up(GUI_low_level *low_level) {
	if (is_pressed) {
		if (func_button_pressed != nullptr) func_button_pressed(0);
	}
	set_is_pressed(low_level, false);
}

bool TRANSFER_DIALOG2_DirsFiles_Checkbox::its_me(int m_x, int m_y) {


	if (m_x >= x && m_x < x + w && m_y >= y && m_y < y + h) {
		return true;
	}
	return false;
}


void TRANSFER_DIALOG2_DirsFiles_Checkbox::set_is_pressed(GUI_low_level *low_level, bool val) {
	if (is_pressed == false && val == true) {
		is_pressed = true;
		low_level->invalidate();
	}
	if (is_pressed == true && val == false) {
		is_pressed = false;
		low_level->invalidate();
	}
}

void TRANSFER_DIALOG2_DirsFiles_Checkbox::Paint(GUI_low_level *low_level) {
	//lock();
	int ww, parent_x = 0, parent_y = 0;

	unsigned int col;

	if (is_pressed == true) {
		col = 0x999999;
	}
	else {
		col = 0xbbbbbb;
	};

	
	if (parent_modal_dialog != nullptr) {
		parent_x = parent_modal_dialog->get_global_x();
		parent_y = parent_modal_dialog->get_global_y();
	};

	//low_level->fill_rectangle(x + parent_x, y + parent_y, w, h, col, 255);

	unsigned int color_border = 0x777777, color_g = 0;
	ww = 13;

	low_level->line_h(x+ parent_x, y+ parent_y, ww, color_border);
	low_level->line_h(x+ parent_x, y+ parent_y+ww-1, ww, color_border);

	low_level->line_v(x + parent_x, y + parent_y, ww, color_border);
	low_level->line_v(x + parent_x + ww-1, y + parent_y, ww, color_border);

	if (is_pressed) {

		for (int i = 0; i < 4; i++) {
			low_level->set_pixx(x + parent_x + 2 + i, y + parent_y + 5 + i, color_g);
			low_level->set_pixx(x + parent_x + 2 + i, y + parent_y + 6 + i, color_g);
			low_level->set_pixx(x + parent_x + 2 + i, y + parent_y + 7 + i, color_g);
		}
		for (int i = 0; i < 5; i++) {
			low_level->set_pixx(x + parent_x + 6 + i, y + parent_y + 7 - i, color_g);
			low_level->set_pixx(x + parent_x + 6 + i, y + parent_y + 8 - i, color_g);
			low_level->set_pixx(x + parent_x + 6 + i, y + parent_y + 9 - i, color_g);
		}
	};
	//ww = low_level->font[4]->text_width(label);
	//hh = low_level->font[4]->text_height();

	// low_level->font[4]->paint_text(x + parent_x + ww + ww/2, y + parent_y - 1, label, 0x777777);

	low_level->paint_text(4, x + parent_x + 18, y + parent_y - 1, w, h, label, 0x0, 0, -1);

	//unlock();
}

void TRANSFER_DIALOG2_DirsFiles_Checkbox::set_size(int x_, int y_, int w_, int h_) {
	x = x_;
	y = y_;
	w = w_;
	h = h_;
	if (w < 0) w = 0;
	if (h < 0) h = 0;
}

//**********************************************************************************************************************
//**********************************************************************************************************************

TRANSFER_DIALOG2_DirsFiles_Button::TRANSFER_DIALOG2_DirsFiles_Button(int type_, TRANSFER_DIALOG2_DirsFiles_Toolbar *parent_, wchar_t *label_) {
	
	parent_tulbar = parent_;
	type = type_;
	label = new wchar_t[ my_strlen(label_)+1 ];
	my_strcpy(label, label_);
	is_visible = true;
}

TRANSFER_DIALOG2_DirsFiles_Button::TRANSFER_DIALOG2_DirsFiles_Button(int type_, MODAL_DIALOG *parent_, wchar_t *label_) {
	
	parent_modal_dialog = parent_;
	type = type_;
	label = new wchar_t[my_strlen(label_) + 1];
	my_strcpy(label, label_);
	is_visible = true;
}


TRANSFER_DIALOG2_DirsFiles_Button::~TRANSFER_DIALOG2_DirsFiles_Button() {

}

void TRANSFER_DIALOG2_DirsFiles_Button::Paint(GUI_low_level *low_level) {
	if (is_visible == false) return;
	lock();
	int ww, hh, parent_x = 0, parent_y = 0;

	

	unsigned int color_bg, color_font, color_border;

	if (is_pressed == true) {
		color_bg = TrDialog_Button_pressed;
		color_font = TrDialog_Button_font_pressed;
		color_border = TrDialog_Button_border_pressed;
	}else{
		color_bg = TrDialog_Button_normal;
		color_font = TrDialog_Button_font_normal;
		color_border = TrDialog_Button_border_normal;
	};



	if (parent_tulbar != nullptr) {
		parent_x = parent_tulbar->get_global_x();
		parent_y = parent_tulbar->get_global_y();
	};
	if (parent_modal_dialog != nullptr) {
		parent_x = parent_modal_dialog->get_global_x();
		parent_y = parent_modal_dialog->get_global_y();
	};

	low_level->fill_rectangle(x + parent_x, y + parent_y, w, h, color_bg, 0xff);

	low_level->line_h(x + parent_x+1, y + parent_y, w-1, color_border);
	low_level->line_h(x + parent_x+1, y + parent_y + h, w-1, color_border);
	low_level->line_v(x + parent_x, y + parent_y+1, h-1, color_border);
	low_level->line_v(x + parent_x + w, y + parent_y+1, h-1, color_border);


	ww = low_level->text_width(font_idx, label);
	hh = low_level->text_height(font_idx);

	// 2021 
	// low_level->paint_text( 4, x + parent_x + w/2-ww/2, y + parent_y + h/2 - hh/2-1, 200, 20, label, color_font, 0, -1);

	low_level->paint_text(4, x + parent_x, y + parent_y, w, h, label, color_font, 1, -1);

	unlock();
}

void TRANSFER_DIALOG2_DirsFiles_Button::set_size(int x_, int y_, int w_, int h_) {
	x = x_;
	y = y_;
	w = w_;
	h = h_;
	if (w < 0) w = 0;
	if (h < 0) h = 0;
}

void TRANSFER_DIALOG2_DirsFiles_Button::lock() {

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

void TRANSFER_DIALOG2_DirsFiles_Button::unlock() {
	if (is_locked == true) {
		is_locked = false;
	}
}

void TRANSFER_DIALOG2_DirsFiles_Button::mouse_left_button_down(GUI_low_level *low_level, int mx, int my) {
	if (is_visible == false) return;
	if (its_me(mx, my)) {
		set_is_pressed(low_level, true);
	}
}

void TRANSFER_DIALOG2_DirsFiles_Button::mouse_left_button_up(GUI_low_level *low_level) {
	if (is_pressed) {
		if (func_button_pressed != nullptr) func_button_pressed(low_level, type);
	}
	set_is_pressed(low_level, false);
}

bool TRANSFER_DIALOG2_DirsFiles_Button::its_me(int m_x, int m_y) {
	if (is_visible == false) return false;

	if (m_x >= x && m_x < x + w && m_y >= y && m_y < y + h) {
		return true;
	}
	return false;
}


void TRANSFER_DIALOG2_DirsFiles_Button::set_is_pressed(GUI_low_level *low_level, bool val) {
	if (is_pressed == false && val == true) {
		is_pressed = true;
		low_level->invalidate();
	}
	if (is_pressed == true && val == false) {
		is_pressed = false;
		low_level->invalidate();
	}
}

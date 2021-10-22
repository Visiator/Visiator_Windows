#include "stdafx.h"

#include "GUI_low_level.h"
#include "TRANSFER_DIALOG2_DirsFiles.h"
#include "TRANSFER_DIALOG2_DirsFiles_Toolbar.h"
#include "TRANSFER_DIALOG2_DirsFiles_Button.h"
#include "tools.h"

void button_pressed_DirFilesTree_local_copy(GUI_low_level *low_level, int type);
void button_pressed_DirFilesTree_dest_copy(GUI_low_level *low_level, int type);

void button_pressed_DirFilesTree_local_refresh(GUI_low_level *low_level, int type);
void button_pressed_DirFilesTree_dest_refresh(GUI_low_level *low_level, int type);

void button_pressed_DirFilesTree_local_delete_files(GUI_low_level *low_level, int type);
void button_pressed_DirFilesTree_dest_delete_files(GUI_low_level *low_level, int type);

TRANSFER_DIALOG2_DirsFiles_Toolbar::TRANSFER_DIALOG2_DirsFiles_Toolbar(int local_or_dest_, TRANSFER_DIALOG2_DirsFiles *parent_) {
	
	parent = parent_;
	type = local_or_dest_;
	ss_[0] = 0;
	ss2_[0] = 0;

	if (local_or_dest_ == 1) {
		btn_copy = new TRANSFER_DIALOG2_DirsFiles_Button(local_or_dest_ + 100, this, L"copy ->");
		btn_copy->func_button_pressed = &button_pressed_DirFilesTree_local_copy;

		btn_refresh = new TRANSFER_DIALOG2_DirsFiles_Button(local_or_dest_ + 400, this, L"refresh");
		btn_refresh->func_button_pressed = &button_pressed_DirFilesTree_local_refresh;

		btn_delete = new TRANSFER_DIALOG2_DirsFiles_Button(local_or_dest_ + 400, this, L"delete");
		btn_delete->func_button_pressed = &button_pressed_DirFilesTree_local_delete_files;
	}
	else {
		btn_copy = new TRANSFER_DIALOG2_DirsFiles_Button(local_or_dest_ + 100, this, L"<- copy");
		btn_copy->func_button_pressed = &button_pressed_DirFilesTree_dest_copy;

		btn_refresh = new TRANSFER_DIALOG2_DirsFiles_Button(local_or_dest_ + 400, this, L"refresh");
		btn_refresh->func_button_pressed = &button_pressed_DirFilesTree_dest_refresh;

		btn_delete = new TRANSFER_DIALOG2_DirsFiles_Button(local_or_dest_ + 400, this, L"delete");
		btn_delete->func_button_pressed = &button_pressed_DirFilesTree_dest_delete_files;
	}
	//btn_new_folder = new TRANSFER_DIALOG2_DirsFiles_Button(local_or_dest_ + 200, low_level, this, L"new folder");
	
	TRANSFER_DIALOG2_DirsFiles_Button *q;

	q = btn_copy;

	for (int i = 0; i < 3; i++) {

		if (i == 0) q = btn_copy;
		if (i == 1) q = btn_refresh;
		if (i == 2) q = btn_delete;

		/*
		q->Color_BG_normal = 0xffffff;
		q->Color_border_normal = 0xffffff;
		q->Color_font_normal = 0x777777;
		
		q->Color_BG_pressed = 0xdddddd;		
		q->Color_border_pressed = 0xbbbbbb;
		q->Color_font_pressed = 0;// 0xff00ff;
		*/
		

	};

}
TRANSFER_DIALOG2_DirsFiles_Toolbar::~TRANSFER_DIALOG2_DirsFiles_Toolbar() {

}

void TRANSFER_DIALOG2_DirsFiles_Toolbar::set_size(int x_, int y_, int w_, int h_) {
	
	int btn_ww = 55, btn_marg = 10;
	int xx;
	x = x_;
	y = y_;
	w = w_;
	h = h_;
	if (w < 0) w = 0;
	if (h < 0) h = 0;

	if (type == 1) {

		xx = x + w - 60;

		btn_copy->set_size(xx, y + 6, btn_ww, 20);
		xx -= (btn_ww + btn_marg);
		btn_refresh->set_size(xx, y + 6, btn_ww, 20);
		xx -= (btn_ww + btn_marg);
		btn_delete->set_size(xx, y + 6, btn_ww, 20);
	}
	else {

		xx = x + 5;

		btn_copy->set_size(xx, y + 6, btn_ww, 20);

		xx += (btn_ww + btn_marg);
		btn_refresh->set_size(xx, y + 6, btn_ww, 20);

		xx += (btn_ww + btn_marg);
		btn_delete->set_size(xx, y + 6, btn_ww, 20);
	}

}

void TRANSFER_DIALOG2_DirsFiles_Toolbar::Paint(GUI_low_level *low_level) {

	lock();

	low_level->fill_rectangle( x + parent->get_global_x(), y + parent->get_global_y(), w, h, TrDialog_Toolbar_bgColor, TrDialog_Toolbar_bgColor_alpha);
	//low_level->font[4]->paint_text2(x + parent->get_global_x() + 5, y + parent->get_global_y() + 0, ss_, 0xffffff);
	//low_level->font[4]->paint_text2(x + parent->get_global_x() + 5, y + parent->get_global_y() + 17, ss2_, 0xffffff);

	btn_copy->Paint(low_level);
	btn_refresh->Paint(low_level);
	btn_delete->Paint(low_level);

	unlock();
}

bool TRANSFER_DIALOG2_DirsFiles_Toolbar::its_me(int m_x, int m_y) {


	if (m_x >= x && m_x < x + w && m_y >= y && m_y < y + h) {
		return true;
	}
	return false;
}

void TRANSFER_DIALOG2_DirsFiles_Toolbar::mouse_left_button_down(GUI_low_level *low_level, int mx, int my) {
	if (btn_copy->its_me(mx, my)) {
		btn_copy->mouse_left_button_down(low_level, mx, my);
	}
	if (btn_refresh->its_me(mx, my)) {
		btn_refresh->mouse_left_button_down(low_level, mx, my);
	}
	if (btn_delete->its_me(mx, my)) {
		btn_delete->mouse_left_button_down(low_level, mx, my);
	}
}

void TRANSFER_DIALOG2_DirsFiles_Toolbar::mouse_left_button_up(GUI_low_level *low_level) {
	btn_copy->mouse_left_button_up(low_level);
	//btn_new_folder->mouse_left_button_up();
	btn_delete->mouse_left_button_up(low_level);
	btn_refresh->mouse_left_button_up(low_level);
	

}

void TRANSFER_DIALOG2_DirsFiles_Toolbar::lock() {

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

void TRANSFER_DIALOG2_DirsFiles_Toolbar::unlock() {
	if (is_locked == true) {
		is_locked = false;
	}
}


int TRANSFER_DIALOG2_DirsFiles_Toolbar::get_global_x() {
	return x + parent->get_global_x();
}
int TRANSFER_DIALOG2_DirsFiles_Toolbar::get_global_y() {
	return y + parent->get_global_y();
}

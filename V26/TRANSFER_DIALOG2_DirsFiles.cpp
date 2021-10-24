#include "stdafx.h"

#include "TRANSFER_DIALOG2_DirsFiles.h"
#include "TRANSFER_DIALOG2_DirsFiles_Toolbar.h"
#include "TRANSFER_DIALOG2_DirsFiles_VScroll.h"
#include "TRANSFER_DIALOG2_DirsFiles_TREE.h"
#include "VIEWER.h"

extern VIEWER *viewer;

void button_pressed_DirFilesTree_local_copy(GUI_low_level *low_level, int type);
void button_pressed_DirFilesTree_dest_copy(GUI_low_level *low_level, int type);
void button_pressed_DirFilesTree_local_refresh(GUI_low_level *low_level, int type);
void button_pressed_DirFilesTree_dest_refresh(GUI_low_level *low_level, int type);

class TRANSFER_DIALOG2;

TRANSFER_DIALOG2_DirsFiles::TRANSFER_DIALOG2_DirsFiles(int local_or_dest_, TRANSFER_DIALOG2 *parent_)
{
	parent = parent_;

	
	Toolbar = new TRANSFER_DIALOG2_DirsFiles_Toolbar(local_or_dest_, this);
	VScroll = new TRANSFER_DIALOG2_DirsFiles_VScroll(this);
	Tree = new TRANSFER_DIALOG2_DirsFiles_TREE(this);
	
}

TRANSFER_DIALOG2_DirsFiles::~TRANSFER_DIALOG2_DirsFiles() {

}

void TRANSFER_DIALOG2_DirsFiles::set_size(GUI_low_level *low_level, int x_, int y_, int w_, int h_) {
	x = x_;
	y = y_;
	w = w_;
	h = h_;
	if (w < 0) w = 0;
	if (h < 0) h = 0;

	
	Toolbar->set_size(0, 0, w, _Toolbar_h);

	VScroll->set_size(w_ - _V_Scroll_w, _Toolbar_h, _V_Scroll_w, h - _Toolbar_h);

	Tree->set_size(low_level, 0, _Toolbar_h, w_ - _V_Scroll_w, h - _Toolbar_h);
	
}

void TRANSFER_DIALOG2_DirsFiles::START(GUI_low_level *low_level, int LOCAL_or_PARTNER_files) {
	Tree->START(low_level, LOCAL_or_PARTNER_files);
}

bool TRANSFER_DIALOG2_DirsFiles::its_me(int local_mx, int local_my) {

	int local_local_mx, local_local_my;

	local_local_mx = local_mx - x;
	local_local_my = local_my - y;

	if (local_local_mx >= 0 && local_local_mx < w && local_local_my >= 0 && local_local_my < h) {
		return true;
	}
	return false;
}

int TRANSFER_DIALOG2_DirsFiles::get_global_x() { return x + parent->get_global_x(); };
int TRANSFER_DIALOG2_DirsFiles::get_global_y() { return y + parent->get_global_y(); };
int TRANSFER_DIALOG2_DirsFiles::get_global_w() { return w + parent->get_global_w(); };
int TRANSFER_DIALOG2_DirsFiles::get_global_h() { return h + parent->get_global_h(); };

void button_pressed_DirFilesTree_local_copy(GUI_low_level *low_level, int type) {
	viewer->file_transfer_dialog->START_TRANSFER(viewer->gui->low_level, TRANSFER_from_MY_to_PARTNER);
}
void button_pressed_DirFilesTree_dest_copy(GUI_low_level *low_level, int type) {
	viewer->file_transfer_dialog->START_TRANSFER(viewer->gui->low_level, TRANSFER_from_PARTNER_to_MY);
}

void button_pressed_DirFilesTree_local_refresh(GUI_low_level *low_level, int type) {
	viewer->file_transfer_dialog->REFRESH(viewer->gui->low_level, 1);
}

void button_pressed_DirFilesTree_dest_refresh(GUI_low_level *low_level, int type) {
	viewer->file_transfer_dialog->REFRESH(viewer->gui->low_level, 2);
}

void button_pressed_DirFilesTree_local_delete_files(GUI_low_level *low_level, int type) {
	viewer->file_transfer_dialog->START_DELETE(viewer->gui->low_level, 1);
}
void button_pressed_DirFilesTree_dest_delete_files(GUI_low_level *low_level, int type) {
	viewer->file_transfer_dialog->START_DELETE(viewer->gui->low_level, 2);
}

void TRANSFER_DIALOG2_DirsFiles::refresh(GUI_low_level *low_level) {

	

	if (Tree->current_active_element == nullptr) return;
	if (Tree->need_ASYNC_load != nullptr) return;

	TRANSFER_DIALOG2_DirsFiles_TREE_element *e = nullptr;

	e = Tree->current_active_element;
	if (e->is_folder == true) {
		if (e->is_open == false) {
			if (e->parent != nullptr && e->parent->parent != nullptr) {

				zero_wchar_t(Tree->need_ASYNC_load_name, 5100);
				e->get_folder_full_name(Tree->need_ASYNC_load_name);

				e = e->parent;
			};
		}
	}
	else {
		if (e->parent != nullptr) e = e->parent;
	}

	if (e != nullptr) {

		Tree->need_CLOSE(low_level, e);

		Tree->need_LOAD_fileslist(low_level, e);
	};
	Tree->set_current_active_element(nullptr);
	Tree->top->UNSELECT_all_elements();
	
}

void TRANSFER_DIALOG2_DirsFiles::recalc_v_scroll_sizes(GUI_low_level *low_level) {

}

void TRANSFER_DIALOG2_DirsFiles::Paint(GUI_low_level *low_level) {
	int xx, yy;
	xx = x + parent->get_global_x();
	yy = y + parent->get_global_y();
	low_level->fill_rectangle(xx, yy + _Toolbar_h, w - _V_Scroll_w, h - _Toolbar_h, 0xffffff, 220);

	Tree->Paint(low_level);

	Toolbar->Paint(low_level);
	VScroll->Paint(low_level);
}

void TRANSFER_DIALOG2_DirsFiles::mouse_wheel(GUI_low_level *low_level, int type) {
	if (type == 2) for (int i = 0; i < 3; i++) Tree->top_arrow_click(low_level);
	if (type == 1) for (int i = 0; i < 3; i++) Tree->bottom_arrow_click(low_level);
}

void TRANSFER_DIALOG2_DirsFiles::char_keydown(GUI_low_level *low_level, int msg, int wp, int lp) {
	Tree->char_keydown(low_level, msg, wp, lp);
}

void TRANSFER_DIALOG2_DirsFiles::mouse_left_button_up(GUI_low_level *low_level) {
	Toolbar->mouse_left_button_up(low_level);
	VScroll->mouse_left_button_up(low_level);
	Tree->mouse_left_button_up(low_level);
}

void TRANSFER_DIALOG2_DirsFiles::mouse_left_button_down(GUI_low_level *low_level, int local_mx, int local_my) {
	int local_local_mx, local_local_my;

	local_local_mx = local_mx - x;
	local_local_my = local_my - y;

	if (Toolbar->its_me(local_local_mx, local_local_my)) {
		Toolbar->mouse_left_button_down(low_level, local_local_mx, local_local_my);
	}
	if (VScroll->its_me(local_local_mx, local_local_my)) {
		VScroll->mouse_left_button_down(low_level, local_local_mx, local_local_my);
	}
	if (Tree->its_me(local_local_mx, local_local_my)) {
		Tree->mouse_left_button_down(low_level, local_local_mx, local_local_my);
	}
}

void TRANSFER_DIALOG2_DirsFiles::mouse_over(GUI_low_level *low_level, int mouse_x, int mouse_y) {
	if (VScroll->polzunok_is_pressed) {
		VScroll->mouse_over(low_level, mouse_x, mouse_y);
	}

	if (Tree->its_me(mouse_x - x, mouse_y - y) || 1 == 1) {
		Tree->mouse_over(low_level, mouse_x - x, mouse_y - y);

	}

}

void TRANSFER_DIALOG2_DirsFiles::ASYNC(GUI_low_level *low_level) {

	if (VScroll->top_arrow_is_pressed_tick > 0 && VScroll->top_arrow_is_pressed_tick < GetTickCount()) {
		VScroll->top_arrow_is_pressed_tick = GetTickCount() + 90;
		Tree->top_arrow_click(low_level);
	}
	if (VScroll->bottom_arrow_is_pressed_tick > 0 && VScroll->bottom_arrow_is_pressed_tick < GetTickCount()) {
		VScroll->bottom_arrow_is_pressed_tick = GetTickCount() + 90;
		Tree->bottom_arrow_click(low_level);
	}
	if (VScroll->PgUp_polzunok_is_pressed_tick > 0 && VScroll->PgUp_polzunok_is_pressed_tick < GetTickCount()) {
		VScroll->PgUp_polzunok_is_pressed_tick = GetTickCount() + 90;
		Tree->PgUp_polzunok_click(low_level);
	}
	if (VScroll->PgDown_polzunok_is_pressed_tick > 0 && VScroll->PgDown_polzunok_is_pressed_tick < GetTickCount()) {
		VScroll->PgDown_polzunok_is_pressed_tick = GetTickCount() + 90;
		Tree->PgDown_polzunok_click(low_level);
	}
	Tree->ASYNC(low_level);
}

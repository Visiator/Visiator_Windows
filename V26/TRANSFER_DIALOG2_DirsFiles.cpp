#include "stdafx.h"
#include "TRANSFER_DIALOG2_DirsFiles.h"
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

	/* 2021 10
	Toolbar = new TRANSFER_DIALOG2_DirsFiles_Toolbar(local_or_dest_, this);
	VScroll = new TRANSFER_DIALOG2_DirsFiles_VScroll(this);
	Tree = new TRANSFER_DIALOG2_DirsFiles_TREE(this);
	*/
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

	/* 2021 10
	Toolbar->set_size(0, 0, w, _Toolbar_h);

	VScroll->set_size(w_ - _V_Scroll_w, _Toolbar_h, _V_Scroll_w, h - _Toolbar_h);

	Tree->set_size(low_level, 0, _Toolbar_h, w_ - _V_Scroll_w, h - _Toolbar_h);
	*/
}

void TRANSFER_DIALOG2_DirsFiles::START(GUI_low_level *low_level, int LOCAL_or_PARTNER_files) {
	// 2021 10 Tree->START(low_level, LOCAL_or_PARTNER_files);
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

	/* 2021 10

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
	*/
}

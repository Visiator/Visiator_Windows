#include "stdafx.h"

#include "TRANSFER_DIALOG2.h"
#include "VIEWER.h"
#include "TRANSFER_DIALOG2_DirsFiles.h"
#include "TRANSFER_DIALOG2_DirsFiles_Label.h"
#include "TRANSFER_DIALOG2_Splitter.h"
#include "TRANSFER_DIALOG2_DirsFiles_TREE.h"
#include "TRANSFER_DIALOG2_transfer_from_PARTNER_to_MY.h"
#include "MODAL_DIALOG.h"

extern bool GLOBAL_STOP;
extern VIEWER *viewer;

#define BORDER_TOP 1
#define BORDER_LEFT 2
#define BORDER_RIGHT 3
#define BORDER_BOTTOM 4

#define BORDER_TOPLEFT 5
#define BORDER_TOPRIGHT 6
#define BORDER_BOTTOMLEFT 7
#define BORDER_BOTTOMRIGHT 8

#define BORDER_SPLITTER 9


TRANSFER_DIALOG2::TRANSFER_DIALOG2(VIEWER *viewer_) {
	//low_level = low_level_;
	viewer = viewer_;
	Caption = new TRANSFER_DIALOG2_Caption(this);
	Splitter = new TRANSFER_DIALOG2_Splitter(this);
	Local_DirsFiles = new TRANSFER_DIALOG2_DirsFiles(1, this);
	Dest_DirsFiles = new TRANSFER_DIALOG2_DirsFiles(2, this);

	modal_dialog_progress = new MODAL_DIALOG(this, MD_TRANSFER);
	modal_dialog_progress->set_visible(nullptr, false);

	modal_dialog_confirm_overwrite = new MODAL_DIALOG(this, MD_OVERWRITE);
	modal_dialog_confirm_overwrite->col_back = 0xeeeeff;
	modal_dialog_confirm_overwrite->col_border = 0xFDCA46;
	modal_dialog_confirm_overwrite->set_visible(nullptr, false);

	modal_dialog_skip = new MODAL_DIALOG(this, MD_SKIP);
	modal_dialog_skip->col_back = 0xeeeeff;
	modal_dialog_skip->col_border = 0xFDCA46;
	modal_dialog_skip->set_visible(nullptr, false);

	modal_dialog_info = new MODAL_DIALOG(this, MD_INFO);
	modal_dialog_info->col_back = 0xeeeeff;
	modal_dialog_info->col_border = 0xFDCA46;
	modal_dialog_info->set_visible(nullptr, false);

}

TRANSFER_DIALOG2::~TRANSFER_DIALOG2() {

}

void TRANSFER_DIALOG2::set_visible(GUI_low_level *low_level, bool val) {

	if (is_visible == val) return;

	if (val == true) {
		x = low_level->window_w / 2 - w / 2;
		y = (low_level->window_h - h) / 3;
		if (x < 0) x = 0;
		if (y < 0) y = 0;

		Local_DirsFiles->START(low_level, LOCAL_FILES);

		Dest_DirsFiles->START(low_level, PARTNER_FILES);
		is_visible = true;
	}
	else {
		
		if (modal_dialog_info->get_visible() == true) {
			modal_dialog_info->modal__result = MR_CANCEL;
			modal_dialog_info->set_visible(low_level, false);
		}
		if (modal_dialog_confirm_overwrite->get_visible() == true) {
			modal_dialog_confirm_overwrite->modal__result = MR_CANCEL;
			modal_dialog_confirm_overwrite->set_visible(low_level, false);
		}
		if (modal_dialog_progress->get_visible() == true) {
			modal_dialog_progress->modal__result = MR_CANCEL;
			modal_dialog_progress->set_visible(low_level, false);
		}
		if (modal_dialog_skip->get_visible() == true) {
			modal_dialog_skip->modal__result = MR_CANCEL;
			modal_dialog_skip->set_visible(low_level, false);
		}
		
		is_visible = false;
	}

}

void TRANSFER_DIALOG2::AutoArrangeSizes(GUI_low_level *low_level) {

	if (x > low_level->window_w - 100) x = low_level->window_w - 100;
	if (x < 0) x = 0;
	if (y > low_level->window_h - 100) y = low_level->window_h - 100;
	if (y < 0) y = 0;

	int panel_min_width = 450;

	if (x < 0) x = 0;
	if (y < 0) y = 0;

	if (w < panel_min_width * 2 + 40) w = panel_min_width * 2 + 40;
	if (h < 350) h = 350;

	

	if (Splitter_percent < 1) Splitter_percent = 1;
	if (Splitter_percent > 99) Splitter_percent = 99;

	if (Splitter_x == 0) {
		Splitter_percent = 50;

	}


	Splitter_x = (int)(((float)w - (float)_Border * (float)2) / (float)100 * Splitter_percent);
	if (Splitter_x < panel_min_width) Splitter_x = panel_min_width;
	if (Splitter_x > w - panel_min_width) Splitter_x = w - panel_min_width;

	Caption->set_size(0 + _Border, 0 + _Border, w - _Border * 2, _Caption_h);

	Splitter->set_size(_Border + Splitter_x - _Border / 2, _Caption_h + _Border, _Border, h - _Caption_h - _Border * 2);

	Local_DirsFiles->set_size(low_level, 0 + _Border, _Caption_h + _Border, Splitter_x - _Border / 2, h - _Caption_h - _Border * 2);
	Local_DirsFiles->recalc_v_scroll_sizes(low_level);

	Dest_DirsFiles->set_size(low_level, Splitter_x + _Border + _Border / 2, _Caption_h + _Border, (w - _Border * 2) - Splitter_x - _Border / 2, h - _Caption_h - _Border * 2);
	Dest_DirsFiles->recalc_v_scroll_sizes(low_level);


	modal_dialog_progress->recalc_size(low_level, w, h);
	modal_dialog_confirm_overwrite->recalc_size(low_level, w, h);
	modal_dialog_skip->recalc_size(low_level, w, h);
	modal_dialog_info->recalc_size(low_level, w, h);
	
}

void TRANSFER_DIALOG2::set_size(GUI_low_level *low_level, int x_, int y_, int w_, int h_) {
	if (x_ < 0 || y_ < 0) return;
	if (w_ < 350) return;
	if (h_ < 200) return;

	x = x_;
	y = y_;
	w = w_;
	h = h_;

	AutoArrangeSizes(low_level);

}

void TRANSFER_DIALOG2::Paint(GUI_low_level *low_level) {


	if (is_visible == false) {
		if (modal_dialog_info->get_visible() == true) {
			modal_dialog_info->modal__result = MR_CANCEL;
		}
	}

	low_level->fill_rectangle(x, y, w, h, TrDialog_Border_bgColor, TrDialog_Border_alpha);
	
	
	Caption->Paint(low_level);
	Local_DirsFiles->Paint(low_level);
	Dest_DirsFiles->Paint(low_level);
	Splitter->Paint(low_level);
	
	
	modal_dialog_progress->Paint(low_level);
	modal_dialog_confirm_overwrite->Paint(low_level);
	modal_dialog_skip->Paint(low_level);
	modal_dialog_info->Paint(low_level);
	

}

/*
int TRANSFER_DIALOG2_Caption::get_global_x() { return x + parent->get_global_x(); }
int TRANSFER_DIALOG2_Caption::get_global_y() { return y + parent->get_global_y(); }
int TRANSFER_DIALOG2_Caption::get_global_w() { return w + parent->get_global_w(); }
int TRANSFER_DIALOG2_Caption::get_global_h() { return h + parent->get_global_h(); }
*/

void TRANSFER_DIALOG2::START_TRANSFER(GUI_low_level *low_level, int mode) {

	

	TRANSFER_DIALOG2_DirsFiles *local_tree, *dest_tree;
	TRANSFER_DIALOG2_DirsFiles_TREE_element *dest_target_element, *local_target_element;

	if (mode == TRANSFER_from_PARTNER_to_MY) { // ѕередаем файлы от партнеру к нам

		OVERWRITE_ALL_FILES_mode = false;
		SKIP_ALL_FILES_mode = false;

		viewer->net_client_session->transfer_files_canceled = false;

		if (current_transfer_element_dest != nullptr) return; // еще не закончилось предыдущее копирование, а нас прос€ть уже новое

		local_tree = Local_DirsFiles;
		dest_tree = Dest_DirsFiles;
		if (local_target == nullptr) {
			local_target = new wchar_t[5100];
		}
		zero_wchar_t(local_target, 5100);

		current_transfer_element_dest = dest_tree->Tree->top->get_first_selected_element();
		if (current_transfer_element_dest == nullptr) {
			current_transfer_element_dest = dest_tree->Tree->current_active_element;
		}
		if (current_transfer_element_dest == nullptr) return; // откуда будем копировать

		local_target_element = local_tree->Tree->current_active_element; // куда будем копировать

		if (local_target_element == nullptr) { // TODO сделать оповещение что нужно выбрать папку куда копировать

			FINISH_TRANSFER_from_Partner_to_MY(low_level);



			modal_dialog_info->modal__result = 0;

			modal_dialog_info->label_info->set_label(low_level, L"Select target");
			modal_dialog_info->set_visible(low_level, true);



			return;
		}

		if (local_target_element != nullptr && local_target_element->is_folder == false && local_target_element->parent != nullptr) {
			local_target_element = local_target_element->parent;
		}

		if (local_target_element != nullptr && local_target_element->is_folder != true) return; // в качестве адреса "куда копировать" может быть только папка

		local_target_element->get_folder_full_name(local_target);

		//

		transfer_ASYNC_stage = 200;


		modal_dialog_progress->label_info->set_label(low_level, L"Copy to my");
		modal_dialog_progress->label1->set_label(low_level, L"Prepare...");
		modal_dialog_progress->label3->set_label(low_level, L".");
		modal_dialog_progress->set_visible(low_level, true);


	};

	if (mode == TRANSFER_from_MY_to_PARTNER) { // ѕередаем файлы от нас к партнеру



		OVERWRITE_ALL_FILES_mode = false;
		SKIP_ALL_FILES_mode = false;

		viewer->net_client_session->transfer_files_canceled = false;

		if (current_transfer_element_local != nullptr) return;



		local_tree = Local_DirsFiles;
		dest_tree = Dest_DirsFiles;
		if (dest_target == nullptr) {
			dest_target = new wchar_t[5100];
		}
		zero_wchar_t(dest_target, 5100);

		if (local_tree == nullptr || local_tree->Tree == nullptr && local_tree->Tree->top == nullptr) return; // пользователь ничего не выбрал дл€ передачи

		if (dest_tree != nullptr && dest_tree->Tree != nullptr && dest_tree->Tree->top != nullptr) {
			dest_target_element = dest_tree->Tree->current_active_element;// top->get_first_selected_element();
			if (dest_target_element != nullptr && dest_target_element->is_folder == false && dest_target_element->parent != nullptr) {
				dest_target_element = dest_target_element->parent;
			}
			if (dest_target_element != nullptr) {
				if (dest_target_element->is_folder) dest_target_element->get_folder_full_name(dest_target);
				else dest_target_element->get_folder_full_path(dest_target);

				my_str_del_from_left(dest_target, 18);
			}
		}

		current_transfer_element_local = local_tree->Tree->top->get_first_selected_element();
		if (current_transfer_element_local == nullptr) {
			current_transfer_element_local = local_tree->Tree->current_active_element;
		}
		if (current_transfer_element_local == nullptr) return;

		transfer_ASYNC_stage = 2;


		modal_dialog_progress->label_info->set_label(low_level, L"Copy");
		modal_dialog_progress->label1->set_label(low_level, L"Prepare...");
		modal_dialog_progress->label3->set_label(low_level, L".");
		//modal_dialog_progress->label1->set_label(current_transfer_element_local->get_name());
		modal_dialog_progress->set_visible(low_level, true);
	}

	
}

void TRANSFER_DIALOG2::FINISH_TRANSFER_from_Partner_to_MY(GUI_low_level *low_level) {

	

	viewer->net_client_session->need_send_TRANSFER_CANCELED = true;
	while (GLOBAL_STOP == false && viewer->net_client_session->need_send_TRANSFER_CANCELED == true) {
		::Sleep(111);
	}

	transfer_ASYNC_stage = 0;
	current_transfer_element_dest = nullptr;
	modal_dialog_progress->set_visible(low_level, false);

	viewer->file_transfer_dialog->Local_DirsFiles->refresh(low_level);
	
};

void TRANSFER_DIALOG2::FINISH_TRANSFER(GUI_low_level *low_level) {

	

	transfer_ASYNC_stage = 0;
	current_transfer_element_local = nullptr;
	modal_dialog_progress->set_visible(low_level, false);

	viewer->file_transfer_dialog->Dest_DirsFiles->refresh(low_level);

	
}

void TRANSFER_DIALOG2::FINISH_DELETE(GUI_low_level *low_level) {

	

	transfer_ASYNC_stage = 0;
	current_delete_element_dest = nullptr;
	modal_dialog_progress->set_visible(low_level, false);

	viewer->file_transfer_dialog->Dest_DirsFiles->refresh(low_level);

	

}

void TRANSFER_DIALOG2::REFRESH(GUI_low_level *low_level, int mode) {
	
	if (mode == 1) {
		Local_DirsFiles->refresh(low_level);
	}
	if (mode == 2) {
		Dest_DirsFiles->refresh(low_level);
	}
	
};

void TRANSFER_DIALOG2::START_DELETE(GUI_low_level *low_level, int mode) {

	

	viewer->net_client_session->transfer_files_canceled = false;

	TRANSFER_DIALOG2_DirsFiles *tree = nullptr;

	if (current_delete_element_dest != nullptr) return;
	if (mode != 2) return; // TODO удаление локальных файлов/папок

	if (mode == 2) {
		tree = Dest_DirsFiles;
		if (tree != nullptr && tree->Tree != nullptr && tree->Tree->top != nullptr) {
			current_delete_element_dest = tree->Tree->current_active_element;// top->get_first_selected_element();
		}

		transfer_ASYNC_stage = 20;


		modal_dialog_progress->label_info->set_label(low_level, L"Delete");
		modal_dialog_progress->label1->set_label(low_level, L"Prepare delete...");
		modal_dialog_progress->label3->set_label(low_level, L".");

		modal_dialog_progress->set_visible(low_level, true);

	};
	
};

bool TRANSFER_DIALOG2_Caption::its_me(int local_mx, int local_my) {
	if (local_mx >= x && local_mx < x + w && local_my >= y && local_my < y + h) {
		return true;
	}
	return false;
}

int TRANSFER_DIALOG2_Caption::get_global_x() {
	return x + parent->get_global_x();
}
int TRANSFER_DIALOG2_Caption::get_global_y() {
	return y + parent->get_global_y();
}
int TRANSFER_DIALOG2_Caption::get_global_w() {
	return w + parent->get_global_w();
}
int TRANSFER_DIALOG2_Caption::get_global_h() {
	return h + parent->get_global_h();
}

TRANSFER_DIALOG2_Caption::TRANSFER_DIALOG2_Caption(TRANSFER_DIALOG2 *parent_) {
	//low_level = low_level_;
	parent = parent_;
	Close_btn = new TRANSFER_DIALOG2_Close_btn(this);
}

TRANSFER_DIALOG2_Caption::~TRANSFER_DIALOG2_Caption() {

}

void TRANSFER_DIALOG2_Caption::set_size(int x_, int y_, int w_, int h_) {

	x = x_;
	y = y_;
	w = w_;
	h = h_;
	if (w < 0) w = 0;
	if (h < 0) h = 0;

	Close_btn->set_size(w - 20, 5, 20, 20);
}

void TRANSFER_DIALOG2_Caption::Paint(GUI_low_level *low_level) {

	//char ss[500];

	//sprintf__s_i(ss, 490, "WM=%d ", parent->_WORK_MODE_);

	//low_level->fill_rectangle(x+parent->get_global_x(), y+parent->get_global_y(), w, h, 0xff, 100);
	//low_level->font[4]->paint_text_rect(x + 5 + parent->get_global_x(), y + 5 + parent->get_global_y(), w, h, ss, 0);



	Close_btn->Paint(low_level);
	//low_level->paint_text(4, 50, 90, 500, 30, parent->Dest_DirsFiles->Toolbar->ss_, 0xff00ff, 0, -1);
}

TRANSFER_DIALOG2_Close_btn::TRANSFER_DIALOG2_Close_btn(TRANSFER_DIALOG2_Caption *parent_) {
	parent = parent_;
	//low_level = low_level_;
}

TRANSFER_DIALOG2_Close_btn::~TRANSFER_DIALOG2_Close_btn() {

}

bool TRANSFER_DIALOG2_Close_btn::its_me(int local_mx, int local_my) {
	if (local_mx >= x && local_mx < x + w && local_my >= y && local_my < y + h) {
		return true;
	}
	return false;
}

void TRANSFER_DIALOG2_Close_btn::Paint(GUI_low_level *low_level) {
	unsigned int clr;

	clr = 0x0;

	if (parent->parent->_WORK_MODE_ == _CloseBtn_Pressed_) {
		clr = 0xaaaaaa;
	}
	else {

	}

	//low_level->fill_rectangle(x + parent->get_global_x() , y + parent->get_global_y(), w, h, clr, 255);

	low_level->paint_close_krest(x + parent->get_global_x(), y + parent->get_global_y(), w - 6, clr);

}

void TRANSFER_DIALOG2_Close_btn::set_size(int x_, int y_, int w_, int h_) {

	x = x_;
	y = y_;
	w = w_;
	h = h_;
};

void TRANSFER_DIALOG2::char_keydown(GUI_low_level *low_level, int msg, int wp, int lp) {
	if (is_modal_lock()) return;
	if (current_ACTIVE_object == _Local_Dirs_) {
		Local_DirsFiles->char_keydown(low_level, msg, wp, lp);
	}
	if (current_ACTIVE_object == _Dest_Dirs_) {
		Dest_DirsFiles->char_keydown(low_level, msg, wp, lp);
	}
}

bool TRANSFER_DIALOG2::is_modal_lock() {
	if (modal_dialog_info->get_visible()) return true;
	if (modal_dialog_progress->get_visible()) return true;
	if (modal_dialog_skip->get_visible()) return true;
	if (modal_dialog_confirm_overwrite->get_visible()) return true;

	return false;
}

void TRANSFER_DIALOG2::mouse_left_button_up(GUI_low_level *low_level, int mx, int my) {
	int local_mx, local_my;
	local_mx = mx - x;
	local_my = my - y;

	modal_dialog_info->mouse_left_button_up(low_level);
	modal_dialog_skip->mouse_left_button_up(low_level);
	modal_dialog_confirm_overwrite->mouse_left_button_up(low_level);
	modal_dialog_progress->mouse_left_button_up(low_level);

	Local_DirsFiles->mouse_left_button_up(low_level);
	Dest_DirsFiles->mouse_left_button_up(low_level);

	if (_WORK_MODE_ == _MOVE_from_CAPTION_) {
		set_new_WORK_MODE(low_level, _NORMAL_);
		return;
	};
	if (_WORK_MODE_ == _CloseBtn_Pressed_) {
		set_new_WORK_MODE(low_level, _NORMAL_);
		if (Caption->Close_btn->its_me(local_mx, local_my)) {
			Close_btn_Click();
		};
		return;
	};

	if (_WORK_MODE_ == _MOVE_SPLITTER_) {
		set_new_WORK_MODE(low_level, _NORMAL_);
		return;
	};
	if (_WORK_MODE_ == _RESIZE_top_border_
		|| _WORK_MODE_ == _RESIZE_left_border_
		|| _WORK_MODE_ == _RESIZE_right_border_
		|| _WORK_MODE_ == _RESIZE_bottom_border_
		|| _WORK_MODE_ == _RESIZE_topleft_border_
		|| _WORK_MODE_ == _RESIZE_topright_border_
		|| _WORK_MODE_ == _RESIZE_bottomleft_border_
		|| _WORK_MODE_ == _RESIZE_bottomright_border_
		) {
		set_new_WORK_MODE(low_level, _NORMAL_);
		return;
	};

	//WTF?
	return;
};

void TRANSFER_DIALOG2::set_new_WORK_MODE(GUI_low_level *low_level, int new_WORK_MODE) {
	if (_WORK_MODE_ == new_WORK_MODE) return;

	if (new_WORK_MODE == _NORMAL_
		|| new_WORK_MODE == _MOVE_from_CAPTION_
		|| new_WORK_MODE == _CloseBtn_Pressed_
		|| new_WORK_MODE == _MOVE_SPLITTER_
		|| new_WORK_MODE == _RESIZE_top_border_
		|| new_WORK_MODE == _RESIZE_left_border_
		|| new_WORK_MODE == _RESIZE_right_border_
		|| new_WORK_MODE == _RESIZE_bottom_border_

		|| new_WORK_MODE == _RESIZE_topleft_border_
		|| new_WORK_MODE == _RESIZE_topright_border_
		|| new_WORK_MODE == _RESIZE_bottomleft_border_
		|| new_WORK_MODE == _RESIZE_bottomright_border_

		) {
		_WORK_MODE_ = new_WORK_MODE;
		low_level->invalidate();
		return;
	}

	low_level->invalidate();
	// WTF ??
	return;
}

void TRANSFER_DIALOG2::Close_btn_Click() {
	viewer->transfer_dialog_HIDE();
}


void TRANSFER_DIALOG2::mouse_leave(GUI_low_level *low_level) {

}

void TRANSFER_DIALOG2::mouse_left_button_down(GUI_low_level *low_level, int mx, int my) {

	int local_mx, local_my, bt;
	local_mx = mx - x;
	local_my = my - y;

	if (modal_dialog_info->its_me(local_mx, local_my)) {
		modal_dialog_info->mouse_left_button_down(low_level, local_mx, local_my);
		return;
	}

	if (modal_dialog_skip->its_me(local_mx, local_my)) {
		modal_dialog_skip->mouse_left_button_down(low_level, local_mx, local_my);
		return;
	}

	if (modal_dialog_confirm_overwrite->its_me(local_mx, local_my)) {
		modal_dialog_confirm_overwrite->mouse_left_button_down(low_level, local_mx, local_my);
		return;
	}

	if (is_modal_lock()) {
		if (modal_dialog_progress->its_me(local_mx, local_my)) {
			modal_dialog_progress->mouse_left_button_down(low_level, local_mx, local_my);
		}
		return;
	};


	if (_WORK_MODE_ == _NORMAL_) {



		if (Caption->Close_btn->its_me(local_mx, local_my)) {
			set_new_WORK_MODE(low_level, _CloseBtn_Pressed_);
			return;
		};
		if (Caption->its_me(local_mx, local_my)) {
			mouse_pressed_local_mx_delta = mx - x;
			mouse_pressed_local_my_delta = my - y;
			set_new_WORK_MODE(low_level, _MOVE_from_CAPTION_);
			return;
		}

		if (Splitter->its_me(local_mx, local_my)) {
			//mouse_pressed_local_mx_delta = mx - x;
			//mouse_pressed_local_my_delta = my - y;
			set_new_WORK_MODE(low_level, _MOVE_SPLITTER_);
			return;
		}




		bt = recognize_border_type(mx, my);

		if (bt == BORDER_TOP) {
			set_new_WORK_MODE(low_level, _RESIZE_top_border_);
			return;
		}
		if (bt == BORDER_LEFT) {
			set_new_WORK_MODE(low_level, _RESIZE_left_border_);
			return;
		}
		if (bt == BORDER_RIGHT) {
			set_new_WORK_MODE(low_level, _RESIZE_right_border_);
			return;
		}
		if (bt == BORDER_BOTTOM) {
			set_new_WORK_MODE(low_level, _RESIZE_bottom_border_);
			return;
		}

		if (bt == BORDER_TOPRIGHT) {
			set_new_WORK_MODE(low_level, _RESIZE_topright_border_);
			return;
		}
		if (bt == BORDER_TOPLEFT) {
			set_new_WORK_MODE(low_level, _RESIZE_topleft_border_);
			return;
		}
		if (bt == BORDER_BOTTOMLEFT) {
			set_new_WORK_MODE(low_level, _RESIZE_bottomleft_border_);
			return;
		}
		if (bt == BORDER_BOTTOMRIGHT) {
			set_new_WORK_MODE(low_level, _RESIZE_bottomright_border_);
			return;
		}


		if (Local_DirsFiles->its_me(local_mx, local_my)) {
			set_ACTIVE_object(_Local_Dirs_);
			Local_DirsFiles->mouse_left_button_down(low_level, local_mx, local_my);
			return;
		}

		if (Dest_DirsFiles->its_me(local_mx, local_my)) {
			set_ACTIVE_object(_Dest_Dirs_);
			Dest_DirsFiles->mouse_left_button_down(low_level, local_mx, local_my);
			return;
		}

	}

	// WTF ?
	return;
}

int TRANSFER_DIALOG2::recognize_border_type(int mx, int my) {

	int d;
	d = 20;

	if (mx >= x && mx < x + _Border && my >= y + d && my < y + h - d) { // лева€ граница

		return BORDER_LEFT;
	}
	if (mx >= x + w - _Border && mx < x + w && my >= y + d && my < y + h - d) { // права€ граница

		return BORDER_RIGHT;
	}

	if (mx >= x + d && mx < x + w - d && my >= y && my < y + _Border) { // верхна€€ граница

		return BORDER_TOP;
	}

	if (mx >= x + d && mx < x + w - d && my >= y + h - _Border && my < y + h) { // нижн€€ граница

		return BORDER_BOTTOM;
	}

	if ((mx >= x && mx < x + d && my >= y && my < y + _Border) ||
		(mx >= x && mx < x + _Border && my >= y && my < y + d)) { // левый верхний

		return BORDER_TOPLEFT;
	}

	if ((mx >= x + w - d && mx < x + w && my >= y && my < y + _Border) ||
		(mx >= x + w - _Border && mx < x + w && my >= y && my < y + d)) { // правый верхний

		return BORDER_TOPRIGHT;
	}

	if ((mx >= x + w - _Border && mx < x + w && my > y + h - d && my < y + h) ||
		(mx >= x + w - d && mx < x + w && my >= y + h - _Border && my < y + h)) { // правый нижний

		return BORDER_BOTTOMRIGHT;
	}



	if ((mx >= x && mx < x + _Border && my >= y + h - d && my < y + h) ||
		(mx >= x && mx < x + d && my >= y + h - _Border && my < y + h)) {

		return BORDER_BOTTOMLEFT;
	}

	if (mx >= x + _Border + Splitter_x - _Border / 2 && mx < x + _Border + Splitter_x + _Border / 2) {
		return BORDER_SPLITTER;
	}

	return 0;
};

void TRANSFER_DIALOG2::set_ACTIVE_object(int _id_) { // 0-unset, 1-_Local_Dirs_, 2-_Dest_Dirs_
	current_ACTIVE_object = _id_;
}

void TRANSFER_DIALOG2::mouse_right_button_up(GUI_low_level *low_level, int mx, int my) {

}

void TRANSFER_DIALOG2::mouse_right_button_down(GUI_low_level *low_level, int mx, int my) {
	if (is_modal_lock()) return;

}

void TRANSFER_DIALOG2::mouse_over(GUI_low_level *low_level, int mx, int my) {
	if (is_modal_lock()) return;
	int local_mx, local_my, dx, xx, ww;

	local_mx = mx - x;
	local_my = my - y;

	if (_WORK_MODE_ == _MOVE_from_CAPTION_) {

		set_size(low_level, mx - mouse_pressed_local_mx_delta, my - mouse_pressed_local_my_delta, w, h);

		return;
	}

	if (_WORK_MODE_ == _MOVE_SPLITTER_) {

		xx = x + _Border;
		ww = w - _Border * 2;
		dx = local_mx - _Border;

		Splitter_percent = ((float)dx / (float)ww) * (float)100;

		AutoArrangeSizes(low_level);

		return;
	}

	/*
	|| new_WORK_MODE == _RESIZE_top_border_
		|| new_WORK_MODE == _RESIZE_left_border_
		|| new_WORK_MODE == _RESIZE_right_border_
		|| new_WORK_MODE == _RESIZE_bottom_border_
		*/

	if (_WORK_MODE_ == _RESIZE_topleft_border_) {
		h -= (my - y);
		y = my;
		w -= (mx - x);
		x = mx;
		AutoArrangeSizes(low_level);
		return;
	}
	if (_WORK_MODE_ == _RESIZE_topright_border_) {
		h -= (my - y);
		y = my;
		w = mx - x;
		AutoArrangeSizes(low_level);
		return;
	}
	if (_WORK_MODE_ == _RESIZE_bottomleft_border_) {
		h = my - y;
		w -= (mx - x);
		x = mx;
		AutoArrangeSizes(low_level);
		return;
	}
	if (_WORK_MODE_ == _RESIZE_bottomright_border_) {
		h = my - y;
		w = mx - x;
		AutoArrangeSizes(low_level);
		return;
	}

	if (_WORK_MODE_ == _RESIZE_top_border_) {
		h -= (my - y);
		y = my;
		AutoArrangeSizes(low_level);
		return;
	}
	if (_WORK_MODE_ == _RESIZE_bottom_border_) {
		h = my - y;
		AutoArrangeSizes(low_level);
		return;
	}

	if (_WORK_MODE_ == _RESIZE_left_border_) {
		w -= (mx - x);
		x = mx;
		AutoArrangeSizes(low_level);
		return;
	}
	if (_WORK_MODE_ == _RESIZE_right_border_) {
		w = mx - x;
		AutoArrangeSizes(low_level);
		return;
	}

	if (_WORK_MODE_ == _NORMAL_) {

		int bt;

		bt = recognize_border_type(mx, my);




		if (bt == BORDER_TOP) {
			need_cursor = 111;
			return;
		}
		if (bt == BORDER_LEFT) {
			need_cursor = 110;
			return;
		}
		if (bt == BORDER_RIGHT) {
			need_cursor = 110;
			return;
		}
		if (bt == BORDER_BOTTOM) {
			need_cursor = 111;
			return;
		}
		if (bt == BORDER_TOPLEFT) {
			need_cursor = 108;
			return;
		}
		if (bt == BORDER_TOPRIGHT) {
			need_cursor = 109;
			return;
		}
		if (bt == BORDER_BOTTOMLEFT) {
			need_cursor = 109;
			return;
		}
		if (bt == BORDER_BOTTOMRIGHT) {
			need_cursor = 108;
			return;
		}

		if (bt == BORDER_SPLITTER) {
			need_cursor = 110;
			return;
		}

		need_cursor = 0;


		Local_DirsFiles->mouse_over(low_level, local_mx, local_my);
		Dest_DirsFiles->mouse_over(low_level, local_mx, local_my);


		return;
	}
}

void TRANSFER_DIALOG2::ASYNC_TRANSFER_EXECUTE(GUI_low_level *low_level) {
	if (transfer_ASYNC_stage == 0) return;
	if (current_transfer_element_local == nullptr && current_delete_element_dest == nullptr && current_transfer_element_dest == nullptr) return;

	wchar_t name[5100], local_path[5100], dest_path[5100];


	if (viewer->net_client_session->transfer_files_canceled == true) { // где то асинхронно нажали CANCEL
		if (transfer_ASYNC_stage == 1 || transfer_ASYNC_stage == 2) {
			FINISH_TRANSFER(low_level);
		};
		if (transfer_ASYNC_stage == 20 || transfer_ASYNC_stage == 21) {
			FINISH_DELETE(low_level);
		}
		return;
	}

	if (transfer_ASYNC_stage == 20) { // удаление

		// —оставим список файлов/папок на удаление, отправим партнеру. ѕартнер будет периодически присылать нам информацию о ходе удалени€.

		wchar_t *w, *f_name;

		unsigned char *buf;
		int buf_size = 50000;
		buf = new unsigned char[50000];

		f_name = new wchar_t[5100];
		zero_wchar_t(f_name, 5100);

		zero_unsigned_char(buf, 50000);
		w = (wchar_t *)&buf[32];

		int i, j;
		j = 0;

		while (current_delete_element_dest != nullptr) {
			f_name[0] = 0;
			current_delete_element_dest->get_folder_full_name(f_name);
			my_str_del_from_left(f_name, 18);
			//send_udp2(L"ADD ", f_name);
			i = 0;
			while (i < 5000 && f_name[i] != 0) {
				if (j >= buf_size - 50) {
					FINISH_DELETE(low_level);
					return;
				}
				w[j++] = f_name[i++];
			}
			w[j++] = 0;

			current_delete_element_dest = current_delete_element_dest->get_next_selected_element();
		}
		w[j++] = 0;

		viewer->net_client_session->delete_files_list_from_partner(buf, 32 + j * 2 + 2);

		FINISH_DELETE(low_level);


		transfer_ASYNC_stage = 0;

		return;
	};

	if (transfer_ASYNC_stage == 200) { // копирование от партнера ко мне
		// копируем от партнера к себе папку/файл

		zero_wchar_t(name, 5100);
		zero_wchar_t(dest_path, 5100);

		current_transfer_element_dest->get_folder_full_path(dest_path);
		my_str_del_from_left(dest_path, 18);
		//current_transfer_element_dest->get_folder_full_name(name);
		//my_str_del_from_left(name, 18);

		current_transfer_element_dest->get_name(name);
		my_strcat(dest_path, L"\\");
		my_strcat(dest_path, name);


		//send_udp2(L"COPY from PARTNER to MY -> ", name);

		if (transfer_PARTNER_to_MY == nullptr) transfer_PARTNER_to_MY = new TRANSFER_DIALOG2_transfer_from_PARTNER_to_MY(this);

		zero_wchar_t(local_path, 5100);
		my_strcpy(local_path, local_target);
		my_strcat(local_path, L"\\");
		my_strcat(local_path, name);

		if (transfer_PARTNER_to_MY->RUN_TRANSFER_from_PARTNER_to_MY(dest_path, local_path, &modal_dialog_progress->modal__result) == false) {
			FINISH_TRANSFER_from_Partner_to_MY(low_level);
			return;

		}
		else
		{
			transfer_ASYNC_stage = 201;
		};
	}

	if (transfer_ASYNC_stage == 201) {

		current_transfer_element_dest = current_transfer_element_dest->get_next_selected_element();
		if (current_transfer_element_dest == nullptr) {
			FINISH_TRANSFER_from_Partner_to_MY(low_level);
			return;
		}
		modal_dialog_progress->label1->set_label(low_level, current_transfer_element_dest->get_name());
		modal_dialog_progress->label3->set_label(low_level, L".");

		transfer_ASYNC_stage = 200;

		return;
	}

	if (transfer_ASYNC_stage == 1) {
		current_transfer_element_local = current_transfer_element_local->get_next_selected_element();
		if (current_transfer_element_local == nullptr) {
			FINISH_TRANSFER(low_level);
			return;
		}
		modal_dialog_progress->label1->set_label(low_level, current_transfer_element_local->get_name());
		modal_dialog_progress->label3->set_label(low_level, L".");

		transfer_ASYNC_stage = 2;

		return;
	}

	if (transfer_ASYNC_stage == 2) { // копирование от мен€ к партнеру



		zero_wchar_t(name, 5100);


		current_transfer_element_local->get_name(name);


		if (current_transfer_element_local->is_folder) {

			zero_wchar_t(local_path, 5100);
			current_transfer_element_local->get_folder_full_path(local_path);

			if (viewer->net_client_session->transfer_DIRECTORY_to_parnter(dest_target, local_path, name) == false) {

				// по какой то причине не удалось скопировать папку, мы выдали сообщение пользователю и пользователь нажал "CANCEL"
				FINISH_TRANSFER(low_level);
				return;
			}
		}
		else {
			zero_wchar_t(local_path, 5100);
			current_transfer_element_local->get_folder_full_path(local_path);

			if (viewer->net_client_session->transfer_FILE_to_parnter(dest_target, local_path, name, false) == false) {

			}
		}
		transfer_ASYNC_stage = 1;

		return;
	};
	if (transfer_ASYNC_stage == 1) {
		current_transfer_element_local = current_transfer_element_local->get_next_selected_element();
		if (current_transfer_element_local == nullptr) {
			FINISH_TRANSFER(low_level);
			return;
		}
		modal_dialog_progress->label1->set_label(low_level, current_transfer_element_local->get_name());
		modal_dialog_progress->label3->set_label(low_level, L".");

		transfer_ASYNC_stage = 2;

		return;
	}

}

void TRANSFER_DIALOG2::ASYNC_LOAD_EXECUTE(GUI_low_level *low_level) {
	if (Local_DirsFiles != nullptr) Local_DirsFiles->ASYNC(low_level);
	if (Dest_DirsFiles != nullptr)  Dest_DirsFiles->ASYNC(low_level);
}

void TRANSFER_DIALOG2::mouse_wheel(GUI_low_level *low_level, int type, int mx, int my) {
	if (is_modal_lock()) return;
	// type = 1 scroll down
	// type = 2 scroll up
	int local_mx, local_my;
	local_mx = mx - x;
	local_my = my - y;
	if (Local_DirsFiles->its_me(local_mx, local_my))	Local_DirsFiles->mouse_wheel(low_level, type);
	if (Dest_DirsFiles->its_me(local_mx, local_my))	Dest_DirsFiles->mouse_wheel(low_level, type);
	//Dest_DirsFiles->mouse_wheel(type);

}

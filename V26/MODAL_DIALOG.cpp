#include "stdafx.h"

#include "MODAL_DIALOG.h"
#include "TRANSFER_DIALOG2_DirsFiles_Button.h"
#include "TRANSFER_DIALOG2_DirsFiles_Label.h"
#include "VIEWER.h"

extern VIEWER  *viewer;

void button_pressed_transfer_cancel(GUI_low_level *low_level, int type);
void button_pressed_confirm_cancel(GUI_low_level *low_level, int type);
void button_pressed_confirm_overwrite(GUI_low_level *low_level, int type);
void button_pressed_confirm_skip(GUI_low_level *low_level, int type);

void button_pressed_skip_cancel(GUI_low_level *low_level, int type);
void button_pressed_skip_skip(GUI_low_level *low_level, int type);

void button_pressed_info_ok(GUI_low_level *low_level, int type);


MODAL_DIALOG::MODAL_DIALOG(TRANSFER_DIALOG2 *parent_, int MD_type_) {

	parent = parent_;
	MD_type = MD_type_;

	if (MD_type == MD_TRANSFER) {

		w = 280;
		h = 100;

		btn_CANCEL = new TRANSFER_DIALOG2_DirsFiles_Button(0, this, L"Cancel");
		btn_CANCEL->func_button_pressed = &button_pressed_transfer_cancel;

		label_info = new TRANSFER_DIALOG2_DirsFiles_Label(this); label_info->set_label(nullptr, L"Copy");

		label1 = new TRANSFER_DIALOG2_DirsFiles_Label(this);
		label1->set_label(nullptr, L"document.xls");

		//label2 = new TRANSFER_DIALOG2_DirsFiles_Label(low_level, this);
		//label2->set_label(L"- - - - -");

		label3 = new TRANSFER_DIALOG2_DirsFiles_Label(this);
		label3->set_label(nullptr, L"progress - 1/2 - 345 kb/sec");


		return;
	}

	if (MD_type == MD_OVERWRITE) {

		w = 380;
		h = 168;

		label_warning = new TRANSFER_DIALOG2_DirsFiles_Label(this); label_warning->set_label(nullptr, L"Warning");
		label_info = new TRANSFER_DIALOG2_DirsFiles_Label(this); label_info->set_label(nullptr, L"File already exists");
		label_file_name = new TRANSFER_DIALOG2_DirsFiles_Label(this); label_file_name->set_label(nullptr, L"file name");
		label_new_file_info = new TRANSFER_DIALOG2_DirsFiles_Label(this); label_new_file_info->set_label(nullptr, L"new file info");
		label_old_file_info = new TRANSFER_DIALOG2_DirsFiles_Label(this); label_old_file_info->set_label(nullptr, L"old file info");

		label_new_txt = new TRANSFER_DIALOG2_DirsFiles_Label(this); label_new_txt->set_label(nullptr, L"New");
		label_old_txt = new TRANSFER_DIALOG2_DirsFiles_Label(this); label_old_txt->set_label(nullptr, L"Existing");

		chbox_REMEMBER = new TRANSFER_DIALOG2_DirsFiles_Checkbox(this, L"Remember choise");

		btn_OVERWRITE = new TRANSFER_DIALOG2_DirsFiles_Button(0, this, L"Overwrite");
		btn_OVERWRITE->func_button_pressed = &button_pressed_confirm_overwrite;

		btn_SKIP = new TRANSFER_DIALOG2_DirsFiles_Button(0, this, L"Skip");
		btn_SKIP->func_button_pressed = &button_pressed_confirm_skip;

		btn_CANCEL = new TRANSFER_DIALOG2_DirsFiles_Button(0, this, L"Cancel");
		btn_CANCEL->func_button_pressed = &button_pressed_confirm_cancel;

		return;
	}


	if (MD_type == MD_SKIP) {

		w = 380;
		h = 125;

		label_warning = new TRANSFER_DIALOG2_DirsFiles_Label(this); label_warning->set_label(nullptr, L"Warning");
		label_info = new TRANSFER_DIALOG2_DirsFiles_Label(this); label_info->set_label(nullptr, L"File already exists");
		label_file_name = new TRANSFER_DIALOG2_DirsFiles_Label(this); label_file_name->set_label(nullptr, L"file name");

		chbox_REMEMBER = new TRANSFER_DIALOG2_DirsFiles_Checkbox(this, L"Remember choise");

		btn_SKIP = new TRANSFER_DIALOG2_DirsFiles_Button(0, this, L"Skip");
		btn_SKIP->func_button_pressed = &button_pressed_skip_skip;

		btn_CANCEL = new TRANSFER_DIALOG2_DirsFiles_Button(0, this, L"Cancel");
		btn_CANCEL->func_button_pressed = &button_pressed_skip_cancel;

		return;
	};

	if (MD_type == MD_INFO) {

		w = 300;
		h = 85;


		label_info = new TRANSFER_DIALOG2_DirsFiles_Label(this); label_info->set_label(nullptr, L"File already exists");

		btn_OK = new TRANSFER_DIALOG2_DirsFiles_Button(0, this, L"Ok");
		btn_OK->func_button_pressed = &button_pressed_info_ok;

		return;
	};

}

MODAL_DIALOG::~MODAL_DIALOG() {

}

void button_pressed_transfer_cancel(GUI_low_level *low_level, int type) {
	viewer->file_transfer_dialog->modal_dialog_progress->pressed_btn_transfer_cancel(low_level);
}

void button_pressed_confirm_cancel(GUI_low_level *low_level, int type) {
	viewer->file_transfer_dialog->modal_dialog_confirm_overwrite->pressed_btn_confirm_cancel(low_level);
}

void button_pressed_confirm_overwrite(GUI_low_level *low_level, int type) {
	viewer->file_transfer_dialog->modal_dialog_confirm_overwrite->pressed_btn_confirm_overwrite(low_level);
}

void button_pressed_confirm_skip(GUI_low_level *low_level, int type) {
	viewer->file_transfer_dialog->modal_dialog_confirm_overwrite->pressed_btn_confirm_skip(low_level);
}


void button_pressed_skip_cancel(GUI_low_level *low_level, int type) {
	viewer->file_transfer_dialog->modal_dialog_skip->pressed_btn_skip_cancel(low_level);
}
void button_pressed_skip_skip(GUI_low_level *low_level, int type) {
	viewer->file_transfer_dialog->modal_dialog_skip->pressed_btn_skip_skip(low_level);
}
void button_pressed_info_ok(GUI_low_level *low_level, int type) {
	viewer->file_transfer_dialog->modal_dialog_info->pressed_btn_info_ok(low_level);
}

void MODAL_DIALOG::pressed_btn_transfer_cancel(GUI_low_level *low_level) {
	if (viewer != nullptr && viewer->net_client_session != nullptr)	viewer->net_client_session->transfer_files_canceled = true;
	modal__result = MR_TRANSFER_CANCEL;
	set_visible(low_level, false);
}

void MODAL_DIALOG::pressed_btn_confirm_cancel(GUI_low_level *low_level) {
	modal__result = MR_CONFIRM_CANCEL;
	set_visible(low_level, false);
}

void MODAL_DIALOG::pressed_btn_confirm_overwrite(GUI_low_level *low_level) {
	modal__result = MR_OVERWRITE;
	set_visible(low_level, false);
}

void MODAL_DIALOG::pressed_btn_confirm_skip(GUI_low_level *low_level) {
	modal__result = MR_CONFIRM_SKIP;
	set_visible(low_level, false);
}

void MODAL_DIALOG::pressed_btn_skip_cancel(GUI_low_level *low_level) {
	modal__result = MR_SKIP_CANCEL;
	set_visible(low_level, false);
}
void MODAL_DIALOG::pressed_btn_skip_skip(GUI_low_level *low_level) {
	modal__result = MR_SKIP_SKIP;
	set_visible(low_level, false);
}

void MODAL_DIALOG::pressed_btn_info_ok(GUI_low_level *low_level) {
	modal__result = MR_INFO_OK;
	set_visible(low_level, false);
}

void MODAL_DIALOG::set_visible(GUI_low_level *low_level, bool val) {
	if (is_visible != val) {
		/*
		if (label_warning != nullptr)       label_warning->set_label(L"");
		if (label_info != nullptr)          label_info->set_label(L"");
		if (label_file_name != nullptr)     label_file_name->set_label(L"");
		if (label_new_file_info != nullptr) label_new_file_info->set_label(L"");
		if (label_old_file_info != nullptr) label_old_file_info->set_label(L"");
		if (label1 != nullptr)              label1->set_label(L"");
		if (label2 != nullptr)              label2->set_label(L"");
		if (label3 != nullptr)              label3->set_label(L"");
		*/
		if (val == true) {
			modal__result = 0;
		}
		is_visible = val;
		if (low_level != nullptr) 		low_level->invalidate();

	}
}
int MODAL_DIALOG::get_global_x() {
	return x + parent->get_global_x();
}
int MODAL_DIALOG::get_global_y() {
	return y + parent->get_global_y();
}

void MODAL_DIALOG::Paint(GUI_low_level *low_level) {
	if (is_visible == false) return;

	low_level->fill_rectangle(x + parent->get_global_x(), y + parent->get_global_y(), w, h, col_back, 255);
	low_level->line_h(x + parent->get_global_x(), y + parent->get_global_y(), w, col_border);
	low_level->line_h(x + parent->get_global_x(), y + parent->get_global_y() + h - 1, w, col_border);

	low_level->line_v(x + parent->get_global_x(), y + parent->get_global_y(), h, col_border);
	low_level->line_v(x + parent->get_global_x() + w - 1, y + parent->get_global_y(), h, col_border);

	if (MD_type == MD_INFO) {

		label_info->Paint(low_level);
		btn_OK->Paint(low_level);

		return;
	}


	if (MD_type == MD_SKIP) {

		label_warning->Paint_center(low_level);
		label_info->Paint_center(low_level);
		label_file_name->Paint(low_level);



		chbox_REMEMBER->Paint(low_level);

		btn_SKIP->Paint(low_level);
		btn_CANCEL->Paint(low_level);

		return;
	}


	if (MD_type == MD_OVERWRITE) {

		label_warning->Paint_center(low_level);
		label_info->Paint_center(low_level);
		label_file_name->Paint(low_level);

		label_new_txt->Paint(low_level);
		label_old_txt->Paint(low_level);

		label_new_file_info->Paint_right(low_level);
		label_old_file_info->Paint_right(low_level);

		chbox_REMEMBER->Paint(low_level);

		btn_OVERWRITE->Paint(low_level);
		btn_SKIP->Paint(low_level);
		btn_CANCEL->Paint(low_level);

		return;
	}

	if (MD_type == MD_TRANSFER) {

		label_info->Paint_center(low_level);

		label1->Paint(low_level);
		//label2->Paint();
		label3->Paint_center(low_level);

		btn_CANCEL->Paint(low_level);

		return;
	}




}

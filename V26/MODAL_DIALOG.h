#pragma once

#define MD_TRANSFER 1
#define MD_OVERWRITE 2
#define MD_SKIP 3
#define MD_INFO 4


#define MR_OK 1
#define MR_CONFIRM_CANCEL 2
#define MR_CONFIRM_SKIP 3
#define MR_OVERWRITE 4
#define MR_TRANSFER_CANCEL 5
#define MR_SKIP_CANCEL 6
#define MR_SKIP_SKIP 7
#define MR_INFO_OK 8
#define MR_CANCEL 9

class GUI_low_level;
class TRANSFER_DIALOG2;
class TRANSFER_DIALOG2_DirsFiles_Button;
class TRANSFER_DIALOG2_DirsFiles_Label;
class TRANSFER_DIALOG2_DirsFiles_Checkbox;


class MODAL_DIALOG
{
private:
	TRANSFER_DIALOG2 *parent = nullptr;

	int type = 0;
	bool is_visible = false;
	int MD_type = 0;

public:
	int modal__result = 0;

	int x = 0, y = 0, w = 0, h = 0;
	unsigned int col_back = 0xf5f5f5, col_border = 0xFDCA46;

	bool its_me(int m_x, int m_y);

	void pressed_btn_transfer_cancel(GUI_low_level *low_level);
	void pressed_btn_confirm_cancel(GUI_low_level *low_level);
	void pressed_btn_confirm_overwrite(GUI_low_level *low_level);
	void pressed_btn_confirm_skip(GUI_low_level *low_level);
	void pressed_btn_skip_cancel(GUI_low_level *low_level);
	void pressed_btn_skip_skip(GUI_low_level *low_level);
	void pressed_btn_info_ok(GUI_low_level *low_level);

	

	

	void mouse_left_button_down(GUI_low_level *low_level, int mx, int my);
	void mouse_left_button_up(GUI_low_level *low_level);

	int get_global_x();
	int get_global_y();

	TRANSFER_DIALOG2_DirsFiles_Checkbox *chbox_REMEMBER = nullptr;
	TRANSFER_DIALOG2_DirsFiles_Label
		  *label_warning = nullptr
		, *label_info = nullptr
		, *label_file_name = nullptr
		, *label_new_file_info = nullptr, *label_new_txt = nullptr
		, *label_old_file_info = nullptr, *label_old_txt = nullptr
		;
	TRANSFER_DIALOG2_DirsFiles_Button
		  *btn_CANCEL = nullptr
		, *btn_YES = nullptr
		, *btn_NO = nullptr
		, *btn_OVERWRITE = nullptr
		, *btn_OK = nullptr
		, *btn_SKIP = nullptr;

	TRANSFER_DIALOG2_DirsFiles_Label *label1 = nullptr, *label2 = nullptr, *label3 = nullptr;

	void set_visible(GUI_low_level *low_level, bool val);
	bool get_visible() { return is_visible; };



	void recalc_size(GUI_low_level *low_level, int w_, int h_);

	void set_size(int x_, int y_, int w_, int h_);

	void Paint(GUI_low_level *low_level);

	MODAL_DIALOG(TRANSFER_DIALOG2 *parent_, int MD_type);
	~MODAL_DIALOG();

};


#pragma once

class VIEWER;
class GUI_low_level;
class TRANSFER_DIALOG2_DirsFiles;
class MODAL_DIALOG;

#define _Local_Dirs_ 1
#define _Dest_Dirs_  2
#define TRANSFER_from_MY_to_PARTNER 3
#define TRANSFER_from_PARTNER_to_MY 4

#define _NORMAL_ 1
#define _MOVE_from_CAPTION_ 2
#define _RESIZE_left_border_ 3
#define _RESIZE_right_border_ 4
#define _RESIZE_top_border_ 5
#define _RESIZE_bottom_border_ 6
#define _MOVE_VScroll_local_ 7
#define _MOVE_VScroll_dest_ 8
#define _PRESSED_CloseBtn_ 9
#define _PRESSED_VScroll_local_up_ 10
#define _PRESSED_VScroll_local_down_ 11
#define _PRESSED_VScroll_dest_up_ 12
#define _PRESSED_VScroll_dest_down_ 13
#define _PRESSED_VScroll_local_PGup_ 14
#define _PRESSED_VScroll_local_PGdown_ 15
#define _PRESSED_VScroll_dest_PGup_ 16
#define _PRESSED_VScroll_dest_PGdown_ 17
#define _CloseBtn_Pressed_ 18
#define _MOVE_SPLITTER_ 19
#define _RESIZE_topleft_border_ 20
#define _RESIZE_topright_border_ 21
#define _RESIZE_bottomleft_border_ 22
#define _RESIZE_bottomright_border_ 23


class TRANSFER_DIALOG2_Caption;
class TRANSFER_DIALOG2_Splitter;



class TRANSFER_DIALOG2_Close_btn
{
private:

	TRANSFER_DIALOG2_Caption *parent = nullptr;
	int x = 0, y = 0, w = 0, h = 0;
public:

	

	bool its_me(int local_mx, int local_my);

	void Paint(GUI_low_level *low_level);

	void set_size(int x_, int y_, int w_, int h_);

	TRANSFER_DIALOG2_Close_btn(TRANSFER_DIALOG2_Caption *parent_);
	~TRANSFER_DIALOG2_Close_btn();
};


class TRANSFER_DIALOG2
{
private:

	int x = 0, y = 0, w = 0, h = 0;

	int mouse_pressed_local_mx_delta = 0, mouse_pressed_local_my_delta = 0;
	bool is_visible = false;

public:

	TRANSFER_DIALOG2_DirsFiles *Local_DirsFiles = nullptr, *Dest_DirsFiles = nullptr;
	TRANSFER_DIALOG2_Caption   *Caption = nullptr;
	TRANSFER_DIALOG2_Splitter  *Splitter = nullptr;

	MODAL_DIALOG *modal_dialog_progress = nullptr;
	MODAL_DIALOG *modal_dialog_confirm_overwrite = nullptr;
	MODAL_DIALOG *modal_dialog_skip = nullptr;
	MODAL_DIALOG *modal_dialog_info = nullptr;


	void AutoArrangeSizes(GUI_low_level *low_level);

	void Paint(GUI_low_level *low_level);
	void set_size(GUI_low_level *low_level, int x_, int y_, int w_, int h_);

	void set_visible(GUI_low_level *low_level, bool val);
	bool get_visible();

	int get_global_x() { return x; };
	int get_global_y() { return y; };
	int get_global_w() { return w; };
	int get_global_h() { return h; };

	void START_TRANSFER(GUI_low_level *low_level, int mode);
	void FINISH_TRANSFER_from_Partner_to_MY(GUI_low_level *low_level);
	void FINISH_TRANSFER(GUI_low_level *low_level);
	void FINISH_DELETE(GUI_low_level *low_level);
	void REFRESH(GUI_low_level *low_level, int mode);
	void START_DELETE(GUI_low_level *low_level, int mode);

	int _WORK_MODE_ = _NORMAL_; // рабочий режим
						 // 1 - _NORMAL_ основной рабочий режим
						 // 2 - _MOVE_from_CAPTION_ перетаскиваем за заголовок
						 // 3 - _RESIZE_left_border_ 
						 // 4 - _RESIZE_right_border_ 
						 // 5 - _RESIZE_top_border_ 
						 // 6 - _RESIZE_bottom_border_ 
						 // 7 - _MOVE_VScroll_local_
						 // 8 - _MOVE_VScroll_dest_
						 // 9 - _PRESSED_CloseBtn_
						 // 10 - _PRESSED_VScroll_local_up_
						 // 11 - _PRESSED_VScroll_local_down_
						 // 12 - _PRESSED_VScroll_dest_up_
						 // 13 - _PRESSED_VScroll_dest_down_
						 // 14 - _PRESSED_VScroll_local_PGup_
						 // 15 - _PRESSED_VScroll_local_PGdown_
						 // 16 - _PRESSED_VScroll_dest_PGup_
						 // 17 - _PRESSED_VScroll_dest_PGdown_

	 TRANSFER_DIALOG2(VIEWER *viewer_);
	~TRANSFER_DIALOG2();
};

class TRANSFER_DIALOG2_Caption
{
private:

	int x = 0, y = 0, w = 0, h = 0;
public:
	int get_global_x();
	int get_global_y();
	int get_global_w();
	int get_global_h();
	bool its_me(int local_mx, int local_my);
	TRANSFER_DIALOG2_Close_btn *Close_btn = nullptr;
	TRANSFER_DIALOG2 *parent = nullptr;

	void set_size(int x_, int y_, int w_, int h_);


	void Paint(GUI_low_level *low_level);
	TRANSFER_DIALOG2_Caption(TRANSFER_DIALOG2 *parent_);
	~TRANSFER_DIALOG2_Caption();
};

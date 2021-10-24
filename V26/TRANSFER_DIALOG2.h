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
class TRANSFER_DIALOG2_DirsFiles_TREE_element;
class TRANSFER_DIALOG2_transfer_from_PARTNER_to_MY;


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

	TRANSFER_DIALOG2_transfer_from_PARTNER_to_MY *transfer_PARTNER_to_MY = nullptr;
	unsigned long long need_cursor = 0; // курсор мыши который нужно показывать

	VIEWER *viewer = nullptr; // parent object
	bool OVERWRITE_ALL_FILES_mode = false;
	bool SKIP_ALL_FILES_mode = false;
	wchar_t *dest_target = nullptr; // путь куда пользователь хочет копировать файлы
	wchar_t *local_target = nullptr; // путь куда пользователь хочет копировать файлы


	TRANSFER_DIALOG2_DirsFiles_TREE_element *current_transfer_element_local = nullptr; // копируем от нас к партнеру
	TRANSFER_DIALOG2_DirsFiles_TREE_element *current_transfer_element_dest = nullptr; // копироуем от партнера к нам
	TRANSFER_DIALOG2_DirsFiles_TREE_element *current_delete_element_dest = nullptr;

	int transfer_ASYNC_stage = 0; // 0 - передача не идет
						// 1 - нужно определить следующий файл для передачи 
						// 2 - нужно передать текущий файл
						// 20 - удаляем


	TRANSFER_DIALOG2_DirsFiles *Local_DirsFiles = nullptr, *Dest_DirsFiles = nullptr;
	TRANSFER_DIALOG2_Caption   *Caption = nullptr;
	TRANSFER_DIALOG2_Splitter  *Splitter = nullptr;
	int Splitter_x = 0;
	float Splitter_percent = 50;

	MODAL_DIALOG *modal_dialog_progress = nullptr;
	MODAL_DIALOG *modal_dialog_confirm_overwrite = nullptr;
	MODAL_DIALOG *modal_dialog_skip = nullptr;
	MODAL_DIALOG *modal_dialog_info = nullptr;


	void AutoArrangeSizes(GUI_low_level *low_level);

	void Paint(GUI_low_level *low_level);
	void set_size(GUI_low_level *low_level, int x_, int y_, int w_, int h_);

	void set_visible(GUI_low_level *low_level, bool val);
	bool get_visible() { return is_visible; }

	int get_global_x() { return x; };
	int get_global_y() { return y; };
	int get_global_w() { return w; };
	int get_global_h() { return h; };

	void ASYNC_TRANSFER_EXECUTE(GUI_low_level *low_level);

	void START_TRANSFER(GUI_low_level *low_level, int mode);
	void FINISH_TRANSFER_from_Partner_to_MY(GUI_low_level *low_level);
	void FINISH_TRANSFER(GUI_low_level *low_level);
	void FINISH_DELETE(GUI_low_level *low_level);
	void REFRESH(GUI_low_level *low_level, int mode);
	void START_DELETE(GUI_low_level *low_level, int mode);

	void set_new_WORK_MODE(GUI_low_level *low_level, int new_WORK_MODE);
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

	void mouse_wheel(GUI_low_level *low_level, int type, int mx, int my);//+
	void char_keydown(GUI_low_level *low_level, int msg, int wp, int lp); //+
	void mouse_over(GUI_low_level *low_level, int mx, int my);//+
	void mouse_left_button_down(GUI_low_level *low_level, int mx, int my);//+
	void mouse_left_button_up(GUI_low_level *low_level, int mx, int my);
	void mouse_right_button_down(GUI_low_level *low_level, int mx, int my);//+
	void mouse_right_button_up(GUI_low_level *low_level, int mx, int my);
	void mouse_leave(GUI_low_level *low_level);

	int  current_ACTIVE_object = 0; // 0-unset, 1-_Local_Dirs_, 2-_Dest_Dirs_
	bool is_modal_lock();

	void Close_btn_Click();

	void set_ACTIVE_object(int _id_);

	int recognize_border_type(int mx, int my);

	void need_ASYNC_LOAD_fileslist(TRANSFER_DIALOG2_DirsFiles_TREE_element *e);
	void ASYNC_LOAD_EXECUTE(GUI_low_level *low_level);


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

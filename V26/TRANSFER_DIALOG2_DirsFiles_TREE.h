#pragma once

#include "tools.h"
#include "FILE__LIST.h"

class GUI_low_level;
class TRANSFER_DIALOG2_DirsFiles;
class TRANSFER_DIALOG2_DirsFiles_TREE_element;
class TRANSFER_DIALOG2_DirsFiles_TREE;

class SELECT_RECTANGLE
{
public:
	
	TRANSFER_DIALOG2_DirsFiles_TREE *parent = nullptr;
	int x = -1, y = -1, w = 0, h = 0;

	TRANSFER_DIALOG2_DirsFiles_TREE_element *SOLO_selection = nullptr;

	void set_SOLO_selection(GUI_low_level *low_level, TRANSFER_DIALOG2_DirsFiles_TREE_element *v);

	void SET_begin_selection_range(GUI_low_level *low_level, int xx, int yy, int control_state);
	void SET_end_selection_range(GUI_low_level *low_level, int xx, int yy, int control_state);

	void moveUP_SOLO_selection(GUI_low_level *low_level, SHORT shift_state);
	void moveDOWN_SOLO_selection(GUI_low_level *low_level, SHORT shift_state);

	void movePGUP_SOLO_selection(GUI_low_level *low_level);
	void movePGDOWN_SOLO_selection(GUI_low_level *low_level);

	void Paint(GUI_low_level *low_level);
	SELECT_RECTANGLE(TRANSFER_DIALOG2_DirsFiles_TREE *parent_);
	~SELECT_RECTANGLE();
};

#define LOAD_Indicator_ww 150
#define LOAD_Indicator_hh 60



class LOAD_INDICATOR_tree
{
public:
	int line[20];
	
	int progress = 0;
	void set_progress(int p) { progress = p; };
	void increase_progress() { progress++; };

	void Paint(GUI_low_level *low_level, int xx, int yy);

	void turn();
	DWORD last_turn = 0;
	void ASYNC(GUI_low_level *low_level);

	LOAD_INDICATOR_tree();
};

class TRANSFER_DIALOG2_DirsFiles_TREE
{
private:
	
	TRANSFER_DIALOG2_DirsFiles *parent = nullptr;

	

public:
	//#################################################################################################
	//## асинхронная загрузка списка локальных файлов
	
	FILE__LIST FL_, LL;

	void ASYNC_finish(GUI_low_level *low_level);

	TRANSFER_DIALOG2_DirsFiles_TREE_element *current_active_element = nullptr;
	void set_current_active_element(TRANSFER_DIALOG2_DirsFiles_TREE_element *val);
	wchar_t need_ASYNC_load_name[5100];
	TRANSFER_DIALOG2_DirsFiles_TREE_element *need_ASYNC_load = nullptr;
	TRANSFER_DIALOG2_DirsFiles_TREE_element **need_ASYNC_load_q = nullptr;
	WIN32_FIND_DATA ASYNC_fd;
	HANDLE          ASYNC_h;
	
	void file_list_from_partner_RESPONCE(GUI_low_level *low_level, unsigned char *buf, int buf_size);
	void ASYNC(GUI_low_level *low_level);
	
	
	int request_FILE_LIST_from_partner_STATUS = 0;
	//#################################################################################################
	int LOCAL_or_PARTNER = 0;

	LOAD_INDICATOR_tree *LOAD_INDICATOR = nullptr;
	int get_global_x();
	int get_global_y();
	//int get_global_w();
	//int get_global_h();

	int   top_visible_line = 0;
	int   visible_line_count = 0;
	int   all_line_count = 0;
	int   polzunok_position_in_pixel = 0;
	float polzunok_position_in_percent = 0;
	float polzunok_size_in_percent = 0;
	int   polzunok_size_in_pixel = 0;

	void SET_polzunok_position_in_percent(GUI_low_level *low_level, float v);

	void generate_info();

	void need_LOAD_fileslist(GUI_low_level *low_level, TRANSFER_DIALOG2_DirsFiles_TREE_element *e);
	void need_CLOSE(GUI_low_level *low_level, TRANSFER_DIALOG2_DirsFiles_TREE_element *e);

	bool is_locked = false;
	void lock();
	void unlock();

	void top_arrow_click(GUI_low_level *low_level);
	void bottom_arrow_click(GUI_low_level *low_level);
	void PgUp_polzunok_click(GUI_low_level *low_level);
	void PgDown_polzunok_click(GUI_low_level *low_level);

	int x = 0, y = 0, w = 0, h = 0;
	TRANSFER_DIALOG2_DirsFiles_TREE_element *top = nullptr;

	TRANSFER_DIALOG2_DirsFiles_TREE_element *get_element_by_line_idx(int val);

	SELECT_RECTANGLE *SelectRectangle = nullptr;

	void mouse_left_button_down(GUI_low_level *low_level, int mouse_x, int mouse_y);
	void mouse_left_button_up(GUI_low_level *low_level);

	void mouse_over(GUI_low_level *low_level, int mouse_x, int mouse_y);

	void char_keydown(GUI_low_level *low_level, int msg, int wp, int lp);

	void START(GUI_low_level *low_level, int LOCAL_or_PARTNER_);

	
	
	void Paint(GUI_low_level *low_level);

	void set_size(GUI_low_level *low_level, int x_, int y_, int w_, int h_);

	bool its_me(int m_x, int m_y);

	TRANSFER_DIALOG2_DirsFiles_TREE_element *add_child(TRANSFER_DIALOG2_DirsFiles_TREE_element *element, wchar_t *name_, bool is_folder_, unsigned long long size_, unsigned long long date_);
	TRANSFER_DIALOG2_DirsFiles_TREE_element *add_next (TRANSFER_DIALOG2_DirsFiles_TREE_element *element, wchar_t *name_, bool is_folder_, unsigned long long size_, unsigned long long date_);

	void RECALC_polzunok_size(GUI_low_level *low_level);
	void RECALC_polzunok_position(GUI_low_level *low_level);

	void auto_arrange(GUI_low_level *low_level);

	TRANSFER_DIALOG2_DirsFiles_TREE_element *find_element_by_fullname(wchar_t *v);

	TRANSFER_DIALOG2_DirsFiles_TREE(TRANSFER_DIALOG2_DirsFiles *parent_);
	~TRANSFER_DIALOG2_DirsFiles_TREE();
};




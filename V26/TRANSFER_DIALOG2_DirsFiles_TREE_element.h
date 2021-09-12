#pragma once

class GUI_low_level;

class TRANSFER_DIALOG2_DirsFiles_TREE_element
{
private:
	
	unsigned long long size = 0, date = 0;
	
	wchar_t *name = nullptr;
public:
	
	int lvl = 0, line_idx = -1;

	void mouse_left_button_down(int mouse_x, int mouse_y);

	bool is_selected = false;
	bool is_open = false;
	bool is_folder = false;
	bool is_current_active_element = false;

	int  set_line_idx(int idx_, int lvl_);
	//int  calc_line_count();

	void paint_one_element(GUI_low_level *low_level, int x, int y, int w, int h);

	void paint_element_and_all_opened_sub_elements(GUI_low_level *low_level, int top_element, int xx, int yy, int ww, int hh);

	TRANSFER_DIALOG2_DirsFiles_TREE_element *get_element_by_line_idx(int val);
	TRANSFER_DIALOG2_DirsFiles_TREE_element *get_element_by_fullname(wchar_t *val);

	void clean_child();

	void get_folder_full_name(wchar_t *dir);
	void get_folder_full_path(wchar_t *dir);
	void get_name(wchar_t *buf);
	wchar_t *get_name() { return name; };

	void SELECT_element_from_to(int i1, int i2, int control_state);
	void UNSELECT_all_elements();

	TRANSFER_DIALOG2_DirsFiles_TREE_element *next = nullptr, *prev = nullptr, *child = nullptr, *parent = nullptr;
	void clean();

	TRANSFER_DIALOG2_DirsFiles_TREE_element *get_first_selected_element();
	TRANSFER_DIALOG2_DirsFiles_TREE_element *get_next_selected_element();

	TRANSFER_DIALOG2_DirsFiles_TREE_element *get_next_element();

	TRANSFER_DIALOG2_DirsFiles_TREE_element(wchar_t *name_, bool is_folder_, unsigned long long size_, unsigned long long date_, TRANSFER_DIALOG2_DirsFiles_TREE_element *parent_, TRANSFER_DIALOG2_DirsFiles_TREE_element *prev_);
	~TRANSFER_DIALOG2_DirsFiles_TREE_element();
};


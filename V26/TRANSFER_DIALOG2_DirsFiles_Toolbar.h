#pragma once
class GUI_low_level;
class TRANSFER_DIALOG2_DirsFiles;
class TRANSFER_DIALOG2_DirsFiles_Button;

class TRANSFER_DIALOG2_DirsFiles_Toolbar
{
private:
	
	TRANSFER_DIALOG2_DirsFiles *parent = nullptr;
	int x = 0, y = 0, w = 0, h = 0;
	int type = 0;
public:
	
	TRANSFER_DIALOG2_DirsFiles_Button *btn_copy = nullptr, *btn_new_folder = nullptr, *btn_refresh = nullptr, *btn_delete = nullptr;

	char ss_[500];
	char ss2_[500];
	bool is_locked = false;

	void mouse_left_button_down(GUI_low_level *low_level, int mx, int my);

	void mouse_left_button_up(GUI_low_level *low_level);

	void set_size(int x_, int y_, int w_, int h_);

	void Paint(GUI_low_level *low_level);

	void lock();
	void unlock();

	int get_global_x();
	int get_global_y();

	bool its_me(int m_x, int m_y);


	TRANSFER_DIALOG2_DirsFiles_Toolbar(int local_or_dest_, TRANSFER_DIALOG2_DirsFiles *parent_);
	~TRANSFER_DIALOG2_DirsFiles_Toolbar();
};


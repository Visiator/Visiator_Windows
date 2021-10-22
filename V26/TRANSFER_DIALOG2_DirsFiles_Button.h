#pragma once

class GUI_low_level;
class TRANSFER_DIALOG2_DirsFiles_Toolbar;
class MODAL_DIALOG;

class TRANSFER_DIALOG2_DirsFiles_Checkbox
{
private:
	
	
	MODAL_DIALOG *parent_modal_dialog = nullptr;
	int x = 0, y = 0, w = 0, h = 0;
	bool is_visible = false;
public:
	wchar_t *label = nullptr;

	void mouse_left_button_down(GUI_low_level *low_level, int mx, int my);
	void mouse_left_button_up(GUI_low_level *low_level);

	void(*func_button_pressed)(int type) = nullptr;

	bool its_me(int m_x, int m_y);

	bool is_pressed = false;
	void set_is_pressed(GUI_low_level *low_level, bool val);

	void set_size(int x_, int y_, int w_, int h_);

	void Paint(GUI_low_level *low_level);

	TRANSFER_DIALOG2_DirsFiles_Checkbox(MODAL_DIALOG *parent_, wchar_t *label_);
	~TRANSFER_DIALOG2_DirsFiles_Checkbox();
};

class TRANSFER_DIALOG2_DirsFiles_Button
{
private:
	int type = 0;
	int font_idx = 0;
	TRANSFER_DIALOG2_DirsFiles_Toolbar *parent_tulbar = nullptr;
	MODAL_DIALOG *parent_modal_dialog = nullptr;
	
	

	bool is_locked = false;
public:

	int x = 0, y = 0, w = 0, h = 0;
	bool is_visible = false;
	wchar_t *label = nullptr;
	void mouse_left_button_down(GUI_low_level *low_level, int mx, int my);
	void mouse_left_button_up(GUI_low_level *low_level);

	void(*func_button_pressed)(GUI_low_level *low_level, int type) = nullptr;

	bool its_me(int m_x, int m_y);

	bool is_pressed = false;
	void set_is_pressed(GUI_low_level *low_level, bool val);

	void lock();
	void unlock();

	void set_size(int x_, int y_, int w_, int h_);

	void Paint(GUI_low_level *low_level);

	TRANSFER_DIALOG2_DirsFiles_Button(int type_, TRANSFER_DIALOG2_DirsFiles_Toolbar *parent_, wchar_t *label_);
	TRANSFER_DIALOG2_DirsFiles_Button(int type_, MODAL_DIALOG *parent_, wchar_t *label_);
	
	~TRANSFER_DIALOG2_DirsFiles_Button();
};


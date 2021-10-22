#pragma once

class GUI_low_level;
class MODAL_DIALOG;
class FONT;

class TRANSFER_DIALOG2_DirsFiles_Label
{
private:
	
	MODAL_DIALOG *parent = nullptr;
	int x = 0, y = 0, w = 0, h = 0;
	bool is_locked = false;
	
	

public:
	wchar_t *label = nullptr; int label_len = 0;
	bool is_visible = false;
	int font_idx = 4;
	int label_color = 0;

	int text_width(GUI_low_level *low_level);

	bool its_me(int m_x, int m_y);

	void lock();
	void unlock();

	void set_label(GUI_low_level *low_level, wchar_t *val);

	void set_size(int x_, int y_, int w_, int h_);

	void Paint(GUI_low_level *low_level);
	void Paint_right(GUI_low_level *low_level);
	void Paint_center(GUI_low_level *low_level);

	TRANSFER_DIALOG2_DirsFiles_Label(MODAL_DIALOG *parent_);
	~TRANSFER_DIALOG2_DirsFiles_Label();
};


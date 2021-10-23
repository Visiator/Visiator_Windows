#pragma once

class GUI_low_level;
class TRANSFER_DIALOG2_DirsFiles;

class TRANSFER_DIALOG2_DirsFiles_VScroll
{
private:
	
	TRANSFER_DIALOG2_DirsFiles *parent = nullptr;

	int begin_move_polzunok_dy = 0;

public:

	bool top_arrow_is_pressed = false;    unsigned int top_arrow_is_pressed_tick = 0;
	bool bottom_arrow_is_pressed = false; unsigned int bottom_arrow_is_pressed_tick = 0;
	bool polzunok_is_pressed = false;

	bool PgUp_polzunok_is_pressed = false;   unsigned int PgUp_polzunok_is_pressed_tick = 0;
	bool PgDown_polzunok_is_pressed = false; unsigned int PgDown_polzunok_is_pressed_tick = 0;

	int x = 0, y = 0, w = 0, h = 0;
	void set_size(int x_, int y_, int w_, int h_);

	void Paint(GUI_low_level *low_level);

	bool its_me(int m_x, int m_y);

	void mouse_over(GUI_low_level *low_level, int mouse_x, int mouse_y);

	void mouse_left_button_down(GUI_low_level *low_level, int mouse_x, int mouse_y);
	void mouse_left_button_up(GUI_low_level *low_level);

	void top_arrow_press(GUI_low_level *low_level);
	void bottom_arrow_press(GUI_low_level *low_level);

	void top_arrow_unpress(GUI_low_level *low_level);
	void bottom_arrow_unpress(GUI_low_level *low_level);

	void polzunok_PgUp_pressed(GUI_low_level *low_level);
	void polzunok_PgDown_pressed(GUI_low_level *low_level);

	void polzunok_PgUp_unpressed(GUI_low_level *low_level);
	void polzunok_PgDown_unpressed(GUI_low_level *low_level);

	void polzunok_press(GUI_low_level *low_level, int mx, int my);
	void polzunok_unpress(GUI_low_level *low_level);

	TRANSFER_DIALOG2_DirsFiles_VScroll(TRANSFER_DIALOG2_DirsFiles *parent_);
	~TRANSFER_DIALOG2_DirsFiles_VScroll();

};


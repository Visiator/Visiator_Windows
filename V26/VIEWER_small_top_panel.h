#pragma once

#include "VIEWER.h"

class VIEWER;
class TEXTURA;
class GUI_low_level;

class VIEWER_small_top_panel
{
public:
	//GUI_low_level *low_level = NULL;

	int panel_x= 100, panel_y = 30, panel_w = 185;
	int s1_x = 14, s1_y = 3; // смещение первой кнопки
	int btn_wm = 23; // ширина кнопки вместе с промежутком
	int btn_mouse_over = -1;
	int btn_mouse_press = -1;

	int move_delta_mx = 0, move_delta_my = 0;
	bool is_move = false, is_visible_ = false, is_pin_fixed = false;

	unsigned int color_unselect = 0x77ff77, color_select = 0xaaffaa, color_pressed = 0xffffff;
	unsigned int calc_btn_color(int idx);


	TEXTURA *panel_bg = nullptr;
	TEXTURA *btn_clipboard_send = nullptr, *btn_clipboard_get = nullptr, *btn_f1 = nullptr, *btn_CAD = nullptr, *btn_Lg = nullptr, *btn_normal_view = nullptr, *btn_fullscreen_view = nullptr;// , *btn_close = NULL;
	TEXTURA *btn_pin_open = nullptr, *btn_pin_fixed = nullptr;
	VIEWER *parent = nullptr;
	
	bool l_mouse_down(GUI_low_level *low_level, int mx, int my);
	bool l_mouse_up(GUI_low_level *low_level, int mx, int my);
	bool mouse_over(GUI_low_level *low_level, int mx, int my);
	void resize(int mx, int my);


	void paint(GUI_low_level *low_level);

	VIEWER_small_top_panel(VIEWER *parent_);
	~VIEWER_small_top_panel();
};


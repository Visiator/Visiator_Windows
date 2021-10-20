#include "stdafx.h"

#include "VIEWER.h"
#include "VIEWER_small_top_panel.h"
#include "Application_Attributes.h"
#include "tools.h"
#include "GUI.h"
//#include "mem.h"

//extern my_GUI *gui;

VIEWER_small_top_panel::VIEWER_small_top_panel(VIEWER *parent_) {
	//low_level = l;
	parent = parent_;
}


VIEWER_small_top_panel::~VIEWER_small_top_panel()
{
}

void VIEWER_small_top_panel::resize(int mx, int my) {
	panel_x = mx / 2 - panel_w / 2;
	panel_y = 15;
}

bool VIEWER_small_top_panel::l_mouse_down(GUI_low_level *low_level, int mx, int my) {

	if (is_visible_ == false) return false;

	if (mx < panel_x || mx > panel_x + panel_w || my < panel_y || my > panel_y + 26) return false;
	if (mx < panel_x + s1_x || mx > panel_x + s1_x + btn_wm * 7 ) {
		move_delta_mx = panel_x - mx;
		move_delta_my = panel_y - my;
		is_move = true;
		return true;
	}

	if (btn_mouse_press == -1) {

		int x, idx;
		x = panel_x + s1_x;
		idx = -1;
		if (mx > x && mx < x + btn_wm) { idx = 0; }; x += btn_wm;
		if (mx > x && mx < x + btn_wm) { idx = 1; }; x += btn_wm;
		if (mx > x && mx < x + btn_wm) { idx = 2; }; x += btn_wm;
		if (mx > x && mx < x + btn_wm) { idx = 3; }; x += btn_wm;
		if (mx > x && mx < x + btn_wm) { idx = 4; }; x += btn_wm;
		if (mx > x && mx < x + btn_wm) { idx = 5; }; x += btn_wm;
		if (mx > x && mx < x + btn_wm) { idx = 6; }; x += btn_wm;
		if (btn_mouse_press != idx) {
			btn_mouse_press = idx;

			if (btn_mouse_press == 0) {
				// 2021 09 parent->send_Clipboard_to_partner();
			};
			if (btn_mouse_press == 1) {
				// 2021 09 parent->request_Clipboard_from_partner();
			};
			if (btn_mouse_press == 2) {
				// 2021 09 parent->char_keydown( 0x100, 0x70, 0x3b0001); // press F1
				// 2021 09 parent->char_keyup(0x101, 0x70, 0x3b0001); // unpress F1
			};
			if (btn_mouse_press == 3) parent->send_CtrlAltDel();
			if (btn_mouse_press == 4) parent->send_Change_LNG();
			if (btn_mouse_press == 5) {
				
				

				if (parent->view_mode == VIEW_MODE_STRETCH) { parent->change_view_mode(VIEW_MODE_FULLSCREEN); }
				else {
					if (parent->view_mode == VIEW_MODE_FULLSCREEN) { parent->change_view_mode(VIEW_MODE_STRETCH); };
				};

				btn_mouse_press = -1;
			};
			if (btn_mouse_press == 6) {
				if (is_pin_fixed == true) is_pin_fixed = false; else is_pin_fixed = true;
				
			};
			low_level->invalidate();
		}
	};
	return true;
}

bool VIEWER_small_top_panel::l_mouse_up(GUI_low_level *low_level, int mx, int my) {

	if (is_move == true || btn_mouse_press != -1 ) {
		btn_mouse_press = -1;
		is_move = false;
		low_level->invalidate();
		return true;
	}

	
	return false;
}


bool VIEWER_small_top_panel::mouse_over(GUI_low_level *low_level, int mx, int my) {

	if (is_visible_ == false) return false;

	if (mx < panel_x || mx > panel_x + panel_w || my < panel_y || my > panel_y + 26 || mx < panel_x + s1_x) {
		if (btn_mouse_over != -1) {
			btn_mouse_over = -1;
			low_level->invalidate();
		};
		return false;
	};

	int idx; // , i
	idx = -1;
	int x;
	x = panel_x + s1_x;

	if (mx >= x && mx <= x + btn_wm) { idx = 0; }; x += btn_wm;
	if (mx >= x && mx <= x + btn_wm) { idx = 1; }; x += btn_wm;
	if (mx >= x && mx <= x + btn_wm) { idx = 2; }; x += btn_wm;
	if (mx >= x && mx <= x + btn_wm) { idx = 3; }; x += btn_wm;
	if (mx >= x && mx <= x + btn_wm) { idx = 4; }; x += btn_wm;
	if (mx >= x && mx <= x + btn_wm) { idx = 5; }; x += btn_wm;
	if (mx >= x && mx <= x + btn_wm) { idx = 6; }; x += btn_wm;
	if (btn_mouse_over != idx) {
		btn_mouse_over = idx;
		low_level->invalidate();
	};
	return true;
};
unsigned int VIEWER_small_top_panel::calc_btn_color(int idx) {
	if (idx == btn_mouse_press) return color_pressed;
	if (idx == btn_mouse_over) return color_select;
	return color_unselect;
}
void VIEWER_small_top_panel::paint(GUI_low_level *low_level) {

	if (is_visible_ == false) return;

	if (parent->view_mode != VIEW_MODE_STRETCH && parent->view_mode != VIEW_MODE_FULLSCREEN) return;

	if (btn_clipboard_send == NULL) {
		
		btn_clipboard_send = new TEXTURA(1000);
		btn_clipboard_get = new TEXTURA(1001);
		btn_f1 = new TEXTURA(1002);
		btn_CAD = new TEXTURA(1003);
		btn_Lg = new TEXTURA(1004);
		btn_normal_view = new TEXTURA(1005);
		btn_fullscreen_view = new TEXTURA(1008);
		//btn_close = new_TEXTURA(1007, 740);
		btn_pin_open = new TEXTURA(1009);
		btn_pin_fixed = new TEXTURA(1010);

		panel_bg = new TEXTURA(1006);
	}

	//gui->low_level->fill_rectangle(500, 50, 200, 35, 0x99ff99);

	panel_bg->paint_transparent_green(low_level, panel_x, panel_y);

	//unsigned int color;

	

	int xx,  yy; // d,
	xx = panel_x + s1_x;
	yy = panel_y + s1_y;
	
	
	btn_clipboard_send->paint_mono(low_level, xx, yy, calc_btn_color(0)); xx += btn_wm;
	btn_clipboard_get->paint_mono(low_level, xx, yy, calc_btn_color(1)); xx += btn_wm;

	btn_f1->paint_mono(low_level, xx, yy, calc_btn_color(2)); xx += btn_wm;
	btn_CAD->paint_mono(low_level, xx, yy, calc_btn_color(3)); xx += btn_wm;
	btn_Lg->paint_mono(low_level, xx, yy, calc_btn_color(4)); xx += btn_wm;
	if (parent->view_mode == VIEW_MODE_STRETCH) { btn_fullscreen_view->paint_mono(low_level, xx, yy, calc_btn_color(5)); xx += btn_wm; };
	if (parent->view_mode == VIEW_MODE_FULLSCREEN) { btn_normal_view->paint_mono(low_level, xx, yy, calc_btn_color(5)); xx += btn_wm; };

	if (is_pin_fixed == false) { btn_pin_open->paint_mono(low_level, xx, yy, calc_btn_color(6)); xx += btn_wm; }
	else { btn_pin_fixed->paint_mono(low_level, xx, yy, calc_btn_color(6)); xx += btn_wm; }
	
}


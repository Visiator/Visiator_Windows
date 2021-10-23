#include "stdafx.h"

#include "TRANSFER_DIALOG2_DirsFiles.h"
#include "TRANSFER_DIALOG2_DirsFiles_VScroll.h"
#include "TRANSFER_DIALOG2_DirsFiles_TREE.h"
#include "GUI_low_level.h"

TRANSFER_DIALOG2_DirsFiles_VScroll::TRANSFER_DIALOG2_DirsFiles_VScroll(TRANSFER_DIALOG2_DirsFiles *parent_) {
	
	parent = parent_;
}
TRANSFER_DIALOG2_DirsFiles_VScroll::~TRANSFER_DIALOG2_DirsFiles_VScroll() {

}
void TRANSFER_DIALOG2_DirsFiles_VScroll::set_size(int x_, int y_, int w_, int h_) {
	x = x_;
	y = y_;
	w = w_;
	h = h_;
	if (w < 0) w = 0;
	if (h < 0) h = 0;
}

void TRANSFER_DIALOG2_DirsFiles_VScroll::Paint(GUI_low_level *low_level) {

	int global_x, global_y;
	global_x = parent->get_global_x();
	global_y = parent->get_global_y();

	unsigned int top_arrow_bg_color, bottom_arrow_bg_color, polzunok_bg_color;

	// общий полигон
	//low_level->fill_rectangle(x + global_x, y + global_y, w, h, 0xccFFcc, 255);

	// верхняя стрелка
	if (top_arrow_is_pressed) { top_arrow_bg_color = TrDialog_ScrollBar_bgColor_Tri_Pressed; }
	else { top_arrow_bg_color = TrDialog_ScrollBar_bgColor_Tri_Normal; }
	low_level->fill_rectangle(x + global_x, y + global_y, w, _V_Scroll_w, top_arrow_bg_color, 255); // top arrow (up)
	low_level->paint_tre_up(x + global_x+4, y + global_y+10, 12, 0xffffff);

	// нижняя стрелка
	if (bottom_arrow_is_pressed) { bottom_arrow_bg_color = TrDialog_ScrollBar_bgColor_Tri_Pressed; }
	else { bottom_arrow_bg_color = TrDialog_ScrollBar_bgColor_Tri_Normal; }
	low_level->fill_rectangle(x + global_x, y + global_y + h - _V_Scroll_w , w, _V_Scroll_w, bottom_arrow_bg_color, 255);
	low_level->paint_tre_down(x + global_x+4, y + global_y + h - _V_Scroll_w +7, w, 0xffffff);

	// ползунок
	if (polzunok_is_pressed) { polzunok_bg_color = TrDialog_ScrollBar_bgColor_Polzunok_Pressed; }
	else { polzunok_bg_color = TrDialog_ScrollBar_bgColor_Polzunok_Normal; };
	low_level->fill_rectangle(x + global_x, y + global_y + _V_Scroll_w + 0 + parent->Tree->polzunok_position_in_pixel , w, parent->Tree->polzunok_size_in_pixel-0, polzunok_bg_color, 255 );


	unsigned int clr1;
	
	clr1 = TrDialog_ScrollBar_bgColor_Bar_Normal;

	if (PgUp_polzunok_is_pressed == true) clr1 = TrDialog_ScrollBar_bgColor_Bar_Pressed;

	// верхняя часть над ползунком
	low_level->fill_rectangle(x + global_x, y + global_y + _V_Scroll_w, _V_Scroll_w, parent->Tree->polzunok_position_in_pixel, clr1, 255);

	int y0, h0;

	y0 = y + global_y + _V_Scroll_w + parent->Tree->polzunok_position_in_pixel + parent->Tree->polzunok_size_in_pixel;

	h0 = h - _V_Scroll_w - _V_Scroll_w - (parent->Tree->polzunok_position_in_pixel + parent->Tree->polzunok_size_in_pixel);

	clr1 = TrDialog_ScrollBar_bgColor_Bar_Normal;

	if (PgDown_polzunok_is_pressed == true) clr1 = TrDialog_ScrollBar_bgColor_Bar_Pressed;


	low_level->fill_rectangle(x + global_x, y0, _V_Scroll_w, h0, clr1, 255);
}

bool TRANSFER_DIALOG2_DirsFiles_VScroll::its_me(int m_x, int m_y) {
	if (m_x >= x && m_x < x + w && m_y >= y && m_y < y + h) {
		return true;
	}
	return false;
}

void TRANSFER_DIALOG2_DirsFiles_VScroll::mouse_over(GUI_low_level *low_level, int mouse_x, int mouse_y) {
	int local_local_my;
	local_local_my = mouse_y - y - parent->y;

	if (polzunok_is_pressed) {
		parent->Tree->polzunok_position_in_pixel = local_local_my - _V_Scroll_w - begin_move_polzunok_dy;
		

		if (parent->Tree->polzunok_position_in_pixel > h - _V_Scroll_w*2 - parent->Tree->polzunok_size_in_pixel) {
			parent->Tree->polzunok_position_in_pixel = h - _V_Scroll_w * 2 - parent->Tree->polzunok_size_in_pixel;
		}
		if (parent->Tree->polzunok_position_in_pixel < 0) parent->Tree->polzunok_position_in_pixel = 0;

		int vv;
		vv = h - _V_Scroll_w * 2 - parent->Tree->polzunok_size_in_pixel;
		if (vv == 0) {
			parent->Tree->SET_polzunok_position_in_percent(low_level,  0 );
		}
		else {
			parent->Tree->SET_polzunok_position_in_percent(low_level, (float)parent->Tree->polzunok_position_in_pixel / (float)vv * (float)100 );

		};

		parent->Tree->generate_info();
		low_level->invalidate();
	}
}

void TRANSFER_DIALOG2_DirsFiles_VScroll::mouse_left_button_down(GUI_low_level *low_level, int mouse_x, int mouse_y) {
	
	int local_local_mx, local_local_my;

	local_local_mx = mouse_x - x;
	local_local_my = mouse_y - y;

	if (local_local_my >= 0 && local_local_my < _V_Scroll_w) {
		// top arrow pressed
		top_arrow_is_pressed = true;
		low_level->invalidate();

		top_arrow_press(low_level);

		return;
	}

	if (local_local_my >= h - _V_Scroll_w && local_local_my < h) {
		bottom_arrow_is_pressed = true;
		low_level->invalidate();

		bottom_arrow_press(low_level);

		return;
	}


	if(local_local_my >= _V_Scroll_w + parent->Tree->polzunok_position_in_pixel && local_local_my < _V_Scroll_w + parent->Tree->polzunok_position_in_pixel + parent->Tree->polzunok_size_in_pixel) {
		// top arrow pressed
		polzunok_is_pressed = true;
		low_level->invalidate();

		polzunok_press(low_level, local_local_mx, local_local_my - _V_Scroll_w);

		return;
	}

	if (local_local_my - _V_Scroll_w > 0 && local_local_my - _V_Scroll_w < parent->Tree->polzunok_position_in_pixel) {	
		PgUp_polzunok_is_pressed = true;		
		low_level->invalidate();

		polzunok_PgUp_pressed(low_level);
		return;
	}

	if (local_local_my - _V_Scroll_w > parent->Tree->polzunok_position_in_pixel + parent->Tree->polzunok_size_in_pixel && local_local_my < h - _V_Scroll_w) {
		PgDown_polzunok_is_pressed = true;
		low_level->invalidate();

		polzunok_PgDown_pressed(low_level);
		return;
	}

	/*
	if (local_local_my - _V_Scroll_w > 0 && local_local_my - _V_Scroll_w < parent->Tree->polzunok_position_in_pixel) {
		PgUp_polzunok_is_pressed = true;
		low_level->invalidate();

		polzunok_PgUp_pressed();
		return;
	}
	*/

}

void TRANSFER_DIALOG2_DirsFiles_VScroll::mouse_left_button_up(GUI_low_level *low_level) {

	if (top_arrow_is_pressed == true) {
		top_arrow_unpress(low_level);
		low_level->invalidate();
	}
	if (bottom_arrow_is_pressed == true) {
		bottom_arrow_unpress(low_level);
		low_level->invalidate();
	}
	if (polzunok_is_pressed == true) {
		polzunok_is_pressed = false;
		polzunok_unpress(low_level);
		low_level->invalidate();
	}

	if (PgUp_polzunok_is_pressed == true) {
		PgUp_polzunok_is_pressed = false;
		polzunok_PgUp_unpressed(low_level);
		low_level->invalidate();
	}

	if (PgDown_polzunok_is_pressed == true) {
		PgDown_polzunok_is_pressed = false;
		polzunok_PgDown_unpressed(low_level);
		low_level->invalidate();
	}
}

void TRANSFER_DIALOG2_DirsFiles_VScroll::top_arrow_press(GUI_low_level *low_level) {
	top_arrow_is_pressed_tick = GetTickCount() + 800; // следующее автонажатие нужно сдеклать через этот промежуток времени
	parent->Tree->top_arrow_click(low_level);
}

void TRANSFER_DIALOG2_DirsFiles_VScroll::bottom_arrow_press(GUI_low_level *low_level) {
	bottom_arrow_is_pressed_tick = GetTickCount() + 800; // следующее автонажатие нужно сдеклать через этот промежуток времени
	parent->Tree->bottom_arrow_click(low_level);
}


void TRANSFER_DIALOG2_DirsFiles_VScroll::top_arrow_unpress(GUI_low_level *low_level) {
	top_arrow_is_pressed_tick = 0;
	top_arrow_is_pressed = false;
}

void TRANSFER_DIALOG2_DirsFiles_VScroll::bottom_arrow_unpress(GUI_low_level *low_level) {
	bottom_arrow_is_pressed_tick = 0;
	bottom_arrow_is_pressed = false;
}

void TRANSFER_DIALOG2_DirsFiles_VScroll::polzunok_PgUp_pressed(GUI_low_level *low_level) {
	PgUp_polzunok_is_pressed_tick = GetTickCount() + 800; // следующее автонажатие нужно сдеклать через этот промежуток времени
	parent->Tree->PgUp_polzunok_click(low_level);
}
void TRANSFER_DIALOG2_DirsFiles_VScroll::polzunok_PgUp_unpressed(GUI_low_level *low_level) {
	PgUp_polzunok_is_pressed_tick = 0;
	PgUp_polzunok_is_pressed = false;
}
void TRANSFER_DIALOG2_DirsFiles_VScroll::polzunok_PgDown_pressed(GUI_low_level *low_level) {
	PgDown_polzunok_is_pressed_tick = GetTickCount() + 800; // следующее автонажатие нужно сдеклать через этот промежуток времени
	parent->Tree->PgDown_polzunok_click(low_level);
}
void TRANSFER_DIALOG2_DirsFiles_VScroll::polzunok_PgDown_unpressed(GUI_low_level *low_level) {
	PgDown_polzunok_is_pressed_tick = 0;
	PgDown_polzunok_is_pressed = false;
}

void TRANSFER_DIALOG2_DirsFiles_VScroll::polzunok_press(GUI_low_level *low_level, int mx, int my) {
	begin_move_polzunok_dy = my - parent->Tree->polzunok_position_in_pixel;

	//parent->Tree->polzunok_position = 20;

}

void TRANSFER_DIALOG2_DirsFiles_VScroll::polzunok_unpress(GUI_low_level *low_level) {

}
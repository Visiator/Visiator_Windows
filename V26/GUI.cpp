#include "stdafx.h"

#include "GUI.h"
#include "FONT.h"

extern bool GLOBAL_STOP;

extern FONT *font[10];

GUI::GUI(HWND hw_) {
	hw = hw_;
	if (low_level == nullptr) {
		low_level = new GUI_low_level(hw_);
	}
}

void GUI::change_size(HWND hw_, int w_, int h_) {
	if (low_level == nullptr) return;
	low_level->change_size(hw_, w_, h_);
}

void GUI::invalidate() {
	low_level->invalidate();
}

void GUI::Paint() {
	//const_iterator it, end;
	std::list <GUI_Element>::iterator it, end;

	for (it = elements.begin(), end = elements.end(); it != end; ++it)
	{
		// do nothing, for loop terminates if "it" points to "myElem"
		  // or if we don't find your element.
		(*it).Paint(low_level);
	};

	

	low_level->Paint();
}

GUI_Element *GUI::add_element(int type_, int x_, int y_, int w_, int h_, uint32_t color_) {

	elements.emplace_back(type_, x_, y_, w_, h_, color_);
	return &(elements.back());
}

GUI_Element *GUI::get_active_Element_mouse_over(int mx, int my) {

	std::list <GUI_Element>::iterator it, end;

	for (it = elements.begin(), end = elements.end(); it != end; ++it)
	{
		// do nothing, for loop terminates if "it" points to "myElem"
		// or if we don't find your element.
		//it->mouse_move(mx, my);
		bool visible;
		visible = true;

		if (it->is_visible == false || (it->parent != nullptr && it->parent->is_visible == false)) {
			visible = false;
		};
		if (it->is_active && visible == true
			&& it->x <= mx
			&& it->x + it->w > mx
			&& it->y <= my
			&& it->y + it->h > my) {

			return &*it;
		}
		
	}

	return nullptr;
}

void GUI::left_button_mouse_up(int mx, int my) {

	/*
	GUI_Element *q;

	q = nullptr;

	q = get_active_Element_mouse_over(mx, my);
	*/

	std::list <GUI_Element>::iterator it, end;

	for (it = elements.begin(), end = elements.end(); it != end; ++it)
	{
		if (it->func__mouse_unpress != nullptr) {
			it->func__mouse_unpress(&*it, mx, my);
		}

		/*
		// сбросим все отметки is_mouse_pressed

		if (it->is_mouse_pressed == true) {
			
			if (it->is_mouse_pressed == true) {
				it->set_mouse_pressed(false);
				invalidate();
			}
			if (&*it == q) {
				if (it->func__mouse_unpress != nullptr) {
					it->func__mouse_unpress();
				}
				return;
			}
			
		}
		*/
	}

	
}


void GUI::left_button_mouse_down(int mx, int my) {
	
	GUI_Element *q;

	q = nullptr;

	q = get_active_Element_mouse_over(mx, my);

	
	std::list <GUI_Element>::iterator it, end;


	for (it = elements.begin(), end = elements.end(); it != end; ++it)
	{
		// сбросим все отметки is_mouse_pressed
		
			

			if (q == nullptr || &*it != q) {
				if (it->is_edit_begin) {
					it->edit_end();
					invalidate();
				}
				if (it->func__mouse_unpress != nullptr) {
					it->func__mouse_unpress(&*it, mx, my);
				}
			}
			else {
				if (it->func__mouse_press != nullptr) {
					if (it->it_me(mx, my)) {
						it->func__mouse_press(mx, my);
					};
				}
			}
		
	}

	if (q != nullptr && q->type == GUI_Element_Type_edit) {
		if (q->is_active == true) {
			if (q->is_edit_begin == false) {
				q->edit_begin();
			}
			if (mx > q->x && mx < q->x + q->w) {
				if (mx > q->x + q->w - 18) {
					if (q->pass_eye.is_eye_open) {
						q->pass_eye.is_eye_open = false;
					}
					else {
						q->pass_eye.is_eye_open = true;
					}
				}
			}
		}
		
	}
	invalidate();
}

void GUI::mouse_move(int mx, int my) {

	GUI_Element *q;

	q = nullptr;

	q = get_active_Element_mouse_over(mx, my);

	std::list <GUI_Element>::iterator it, end;

	for (it = elements.begin(), end = elements.end(); it != end; ++it)
	{
		// сбросим все отметки is_mouse_hover

		if (it->is_mouse_hover == true) {
			if (q == nullptr || &*it != q) {
				if (it->is_mouse_hover == true) {
					it->set_mouse_hover(false);
					invalidate();
				}
			}
			else {

			}
		}
	}

	if (q != nullptr && q->is_mouse_hover == false) {
		q->set_mouse_hover(true);
		invalidate();
	}

}

void GUI::close_application() {
	GLOBAL_STOP = true;
	PostMessage(hw, WM_DESTROY, 0, 0);
}


void GUI::char_keydown(int msg, int wp, int lp, wchar_t *wbuffer) {
	
	std::list <GUI_Element>::iterator it, end;

	for (it = elements.begin(), end = elements.end(); it != end; ++it)
	{
		// сбросим все отметки is_mouse_hover

		if (it->is_edit_begin) {
			it->char_keydown(msg, wp, lp, wbuffer);
			invalidate();
		}
	}

}

void GUI::char_keyup(int msg, int wp, int lp) {

	std::list <GUI_Element>::iterator it, end;

	for (it = elements.begin(), end = elements.end(); it != end; ++it)
	{
		// сбросим все отметки is_mouse_hover

		if (it->is_edit_begin) {
			it->char_keyup(msg, wp, lp);
			invalidate();
		}
	}

}

void GUI::char_(int msg, int wp, int lp) {
	/* 2021 -
	if (app_attributes.modal_process == 0)
		elements->char_(msg, wp, lp);
	low_level->invalidate();
	*/
}

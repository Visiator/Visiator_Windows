

#include "GUI_Element.h"
#include "GUI_low_level.h"
#include "FONT.h"
#include "VIEWER.h"

extern VIEWER  *viewer;

extern FONT *font[10];

GUI_Element::GUI_Element(int type_, int x_, int y_, int w_, int h_,  uint32_t color_) {
	clean();
	type = type_;
	x = x_;
	y = y_;
	w = w_;
	h = h_;
	color = color_;
	is_active = false;
	if (type_ == GUI_Element_Type_button) {
		is_active = true;
	}
}

GUI_Element::~GUI_Element() {

}


void GUI_Element::set_mouse_hover(bool val) {
	if (val == true) {
		if (is_mouse_hover != true) {
			is_mouse_hover = true;
		}
	}
	else {
		if (is_mouse_hover != false) {
			is_mouse_hover = false;
		}
	}
}

void GUI_Element::set_mouse_pressed(bool val) {
	if (val == true) {
		if (is_mouse_pressed != true) {
			is_mouse_pressed = true;
		}
	}
	else {
		if (is_mouse_pressed != false) {
			is_mouse_pressed = false;
		}
	}
}


void GUI_Element::clean() {
	type = 0;
	x = 0;
	y = 0;
	w = 0; 
	h = 0;
	color = 0;
	is_visible = true;
	is_active = false;
	is_mouse_hover = false;
	is_mouse_pressed = false;
}

void GUI_Element::Paint(GUI_low_level *low_level) {

	
	uint32_t clr;
	int cur_pos;



	if (is_visible == false) return;
	if (parent != nullptr && parent->is_visible == false) return;

	if(color != 1) low_level->rectangle(x, y, w, h, color);

	if (type == GUI_Element_Type_viewer) {
		wchar_t ss[500];
		ss[0] = 0;
		
		if (viewer->prepare_pass_tik >= 0 && viewer->prepare_pass_tik < 20) {
			wsprintf(ss, L"prepare pass %d", viewer->prepare_pass_tik);
		}
		else {
			swprintf_s(ss, 450, L"R=%lld S=%lld", viewer->net_client_session->recv__counter, viewer->net_client_session->send__countern);
		}

		font[0]->paintAAA(low_level, 300/2 - font[0]->text_width(ss)/2, 80, ss, 0x999999, -1, false);
		font[1]->paintAAA(low_level, 300/2 - font[1]->text_width(L"123-456-321")/2, 55, L"123-456-321", 0x999999, -1, false);
		return;
	}

	if (type == GUI_Element_Type_indicator) {

		for (int i = 0; i < 20; i++) {
			if (i < cursor_position) clr = 0;
			else clr = 0xdddddd;
			low_level->line_v(x+i*5 + 15, y+7, 20, clr);
		};
		return;
	}

	if (is_mouse_pressed == true && textura_pressed != nullptr) {
		textura_pressed->Paint(low_level, x, y);
		return;
	}
	else {
		if (is_mouse_hover == true && textura_hover != nullptr) {
			textura_hover->Paint(low_level, x, y);
			return;
		}
		else {
			if (textura_normal != nullptr) {
				textura_normal->Paint(low_level, x, y);
			}
		};
	};
	if (type == GUI_Element_Type_edit) {

		//if (text.empty() == false) 
		{
			if (is_active == true && is_edit_begin == true) {
				cur_pos = cursor_position;
			}
			else {
				cur_pos = -1;
			}
			bool cc;
			FONT *fn;
			int dy, i, w;
			if (font[1]->text_width(text.c_str()) < 130) {
				fn = font[1];
				dy = 0;
			}
			else {
				fn = font[0];
				dy = 4;
			}
			cc = false;
			if (is_password && pass_eye.is_eye_open == false) cc = true;
			fn->paintAAA(low_level, x, y + dy, text.c_str(), 0x000000, cur_pos, cc);
			/*i = 0;
			w = 0;
			while (i < text.length()) {
				cc = false;
				w += fn->bukva[150].paintAAA(low_level, x+w, y + dy, 0x000000, cc);
				i++;
			};*/

			if (is_password == true && pass_eye.eye != nullptr && pass_eye.is_eye_open == false) {
				pass_eye.eye->Paint(low_level, x + 134, y + 5);
			}
			if (is_password == true && pass_eye.eye_open != nullptr && pass_eye.is_eye_open == true) {
				pass_eye.eye_open->Paint(low_level, x + 134, y + 5);
			}
		};
		
	}
}

void GUI_Element::load_BMP_from_resource(int id_normal, int id_hover, int id_pressed) {
	if (id_normal != 0 && textura_normal == nullptr) {
		textura_normal = new TEXTURA(id_normal);
	}
	if (id_hover != 0 && textura_hover == nullptr) {
		textura_hover = new TEXTURA(id_hover);
	}
	if (id_pressed != 0 && textura_pressed == nullptr) {
		textura_pressed = new TEXTURA(id_pressed);
	}
}

void GUI_Element::set_text(wchar_t *txt_) {
	text.clear();
	text = txt_;
}

void GUI_Element::edit_begin() {
	if (is_edit_begin == true) return;
	is_edit_begin = true;
	cursor_position = text.length();
}

void GUI_Element::edit_end() {
	if (is_edit_begin == false) return;
	is_edit_begin = false;
}

void GUI_Element::char_keydown(int msg, int wp, int lp, wchar_t *wbuffer) {
	if (wp == VK_LEFT) {
		if (is_password == true && cursor_position > 0) {
			cursor_position--;
		}
		return;
	}
	if (wp == VK_RIGHT) {
		if (is_password == true && cursor_position < text.length()) {
			cursor_position++;
		}
		return;
	};
	if (wp == VK_BACK) {
		if (cursor_position > 0) {
			text.erase(cursor_position-1, 1);
			cursor_position--;
			if (is_password == false && (text.length() == 4 || text.length() == 8)) {
				text.erase(cursor_position - 1, 1);
				cursor_position--;
			}
		}

		return;
	}
	if (wp == VK_DELETE) {
		text.erase(cursor_position, 1);
		return;
	}
	if (wbuffer[0] != 0) {
		if (is_password == false) {
			if (text.length() < 11) {
				if (wbuffer[0] >= L'0' && wbuffer[0] <= L'9') {
					text.insert(cursor_position, wbuffer);
					cursor_position++;
					if (text.length() == 3 || text.length() == 7) {
						text.insert(cursor_position, L"-");
						cursor_position++;
					}
				}
			};
		}
		else {
			if (text.length() < 16) {
				text.insert(cursor_position, wbuffer);
				cursor_position++;
			};
		};
	}
}

void GUI_Element::char_keyup(int msg, int wp, int lp) {

}


PASS_EYE::PASS_EYE() {

}
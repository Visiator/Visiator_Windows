

#include "GUI_Element.h"
#include "GUI_low_level.h"
#include "FONT.h"
#include "VIEWER.h"

extern VIEWER  *viewer;

extern FONT *font[10];

CHESS_SHADOW chess_shadow;

void show_mouse_pointer(unsigned int *v1, unsigned int color, unsigned int window_w, int ww, int hh);

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
		
		if (viewer->view_mode == VIEW_MODE_NOCONNECT) {

			if (viewer->prepare_pass_tik >= 0 && viewer->prepare_pass_tik < 20) {
				wsprintf(ss, L"prepare pass %d", viewer->prepare_pass_tik);
			}
			else {
				swprintf_s(ss, 450, L"R=%lld S=%lld", viewer->net_client_session->recv__counter, viewer->net_client_session->send__countern);
			}

			font[0]->paintAAA(low_level, 300 / 2 - font[0]->text_width(ss) / 2, 80, ss, 0x999999, -1, false);
			font[1]->paintAAA(low_level, 300 / 2 - font[1]->text_width(L"123-456-321") / 2, 55, L"123-456-321", 0x999999, -1, false);

		}
		else {
			if (viewer->view_mode == VIEW_MODE_NOCONNECT) {

			}
		}
		Pain_VIEWER(low_level);

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


void GUI_Element::Pain_VIEWER(GUI_low_level *low_level) {
	SCREEN_LIGHT  *screen;	// *scr_fullsize_, *scr_resized,

	if (viewer == NULL || viewer->screen_light_from_server_fullsize == NULL) return;

	/*if (viewer->screen_light_from_server_resized->resized_w == low_level->window_w &&
		viewer->screen_light_from_server_resized->resized_h == low_level->window_h) {
		screen = viewer->screen_light_from_server_resized;
	}
	else {
		screen = viewer->screen_light_from_server_fullsize;
	}*/

	screen = viewer->screen_light_from_server_fullsize;
	screen->lock_READ(1000);




	if (screen->w == 0) {

		screen->unlock_READ(1001);

		if (font != NULL) {

			/* 2021 09
			if (viewer->prepare_status.txt != NULL && viewer->prepare_status.txt[0] != 0) {
				font->paint_text(1, 1, low_level->window_w, low_level->window_h, viewer->prepare_status.c_str(), 0xbe6100, 1, -1);
				if (viewer->net_client_session != NULL) {
					font->paint_text(1, 30, low_level->window_w, low_level->window_h, viewer->net_client_session->n_status.c_str(), 0xbe6100, 1, -1);
				}
			}
			else {
				if (viewer->net_client_session != NULL) {
					font->paint_text(1, 1, low_level->window_w, low_level->window_h, viewer->net_client_session->n_status.c_str(), 0xbe6100, 1, -1);
				}
			};
			*/
		};

		return;
	};
	if (screen->w > 0) {

		int x, xx, y, yy;
		int dx, dy, ww, hh, i;
		unsigned int *v, val, v_idx;
		float xxx, yyy, dd;
		unsigned int *sscr_buf;
		int window_w;

		if (viewer->view_mode == VIEW_MODE_STRETCH) {

			if (viewer->screen_light_from_server_resized->is_lock_READ == false
				&& viewer->screen_light_from_server_resized->resized_w == low_level->window_w
				&& viewer->screen_light_from_server_resized->resized_h == low_level->window_h
				) {
				screen->unlock_READ(1002);

				screen = viewer->screen_light_from_server_resized;
				screen->lock_READ(2000);


				if (low_level->window_w < screen->resized_w) xx = low_level->window_w; else xx = screen->resized_w;
				if (low_level->window_h < screen->resized_h) yy = low_level->window_h; else yy = screen->resized_h;

				dd = (float)screen->resized_w / (float)low_level->window_w;

				dx = 0;
				dy = 0;

				ww = low_level->window_w;
				hh = low_level->window_h;
				yyy = 0;
				v = screen->get_buf();

				window_w = low_level->window_w;

				for (y = 0; y < hh && (int)yyy + dx < screen->h; y++) {

					sscr_buf = low_level->buf + y * window_w;

					xxx = 0;
					for (x = 0; x < ww; x++) {

						v_idx = ((int)yyy + dy)*screen->w + (int)xxx + dx;

						val = v[v_idx];

						*sscr_buf++ = val;

						//low_level->set_pixx(x, y, val);
						xxx += dd;
					}
					yyy += dd;
				};

				// рисуем курсор мыши

				if (screen->_itis_user_move_mouse_ == 200) {
					if (viewer->aspect_h != 0 && viewer->aspect_w != 0) {

						if (screen->mouse_y >= 0 && screen->mouse_y < 0xffff &&
							screen->mouse_x >= 0 && screen->mouse_x < 0xffff) {

							yyy = (float)screen->mouse_y / viewer->aspect_h;
							xxx = (float)screen->mouse_x / viewer->aspect_w;

							unsigned int *v1;

							v1 = low_level->buf + (int)yyy * window_w + (int)xxx;

							show_mouse_pointer(v1, 0xff00ff, window_w, low_level->window_w - (int)xxx, (int)(low_level->window_h - (int)yyy));

						};

					};
				};
			}
			else {

				if (low_level->window_w < screen->w) xx = low_level->window_w; else xx = screen->w;
				if (low_level->window_h < screen->h) yy = low_level->window_h; else yy = screen->h;

				dd = (float)screen->w / (float)low_level->window_w;

				dx = 0;
				dy = 0;

				ww = low_level->window_w;
				hh = low_level->window_h;
				yyy = 0;
				v = screen->get_buf();



				window_w = low_level->window_w;


				for (y = 0; y < hh && (int)yyy + dx < screen->h; y++) {

					sscr_buf = low_level->buf + y * window_w;

					xxx = 0;
					for (x = 0; x < ww; x++) {

						v_idx = ((int)yyy + dy)*screen->w + (int)xxx + dx;

						val = v[v_idx];

						*sscr_buf++ = val;

						//low_level->set_pixx(x, y, val);
						xxx += dd;
					}
					yyy += dd;
				};
			};
			screen->unlock_READ(1010);
			return;
		};

		int k, o, j;
		unsigned int *sh;

		if (viewer->view_mode == VIEW_MODE_FULLSCREEN) {
			//low_level->fill_color(0xffff00);

			if (chess_shadow.len != low_level->window_w) chess_shadow.set_size(low_level->window_w);

			//int step1_size_h = 0, step1_size_w = 0;
			//int step2_size_h = 0, step21_size_w = 0, step22_size_x = 0, step22_size_w = 0;
			//int step3_size_h, step3_size_w;

			viewer->step1_size_h = (low_level->window_h - screen->h) / 2; if (viewer->step1_size_h < 0) viewer->step1_size_h = 0;
			viewer->step1_size_w = low_level->window_w; //if (step1_size_w > screen->w) step1_size_w = screen->w;

			viewer->step2_size_h = screen->h; if (viewer->step2_size_h > low_level->window_h) viewer->step2_size_h = low_level->window_h;

			viewer->step21_size_w = (low_level->window_w - screen->w) / 2; if (viewer->step21_size_w < 0) viewer->step21_size_w = 0;

			viewer->step22_size_x = viewer->step21_size_w + screen->w;
			viewer->step22_size_w = low_level->window_w - viewer->step22_size_x;

			viewer->step3_size_h = low_level->window_h - screen->h - viewer->step1_size_h;
			viewer->step3_size_w = low_level->window_w;


			window_w = low_level->window_w;

			y = 0;

			k = 0;
			o = 0;
			for (i = 0; i < viewer->step1_size_h; i++) {

				if (o == 0) { sh = chess_shadow.line_0; }
				else { sh = chess_shadow.line_2; }

				sscr_buf = low_level->buf + y * window_w;

				for (j = 0; j < viewer->step1_size_w; j++) {
					*sscr_buf++ = *sh++;	//low_level->set_pixx(j, y, *sh++);			
				};

				y++;

				k++;
				if (k > 5) {
					k = 0;
					o++;
					if (o > 1) o = 0;
				}
			}

			for (i = 0; i < viewer->step2_size_h; i++) {
				if (o == 0) { sh = chess_shadow.line_0; }
				else { sh = chess_shadow.line_2; }

				sscr_buf = low_level->buf + y * window_w;

				for (j = 0; j < viewer->step21_size_w; j++) {
					*sscr_buf++ = sh[j];// low_level->set_pixx(j, y, sh[j]); 
				};

				sscr_buf = low_level->buf + y * window_w + viewer->step22_size_x;

				for (j = 0; j < viewer->step22_size_w; j++) {
					*sscr_buf++ = sh[j + viewer->step22_size_x];
					//low_level->set_pixx(j + step22_size_x, y, sh[j + step22_size_x]);
				};
				y++;

				k++;
				if (k > 5) {
					k = 0;
					o++;
					if (o > 1) o = 0;
				}
			}

			for (i = 0; i < viewer->step3_size_h; i++) {
				if (o == 0) { sh = chess_shadow.line_0; }
				else { sh = chess_shadow.line_2; }

				sscr_buf = low_level->buf + y * window_w;

				for (j = 0; j < viewer->step3_size_w; j++) {
					*sscr_buf++ = *sh++;
					//low_level->set_pixx(j, y, *sh++);
				};

				y++;

				k++;
				if (k > 5) {
					k = 0;
					o++;
					if (o > 1) o = 0;
				}
			}

			/////////////////////////
			//screen->unlock();
			//return;
			/////////////////////////


			hh = low_level->window_h;
			if (hh > screen->h) hh = screen->h;

			ww = low_level->window_w;
			if (ww > screen->w) ww = screen->w;

			if ((hh - viewer->delta_y) <= screen->h) {

				y = 0;
				while (y < hh) {

					sscr_buf = low_level->buf + (y + viewer->step1_size_h) * window_w + viewer->step21_size_w;

					v = screen->get_buf();
					v += (y - viewer->delta_y) * screen->w - viewer->delta_x;
					x = 0;
					while (x < ww) {
						*sscr_buf++ = *v;
						//low_level->set_pixx(x+ step21_size_w, y+ step1_size_h, *v);

						v++;
						x++;
					}

					y++;
				}

			}
			else {
				//send_udp("WTF?");
			}

		};
		screen->unlock_READ(1011);
		return;

	}
	screen->unlock_READ(1012);
	return;



}

void show_mouse_pointer(unsigned int *v1, unsigned int color, unsigned int window_w, int ww, int hh) {
	int i;
	unsigned int *v2;

	if (ww > 13 && hh > 19) {
		v2 = v1;
		for (i = 0; i < 19; i++) {
			*v1 = color;
			v1 += window_w;

			if (i < 13) {
				*v2 = color;
				v2 += window_w + 1;
			};
		}

		for (i = 0; i < 7; i++) {
			*v1 = color;
			v1 -= (window_w - 1);

			*v2 = color;
			v2--;
		}
		return;
	}

	if (ww > 7 && hh > 7) {
		v2 = v1;
		for (i = 0; i < 7; i++) {
			*v1 = color;
			v1 += window_w;

			if (i < 7) {
				*v2 = color;
				v2 += window_w + 1;
			};
		}

		for (i = 0; i < 7; i++) {
			//*v1 = color;
			//v1 -= (window_w - 1);

			*v2 = color;
			v2--;
		}
		return;
	}

	if (ww > 3 && hh > 3) {
		v2 = v1;
		for (i = 0; i < 3; i++) {
			*v1 = color;
			v1 += window_w;

			if (i < 3) {
				*v2 = color;
				v2 += window_w + 1;
			};
		}

		for (i = 0; i < 3; i++) {
			//*v1 = color;
			//v1 -= (window_w - 1);

			*v2 = color;
			v2--;
		}
		return;
	}
	*v1 = color;
}

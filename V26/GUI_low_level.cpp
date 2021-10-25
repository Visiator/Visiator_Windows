#include "stdafx.h"

//#include "stdint.h"

#include "FONT.h"
#include "GUI_low_level.h"
#include "tools.h"

extern bool GLOBAL_STOP;

extern FONT *font[10];

GUI_low_level::GUI_low_level(HWND hw_) {
	hw = hw_;
}

void GUI_low_level::invalidate() {
	RECT r;
	r.left = 0;
	r.top = 0;
	r.right = 5000;
	r.bottom = 5000;
	InvalidateRect(hw, nullptr, FALSE);
}


void GUI_low_level::change_size(HWND hw, int w_, int h_) {
	if (w_ < 50) w_ = 50;
	if (h_ < 50) h_ = 50;

	//window_x = x;
	//window_y = y;


	if ((w_ * h_) > buf_size) {

		lock_FULL();

		if (buf != NULL) delete[] buf;

		buf = (unsigned int *)(new char[w_ * h_ * 4]); //neew(w_ * h_ * 4, sscr_buf, "MyGUI_low_level::set_window_size()");
		
		/*** 2021
		buf2 = (unsigned int *)(new char[w_ * h_ * 4]); //neew(w_ * h_ * 4, sscr_buf, "MyGUI_low_level::set_window_size()");
		***/

		buf_size = w_ * h_;
		window_w = w_;
		window_h = h_;

		unlock_FULL();

	}

	fill_color(0x1d364d);
	//for (int i = 0; i < 150; i++) buf[i] = 0xff0000;
	window_w = w_;
	window_h = h_;



	if (bmInfo != NULL) delete bmInfo;

	bmInfo = new BITMAPINFO(); //neew(sizeof(BITMAPINFO), bmInfo, "BITMAPINFO()");
	bmInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmInfo->bmiHeader.biBitCount = 32;
	bmInfo->bmiHeader.biClrUsed = 0;
	bmInfo->bmiHeader.biClrImportant = 0;
	bmInfo->bmiHeader.biPlanes = 1;
	bmInfo->bmiHeader.biCompression = BI_RGB;
	bmInfo->bmiHeader.biSizeImage = 0;
	bmInfo->bmiHeader.biHeight = window_h * -1;
	bmInfo->bmiHeader.biWidth = window_w;


}

void GUI_low_level::rectangle(int x, int y, int w, int h, uint32_t color) {
	unsigned int *q;
	if (buf == nullptr) return;
	if (x < 0 || y < 0 || w < 0 || h < 0 || x + w > window_w || y + h > window_h) return;
	for (int yy = y; yy < y+h; yy++) {
		q = &buf[yy*window_w + x];
		for (int xx = x; xx < x+w; xx++) {
			*q = color;
			q++;
		}
	}
}

void GUI_low_level::fill_color(unsigned int col) {
	int i;
	i = 0;
	while (i < buf_size) {
		buf[i] = col;
		i++;
	}
}


void GUI_low_level::lock_FULL() {
	while (GLOBAL_STOP == false) {

		if (is_lock_FULL == false && is_lock_READ == false && is_lock_WRITE == false) {
			enter_crit(7);
			if (is_lock_FULL == false && is_lock_READ == false && is_lock_WRITE == false) {
				is_lock_FULL = true;

				leave_crit(7);
				return;
			}
			leave_crit(7);
		}
		else {
			Sleep(0);
		}
	}
}
void GUI_low_level::unlock_FULL() {
	is_lock_FULL = false;

}

void GUI_low_level::lock_READ() {
	while (GLOBAL_STOP == false) {

		if (is_lock_FULL == false && is_lock_READ == false) {
			enter_crit(7);
			if (is_lock_FULL == false && is_lock_READ == false) {
				is_lock_READ = true;

				leave_crit(7);
				return;
			}
			leave_crit(7);
		}
		else {
			Sleep(0);
		}
	}
}
void GUI_low_level::unlock_READ() {
	is_lock_READ = false;

}

void GUI_low_level::set_pixx(int x, int y, uint32_t color)
{

	if (y < 0) {
		return;
	}
	if (x < 0 || y < 0 || x >= window_w || y >= window_h) {
		return;
	};
	buf[x + y * window_w] = color;
};

uint32_t GUI_low_level::get_pix(int x, int y)
{
	if (x < 0 || y < 0 || x >= window_w || y >= window_h) return 0;
	return buf[x + y * window_w];
};


void GUI_low_level::Paint() {

	PAINTSTRUCT ps;
	HDC hdc;

	hdc = BeginPaint(hw, &ps);

	lock_READ();

	int liness;

	unsigned int *q2, *q, r, g, b;

	int w2 = window_w * 2;

	/*** 2021
	q2 = &(buf2[(window_h - 1)* window_w]);
	q = buf;
	for (int y = 0; y < window_h; y++)
	{
		for (int x = 0; x < window_w; x++)
		{

			r = (*q) & 0xff0000; r = r >> 16;
			g = (*q) & 0x00ff00;
			b = (*q) & 0x0000ff; b = b << 16;

			*q2 = r | g | b;

			//*q2 = *q;

			q++;
			q2++;
		};
		q2 -= w2;
	}
	***/


	liness = SetDIBitsToDevice(hdc, 0, 0
		, window_w
		, window_h
		, 0, 0, 0
		, window_h
		, buf //, buf2   - 2021
		, bmInfo
		, DIB_RGB_COLORS);


	unlock_READ();

	EndPaint(hw, &ps);


}

void GUI_low_level::line_v(int x, int y, int h, uint32_t color) {
	if (x < 0 || y < 0) return;
	if (y + h >= window_h) {
		h = window_h - y;
		if (h <= 0) return;
	}
	if (x >= window_w) {
		return;
	}

	unsigned int *qq;
	qq = buf + x + y * window_w;
	for (int i = 0; i < h; i++) {
		//set_pix(x, y + i, color);
		*qq = color;
		qq += window_w;

	}
}

void GUI_low_level::fill_rectangle(int x, int y, int w, int h, unsigned int color, int transparent) {

	if (w == 0 || h == 0) return;

	unsigned int *qq, *q, old_color, new_color; // zz,
	if (y < 0) {
		h += y;
		y = 0;
	}
	if (x < 0) {
		w += x;
		x = 0;
	}
	if (w < 0) return;
	if (y + h > window_h) {
		h = window_h - y;
		if (h < 0) return;
	}
	q = buf + x + y * window_w;
	if (x + w > window_w) { w = window_w - x; };
	for (int i = 0; i < h; i++) {
		qq = q;
		for (int j = 0; j < w; j++) {
			//set_pix(x + j, y + i, color);

			old_color = *qq;

			new_color = transparent_color(old_color, color, transparent);



			*qq = new_color;
			qq++;
		};
		q += window_w;
	}
};

void GUI_low_level::paint_tri_1_close(int x, int y, unsigned int color) {
	if (y + 10 >= window_h) return;
	if (x < 0) return;
	if (x + 9 >= window_w) return;
	if (y + 9 >= window_w) return;

	//fill_rectangle(x, y, _element_icon_w, _element_icon_w, 0xffffff, 255);

	unsigned int *qq;
	qq = buf + x + (y + 4) * window_w + 4;
	qq++; *qq++ = color; *qq = color;  qq += window_w - 1;
	*qq++ = color; *qq++ = color; *qq = color; qq += window_w - 1;
	*qq++ = color; *qq++ = color; *qq = color; qq += window_w - 1;
	*qq++ = color; *qq++ = color; *qq = color; qq += window_w - 1;
	*qq++ = color; *qq++ = color; *qq = color; qq += window_w - 3;
	*qq++ = color; *qq++ = color; *qq = color; qq += window_w - 3;
	*qq++ = color; *qq++ = color; *qq = color; qq += window_w - 3;
	*qq++ = color; *qq++ = color; *qq = color; qq += window_w - 3;
	qq++; *qq++ = color;  *qq = color;
}

void GUI_low_level::paint_tri_1_open(int x, int y, unsigned int color) {
	if (y >= window_h) return;
	if (x < 0) return;
	if (x + _element_icon_w >= window_w) return;
	if (y + _element_icon_w >= window_h) return;

	//fill_rectangle(x, y, _element_icon_w, _element_icon_w, 0xffffff, 255);

	unsigned int *qq;
	qq = buf + x + (y + 6) * window_w + 3;

	*qq++ = color; *qq++ = color; qq += window_w - 2;
	*qq++ = color; *qq++ = color; *qq = color; qq += window_w - 1;
	*qq++ = color; *qq++ = color; *qq = color; qq += window_w - 1;
	*qq++ = color; *qq++ = color; *qq = color; qq += window_w - 1;
	*qq++ = color; *qq++ = color; *qq = color; qq += window_w - 1;

	*qq = color; *qq++ = color;  qq -= window_w + 2;
	*qq++ = color; *qq++ = color; *qq = color; qq -= window_w + 1;
	*qq++ = color; *qq++ = color; *qq = color; qq -= window_w + 1;
	*qq++ = color; *qq++ = color; *qq = color; qq -= window_w + 1;
	*qq++ = color; *qq++ = color; *qq = color; qq -= window_w + 1;
	*qq++ = color; *qq++ = color;
}

bool GUI_low_level::paint_text_rect(int font_idx, int x, int y, int w, int h, wchar_t *text, unsigned int color) {
	if (text == NULL) return false;
	if (font_idx < 0 || font_idx >= 10) return false;

	FONT *f;
	f = font[font_idx];
	if (f == nullptr) return false;

	//this->fill_rectangle(x, y, w, h, 0xffff00, 50);

	int i, ww;
	i = 0;
	ww = 0;
	while (i < 290 && text[i] != 0) {

		ww += f->bukva[wchar_to_ascii(text[i])].w;
		if (ww > w) {
			return false;
		}
		x += f->bukva[wchar_to_ascii(text[i])].paint2(this, x, y, color, false);
		i++;
	}

	return true;
}


int GUI_low_level::text_height(int font_idx) {

	if (font_idx < 0 || font_idx >= 10) return 0;
	if (font[font_idx] == nullptr) return 0;
	return font[font_idx]->text_height();
}

int GUI_low_level::text_width(int font_idx, wchar_t *text) {
	if (text == nullptr) return 0;
	if (font_idx < 0 || font_idx >= 10) return 0;
	if (font[font_idx] == nullptr) return 0;
	return font[font_idx]->text_width(text);

}

void GUI_low_level::line_h(int x, int y, int w, unsigned int color) {
	unsigned int *qq;

	if (y >= window_h) return;

	if (x < 0) {
		w += x;
		x = 0;
	}
	if (x + w >= window_w) {
		w -= ((x + w) - window_w);
	}

	if (w < 0 || x >= window_w) return;

	qq = buf + x + y * window_w;
	for (int i = 0; i < w; i++) {
		//set_pix(x+i, y, color);
		*qq = color;
		qq++;

	}
}


void GUI_low_level::paint_text(int font_idx, int x, int y, int w, int h, char *text, unsigned int color, int aligment, int edit_cursor_position) {

	//return;

	if (font_idx < 0 || font_idx >= 10) return;
	FONT *fnt;

	fnt = font[font_idx];
	if (fnt == nullptr) return;

	if (y < 0) return;

	int r;
	if (edit_cursor_position >= 0) {
		r = 1;
	}
	if (text == NULL) {
		return;
	};
	int i, ww, hh, dx, dy;

	dx = 0;
	dy = 0;
	if (aligment == 1) // center - center
	{
		ww = fnt->text_width( (const char *)text );
		hh = fnt->text_height();
		dx = (w - ww) / 2;
		dy = (h - hh) / 2;
	}
	if (aligment == 2) // left - center
	{
		ww = fnt->text_width((const char *)text);
		hh = fnt->text_height();
		//dx = (w - ww) / 2;
		dy = (h - hh) / 2;
	}
	if (aligment == 3) // left - center
	{
		ww = fnt->text_width((const char *)text);
		hh = fnt->text_height();
		dx = w - ww - 10;
		dy = (h - hh) / 2;
	}
	if (aligment == 4) // 
	{
		ww = fnt->text_width((const char *)text);
		hh = fnt->text_height();
		dx = w - ww;
		dy = (h - hh) / 2;
	}
	int b_idx;
	int i2;



	i = 0;
	while (i < 120 && text[i] != 0) {

		i2 = 0;
		while (i2 < 120 && text[i2] != 0) {

			b_idx = text[i2];
			if (b_idx < 0 || b_idx > 255) {
				return;
			}
			i2++;
		};


		b_idx = text[i];
		if (b_idx < 0 || b_idx > 255) {
			return;
		}

		if (i == edit_cursor_position) {
			x += fnt->bukva[text[i]].paint2(this, x + dx, y + dy, color, true);
			i2 = 0;
			while (i2 < 120 && text[i2] != 0) {

				b_idx = text[i2];
				if (b_idx < 0 || b_idx > 255) {
					return;
				}
				i2++;
			};
		}
		else {
			x += fnt->bukva[text[i]].paint2(this, x + dx, y + dy, color, false);
			i2 = 0;
			while (i2 < 120 && text[i2] != 0) {

				b_idx = text[i2];
				if (b_idx < 0 || b_idx > 255) {
					return;
				}
				i2++;
			};
		}



		i++;
	}
	if (edit_cursor_position >= 0) {
		if (i == edit_cursor_position) {
			x += fnt->bukva[0].paint2(this, x + dx, y + dy, color, true);
		}
	};
};

void GUI_low_level::paint_text(int font_idx, int x, int y, int w, int h, wchar_t *text, unsigned int color, int aligment, int edit_cursor_position) {

	//return;

	if (font_idx < 0 || font_idx >= 10) return;
	FONT *fnt;

	fnt = font[font_idx];
	if (fnt == nullptr) return;

	if (y < 0) return;

	int r;
	if (edit_cursor_position >= 0) {
		r = 1;
	}
	if (text == NULL) {
		return;
	};
	int i, ww, hh, dx, dy;

	dx = 0;
	dy = 0;
	if (aligment == 1) // center - center
	{
		ww = fnt->text_width(text);
		hh = fnt->text_height();
		dx = (w - ww) / 2;
		dy = (h - hh) / 2;
	}
	if (aligment == 2) // left - center
	{
		ww = fnt->text_width(text);
		hh = fnt->text_height();
		//dx = (w - ww) / 2;
		dy = (h - hh) / 2;
	}
	if (aligment == 3) // left - center
	{
		ww = fnt->text_width(text);
		hh = fnt->text_height();
		dx = w - ww - 10;
		dy = (h - hh) / 2;
	}
	if (aligment == 4) // 
	{
		ww = fnt->text_width(text);
		hh = fnt->text_height();
		dx = w - ww;
		dy = (h - hh) / 2;
	}
	int b_idx;
	int i2;



	i = 0;
	while (i < 120 && text[i] != 0) {

		i2 = 0;
		while (i2 < 120 && text[i2] != 0) {

			b_idx = text[i2];
			if (b_idx < 0 || b_idx > 255) {
				return;
			}
			i2++;
		};


		b_idx = text[i];
		if (b_idx < 0 || b_idx > 255) {
			return;
		}

		if (i == edit_cursor_position) {
			x += fnt->bukva[wchar_to_ascii(text[i])].paint2(this, x + dx, y + dy, color, true);
			i2 = 0;
			while (i2 < 120 && text[i2] != 0) {

				b_idx = text[i2];
				if (b_idx < 0 || b_idx > 255) {
					return;
				}
				i2++;
			};
		}
		else {
			x += fnt->bukva[wchar_to_ascii(text[i])].paint2(this, x + dx, y + dy, color, false);
			i2 = 0;
			while (i2 < 120 && text[i2] != 0) {

				b_idx = text[i2];
				if (b_idx < 0 || b_idx > 255) {
					return;
				}
				i2++;
			};
		}



		i++;
	}
	if (edit_cursor_position >= 0) {
		if (i == edit_cursor_position) {
			x += fnt->bukva[0].paint2(this, x + dx, y + dy, color, true);
		}
	};
};

void GUI_low_level::paint_close_krest(int x, int y, int w, unsigned int color) {
	if (x < 0 || y < 0) return;
	if (x + w + 1 > window_w) return;
	if (y + w + 1 > window_h) return;

	unsigned int *qq;
	qq = buf + x + (y)* window_w;

	for (int i = 0; i < w + 1; i++) {

		//if (i < w) *(qq + i + i * window_w + window_w * 2) = 0;
		//if (i > 0) *(qq + i + (w - i) * window_w + window_w * 2) = 0;

		*(qq + i + i * window_w) = color;
		if (i < w) {
			*(qq + i + i * window_w + 1) = color;
			*(qq + i + i * window_w + window_w) = color;
		};

		*(qq + i + (w - i) * window_w) = color;
		if (i < w) {
			*(qq + i + (w - i) * window_w + 1) = color;
			*(qq + i + (w - i) * window_w - window_w) = color;
		};


	}

};

void GUI_low_level::paint_tre_up(int x, int y, int w, unsigned int color) {
	if (x < 0 || y < 0) return;
	if (x + w + 1 > window_w) return;
	if (y + w + 1 > window_h) return;

	unsigned int *qq;


	int i, j;

	for (j = 0; j < 5; j++) {

		qq = buf + x + j + (y - j)* window_w;

		for (i = 0; i < 10 - j * 2; i++) {
			*(qq + i) = color;
		};
	};
};
void GUI_low_level::paint_tre_down(int x, int y, int w, unsigned int color) {
	if (x < 0 || y < 0) return;
	if (x + w + 1 > window_w) return;
	if (y + w + 1 > window_h) return;

	unsigned int *qq;


	int i, j;

	for (j = 0; j < 5; j++) {

		qq = buf + x + j + (y + j)* window_w;

		for (i = 0; i < 10 - j * 2; i++) {
			*(qq + i) = color;
		};
	};
};

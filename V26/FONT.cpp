#include "stdafx.h"

#include "FONT.h"
#include "tools.h"

BUKVA::BUKVA() {

}


BUKVA::BUKVA(int idx_, int w_, int h_, uint8_t *buf_) {
	idx = idx_;
	w = w_;
	h = h_;

	buf_size = w * h + 1;
	buf = new uint8_t[buf_size];// , start_id + 332);

	int i = 0;
	while (i < w*h) {
		buf[i] = buf_[i];
		i++;
	};

}
BUKVA::BUKVA(int idx_, int w_, int h_, uint8_t *buf_, int v2) {
	idx = idx_;
	w = w_;
	h = h_;

	buf_size = w * h + 1;
	buf = new uint8_t[buf_size];// , start_id + 332);

	int i = 0;
	while (i < w*h) {
		buf[i] = buf_[i];
		i++;
	};

}

int BUKVA::paint_dummy(GUI_low_level *low_level, int x, int y, uint32_t color) {

	if (buf == NULL) return 0;


	unsigned int *v;
	int i, j, d;

	for (j = 0; j < h; j++) {
		v = low_level->buf;
		//d = low_level->window_h - 1 - y - j;
		d = y + j;
		if (d >= 0) {
			d *= low_level->window_w;
			d += x;
			
			v += d;

			//v = scr_buf + (scr_h - 1 - y - j)*scr_w + x;

			for (i = 0; i < w; i++)
			{
				if (buf[i + j * w] == '1') {
					*v++ = color;
				}
				else {
					v++;// = 0xff0099;
				}
				//low_level_gui->set_pixx(i + x, j + y, get_color(i, j));
				//low_level_gui->set_pixx(i + x, j + y, 255);
			};
		};
	};
	return w;
}

int BUKVA::paintAAA(GUI_low_level *low_level, int x, int y, uint32_t color, bool show_cursor) {
	if (buf == NULL)
	{
		w = 15;
		h = 20;
		buf = new unsigned char[w*h];
		for (int i = 0; i < w*h; i++)
			buf[i] = '1';

	}
	//unsigned int new_color;
	//new_color = 0 | (70 << 8) | (180 << 16);
	int i, j, percent;// , aa_r, need_r, need_g, need_b;

	

	//need_r = 0;
	//need_g = 70;
	//need_b = 180;

	int clr, old;// , old_r, old_g, old_b, new_r, new_g, new_b;

	int p255 = 0, p0 = 0;

	for (j = 0; j < h; j++)
		for (i = 0; i < w; i++)
		{
			old = low_level->get_pix(i + x, j + y);

			percent = 255 - buf[i + j * (w)];

			if (percent == 255) p255++;
			if (percent == 0) p0++;

			clr = transparent_color(old, color, percent);

			low_level->set_pixx(i + x, j + y, clr);

		};

	if (show_cursor) {
		int hh, yy;
		if (h == 34) {
			hh = 22;
			yy = y + 5;
		}
		else {
			hh = h;
			yy = y;
		}
		low_level->line_v(x, yy, hh, 0xff0000);
	}
	return w;
}

int BUKVA::paint2(GUI_low_level *low_level, int x, int y, unsigned int color, bool show_cursor) {
	if (low_level == nullptr) {
		return 0;
	}
	unsigned int vv;
	int i, j;
	if (idx != 10) {
		for (i = 0; i < w; i++) {
			for (j = 0; j < h; j++) {
				if (i + j * w > buf_size) {
					return 0;
				}
				if (buf[i + j * w] == '1') {
					vv = low_level->get_pix(x + i - 1, y + j - 1);
					//if( vv != color && vv != 0 ) set_pix(x + i - 1, y + j - 1, 0xffffffff);
					low_level->set_pixx(x + i, y + j, color);
					//set_pix(x+i+1, y+j+1, 0xff000000);
					//set_pix(x + i + 1, y + j + 0, 0xff000000);
				}
			}

		}
	};
	if (show_cursor) {
		low_level->line_v(x - 1, y, h, 255);
		//for (j = 0; j < h; j++) {
			//vv = low_level_gui->get_pix(x + i - 1, y + j - 1);
			//if( vv != color && vv != 0 ) set_pix(x + i - 1, y + j - 1, 0xffffffff);
			//low_level_gui->set_pixx(x -1, y + j, color);
		//};
	}
	return w;

}


/////////////////////////////////////////////////////////////////////////////////

FONT::FONT() {
	
}

void FONT::load_from_buffer(unsigned char *buf, int buf_size) {

	

	int i, jj, w, h, kk;
	i = 0;
	jj = 0;
	while (i < buf_size) {
		kk = buf[i++];
		w = buf[i++];
		h = buf[i++];
	
		bukva.insert({ jj , BUKVA(jj, w, h, &(buf[i])) });

		//bukva.insert(std::pair<int, BUKVA>(jj,   (jj, w, h)));

		//bukva[jj].addbuffer(w, h, &(buf[i]));
		//bukva[jj].idx = jj;
		jj++;
		i += w * h;
	};

};
int FONT::text_height() {
	return bukva['$'].h;
}

int FONT::text_width(const char *txt) {
	int i, ww;
	i = 0;
	ww = 0;
	while (txt[i] != 0) {
		ww += bukva[ txt[i] ].w;
		i++;
	}
	return ww;
}


int FONT::text_width(const wchar_t *txt) {
	int i, ww;
	i = 0;
	ww = 0;
	while (txt[i] != 0) {
		ww += bukva[wchar_to_ascii(txt[i])].w;
		i++;
	}
	return ww;
}

void FONT::paint_dummy(GUI_low_level *low_level, int x, int y, const wchar_t *txt, uint32_t color_) {
	int i, ww;
	i = 0;
	ww = 0;
	while (txt[i] != 0) {
		
		ww += bukva[wchar_to_ascii(txt[i])].paint_dummy(low_level, x + ww, y, color_);
				
		i++;
	}
}

void FONT::paintAAA(GUI_low_level *low_level, int x, int y, const wchar_t *txt, uint32_t color_, int cursor_position, bool is_pass) {
	bool show_cursor;
	int i, ww;
	i = 0;
	ww = 0;
	while (txt[i] != 0) {
		show_cursor = false;
		if (i == cursor_position) show_cursor = true;
		if (is_pass == false) {
			ww += bukva[wchar_to_ascii(txt[i])].paintAAA(low_level, x + ww, y, color_, show_cursor);
		}
		else {
			ww += bukva[150].paintAAA(low_level, x + ww, y, color_, show_cursor);
		}
		i++;
	}
	if (i == cursor_position) {
		show_cursor = true;
		ww += bukva[0].paintAAA(low_level, x + ww, y, color_, show_cursor);
	};
}

int FONT::paint_text_in_width(GUI_low_level *low_level, int x, int y, int max_w, wchar_t *text, unsigned int color) {
	if (text == NULL) return 0;
	int i, w, ww;

	i = 0;
	ww = 0;
	while (text[i] != 0) {

		w = bukva[wchar_to_ascii(text[i])].w;

		if (ww + w >= max_w) {
			return ww;
		};

		ww += w;

		x += bukva[wchar_to_ascii(text[i])].paintAAA(low_level, x, y, color, false);
		i++;
	}

	return ww;
};

int FONT::load_dummy(unsigned char *buf, int buf_size) {

	/* 2021 09 int ii, jj, w, h;
	ii = 0;
	jj = 0;
	while (ii < buf_size) {
		w = buf[ii++];
		h = buf[ii++];
		bukva[jj].addbuffer(low_lvl, w, h, &(buf[ii]), start_id);
		jj++;
		ii += w * h;
	};*/

	int i, jj, w, h, kk;
	i = 0;
	jj = 0;
	while (i < buf_size) {
		//kk = buf[i++];
		w = buf[i++];
		h = buf[i++];

		bukva.insert({ jj , BUKVA(jj, w, h, &(buf[i]), 2) });

		//bukva.insert(std::pair<int, BUKVA>(jj,   (jj, w, h)));

		//bukva[jj].addbuffer(w, h, &(buf[i]));
		//bukva[jj].idx = jj;
		jj++;
		i += w * h;
	};

	return buf_size;
}

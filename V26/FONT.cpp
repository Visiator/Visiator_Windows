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
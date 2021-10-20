#include "stdafx.h"
/*
#ifndef WIN32

#define WIN32_LEAN_AND_MEAN
#define WIN32_WINNT 0x0501


#define _WINSOCKAPI_ 
#include <windows.h>
#undef _WINSOCKAPI_
#include <winsock2.h>
#include <stdlib.h>
#include <iphlpapi.h>
#include <stdio.h>
#undef _WINSOCKAPI_



#endif
#include <stdio.h>
*/
//#include "bzlib.h"

//#include "lzoconf.h"
#include "minilzo.h"




//#define ZLIB_STAT

//#include "zlib/zlib.h"

//#pragma comment(lib, "zlibwapi.lib")

//  Z_PREFIX

#include "SCREEN.h"

#include "tools.h"
//#include "mem.h"

extern unsigned char *encode_color_matrix_onebyte;
extern unsigned char *encode_color_matrix_5_in_8;
extern unsigned char *encode_color_matrix_6_in_8;

unsigned short encode_rgb_to_12bit(unsigned int v);

bool decode_screen_8bit_second(unsigned char *buf, int buf_size, SCREEN_LIGHT *raw_screen);

CHESS_SHADOW::CHESS_SHADOW() {

}

void CHESS_SHADOW::set_size(int len_) {
	if (line_0 != nullptr) delete[] line_0;
	if (line_2 != nullptr) delete[] line_2;
	line_0 = new unsigned int[len_]; // neew(len_, line_0);
	line_2 = new unsigned int[len_]; // neew(len_, line_2);

	unsigned int color1 = 0x554444, color2 = 0x554444;

	int i, j, k;
	i = 0;
	j = 0;
	k = 0;
	while (i < len_) {
		if (j == 0) { line_0[i] = color1; line_2[i] = color2; }
		else { line_0[i] = color2; line_2[i] = color1; };
		k++;
		if (k >= 5) {
			k = 0;
			j++;
			if (j > 1) j = 0;
		}
		i++;
	}

	len = len_;
}

//------------------------------------------------------------------

SCREEN_LIGHT_one_byte::SCREEN_LIGHT_one_byte() {
	clean();
}

SCREEN_LIGHT_one_byte::~SCREEN_LIGHT_one_byte() {
	if (buf_one_byte != nullptr) delete[] buf_one_byte;
	buf_one_byte = nullptr;


}

void SCREEN_LIGHT_one_byte::buf_one_byte_NEW(int size) {
	buf_one_byte = new unsigned char[size];
}

void SCREEN_LIGHT_one_byte::lock() {

	int flag = 0;

	while (flag == 0) {

		if (is_lock == false) {
			enter_crit(21);
			if (is_lock == false) {
				is_lock = true;
				flag = 1;
			}
			leave_crit(21);

		}
		else {
			Sleep(0); // кто то  уже заблокировал. ждем пока освободит
		}

	};
}
void SCREEN_LIGHT_one_byte::unlock() {
	if (is_lock != true) {
		//send_udp("SCREEN_LIGHT::unlock() is_lock != true WTF?");
	}
	is_lock = false;
}

void SCREEN_LIGHT_one_byte::get_screen_from_BitBlt_buffer(void *BitBlt_raw_buffer, int g_nWidth, int g_nHeight, int g_nColorMode) {

	unsigned char *ee;
	unsigned int *z, *zz, zzz;
	int jj, y, x; // fe

	ee = get_buf_one_byte();


	if (g_nColorMode == 32) {

		z = (unsigned int *)BitBlt_raw_buffer;
		jj = 0;
		for (y = 0; y < g_nHeight; y++)
		{
			zz = &(z[(g_nHeight - 1 - y)*g_nWidth]);
			for (x = 0; x < g_nWidth; x++)
			{
				zzz = *zz & 0xffffff;
				//zzz = 0xffffff;

				*ee = encode_color_matrix_onebyte[zzz];//++ 
				//*ee = __color_multibit_G7C223_(v[j + 2], v[j + 1], v[j + 0]);
				//if (*ee == 0xfe) fe++;
				ee++;
				zz++;

			};
		};

	};

	unsigned short *z16, *zz16;

	if (g_nColorMode == 16) {

		z16 = (unsigned short *)BitBlt_raw_buffer;
		jj = 0;

		unsigned int g16, b16, r16;

		for (y = 0; y < g_nHeight; y++)
		{
			/*
			zz = &(z[(g_nHeight - 1 - y)*g_nWidth]);
			for (x = 0; x < g_nWidth; x++)
			{
				zzz = *zz & 0xffffff;
				*ee = encode_color_matrix_onebyte[zzz];//++ __color_multibit_G7C223_(v[j + 2], v[j + 1], v[j + 0]);
				ee++;
				zz++;

			};
			*/
			zz16 = &(z16[(g_nHeight - 1 - y)*g_nWidth]);
			for (x = 0; x < g_nWidth; x++)
			{
				//f800 07e0 001f


				zzz = *zz16 & 0xffff;

				b16 = 0;
				g16 = 0;
				r16 = 0;

				b16 = zzz & 0x001f;
				g16 = zzz & 0x03e0;
				r16 = zzz & 0x7c00;

				g16 = g16 >> 5;
				r16 = r16 >> 10;

				b16 = encode_color_matrix_5_in_8[b16];
				g16 = encode_color_matrix_5_in_8[g16];
				r16 = encode_color_matrix_5_in_8[r16];


				g16 = g16 << 8;
				r16 = r16 << 16;





				zzz = r16 | b16 | g16;

				//zzz = r16;

				//if (zzz != 0) zzzz++;

				*ee = encode_color_matrix_onebyte[zzz];//++ __color_multibit_G7C223_(v[j + 2], v[j + 1], v[j + 0]);
				ee++;
				zz16++;

			};
		};

	};

	unsigned char *z8, *zz8;// , *v;


	int zzzz = 0;
	unsigned int r8, g8, b8;
	if (g_nColorMode == 24) {

		z8 = (unsigned char *)BitBlt_raw_buffer;
		jj = 0;
		for (y = 0; y < g_nHeight; y++)
		{
			zz8 = &(z8[(g_nHeight - 1 - y)*g_nWidth * 3]);
			for (x = 0; x < g_nWidth; x++)
			{
				r8 = *zz8 & 0xff; zz8++;
				g8 = *zz8 & 0xff; zz8++;
				b8 = *zz8 & 0xff; zz8++;

				zzz = *zz8 & 0xffffff;

				g8 = g8 << 8;
				b8 = b8 << 16;

				zzz = r8 | b8 | g8;

				*ee = encode_color_matrix_onebyte[zzz];
				//*ee = __color_multibit_G7C223_(v[j + 2], v[j + 1], v[j + 0]);
				ee++;


			};
		};

	};

}



void SCREEN_LIGHT_one_byte::set_new_size_(int w_, int h_) {
	if (header.w == w_ && header.h == h_) return;
	if (buf_one_byte != nullptr) delete[] buf_one_byte;
	if (w_ > 0 && h_ > 0) {
		buf_one_byte = new unsigned char[w_ * h_];
	}
	else {
		buf_one_byte = nullptr;
	}

	header.w = (unsigned short)w_;
	header.h = (unsigned short)h_;
	buf_one_byte_size = w_ * h_;
	//send_udp("set_new_size_() - - - - - - -");
}

void SCREEN_LIGHT_one_byte::buf_one_byte_DELETE() {
	delete[] buf_one_byte;
	buf_one_byte = nullptr;
}

unsigned char *SCREEN_LIGHT_one_byte::get_buf_one_byte() {
	int i;
	if (is_lock == false) {
		i = 112;
	}
	return buf_one_byte;
}

void SCREEN_LIGHT_one_byte::clean_() {
	for (int i = 0; i < buf_one_byte_size; i++) {
		buf_one_byte[i] = 0;
	}
	clean_ENCODED_SCREEN_8bit_header(&header);
	screen_id = 0;
}

void SCREEN_LIGHT_one_byte::clean() {
	if (buf_one_byte != nullptr) delete[] buf_one_byte;
	buf_one_byte = nullptr; 
	buf_one_byte_size = 0;

	clean_ENCODED_SCREEN_8bit_header(&header);



}

/*void SCREEN_LIGHT_one_byte::emulate_green_chess() {
	set_new_size_(500, 200);

	unsigned char *q;
	q = buf_one_byte;
	for (int j = 0; j < 100; j++) {
		
		for (int i = 0; i < 500; i++) {
			*q++ = 0x7;
		}
		
		for (int i = 0; i < 500; i++) {
			*q++ = 0x18;
		}
	}
}*/

void SCREEN_LIGHT_one_byte::emulate_red() {
	
	set_new_size_(250, 250);

	unsigned char *q;

	for (int j = 0; j < 250; j++) {
		q = buf_one_byte + j * 250;
		for (int i = 0; i < 250; i++) {
			*q++ = 32;
		}
	}
	// 1 - 7 - красный
	// 8 16 - зеленый
	// 32 - темно синий
}

void SCREEN_LIGHT_one_byte::emulate_dark_blue() {

	set_new_size_(250, 250);

	unsigned char *q;

	for (int j = 0; j < 250; j++) {
		q = buf_one_byte + j * 250;
		for (int i = 0; i < 250; i++) {
			*q++ = 32;
		}
	}
	// 1 - 7 - красный
	// 8 16 - зеленый
	// 32 - темно синий
}

// end
//******************************************************************************************

//------------------------------------------------------------------
SCREEN_LIGHT::SCREEN_LIGHT()
{
	void Clean();
}

SCREEN_LIGHT::~SCREEN_LIGHT()
{
	if (buf != nullptr) delete[] buf;
	buf = nullptr;
	if (buf_one_byte != nullptr) delete[] buf_one_byte;
	buf_one_byte = nullptr;
}

void SCREEN_LIGHT::Clean() {
	enter_crit(22);
	
	w = 0;
	h = 0;

	keyboard_location = 0;
	
	if (buf != nullptr) delete[] buf;
	buf = nullptr;
	if (buf_one_byte != nullptr) delete[] buf_one_byte;
	buf_one_byte = nullptr;

	mouse_x = 0; mouse_y = 0; mouse_cursor_type_id = 0; _itis_user_move_mouse_ = 0;
	screen_id = 0; old_screen_id = 0;

	leave_crit(22);
}

void SCREEN_LIGHT::lock_READ(int nom) {



	int flag = 0;

	while (flag == 0) {

		if (is_lock_FULL == false && is_lock_READ == false) {
			enter_crit(23);
			if (is_lock_FULL == false && is_lock_READ == false) {
				is_lock_READ = true;
				flag = 1;
				nom_lock = nom;
			}
			leave_crit(23);

		}
		else {
			Sleep(0); // кто то  уже заблокировал. ждем пока освободит
		}

	};
}
void SCREEN_LIGHT::unlock_READ(int nom) {

	int flag = 0;

	if (nom_lock == 1000 && nom == 1001) flag = 1;
	if (nom_lock == 1000 && nom == 1002) flag = 1;
	if (nom_lock == 1000 && nom == 1010) flag = 1;
	if (nom_lock == 1000 && nom == 1011) flag = 1;

	if (nom_lock == 2000 && nom == 1010) flag = 1;
	if (nom_lock == 4000 && nom == 4001) flag = 1;
	if (nom_lock == 3000 && nom == 3002) flag = 1;
	if (nom_lock == 6000 && nom == 6001) flag = 1;
	if (nom_lock == 7000 && nom == 7001) flag = 1;

	if (flag == 0) {
		flag = 2;
	}

	if (is_lock_READ != true) {
		//send_udp("SCREEN_LIGHT::unlock() is_lock != true WTF?");
	}
	is_lock_READ = false;
}
void SCREEN_LIGHT::lock_FULL(int nom) {
	
	

	int flag = 0;

	while (flag == 0) {

		if (is_lock_FULL == false && is_lock_READ == false) {
			enter_crit(24);
			if (is_lock_FULL == false && is_lock_READ == false) {
				is_lock_FULL = true;
				is_lock_READ = true;
				flag = 1;
				nom_lock = nom;
			}
			leave_crit(24);
			
		}
		else {
			Sleep(0); // кто то  уже заблокировал. ждем пока освободит
		}

	};
}
void SCREEN_LIGHT::unlock_FULL(int nom) {

	int flag = 0;

	if (nom_lock == 1000 && nom == 1001) flag = 1;
	if (nom_lock == 1000 && nom == 1002) flag = 1;
	if (nom_lock == 1000 && nom == 1010) flag = 1;
	if (nom_lock == 2000 && nom == 1010) flag = 1;
	if (nom_lock == 4000 && nom == 4001) flag = 1;
	if (nom_lock == 4000 && nom == 4004) flag = 1;
	if (nom_lock == 3000 && nom == 3001) flag = 1;
	if (nom_lock == 3000 && nom == 3002) flag = 1;
	if (nom_lock == 3000 && nom == 3003) flag = 1; 
	if (nom_lock == 3000 && nom == 4004) flag = 1; 

	if (nom_lock == 6000 && nom == 6001) flag = 1;
	if (nom_lock == 7000 && nom == 7001) flag = 1;

	if (flag == 0) {
		flag = 2;
	}

	if (is_lock_FULL != true) {
		//send_udp("SCREEN_LIGHT::unlock() is_lock != true WTF?");
	}
	is_lock_FULL = false;
	is_lock_READ = false;
}


void SCREEN_LIGHT::set_new_size(int w_, int h_) {
	if (w == w_ && h == h_) return;

	if (buf_size >= w_ * h_) {
		w = w_;
		h = h_;
		//img_w = w_;
		//img_h = h_;
		if (w == 0 || h == 0) { aspect = 1; }
		else { aspect = (float)w / (float)h; };
		return;
	}
	enter_crit(25);
	w = w_;
	h = h_;
	//img_w = w_;
	//img_h = h_;
	buf_size = w * h;
	if (w == 0 || h == 0) { aspect = 1; } else { aspect = (float)w / (float)h; };
	
	if (buf != NULL) delete[] buf;
	buf = new unsigned int[buf_size];

	if (buf_one_byte != NULL) delete[] buf_one_byte;
	buf_one_byte = new unsigned char[buf_size];

	zero_unsigned_char(buf_one_byte, w*h);
	zero_int(buf, w*h);
	leave_crit(25);
}

unsigned int *SCREEN_LIGHT::get_buf() {

	int i;
	if (is_lock_FULL == false && is_lock_READ == false) {
		i = 112;
	}
	return buf;
}

unsigned char *SCREEN_LIGHT::get_buf_one_byte() {
	return buf_one_byte;
}

unsigned int decode_color_matrix_G7C223[256];

void init_decode_color2(void) {

	//ALOG("SCREEN::init_decode_color2()");

	int i;
	unsigned int r, g, b;
	for (i = 1; i < 256; i++) decode_color_matrix_G7C223[i] = 0xff;

	decode_color_matrix_G7C223[0x80] = 0x000000;
	decode_color_matrix_G7C223[0x81] = 0x242424;
	decode_color_matrix_G7C223[0x82] = 0x494949;
	decode_color_matrix_G7C223[0x83] = 0x6D6D6D;
	decode_color_matrix_G7C223[0x84] = 0x929292;
	decode_color_matrix_G7C223[0x85] = 0xC6C6C6;
	decode_color_matrix_G7C223[0x86] = 0xEBEBEB;
	decode_color_matrix_G7C223[0x87] = 0xffffff;

	for (i = 1; i <= 127; i++) {
		r = i & 0x07; 
		if (r == 0) r = 0x000000;
		if (r == 1) r = 0x240000;
		if (r == 2) r = 0x490000;
		if (r == 3) r = 0x6D0000;
		if (r == 4) r = 0x920000;
		if (r == 5) r = 0xB60000;
		if (r == 6) r = 0xDB0000;
		if (r == 7) r = 0xFF0000;

		g = i & 0x18; g = g >> 3;
		if (g == 0) g = 0x000000;
		if (g == 1) g = 0x005500;
		if (g == 2) g = 0x00AA00;
		if (g == 3) g = 0x00ff00;

		b = i & 0x60; b = b >> 5;
		if (b == 0) b = 0x000000;
		if (b == 1) b = 0x000055;
		if (b == 2) b = 0x0000AA;
		if (b == 3) b = 0x0000ff;

		decode_color_matrix_G7C223[i] = r | g | b;

	};

	decode_color_matrix_G7C223[0xfe] = 0xfe;

};


bool decode_screen_8bit_first(unsigned char *buf, int buf_size, SCREEN_LIGHT *raw_screen) {
	if (buf == NULL) {
		fatal_error("SCREEN::decode() buf == NULL");
		return false;
	};

	if (raw_screen == NULL) return false;

	int ww, hh, scr_id, old_scr_id, bdy_size, i, pl_idx, pl_size, j;
	ENCODED_SCREEN_8bit_header *hdr;
	C8SH16 *pl;
	unsigned char *bdy;

	int header_size = 16 * 4;

	hdr = (ENCODED_SCREEN_8bit_header *)buf;

	if (hdr->format != 2) {

		fatal_error("SCREEN::decode() hdr->format != 2 ");
		return false;
	};

	if (hdr->color_bit != 32) {

		fatal_error("SCREEN::decode() hdr->color_bit != 32");
		//ALOG("hdr->color_bit = %d ", hdr->color_bit );
		return false;
	};

	ww = hdr->w;
	hh = hdr->h;


	if (ww < 0 || ww > 5000 || hh < 0 || hh > 5000) {

		fatal_error("SCREEN::decode() ww < 0 || ww > 5000 || hh < 0 || hh > 5000 ");
		return false;
	}

	raw_screen->keyboard_location = hdr->keyboard_location;

	//desk.keyboard.set_current_layout_by_code(keyboard_location);

	if (ww != raw_screen->w || hh != raw_screen->h) {
		raw_screen->lock_FULL(3000);
	};

	raw_screen->set_new_size(ww, hh);


	unsigned int *q, clr, ii, jj, q_max, q_idx;
	q_max = ww * hh;
	q_idx = 0;
	//ALOG("aaa");

	q = raw_screen->get_buf();

	

	scr_id = hdr->screen_id;
	old_scr_id = hdr->old_screen_id;

	//ALOG("old_scr_id = %d " , old_scr_id );

	bdy_size = hdr->body_size;
	pl_size = hdr->pal_size;

	raw_screen->mouse_x = hdr->mouse_x;
	raw_screen->mouse_y = hdr->mouse_y;
	raw_screen->mouse_cursor_type_id = hdr->mouse_cursor_type_id;
	raw_screen->_itis_user_move_mouse_ = hdr->itis_user_move_mouse;

	raw_screen->keyboard_location = hdr->keyboard_location;
	// 2019 10 ? raw_screen->scr_id = hdr->screen_id;

	pl = (struct C8SH16 *)&(buf[header_size]);

	bdy = (unsigned char *)&(buf[header_size + hdr->pal_size]);


	i = 0;
	j = 0;
	while (i < bdy_size) {
		if (*bdy < 128) {
			pl_idx = *bdy;
			bdy++;
		}
		else {
			pl_idx = (*bdy) & 0x7f;
			bdy++;
			i++;
			pl_idx = pl_idx << 8;
			pl_idx += *bdy;
			bdy++;
		}
		if (pl_idx < 0 || pl_idx >= pl_size) {
			if(raw_screen->is_lock_FULL) raw_screen->unlock_FULL(3003);
			fatal_error("SCREEN::decode()  pl_idx < 0 || pl_idx >= pl_size ");
			return false;
		}

		jj = pl[pl_idx].s;

		j += jj;

		clr = decode_color_matrix_G7C223[pl[pl_idx].c];// decode_clr(pl[pl_idx].c);

		if (clr == 0xfe) {
			q += jj;

		}
		else {
			for (ii = 0; ii < jj; ii++) {

				*q = clr;

				q++;
			}
		};
		i++;
	}


	/*if (raw_screen->mouse_x >= 0 && raw_screen->mouse_x < raw_screen->w &&
		raw_screen->mouse_y >= 0 && raw_screen->mouse_y < raw_screen->h) {

		q = raw_screen->get_buf();

		q += raw_screen->mouse_y * raw_screen->w + raw_screen->mouse_x;

		*q = 0xff00ff;

	}*/

	if (j != ww * hh) {
		if (raw_screen->is_lock_FULL) raw_screen->unlock_FULL(3001);
		//ALOG("SCREEN::decode()  j != ww * hh");
		fatal_error("SCREEN::decode()  j != ww * hh ");
		return false;
	}
	raw_screen->screen_id = scr_id;
	raw_screen->old_screen_id = old_scr_id;
	//ALOG("DECODE END");

	//if (raw_screen->is_lock_FULL) raw_screen->unlock_FULL(3002);

	return true;


}




//*******************************************************************************
// SCREEN_LIGHT_encoded

SCREEN_LIGHT_encoded_8bit_first::SCREEN_LIGHT_encoded_8bit_first() {

};

SCREEN_LIGHT_encoded_8bit_first::~SCREEN_LIGHT_encoded_8bit_first() {

	delete[] old_buf_one_byte;
	old_buf_one_byte_size = 0;

	delete[] body;
	body_count_max = 0;

	encoded_buffer_len_max = 0;
	delete[] encoded_buffer;

	pal_count_max = 0;
	delete[] pal;
}

/*void SCREEN_LIGHT_encoded::set_new_size(int w_, int h_) {
	if (w == w_ && h == h_) return;
	int wh;
	if (buf_size >= w_ * h_) {
		w = w_;
		h = h_;

		return;
	}
	enter_crit();
	w = w_;
	h = h_;
	buf_size = w * h;

	if (buf != NULL) delete_(&buf);
	buf = neww unsigned char[buf_size];

	zero(buf, w*h);
	leave_crit();
}*/

void SCREEN_LIGHT_encoded_8bit_first::pal_increase_size() {
	unsigned int i,k;

	PAL *q;

	k = pal_count_max;

	pal_count_max = pal_count_max + pal_count_max / 2;

	q = new_PAL(pal_count_max);    //   neew(sizeof(PAL) * pal_count_max, q);
	for (i = 0; i < k; i++) {
		q[i].color = pal[i].color;
		q[i].count = pal[i].count;
		q[i].len = pal[i].len;
	}
	while (k < pal_count_max) {
		q[k].color = 0;
		q[k].count = 0;
		q[k].len = 0;
		k++;
	}

	delete_PAL(&pal);
	pal = q;
}

void SCREEN_LIGHT_encoded_8bit_first::clean_() {
	for (int i = 0; i < body_count_max; i++) {
		body[i] = 0;
	}
	old_screen_id = -1;
}

int sort_function_pal(const void *a, const void *b)
{
	PAL *p1, *p2;
	p1 = (PAL *)a;
	p2 = (PAL *)b;
	if (p1->len * p1->count > p2->len * p2->count) return -1;
	if (p1->len * p1->count == p2->len * p2->count) return 0;
	return 1;
};

/*
bool SCREEN_LIGHT_encoded_8bit_first::encode_screen_ONE_BYTE(SCREEN_LIGHT_one_byte *screen_one_byte, int last_set_mouse_x, int last_set_mouse_y ) {

	if (screen_one_byte->screen_id == 0) {
		//send_udp("SCREEN::encode() raw_screen->screen_id == 0 error");
		return false;
	}

	if (screen_one_byte->screen_id == old_screen_id) {
		//fatal_error("SCREEN::encode() screen_id == old_screen_id wtf??");
		return false;
	}

	if (screen_one_byte->header.w == 0 || screen_one_byte->header.h == 0) return false;
	if (screen_one_byte->header.w < 0 || screen_one_byte->header.h < 0) return false;
	if (screen_one_byte->header.w > 5500 || screen_one_byte->header.h > 5500) return false;

	if (screen_one_byte->header.w != old_w || screen_one_byte->header.h != old_h ) {
		old_screen_id = -1;
		old_w = screen_one_byte->header.w;
		old_h = screen_one_byte->header.h;

		if (old_buf_one_byte != NULL) delete[] old_buf_one_byte;
		old_buf_one_byte_size = old_w * old_h;
		old_buf_one_byte = new unsigned char[old_buf_one_byte_size];
	} 

	if (pal == NULL) {
		pal_count_max = 2000;
		pal = new_PAL(pal_count_max);     //        neew(sizeof(PAL) * pal_count_max, pal, "SCREEN::SCREEN() neww PAL[xxx]");

	}

	if (body == NULL) {
		body_count_max = 1980 * 1200 * 4;
		body = new unsigned char[body_count_max]; //neew(body_count_max, body, "SCREEN::SCREEN() body");
	};

	unsigned int pal_count;

	int body_count;

	pal_count = 0;
	body_count = 0;
	encoded_buffer_len = 0;

	unsigned int i, j, k, zz, c1, c2, c3, c4, c5, c6, c7;// , cc1, cc2, cc3;
	unsigned char *q, qq, *o, val;

	//lock();

	q = screen_one_byte->get_buf_one_byte();
	o = old_buf_one_byte;

	
	//send__udp("ENCODE - - - - raw_screen->w = ", raw_screen->w );

	zz = screen_one_byte->header.w * screen_one_byte->header.h;
	//cc1 = 0;
	//cc2 = 0;
	//cc3 = 0;
	if ( old_screen_id > 0 ) {
		for (i = 0; i < zz; i++) {
			val = *q;
			//if (val == 0xfe) cc3++;
			if (*o == val) {
				*q = 0xfe;
				//cc1++;
			}
			else {
				*o = val;
				//cc2++;
			}
			q++;
			o++;
		}
	}
	else {
		for (i = 0; i < zz; i++) {
			*o = *q;
			q++;
			o++;
		}
	}



	q = screen_one_byte->get_buf_one_byte();
	qq = q[0];

	//if (pal == NULL) pal = neww PAL[pal_count_max];

	unsigned short len;

	c1 = clock();

	len = 0;
	pal_count = 0;
	i = 0;
	while (i < zz) {
		if (*q == qq && len < 65000) {
			len++;
		}
		else {
			j = 0;
			k = 0;
			while (j < pal_count)
			{
				if (pal[j].color == qq &&
					pal[j].len == len)
				{
					//fprintf(f, "+[%d] %d , %d\r\n", j, pal[ j ].color , pal[ j ].len );

					pal[j].count++;
					k = 1;
					break;
				};
				j++;
			};

			if (k == 0)
			{
				//fprintf(f, "-[%d] %d , %d\r\n", j, qq , len );
				pal[pal_count].color = qq;
				pal[pal_count].len = len;
				pal[pal_count].count = 1;
				pal_count++;
				if (pal_count >= pal_count_max) {
					pal_increase_size();
				}

			};
			qq = *q;
			len = 1;
		};
		i++;
		q++;
	}

	//send__udp("ENCODE - - - - pal_count = ", pal_count);

	c2 = clock();
	j = 0;
	k = 0;
	while (j < pal_count)
	{
		if (pal[j].color == qq &&
			pal[j].len == len)
		{
			pal[j].count++;
			k = 1;
			break;
		};
		j++;
	};

	if (k == 0) {

		pal[pal_count].color = qq;
		pal[pal_count].len = len;
		//pal_count = 1;

		pal[pal_count].count = 1;
		pal_count++;

	};
	/////////////////////////////////////////////
	//set_status(L"pal_count==", pal_count);
	qsort((void *)pal, pal_count, sizeof(pal[0]), sort_function_pal);
	c3 = clock();
	/////////////////////////////////////////////

	//f = fopen("c:\\1\\ppp.txt", "wb");
	unsigned int jj;
	int c8, c16;
	c8 = 0;
	c16 = 0;

	unsigned char *vv;
	vv = (unsigned char *)&jj;




	//if (body == NULL) body = neww unsigned char[body_count_max];

	body_count = 0;
	q = screen_one_byte->get_buf_one_byte();

	i = 0;
	qq = q[0];
	len = 0;

	while (i < zz)
	{//22222

		if (*q == qq && len < 65000) {
			len++;
		}
		else {
			jj = 0;
			k = 0;
			while (jj < pal_count)
			{
				if (pal[jj].color == qq &&
					pal[jj].len == len)
				{
					if (jj < 128)
					{
						// TODO
						//fprintf(f, "%c", jj );
						body[body_count] = jj;
						body_count++;
						c8++;
						if (body_count >= body_count_max)
						{
							// 2019 11 TODO ! надо сделать body_increase_size();
							//unlock();
							fatal_error("body_count >= body_count_max (1)");
							return false;
						};
					}
					else
					{
						// TODO
						// fprintf(f, "%c%c", (vv[1] | 0x80), vv[0] );
						body[body_count] = (vv[1] | 0x80);
						body_count++;
						body[body_count] = vv[0];
						body_count++;
						c16++;
						if (body_count >= body_count_max)
						{
							// 2019 11 TODO ! надо сделать body_increase_size();
							//unlock();
							fatal_error("body_count >= body_count_max (2)");
							return false;
						};


					};
					//fprintf(f, "[%d] %d , %d\r\n", jj, pal[ jj ].color , pal[ jj ].len );
					k = 1;
					break;
				};
				jj++;
			};
			if (k == 0)
			{
				//unlock();
				fatal_error("err wtf ?? k == 0");
				return false;
			};
			qq = *q;
			len = 1;
		};
		i++;
		q++;
	};//22222



	////////////////////////
	c4 = clock();
	jj = 0;
	k = 0;
	while (jj < pal_count)
	{
		if (pal[jj].color == qq &&
			pal[jj].len == len)
		{
			if (jj < 128)
			{
				// TODO
				// fprintf(f, "%c", jj );
				body[body_count] = jj;
				c8++;
				body_count++;
				if (body_count >= body_count_max)
				{
					// 2019 11 TODO ! надо сделать body_increase_size();
					//unlock();
					fatal_error("body_count >= body_count_max (3)");
					return false;
				};

			}
			else
			{
				// TODO
				// fprintf(f, "%c%c", (vv[1] | 0x80), vv[0] );
				body[body_count] = (vv[1] | 0x80);
				body_count++;
				body[body_count] = vv[0];
				body_count++;
				if (body_count >= body_count_max)
				{
					// 2019 11 TODO ! надо сделать body_increase_size();
					//unlock();
					fatal_error("body_count >= body_count_max (4)");
					return false;
				};
			};
			k = 1;
			break;
		};
		jj++;
	};
	c5 = clock();
	/////////////////////////////////////////////////////////////
	int header_size, pal_size, body_size;

	header_size = 16 * 4;

	pal_size = pal_count * 4;
	body_size = body_count;


	encoded_buffer_len = header_size + pal_size + body_size;

	if (encoded_buffer_len_max < encoded_buffer_len) {

		if (encoded_buffer != NULL) delete[] encoded_buffer;
		encoded_buffer_len_max = (int)((float)encoded_buffer_len * 1.5);
		encoded_buffer = new unsigned char[encoded_buffer_len_max]; //neew(encoded_buffer_len_max, encoded_buffer);
	};

	ENCODED_SCREEN_8bit_header *header;

	header = (ENCODED_SCREEN_8bit_header *)encoded_buffer;

	header->reserv01 = 0;
	header->reserv02 = 0;
	header->reserv03 = 0;
	header->reserv04 = 0;

	header->format = 2;
	header->w = screen_one_byte->header.w;
	header->h = screen_one_byte->header.h;
	header->color_bit = 32;// raw_screen->color_mode;
	header->reserve1 = 0;
	header->reserve2 = 0;
	header->reserve3 = 0;
	header->screen_id = screen_one_byte->screen_id;
	header->old_screen_id = old_screen_id;
	header->pal_size = pal_count * 4;
	header->body_size = body_count;
	header->header_size = header_size;

	header->mouse_x = screen_one_byte->header.mouse_x;
	header->mouse_y = screen_one_byte->header.mouse_y;
	header->mouse_cursor_type_id = screen_one_byte->header.mouse_cursor_type_id;
	
	if (screen_one_byte->header.mouse_x == last_set_mouse_x && screen_one_byte->header.mouse_y == last_set_mouse_y) {
		header->itis_user_move_mouse = 100;
	}
	else {
		header->itis_user_move_mouse = 200;
	}

	header->keyboard_location = screen_one_byte->header.keyboard_location;



	C8SH16 *cs;

	//char ssd[500];
	//sprintf_ s(ssd, 250, "w-%d h-%d c-%d p-%d b-%d k-%X len=%d ", raw_screen->w, raw_screen->h, raw_screen->color_mode, pal_count, body_count , raw_screen->raw_keyboard_location, encoded_buffer_len);
	//send_udp(ssd);

	cs = (C8SH16 *)&encoded_buffer[header_size];
	for (i = 0; i < pal_count; i++)
	{
		//sprintf_ s(ssd, 500, "PAL[%d] %02X %d ", i, pal[i].color, pal[i].len );
		//send_udp(ssd);
		cs->c = pal[i].color;
		cs->s = pal[i].len;
		cs++;
	};
	c6 = clock();

	unsigned char *v, *bd;

	v = (unsigned char *)&(encoded_buffer[header_size + pal_count * 4]);
	bd = body;
	for (int i = 0; i < body_count; i++)
	{
		*v = *bd;
		bd++;
		v++;
	};
	c7 = clock();

	old_screen_id = screen_one_byte->screen_id;

	//save_screen_to_file(encoded_buffer, encoded_buffer_len);

	return true;

}
*/

SCREEN_LIGHT_12bit::SCREEN_LIGHT_12bit() {

}

SCREEN_LIGHT_12bit::~SCREEN_LIGHT_12bit() {

}

void SCREEN_LIGHT_12bit::emulate_red() {

	set_new_size_(250, 250);

	unsigned char *q;

	for (int j = 0; j < 250; j++) {
		q = buf + j * 250;
		for (int i = 0; i < 250; i+=2) {
			*q++ = 0x00;
			*q++ = 0xf0;
			*q++ = 0x0f;
		}
	}

	buf_len = 250 / 2 * 250 * 3;

	// 1 - 7 - красный
	// 8 16 - зеленый
	// 32 - темно синий
}

void SCREEN_LIGHT_12bit::clean_() {
	old_screen_id = -1;
}

void SCREEN_LIGHT_12bit::set_new_size_(int w_, int h_) {
	if (w_ <= 0 || h_ <= 0) return;

	if (w == w_ && h == h_) return;
	if (buf != nullptr) delete[] buf;
	w = w_;
	h = h_;
	buf_max_size_ = w * h * 3;
	buf = new unsigned char[buf_max_size_];
	buf_len = w * h * 3;

	//send_udp("set_new_size_() - - - - - - -");
}



const char hello[] = "hello, hello!";

#define HEAP_ALLOC(var,size) \
    lzo_align_t __LZO_MMODEL var [ ((size) + (sizeof(lzo_align_t) - 1)) / sizeof(lzo_align_t) ]

static HEAP_ALLOC(wrkmem, LZO1X_1_MEM_COMPRESS);


void SCREEN_LIGHT_12bit::encode() {

	DWORD d1 = 0, d2 = 0, d3 = 0, d4 = 0;

	

	unsigned char *xx, *yy;
	xx = new unsigned char[32000000];

	yy = new unsigned char[32000000];

	unsigned long  xxx, yyy;
	xxx = 32000000;
	yyy = 12000000;

	for (int i = 0; i < 32000000; i++) {
		xx[i] = 0;
		yy[i] = 0;
	};

	int res, res2;

	/***

	d1 = GetTickCount();

	res = compress2( xx, &xxx, (unsigned char *)hello, 13 , 9);

	d2 = GetTickCount();

	res2 = uncompress( yy, &yyy, xx, xxx );

	d3 = GetTickCount();
	d4 = GetTickCount();
	***/

	// BZ2_bzBuffToBuffCompress

	int nIn, nOut, nZ;
	int   r;

	if (lzo_init() != LZO_E_OK)
	{
		return;
	}


	d1 = GetTickCount();

	r = lzo1x_1_compress((unsigned char *)buf, buf_len, xx,&xxx, wrkmem);
	if (r == LZO_E_OK) {

	}
		


	//nIn = BZ2_bzBuffToBuffCompress( (char *)xx, &xxx, (char *)buf, buf_size, 9, 0, 0);

	d2 = GetTickCount();

	r = lzo1x_decompress(xx, xxx, yy, &yyy, NULL);
	if (r == LZO_E_OK ) {

	}


	//nOut = BZ2_bzBuffToBuffDecompress((char *)yy, &yyy, (char *)xx, xxx, 1, 0);

	d3 = GetTickCount();

	/***
	if (pal12 == nullptr) {
		pal12 = new_PAL12();
		pal12_index = new PAL12*[PAL12_color_maxcount * PAL12_len_maxcount];
	}

	if (body12 == nullptr) {
		body12_max_count = 600000;
		body12 = new unsigned int[body12_max_count];
	}



	unsigned short color;
	unsigned int ll, color_un_len;
	int i, j, k, kk;

	k = 0;
	kk = header.w * header.h;
	while (k < kk)
	{
		color = buf[k];
		ll = calc12_eqvival_len(k);

		color_un_len = color * PAL12_len_maxcount + ll;

		pal12[color_un_len].count += ll;

		body12[body12_count] = color_un_len;

		body12_count++;
		if (body12_count >= body12_max_count)
		{
			body12_count = 1;
		};


		//ii = add_to_pal12( color , ll );
		k += ll;
	};

	d2 = GetTickCount();

	//for (i = 0; i < PAL12_color_maxcount * PAL12_len_maxcount; i++) pal12_index[i] = nullptr;

	PAL12 *pp;

	pp = pal12_index[0];

	pal12_index_count = 0;
	i = 0;
	j = PAL12_color_maxcount * PAL12_len_maxcount;
	while (i < j)
	{

		if (pal12[i].count > 0)
		{

			pal12_index[pal12_index_count] = &pal12[i];
			pal12_index[pal12_index_count]->sort = 0;
			pal12_index_count++;
		};

		i++;
	};

	pal12_index[pal12_index_count] = nullptr;

	d3 = GetTickCount();

	qsort(&(pal12_index[0]), pal12_index_count, 4, sort_function_pal12);

	d4 = GetTickCount();


	for (i = 0; i < pal12_index_count; i++)
	{
		pal12_index[i]->sort = i;
		HE_add_source_element(pal12_index[i]->count);
	};

	HAFMAN_element *e;

	e = serach_elements_to_PLUS();
	while (e != NULL)
	{

		HE_plus_LEFT(e);
		e = serach_elements_to_PLUS();
	};
	***/



	d4 = GetTickCount();


	char ss[1000];
	ss[0] = 0;

	sprintf_s(ss, 900, "d2=%u\r\nd3=%u\r\nd4=%u\r\n", d2 - d1, d3 - d1, d4 - d1);

	FILE *f;

	fopen_s(&f, "c:\\1\\dd.txt", "wb");

	fprintf(f, "%s", ss);

	fclose(f);


}

void encode_color_12bit_0(unsigned int zzz1, unsigned int zzz2, unsigned char *ee) {

	unsigned int r0, r1, r2, r3, r4, r5;
	
	r0 = (zzz1 & 0xff0000); r0 = (r0 >> 16); r0 /= 16;
	r1 = zzz1 & 0xff00;   	r1 = r1 >> 8;    r1 /= 16;
	r2 = zzz1 & 0xff;                        r2 /= 16;

	r3 = zzz2 & 0xff0000; r3 = r3 >> 16; r3 /= 16;
	r4 = zzz2 & 0xff00;   r4 = r4 >> 8;  r4 /= 16;
	r5 = zzz2 & 0xff;                    r5 /= 16;

	ee[0] = ((r0 << 4) | r1);
	ee[1] = ((r2 << 4) | r3);
	ee[2] = ((r4 << 4) | r5);

}

void SCREEN_LIGHT_12bit::get_screen_from_BitBlt_buffer(void *BitBlt_raw_buffer, int g_nWidth, int g_nHeight, int g_nColorMode) {
	
	unsigned int *q, *o;
	
	unsigned char *ee;
	unsigned int *z, *zz, zzz1, zzz2, val;
	int jj, y, x, wh; // fe

	ee = (unsigned char *)buf;

	buf_len = 0;
	if (g_nColorMode == 32) {

		if (g_nWidth != old_w || g_nHeight != old_h) {
			old_screen_id = -1;
			old_w = g_nWidth;
			old_h = g_nHeight;

			if (old__buf != nullptr) delete[] old__buf;
			old__buf_size = old_w * old_h;
			old__buf = new unsigned int[old__buf_size];
		}

		//////////////////===================================================================

		q = (unsigned int *)BitBlt_raw_buffer;
		o = old__buf;


		//send__udp("ENCODE - - - - raw_screen->w = ", raw_screen->w );

		wh = g_nWidth * g_nHeight;
		//cc1 = 0;
		//cc2 = 0;
		//cc3 = 0;
		if (old_screen_id > 0) {
			for (int i = 0; i < wh; i++) {
				val = *q;
				//if (val == 0xfe) cc3++;
				if (*o == val) {
					*q = 0xffffE0;
					//cc1++;
				}
				else {
					*o = val;
					//cc2++;
				}
				q++;
				o++;
			}
		}
		else {
			for (int i = 0; i < wh; i++) {
				*o = *q;
				q++;
				o++;
			}
		}

		/////////======================================================================

		z = (unsigned int *)BitBlt_raw_buffer;

		jj = 0;

		for (y = 0; y < g_nHeight; y++)
		{

			zz = &(z[(g_nHeight - 1 - y)*g_nWidth]);

			for (x = 0; x < g_nWidth; x+=2)
			{
				zzz1 = *zz & 0xffffff; zz++;
				zzz2 = *zz & 0xffffff; zz++;

				encode_color_12bit_0(zzz1, zzz2, ee);

				ee += 3;				
				buf_len += 3;
			};
		};

	};
}

//*********************************************************************************************************************************


SCREEN_LIGHT_encoded_12bit::SCREEN_LIGHT_encoded_12bit() {

}

SCREEN_LIGHT_encoded_12bit::~SCREEN_LIGHT_encoded_12bit() {

}


unsigned char *decompress_buffer = nullptr;
unsigned int   decompress_buffer_max_size = 0;

bool decode_screen_12bit_first(unsigned char *buf, int buf_size, SCREEN_LIGHT *raw_screen) {
	if (buf == NULL) {
		fatal_error("SCREEN::decode() buf == NULL");
		return false;
	};

	if (raw_screen == NULL) return false;

	int ww, hh, scr_id, old_scr_id, bdy_size, i, pl_idx, pl_size, j;

	ENCODED_SCREEN_12bit_header *hdr;
	
	int header_size = 16 * 4;

	hdr = (ENCODED_SCREEN_12bit_header *)buf;

	if (hdr->format != 3) {

		fatal_error("SCREEN::decode() hdr->format != 3 ");
		return false;
	};

	if (hdr->color_bit != 32) {

		fatal_error("SCREEN::decode() hdr->color_bit != 32");
		//ALOG("hdr->color_bit = %d ", hdr->color_bit );
		return false;
	};

	ww = hdr->w;
	hh = hdr->h;


	if (ww < 0 || ww > 5000 || hh < 0 || hh > 5000) {

		fatal_error("SCREEN::decode() ww < 0 || ww > 5000 || hh < 0 || hh > 5000 ");
		return false;
	}

	raw_screen->keyboard_location = hdr->keyboard_location;

	//desk.keyboard.set_current_layout_by_code(keyboard_location);

	if (ww != raw_screen->w || hh != raw_screen->h) {
		raw_screen->lock_FULL(3000);
		raw_screen->set_new_size(ww, hh);
	};

	


	unsigned int *q, clr, ii, jj, q_max, q_idx, r, x, y, z1, z2, r0, r1, r2, r3, r4, r5;
	unsigned char *d;
	lzo_uint sz;
	q_max = ww * hh;
	q_idx = 0;

	q = raw_screen->get_buf();



	if (decompress_buffer_max_size < ww * hh * 3 * 2) {
		decompress_buffer_max_size = ww * hh * 3 * 2;
		if (decompress_buffer != nullptr) delete[] decompress_buffer;
		decompress_buffer = new unsigned char[decompress_buffer_max_size];
	}
	
	r = lzo1x_decompress(buf + 64, hdr->body_size, decompress_buffer, &sz, NULL);
	if (r == LZO_E_OK) {

	}

	if (sz != ww / 2 * 3 * hh) {

		set_GLOBAL_STOP_true();
		return false;
	}
	
	d = decompress_buffer;

	q = raw_screen->get_buf();
	for (y = 0; y < hh; y++) {
		for (x = 0; x < ww / 2; x++) {

			r0 = *d & 0xf0; r0 = r0 >> 4; 
			r1 = *d & 0x0f;
			d++;
			
			r2 = *d & 0xf0; r2 = r2 >> 4;
			r3 = *d & 0x0f;
			d++;

			r4 = *d & 0xf0; r4 = r4 >> 4;
			r5 = *d & 0x0f;
			d++;

			r0 *= 16; r0 = r0 | 0x0f;
			r1 *= 16; r1 = r1 | 0x0f;
			r2 *= 16; r2 = r2 | 0x0f;
			r3 *= 16; r3 = r3 | 0x0f;
			r4 *= 16; r4 = r4 | 0x0f;
			r5 *= 16; r5 = r5 | 0x0f;

			z1 = (r0 << 16) | (r1 << 8) | r2;
			z2 = (r3 << 16) | (r4 << 8) | r5;

			if (z1 == 0xffffEf) {
				q++;
			}
			else {
				*q++ = z1;
			};

			if (z2 == 0xffffEf) {
				q++;
			}
			else {
				*q++ = z2;
			};
		}
	}
	
	scr_id = hdr->screen_id;
	old_scr_id = hdr->old_screen_id;


	raw_screen->mouse_x = hdr->mouse_x;
	raw_screen->mouse_y = hdr->mouse_y;
	raw_screen->mouse_cursor_type_id = hdr->mouse_cursor_type_id;
	raw_screen->_itis_user_move_mouse_ = hdr->itis_user_move_mouse;
	raw_screen->keyboard_location = hdr->keyboard_location;


	raw_screen->screen_id = scr_id;
	raw_screen->old_screen_id = old_scr_id;

	return true;


}


//============================================================================================

SCREEN_LIGHT_encoded_8bit_second::SCREEN_LIGHT_encoded_8bit_second() {

};

SCREEN_LIGHT_encoded_8bit_second::~SCREEN_LIGHT_encoded_8bit_second() {

	if(old_buf_one_byte != nullptr) delete[] old_buf_one_byte;
	old_buf_one_byte_size = 0;

	
	if(encoded_buffer != nullptr) delete[] encoded_buffer;

}

void SCREEN_LIGHT_encoded_8bit_second::clean_() {
	old_screen_id = -1;
}

void SCREEN_LIGHT_encoded_12bit::clean_() {
	old_screen_id = -1;
}

bool SCREEN_LIGHT_encoded_8bit_first::encode_screen_ONE_BYTE(SCREEN_LIGHT_one_byte *screen_one_byte, int last_set_mouse_x, int last_set_mouse_y) {

	if (screen_one_byte->screen_id == 0) {
		//send_udp("SCREEN::encode() raw_screen->screen_id == 0 error");
		return false;
	}

	if (screen_one_byte->screen_id == old_screen_id) {
		//fatal_error("SCREEN::encode() screen_id == old_screen_id wtf??");
		return false;
	}

	if (screen_one_byte->header.w == 0 || screen_one_byte->header.h == 0) return false;
	if (screen_one_byte->header.w < 0 || screen_one_byte->header.h < 0) return false;
	if (screen_one_byte->header.w > 5500 || screen_one_byte->header.h > 5500) return false;

	if (screen_one_byte->header.w != old_w || screen_one_byte->header.h != old_h) {
		old_screen_id = -1;
		old_w = screen_one_byte->header.w;
		old_h = screen_one_byte->header.h;

		if (old_buf_one_byte != NULL) delete[] old_buf_one_byte;
		old_buf_one_byte_size = old_w * old_h;
		old_buf_one_byte = new unsigned char[old_buf_one_byte_size];
	}

	if (pal == NULL) {
		pal_count_max = 2000;
		pal = new_PAL(pal_count_max);     //        neew(sizeof(PAL) * pal_count_max, pal, "SCREEN::SCREEN() neww PAL[xxx]");

	}

	if (body == NULL) {
		body_count_max = 1980 * 1200 * 4;
		body = new unsigned char[body_count_max]; //neew(body_count_max, body, "SCREEN::SCREEN() body");
	};

	unsigned int pal_count;

	int body_count;

	pal_count = 0;
	body_count = 0;
	encoded_buffer_len = 0;

	unsigned int i, j, k, zz, c1, c2, c3, c4, c5, c6, c7;// , cc1, cc2, cc3;
	unsigned char *q, qq, *o, val;

	//lock();

	q = screen_one_byte->get_buf_one_byte();
	o = old_buf_one_byte;


	//send__udp("ENCODE - - - - raw_screen->w = ", raw_screen->w );

	zz = screen_one_byte->header.w * screen_one_byte->header.h;
	//cc1 = 0;
	//cc2 = 0;
	//cc3 = 0;
	if (old_screen_id > 0) {
		for (i = 0; i < zz; i++) {
			val = *q;
			//if (val == 0xfe) cc3++;
			if (*o == val) {
				*q = 0xfe;
				//cc1++;
			}
			else {
				*o = val;
				//cc2++;
			}
			q++;
			o++;
		}
	}
	else {
		for (i = 0; i < zz; i++) {
			*o = *q;
			q++;
			o++;
		}
	}



	q = screen_one_byte->get_buf_one_byte();
	qq = q[0];

	//if (pal == NULL) pal = neww PAL[pal_count_max];

	unsigned short len;

	c1 = clock();

	len = 0;
	pal_count = 0;
	i = 0;
	while (i < zz) {
		if (*q == qq && len < 65000) {
			len++;
		}
		else {
			j = 0;
			k = 0;
			while (j < pal_count)
			{
				if (pal[j].color == qq &&
					pal[j].len == len)
				{
					//fprintf(f, "+[%d] %d , %d\r\n", j, pal[ j ].color , pal[ j ].len );

					pal[j].count++;
					k = 1;
					break;
				};
				j++;
			};

			if (k == 0)
			{
				//fprintf(f, "-[%d] %d , %d\r\n", j, qq , len );
				pal[pal_count].color = qq;
				pal[pal_count].len = len;
				pal[pal_count].count = 1;
				pal_count++;
				if (pal_count >= pal_count_max) {
					pal_increase_size();
				}
				/*unlock();
				fatal_error("pal_count >= pal_count_max");
				return false;*/

			};
			qq = *q;
			len = 1;
		};
		i++;
		q++;
	}

	//send__udp("ENCODE - - - - pal_count = ", pal_count);

	c2 = clock();
	j = 0;
	k = 0;
	while (j < pal_count)
	{
		if (pal[j].color == qq &&
			pal[j].len == len)
		{
			pal[j].count++;
			k = 1;
			break;
		};
		j++;
	};

	if (k == 0) {

		pal[pal_count].color = qq;
		pal[pal_count].len = len;
		//pal_count = 1;

		pal[pal_count].count = 1;
		pal_count++;

	};
	/////////////////////////////////////////////
	//set_status(L"pal_count==", pal_count);
	qsort((void *)pal, pal_count, sizeof(pal[0]), sort_function_pal);
	c3 = clock();
	/////////////////////////////////////////////

	//f = fopen("c:\\1\\ppp.txt", "wb");
	unsigned int jj;
	int c8, c16;
	c8 = 0;
	c16 = 0;

	unsigned char *vv;
	vv = (unsigned char *)&jj;




	//if (body == NULL) body = neww unsigned char[body_count_max];

	body_count = 0;
	q = screen_one_byte->get_buf_one_byte();

	i = 0;
	qq = q[0];
	len = 0;

	while (i < zz)
	{//22222

		if (*q == qq && len < 65000) {
			len++;
		}
		else {
			jj = 0;
			k = 0;
			while (jj < pal_count)
			{
				if (pal[jj].color == qq &&
					pal[jj].len == len)
				{
					if (jj < 128)
					{
						// TODO
						//fprintf(f, "%c", jj );
						body[body_count] = jj;
						body_count++;
						c8++;
						if (body_count >= body_count_max)
						{
							// 2019 11 TODO ! надо сделать body_increase_size();
							//unlock();
							fatal_error("body_count >= body_count_max (1)");
							return false;
						};
					}
					else
					{
						// TODO
						// fprintf(f, "%c%c", (vv[1] | 0x80), vv[0] );
						body[body_count] = (vv[1] | 0x80);
						body_count++;
						body[body_count] = vv[0];
						body_count++;
						c16++;
						if (body_count >= body_count_max)
						{
							// 2019 11 TODO ! надо сделать body_increase_size();
							//unlock();
							fatal_error("body_count >= body_count_max (2)");
							return false;
						};


					};
					//fprintf(f, "[%d] %d , %d\r\n", jj, pal[ jj ].color , pal[ jj ].len );
					k = 1;
					break;
				};
				jj++;
			};
			if (k == 0)
			{
				//unlock();
				fatal_error("err wtf ?? k == 0");
				return false;
			};
			qq = *q;
			len = 1;
		};
		i++;
		q++;
	};//22222



	////////////////////////
	c4 = clock();
	jj = 0;
	k = 0;
	while (jj < pal_count)
	{
		if (pal[jj].color == qq &&
			pal[jj].len == len)
		{
			if (jj < 128)
			{
				// TODO
				// fprintf(f, "%c", jj );
				body[body_count] = jj;
				c8++;
				body_count++;
				if (body_count >= body_count_max)
				{
					// 2019 11 TODO ! надо сделать body_increase_size();
					//unlock();
					fatal_error("body_count >= body_count_max (3)");
					return false;
				};

			}
			else
			{
				// TODO
				// fprintf(f, "%c%c", (vv[1] | 0x80), vv[0] );
				body[body_count] = (vv[1] | 0x80);
				body_count++;
				body[body_count] = vv[0];
				body_count++;
				if (body_count >= body_count_max)
				{
					// 2019 11 TODO ! надо сделать body_increase_size();
					//unlock();
					fatal_error("body_count >= body_count_max (4)");
					return false;
				};
			};
			k = 1;
			break;
		};
		jj++;
	};
	c5 = clock();
	/////////////////////////////////////////////////////////////
	int header_size, pal_size, body_size;

	header_size = 16 * 4;

	pal_size = pal_count * 4;
	body_size = body_count;


	encoded_buffer_len = header_size + pal_size + body_size;

	if (encoded_buffer_len_max < encoded_buffer_len) {

		if (encoded_buffer != NULL) delete[] encoded_buffer;
		encoded_buffer_len_max = (int)((float)encoded_buffer_len * 1.5);
		encoded_buffer = new unsigned char[encoded_buffer_len_max]; //neew(encoded_buffer_len_max, encoded_buffer);
	};

	ENCODED_SCREEN_8bit_header *header;

	header = (ENCODED_SCREEN_8bit_header *)encoded_buffer;

	header->reserv01 = 0;
	header->reserv02 = 0;
	header->reserv03 = 0;
	header->reserv04 = 0;

	header->format = 2;
	header->w = screen_one_byte->header.w;
	header->h = screen_one_byte->header.h;
	header->color_bit = 32;// raw_screen->color_mode;
	header->reserve1 = 0;
	header->reserve2 = 0;
	header->reserve3 = 0;
	header->screen_id = screen_one_byte->screen_id;
	header->old_screen_id = old_screen_id;
	header->pal_size = pal_count * 4;
	header->body_size = body_count;
	header->header_size = header_size;

	header->mouse_x = screen_one_byte->header.mouse_x;
	header->mouse_y = screen_one_byte->header.mouse_y;
	header->mouse_cursor_type_id = screen_one_byte->header.mouse_cursor_type_id;

	if (screen_one_byte->header.mouse_x == last_set_mouse_x && screen_one_byte->header.mouse_y == last_set_mouse_y) {
		header->itis_user_move_mouse = 100;
	}
	else {
		header->itis_user_move_mouse = 200;
	}

	header->keyboard_location = screen_one_byte->header.keyboard_location;



	C8SH16 *cs;

	//char ssd[500];
	//sprintf_ s(ssd, 250, "w-%d h-%d c-%d p-%d b-%d k-%X len=%d ", raw_screen->w, raw_screen->h, raw_screen->color_mode, pal_count, body_count , raw_screen->raw_keyboard_location, encoded_buffer_len);
	//send_udp(ssd);

	cs = (C8SH16 *)&encoded_buffer[header_size];
	for (i = 0; i < pal_count; i++)
	{
		//sprintf_ s(ssd, 500, "PAL[%d] %02X %d ", i, pal[i].color, pal[i].len );
		//send_udp(ssd);
		cs->c = pal[i].color;
		cs->s = pal[i].len;
		cs++;
	};
	c6 = clock();

	unsigned char *v, *bd;

	v = (unsigned char *)&(encoded_buffer[header_size + pal_count * 4]);
	bd = body;
	for (int i = 0; i < body_count; i++)
	{
		*v = *bd;
		bd++;
		v++;
	};
	c7 = clock();

	old_screen_id = screen_one_byte->screen_id;

	//save_screen_to_file(encoded_buffer, encoded_buffer_len);

	return true;

}


bool SCREEN_LIGHT_encoded_8bit_second::encode_screen_ONE_BYTE_second(SCREEN_LIGHT_one_byte *screen_one_byte, int last_set_mouse_x, int last_set_mouse_y) {

	if (screen_one_byte->screen_id == 0) {
		//send_udp("SCREEN::encode() raw_screen->screen_id == 0 error");
		return false;
	}

	if (screen_one_byte->screen_id == old_screen_id) {
		//fatal_error("SCREEN::encode() screen_id == old_screen_id wtf??");
		return false;
	}

	if (screen_one_byte->header.w == 0 || screen_one_byte->header.h == 0) return false;
	if (screen_one_byte->header.w < 0 || screen_one_byte->header.h < 0) return false;
	if (screen_one_byte->header.w > 5500 || screen_one_byte->header.h > 5500) return false;

	if (screen_one_byte->header.w != old_w || screen_one_byte->header.h != old_h) {
		old_screen_id = -1;
		old_w = screen_one_byte->header.w;
		old_h = screen_one_byte->header.h;

		if (old_buf_one_byte != NULL) delete[] old_buf_one_byte;
		old_buf_one_byte_size = old_w * old_h;
		old_buf_one_byte = new unsigned char[old_buf_one_byte_size];
	}



	encoded_buffer_len = 0;

	unsigned int i, j, k, zz, c1, c2, c3, c4, c5, c6, c7;// , cc1, cc2, cc3;
	unsigned char *q, qq, *o, val;

	//lock();

	q = screen_one_byte->get_buf_one_byte();
	o = old_buf_one_byte;


	//send__udp("ENCODE - - - - raw_screen->w = ", raw_screen->w );

	zz = screen_one_byte->header.w * screen_one_byte->header.h;
	//cc1 = 0;
	//cc2 = 0;
	//cc3 = 0;
	if (old_screen_id > 0) {
		for (i = 0; i < zz; i++) {
			val = *q;
			//if (val == 0xfe) cc3++;
			if (*o == val) {
				*q = 0xfe;
				//cc1++;
			}
			else {
				*o = val;
				//cc2++;
			}
			q++;
			o++;
		}
	}
	else {
		for (i = 0; i < zz; i++) {
			*o = *q;
			q++;
			o++;
		}
	}



	q = screen_one_byte->get_buf_one_byte();
	qq = q[0];

	//if (pal == NULL) pal = neww PAL[pal_count_max];

	unsigned short len;

	

	//send__udp("ENCODE - - - - pal_count = ", pal_count);

	c2 = clock();
	
	c3 = clock();
	/////////////////////////////////////////////

	c5 = clock();
	/////////////////////////////////////////////////////////////
	int header_size, r;

	header_size = 16 * 4;

	if (encoded_buffer_max_size < zz * 2) {
		encoded_buffer_max_size = zz * 2;
		if (encoded_buffer != nullptr) delete[] encoded_buffer;
		encoded_buffer = new unsigned char[encoded_buffer_max_size];
	}

	lzo_uint size;
	size = encoded_buffer_max_size - (16 * 4);

	r = lzo1x_1_compress((unsigned char *)screen_one_byte->get_buf_one_byte(), zz, encoded_buffer + (16 * 4), &size, wrkmem);
	if (r == LZO_E_OK) {

	}
	/*
	FILE *f;
	fopen_s(&f, "c:\\1\\cmp.txt", "ab");
	if (f != NULL) {
		fprintf(f, "%d %02X-%02X-%02X", size, encoded_buffer[64], encoded_buffer[65], encoded_buffer[66]);
		fclose(f);
	}
	*/

	encoded_buffer_len = size;


	ENCODED_SCREEN_8bit_header *header;

	header = (ENCODED_SCREEN_8bit_header *)encoded_buffer;

	header->reserv01 = 0;
	header->reserv02 = 0;
	header->reserv03 = 0;
	header->reserv04 = 0;

	header->format = 4;
	header->w = screen_one_byte->header.w;
	header->h = screen_one_byte->header.h;
	header->color_bit = 32;// raw_screen->color_mode;
	header->reserve1 = 0;
	header->reserve2 = 0;
	header->reserve3 = 0;
	header->screen_id = screen_one_byte->screen_id;
	header->old_screen_id = old_screen_id;
	header->pal_size = 0;
	header->body_size = size;
	header->header_size = header_size;

	header->mouse_x = screen_one_byte->header.mouse_x;
	header->mouse_y = screen_one_byte->header.mouse_y;
	header->mouse_cursor_type_id = screen_one_byte->header.mouse_cursor_type_id;

	if (screen_one_byte->header.mouse_x == last_set_mouse_x && screen_one_byte->header.mouse_y == last_set_mouse_y) {
		header->itis_user_move_mouse = 100;
	}
	else {
		header->itis_user_move_mouse = 200;
	}

	header->keyboard_location = screen_one_byte->header.keyboard_location;


	c6 = clock();

	c7 = clock();

	old_screen_id = screen_one_byte->screen_id;

	//save_screen_to_file(encoded_buffer, encoded_buffer_len);

	return true;

};


bool decode_screen_8bit_second(unsigned char *buf, int buf_size, SCREEN_LIGHT *raw_screen) {
	if (buf == NULL) {
		fatal_error("SCREEN::decode() buf == NULL");
		return false;
	};

	if (raw_screen == NULL) return false;

	int ww, hh, scr_id, old_scr_id, bdy_size, i, pl_idx, pl_size, j;

	ENCODED_SCREEN_12bit_header *hdr;

	int header_size = 16 * 4;

	hdr = (ENCODED_SCREEN_12bit_header *)buf;

	if (hdr->format != 4) {

		fatal_error("SCREEN::decode() hdr->format != 4 ");
		return false;
	};

	if (hdr->color_bit != 32) {

		fatal_error("SCREEN::decode() hdr->color_bit != 32");
		//ALOG("hdr->color_bit = %d ", hdr->color_bit );
		return false;
	};

	ww = hdr->w;
	hh = hdr->h;


	if (ww < 0 || ww > 5000 || hh < 0 || hh > 5000) {

		fatal_error("SCREEN::decode() ww < 0 || ww > 5000 || hh < 0 || hh > 5000 ");
		return false;
	}

	raw_screen->keyboard_location = hdr->keyboard_location;

	//desk.keyboard.set_current_layout_by_code(keyboard_location);

	if (ww != raw_screen->w || hh != raw_screen->h) {
		raw_screen->lock_FULL(3000);
		raw_screen->set_new_size(ww, hh);
	};




	unsigned int *q, clr, ii, jj, q_max, q_idx, r, x, y, z1, z2, r0, r1, r2, r3, r4, r5;
	unsigned char *d;
	lzo_uint sz;
	q_max = ww * hh;
	q_idx = 0;

	q = raw_screen->get_buf();



	if (decompress_buffer_max_size < ww * hh * 4) {
		decompress_buffer_max_size = ww * hh * 4;
		if (decompress_buffer != nullptr) delete[] decompress_buffer;
		decompress_buffer = new unsigned char[decompress_buffer_max_size];
	}

	r = lzo1x_decompress(buf + 64, hdr->body_size, decompress_buffer, &sz, NULL);
	if (r == LZO_E_OK) {

	}

	if (sz != ww * hh) {

		set_GLOBAL_STOP_true();
		return false;
	}

	d = decompress_buffer;

	q = raw_screen->get_buf();
	for (y = 0; y < hh; y++) {
		for (x = 0; x < ww ; x++) {

			if (*d == 0xfe) {
				q++;
				d++;
			}
			else {
				*q++ = decode_color_matrix_G7C223[*d++];
			};
		}
	}

	scr_id = hdr->screen_id;
	old_scr_id = hdr->old_screen_id;


	raw_screen->mouse_x = hdr->mouse_x;
	raw_screen->mouse_y = hdr->mouse_y;
	raw_screen->mouse_cursor_type_id = hdr->mouse_cursor_type_id;
	raw_screen->_itis_user_move_mouse_ = hdr->itis_user_move_mouse;
	raw_screen->keyboard_location = hdr->keyboard_location;


	raw_screen->screen_id = scr_id;
	raw_screen->old_screen_id = old_scr_id;

	return true;


}

void SCREEN_LIGHT_encoded_12bit::encode_screen_12bit(SCREEN_LIGHT_12bit *screen_12bit, int last_set_mouse_x, int last_set_mouse_y) {

	encoded_buffer_len = 0;

	if (encoded_buffer_max_size == 0) {

		init_encode_color_matrix_all();

		encoded_buffer_max_size = 1000;
		encoded_buffer = new unsigned char[encoded_buffer_max_size];
		if (lzo_init() != LZO_E_OK)
		{
			set_GLOBAL_STOP_true();
			return;
		}
	}

	

	

	int r;

	//d1 = GetTickCount();

	if (encoded_buffer_max_size < screen_12bit->buf_len * 2) {
		encoded_buffer_max_size = screen_12bit->buf_len * 2;
		if (encoded_buffer != nullptr) delete[] encoded_buffer;
		encoded_buffer = new unsigned char[encoded_buffer_max_size];
	}

	lzo_uint size;
	size = encoded_buffer_max_size - sizeof(ENCODED_SCREEN_12bit_header);

	r = lzo1x_1_compress((unsigned char *)screen_12bit->buf, screen_12bit->buf_len, encoded_buffer + sizeof(ENCODED_SCREEN_12bit_header), &size, wrkmem);
	if (r == LZO_E_OK) {

	}
	/*
	FILE *f;
	fopen_s(&f, "c:\\1\\cmp.txt", "ab");
	if (f != NULL) {
		fprintf(f, "%d %02X-%02X-%02X", size, encoded_buffer[64], encoded_buffer[65], encoded_buffer[66]);
		fclose(f);
	}
	*/

	encoded_buffer_len = size;

	//************************************************************

	ENCODED_SCREEN_12bit_header *header;

	header = (ENCODED_SCREEN_12bit_header *)encoded_buffer;

	header->reserv01 = 0;
	header->reserv02 = 0;
	header->reserv03 = 0;
	header->reserv04 = 0;

	header->format = 3;
	header->w = screen_12bit->w;
	header->h = screen_12bit->h;
	header->color_bit = 32;// raw_screen->color_mode;
	header->reserve1 = 0;
	header->reserve2 = 0;
	header->reserve3 = 0;
	header->screen_id = screen_12bit->screen_id;
	header->old_screen_id = screen_12bit->old_screen_id;
	header->pal_size = 0;
	header->body_size = size;
	header->header_size = sizeof(ENCODED_SCREEN_12bit_header);

	header->mouse_x = screen_12bit->mouse_x;
	header->mouse_y = screen_12bit->mouse_y;
	header->mouse_cursor_type_id = screen_12bit->mouse_cursor_type_id;

	if (screen_12bit->mouse_x == last_set_mouse_x && screen_12bit->mouse_y == last_set_mouse_y) {
		header->itis_user_move_mouse = 100;
	}
	else {
		header->itis_user_move_mouse = 200;
	}

	header->keyboard_location = screen_12bit->keyboard_location;

	screen_12bit->old_screen_id = screen_12bit->screen_id;
}

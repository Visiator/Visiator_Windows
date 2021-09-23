#include "SCREEN.h"

#include "tools.h"
//#include "mem.h"

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


bool decode_screen(unsigned char *buf, int buf_size, SCREEN_LIGHT *raw_screen) {
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

SCREEN_LIGHT_encoded::SCREEN_LIGHT_encoded() {

};

SCREEN_LIGHT_encoded::~SCREEN_LIGHT_encoded() {

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

void SCREEN_LIGHT_encoded::pal_increase_size() {
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

void SCREEN_LIGHT_encoded::clean_() {
	for (int i = 0; i < body_count_max; i++) {
		body[i] = 0;
	}
	old_screen_id = -1;
}

bool SCREEN_LIGHT_encoded::encode_screen(SCREEN_LIGHT_one_byte *screen_one_byte, int last_set_mouse_x, int last_set_mouse_y ) {

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
	//qsort((void *)pal, pal_count, sizeof(pal[0]), sort_function_pal);
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
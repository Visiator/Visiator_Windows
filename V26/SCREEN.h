#pragma once

#define _WINSOCKAPI_ 
#include <windows.h>
#undef _WINSOCKAPI_
#include <winsock2.h>
#include <stdlib.h>
#include <iphlpapi.h>
#include <stdio.h>
#undef _WINSOCKAPI_

//#include "tools.h"

#define WM_LBUTTONDOWN__ 10101
#define WM_LBUTTONUP__ 10102
#define WM_RBUTTONDOWN__ 10103
#define WM_RBUTTONUP__ 10104
#define WM_MOUSEWHEEL__ 10105

#define EVENT_keyboard 20025 




class CHESS_SHADOW {
public:
	unsigned int *line_0 = NULL, *line_2 = NULL;
	int len = 0;

	void set_size(int len_);

	CHESS_SHADOW();
};


struct C8SH16
{
	unsigned short c;
	unsigned short s;
};

struct PAL
{
	unsigned char color;
	unsigned __int16 len;
	unsigned int count;
};

struct PAL12 {
	unsigned short color;
	unsigned short len;
	unsigned int   count;
	unsigned int   sort;
	PAL12() {
		color = 0;
		len = 0;
		count = 0;
		sort = 0;
	}
};

struct  ENCODED_SCREEN_12bit_header
{
	unsigned int reserv01;
	unsigned int reserv02;
	unsigned int reserv03;
	unsigned int reserv04;

	unsigned int format;
	unsigned int header_size; // 

	unsigned short w, h;

	unsigned char color_bit;
	unsigned char reserve1;
	unsigned char reserve2;
	unsigned char reserve3;

	unsigned int screen_id;
	unsigned int old_screen_id;
	unsigned int pal_size;
	unsigned int body_size;

	unsigned short mouse_x;
	unsigned short mouse_y;
	unsigned int   mouse_cursor_type_id;
	unsigned int   keyboard_location;
	unsigned int   itis_user_move_mouse;

};

#define PAL12_color_maxcount 4096
#define PAL12_len_maxcount 4096

class HAFMAN_element {
public:
	int w;
	int level;
	bool in_use;
	HAFMAN_element *left;
	HAFMAN_element *right;
	HAFMAN_element *top;
	HAFMAN_element *child0;
	HAFMAN_element *child1;

	void generate_code(char *str, char add);

	void clean()
	{
		left = NULL;
		right = NULL;
		top = NULL;
		child0 = NULL;
		child1 = NULL;
		level = 0;
		w = 0;
		in_use = false;
	};
	HAFMAN_element()
	{
		clean();
	};
};

class HAFMAN_pool {
public:
	HAFMAN_element source[100000];
	int source_count;
	int source_max_count;

	HAFMAN_element *get_element();

	HAFMAN_pool();
};

class SCREEN_LIGHT_12bit {
public:

	HAFMAN_pool h_pool;
	HAFMAN_element *he_first = nullptr;
	HAFMAN_element *he_last  = nullptr;
	void HE_add_source_element(int w_);
	HAFMAN_element *serach_elements_to_PLUS();
	int HE_plus_LEFT(HAFMAN_element *v);

	unsigned int *body12 = nullptr;
	unsigned int body12_max_count = 0, body12_count = 0;

	PAL12 *new_PAL12();
	void  delete_PAL12(PAL12 **q);
	
	PAL12 **pal12_index = nullptr;

	PAL12 *pal12 = nullptr;
	unsigned int pal12_index_count = 0;

	ENCODED_SCREEN_12bit_header header;

	unsigned short *buf = nullptr;
	int buf_size = 0;
	
	unsigned int calc12_eqvival_len(unsigned short k);

	void encode();

	void set_new_size_(int w_, int h_);
	void load_from_BMP_buffer(BYTE *buf, SCREEN_LIGHT_12bit *b12);

	
	SCREEN_LIGHT_12bit();
	~SCREEN_LIGHT_12bit();
};


struct  ENCODED_SCREEN_8bit_header
{
	unsigned int reserv01;
	unsigned int reserv02;
	unsigned int reserv03;
	unsigned int reserv04;

	unsigned int format;
	unsigned int header_size; // 

	unsigned short w, h;

	unsigned char color_bit;
	unsigned char reserve1;
	unsigned char reserve2;
	unsigned char reserve3;

	unsigned int screen_id;
	unsigned int old_screen_id;
	unsigned int pal_size;
	unsigned int body_size;

	unsigned short mouse_x;
	unsigned short mouse_y;
	unsigned int   mouse_cursor_type_id;
	unsigned int   keyboard_location;
	unsigned int   itis_user_move_mouse;

};


class SCREEN_LIGHT_one_byte
{
private:
	unsigned char *buf_one_byte = NULL;
public:
	void buf_one_byte_NEW(int size);
	void buf_one_byte_DELETE();

	bool is_lock = false;
	void lock();
	void unlock();

	unsigned char *get_buf_one_byte();

	int screen_id = 0;
	
	ENCODED_SCREEN_8bit_header header;

	
	int buf_one_byte_size = 0;

	

	void set_new_size_(int w_, int h_);

	void clean();
	void clean_();

	void emulate_red();
	void emulate_dark_blue();
	//void emulate_green_chess();

	SCREEN_LIGHT_one_byte();
	~SCREEN_LIGHT_one_byte();
};

class SCREEN_LIGHT
{
private:
	unsigned int  *buf = NULL;
public:
	bool is_lock_FULL = false;
	bool is_lock_READ = false;
	int nom_lock = 0;

	unsigned int keyboard_location = 0;
	int w = 0, h = 0, resized_w = 0, resized_h = 0;
	float aspect = 1;
	
	unsigned char *buf_one_byte = NULL;

	int buf_size = 0;

	int mouse_x = 0, mouse_y = 0, mouse_cursor_type_id = 0, _itis_user_move_mouse_ = 0;
	unsigned int screen_id = 0, old_screen_id = 0;

	unsigned int *get_buf();
	unsigned char *get_buf_one_byte();


	void lock_READ(int nom);
	void unlock_READ(int nom);

	void lock_FULL(int nom);
	void unlock_FULL(int nom);

	void set_new_size(int w_, int h_);

	void Clean();

	SCREEN_LIGHT();
	~SCREEN_LIGHT();
};



void init_decode_color2(void);

bool decode_screen(unsigned char *buf, int buf_size, SCREEN_LIGHT *raw_screen);



class SCREEN_LIGHT_encoded
{
public:
	int w = 0, h = 0;
	int old_w = 0, old_h = 0;
	int old_screen_id = -1;
	//int encoded_buffer_len = 0;
	
	//unsigned char *encoded_buf = NULL;
	//int encoded_buf_size = 0;

	unsigned char *old_buf_one_byte = NULL;
	int old_buf_one_byte_size = 0;

	PAL *pal = NULL;
	unsigned int pal_count_max = 0;

	unsigned char *body = NULL;
	int body_count_max = 0;

	int encoded_buffer_len = 0;
	int encoded_buffer_len_max = 0;
	unsigned char *encoded_buffer = NULL;

	void clean_();

	//void set_new_size(int w_, int h_);

	void pal_increase_size();
	bool encode_screen_ONE_BYTE(SCREEN_LIGHT_one_byte *screen_one_byte, int last_set_mouse_x, int last_set_mouse_y);

	SCREEN_LIGHT_encoded();
	~SCREEN_LIGHT_encoded();
};




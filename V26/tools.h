#pragma once

#include "stdint.h"
//#include <boost/cstdint.hpp>

#define _WINSOCKAPI_ 
#include <windows.h>
#undef _WINSOCKAPI_
#include <winsock2.h>
#include <stdlib.h>
#include <iphlpapi.h>
#include <stdio.h>
#undef _WINSOCKAPI_

#include <winhttp.h>
#include <aclapi.h>
#include <sddl.h>

#include <list>

#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <conio.h>

#include "SCREEN.h"

#define view_visiator_com "view.visiator.com"
#define SERVER_PORT 443

#define PROXY_TYPE_HTTPS 1
#define PROXY_TYPE_HTTP  2 // пока не реализовано
#define PROXY_TYPE_SOCKS 3 // пока не реализовано


#define LOCAL_FILES 1
#define PARTNER_FILES 2

#define _Toolbar_h 33
#define _V_Scroll_w 18
#define _element_icon_w 16
#define _Border 4
#define _line_h 18
#define _Caption_h 28

//unsigned long long gg;

class PROXY_element {
public:
	//int idx;
	unsigned int type, ip, port;
	void clean();
	//void save_in_registry(HKEY key);
	unsigned int get_ip_view_visiator_com();
	PROXY_element(unsigned int type_, unsigned int ip_, unsigned int port_);
};


class PROXY_LIST
{
public:
	bool is_changed;
	//PROXY_element element[30];
	std::list<PROXY_element> elements;
	//int element_max_count;
	void add(unsigned int type_, unsigned int ip_, unsigned int port_ );
	void clean();

	//void load_from_registry_element(HKEY key, int idx, unsigned int *ip, unsigned int *port);
	void load_from_registry(HKEY key, wchar_t *subkey_name);
	void save_in_registry(HKEY key, wchar_t *subkey_name);
	bool Load_ProxySettings();

	DWORD try_connect(unsigned int ip);
	unsigned int get_ip_view_visiator_com();



	PROXY_LIST();
};


void init_crit_section();
void enter_crit(int id);
void leave_crit(int id);

void sprintf__s_c_c_ui(char *s, int s_len, char *format, char *v1, char *v2, unsigned int v3);
void sprintf__s_i_c_c_ull_ull(char *s, int len, char *format, int v1, char *v2, char *v3, unsigned long long v4, unsigned long long v5);
void sprintf__s_D(char *s, int len, char *format, DWORD v1);
void sprintf__s_i(char *s, int len, char *format, int v1);
void sprintf__s_i_c(char *s, int len, char *format, int v1, char *v2);
void sprintf__s_ull(char *s, int len, char *format, unsigned long long v1);
void sprintf__s_ull_ull(char *s, int len, char *format, unsigned long long v1, unsigned long long v2);
void sprintf__s_ui_ui(char *s, int len, char *format, unsigned int v1, unsigned int v2);
void sprintf__s(char *s, int len, char *format);
void sprintf__s_c_i_i(char *s, int len, char *format, char *v1, int v2, int v3);
void sprintf__s_c_i(char *s, int len, char *format, char *v1, int v2);
void sprintf__s_cc_i(char *s, int len, char *format, const char *v1, int v2);
void sprintf__s_c(char *s, int len, char *format, char *v1);
void sprintf__s_c_c(char *s, int len, char *format, char *v1, char *v2);
void sprintf__s_c_D(char *s, int len, char *format, char *v1, DWORD v2);
void sprintf__s_cc(char *s, int len, char *format, const char *v1);
void sprintf__s_D_c(char *s, int len, char *format, DWORD v1, char *v2);
void sprintf__s_i_i_i(char *s, int len, char *format, int v1, int v2, int v3);
void sprintf__s_c_D_c(char *s, int len, char *format, char *v1, int v2, char *v3);
void sprintf__s_i_i_i_i(char *s, int len, char *format, int v1, int v2, int v3, int v4);
void sprintf__s_i_i_i_i_i(char *s, int len, char *format, int v1, int v2, int v3, int v4, int v5);
void sprintf__s_i_i_i_i_i_i_i(char *s, int len, char *format, int v1, int v2, int v3, int v4, int v5, int v6, int v7);
void sprintf__s_wc_i(wchar_t *s, int len, wchar_t *format, wchar_t *v1, int v2);
void sprintf__s_wc_i_i(wchar_t *s, int len, wchar_t *format, wchar_t *v1, int v2, int v3);
void sprintf__s_wc(wchar_t *s, int len, wchar_t *format, wchar_t *v1);
void sprintf__s_wc_wc(wchar_t *s, int len, wchar_t *format, wchar_t *v1, wchar_t *v2);

bool init_net();
void set_GLOBAL_STOP_true();
void fatal_error(const char *s);

void get_screen_size(int *scr_w, int *scr_h);
void get_display_size_pixel(int *display_w, int *display_h);

void load_onetime_screenposition_desktop(int *x, int *y);

void load_fonts();
uint32_t transparent_color(uint32_t old_color, uint32_t new_color, int percent);
int wchar_to_ascii(int p);
void load_all_standart_cursor();

PAL *new_PAL(int count);
void delete_PAL(PAL **q);

void clean_ENCODED_SCREEN_8bit_header(ENCODED_SCREEN_8bit_header *h);

void zero_wchar_t(wchar_t *s, int len);
void zero_unsigned_char(unsigned char *s, int len);
void zero_int(unsigned int *v, int sz);

int  my_strlen(unsigned char *v);
int  my_strlen(wchar_t *v);

void Load_ProxySettings_from_reg(unsigned int *proxy_ip, unsigned int *proxy_port, wchar_t *name);

unsigned long long generate_ID(unsigned char *id);
unsigned long long  decode_cursor_id(unsigned int p);

void Load_private_id_and_public_id_from_USER_registry(uint64_t *public_id, uint64_t *private_id);

int my_strcmp(unsigned char *p1, unsigned char *p2);
int my_strcmp(char *p1, char *p2);
int my_strcmp(wchar_t *p1, wchar_t *p2);

int my_send(uint32_t s, uint8_t *buf, int len, int flag, char *info, uint64_t *send_couner);
int my_recv(uint32_t socket_, uint8_t *buf, int len, uint64_t *recv_couner);

int setnonblocking(int sockfd);
void my_Slip(DWORD p);
unsigned int get_ip_view_visiator_com();


void my_strcpy(wchar_t *dest, wchar_t *source);
void my_strcat(wchar_t *dest, wchar_t *source);

int get_hexch_w(wchar_t v);
unsigned int decode_dig(char v);
bool REG_check_and_create_key(HKEY root, wchar_t *reg_key);
bool my_random(BYTE *buf, int buf_len);
bool Register_new_partner_on_proxy();
void Load_ProxySettings_from_reg(unsigned int *proxy_ip, unsigned int *proxy_port, wchar_t *name);
void check_bit(DWORD v);
DWORD try_connect_proxy(unsigned int proxy_ip, unsigned int proxy_port, unsigned int ip);

void my_strcpy(unsigned char *dest, unsigned char *source);
void my_strcat(char *dest, int max_len, char *source);
char *ip_to_char(char *buf, unsigned int ip);
int  http_result_code(unsigned char *b, int len);
unsigned char *http_get_body(unsigned char *b, int len);
unsigned int get_IP_for_server_location(unsigned long long partner_id, unsigned long long public_id, unsigned long long private_id);
bool check_admin();
int  check_IsWow64(DWORD pid);
unsigned int get_sol();
void format_sz(char *s, unsigned long long sz);
void format_sz(wchar_t *s, unsigned long long sz);

int my_big_send(unsigned int socket_, unsigned char *buf, int len, unsigned long long *send_counter);
int my_big_recv(unsigned int socket_, unsigned char *buf, int len, unsigned long long *recv_counter);
void hexstr_to_char16_w(wchar_t *hstr, unsigned char *buf);
bool check_run_as_service();



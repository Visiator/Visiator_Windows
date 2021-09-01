

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

#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <conio.h>



#pragma once

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
void load_onetime_screenposition_desktop(int *x, int *y);
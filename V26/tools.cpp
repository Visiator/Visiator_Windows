


#include "tools.h"
#include "APPLICATION_ATTRIBUTES.h"
#include "FONT.h"
#include "resource.h"
#include "NET_SERVER_SESSION.h"
#include "CRYPTO.h"
#include "KERNEL.h"

#define ACE_NAME_SIZE 1500


extern FONT *font[10];


void fatal_error(const char *s) {
	FILE *f;
	fopen_s(&f, "c:\\1\\fatal_error.txt", "ab");
	if (f != NULL) {
		fprintf(f, s);
		fprintf(f, "\r\n");
		fclose(f);
	}

}


static CRITICAL_SECTION CS;
int init_CS = 0;

extern bool GLOBAL_STOP;
extern APPLICATION_ATTRIBUTES app_attributes;



void init_crit_section() {
	InitializeCriticalSection(&CS);
	init_CS = 1;
}

bool eff = false;
int enter_crit_id = 0;

void save_crit_error(char *s) {

	FILE *f;
	fopen_s(&f, "c:\\1\\crit_error.txt", "ab");
	if (f != NULL) {
		fprintf(f, s);
		fprintf(f, "\r\n");
		fclose(f);
	}

}


void save_crit_error(char *s, int v1) {
	char buf[500];
	sprintf__s_c_i(buf, 450, (char *)"%s %d\r\n", s, v1);
	FILE *f;
	fopen_s(&f, "c:\\1\\crit_error.txt", "ab");
	if (f != NULL) {
		fprintf(f, buf);
		fprintf(f, "\r\n");
		fclose(f);
	}

}

void save_crit_error(char *s, int v1, int v2) {
	char buf[500];
	sprintf__s_c_i_i(buf, 450, (char *)"%s %d %d\r\n", s, v1, v2);
	FILE *f;
	fopen_s(&f, "c:\\1\\crit_error.txt", "ab");
	if (f != NULL) {
		fprintf(f, buf);
		fprintf(f, "\r\n");
		fclose(f);
	}

}


void enter_crit(int id)
{

	EnterCriticalSection(&CS);

	if (eff == true) { save_crit_error((char *)"eff == true WTF??"); }

	if (enter_crit_id != 0) {
		save_crit_error((char *)"enter_crit_id != 0 WTF?? ", enter_crit_id);
	}

	enter_crit_id = id;
	eff = true;

};

void leave_crit(int id)
{


	if (id != enter_crit_id && enter_crit_id != 0) {
		save_crit_error((char *)"leave_crit error ", id, enter_crit_id);
	};

	eff = false;
	enter_crit_id = 0;
	LeaveCriticalSection(&CS);

};

void sprintf__s_c_c_ui(char *s, int s_len, char *format, char *v1, char *v2, unsigned int v3) {

	try
	{
		sprintf_s(s, s_len, format, v1, v2, v3);
	}
	catch (...)
	{
		s[0] = '?';
		s[1] = 0;
	};

}


void sprintf__s_i_c_c_ull_ull(char *s, int len, char *format, int v1, char *v2, char *v3, unsigned long long v4, unsigned long long v5) {
	try
	{
		sprintf_s(s, len, format, v1, v2, v3, v4, v5);
	}
	catch (...)
	{
		s[0] = '?';
		s[1] = 0;
	};
}

void sprintf__s_D(char *s, int len, char *format, DWORD v1) {
	try
	{
		sprintf_s(s, len, format, v1);
	}
	catch (...)
	{
		s[0] = '?';
		s[1] = 0;
	};
}

void sprintf__s_i(char *s, int len, char *format, int v1) {
	try
	{
		sprintf_s(s, len, format, v1);
	}
	catch (...)
	{
		s[0] = '?';
		s[1] = 0;
	};
}

void sprintf__s_i_c(char *s, int len, char *format, int v1, char *v2) {
	try
	{
		sprintf_s(s, len, format, v1, v2);
	}
	catch (...)
	{
		s[0] = '?';
		s[1] = 0;
	};
}

void sprintf__s_ull(char *s, int len, char *format, unsigned long long v1) {
	try
	{
		sprintf_s(s, len, format, v1);
	}
	catch (...)
	{
		s[0] = '?';
		s[1] = 0;
	};
}


void sprintf__s_ull_ull(char *s, int len, char *format, unsigned long long v1, unsigned long long v2) {
	try
	{
		sprintf_s(s, len, format, v1, v2);
	}
	catch (...)
	{
		s[0] = '?';
		s[1] = 0;
	};
}

void sprintf__s_ui_ui(char *s, int len, char *format, unsigned int v1, unsigned int v2) {
	try
	{
		sprintf_s(s, len, format, v1, v2);
	}
	catch (...)
	{
		s[0] = '?';
		s[1] = 0;
	};
}

void sprintf__s(char *s, int len, char *format) {
	try
	{
		sprintf_s(s, len, format);
	}
	catch (...)
	{
		s[0] = '?';
		s[1] = 0;
	};
}

void sprintf__s_c_i_i(char *s, int len, char *format, char *v1, int v2, int v3) {
	try
	{
		sprintf_s(s, len, format, v1, v2, v3);
	}
	catch (...)
	{
		s[0] = '?';
		s[1] = 0;
	};
}
void sprintf__s_c_i(char *s, int len, char *format, char *v1, int v2) {
	try
	{
		sprintf_s(s, len, format, v1, v2);
	}
	catch (...)
	{
		s[0] = '?';
		s[1] = 0;
	};
}
void sprintf__s_cc_i(char *s, int len, char *format, const char *v1, int v2) {
	try
	{
		sprintf_s(s, len, format, v1, v2);
	}
	catch (...)
	{
		s[0] = '?';
		s[1] = 0;
	};
}
void sprintf__s_c(char *s, int len, char *format, char *v1) {
	try
	{
		sprintf_s(s, len, format, v1);
	}
	catch (...)
	{
		s[0] = '?';
		s[1] = 0;
	};
}
void sprintf__s_c_c(char *s, int len, char *format, char *v1, char *v2) {
	try
	{
		sprintf_s(s, len, format, v1, v2);
	}
	catch (...)
	{
		s[0] = '?';
		s[1] = 0;
	};
}
void sprintf__s_c_D(char *s, int len, char *format, char *v1, DWORD v2) {
	try
	{
		sprintf_s(s, len, format, v1, v2);
	}
	catch (...)
	{
		s[0] = '?';
		s[1] = 0;
	};
}

void sprintf__s_cc(char *s, int len, char *format, const char *v1) {
	try
	{
		sprintf_s(s, len, format, v1);
	}
	catch (...)
	{
		s[0] = '?';
		s[1] = 0;
	};
}
void sprintf__s_D_c(char *s, int len, char *format, DWORD v1, char *v2) {
	try
	{
		sprintf_s(s, len, format, v1, v2);
	}
	catch (...)
	{
		s[0] = '?';
		s[1] = 0;
	};
}
void sprintf__s_i_i_i(char *s, int len, char *format, int v1, int v2, int v3) {
	try
	{
		sprintf_s(s, len, format, v1, v2, v3);
	}
	catch (...)
	{
		s[0] = '?';
		s[1] = 0;
	};
}
void sprintf__s_c_D_c(char *s, int len, char *format, char *v1, int v2, char *v3) {
	try
	{
		sprintf_s(s, len, format, v1, v2, v3);
	}
	catch (...)
	{
		s[0] = '?';
		s[1] = 0;
	};
}
void sprintf__s_i_i_i_i(char *s, int len, char *format, int v1, int v2, int v3, int v4) {
	try
	{
		sprintf_s(s, len, format, v1, v2, v3, v4);
	}
	catch (...)
	{
		s[0] = '?';
		s[1] = 0;
	};
}
void sprintf__s_i_i_i_i_i(char *s, int len, char *format, int v1, int v2, int v3, int v4, int v5) {
	try
	{
		sprintf_s(s, len, format, v1, v2, v3, v4, v5);
	}
	catch (...)
	{
		s[0] = '?';
		s[1] = 0;
	};
}
void sprintf__s_i_i_i_i_i_i_i(char *s, int len, char *format, int v1, int v2, int v3, int v4, int v5, int v6, int v7) {
	try
	{
		sprintf_s(s, len, format, v1, v2, v3, v4, v5, v6, v7);
	}
	catch (...)
	{
		s[0] = '?';
		s[1] = 0;
	};
}

void sprintf__s_wc_i(wchar_t *s, int len, wchar_t *format, wchar_t *v1, int v2) {
	try
	{
		swprintf_s(s, len, format, v1, v2);
	}
	catch (...)
	{
		s[0] = '?';
		s[1] = 0;
	};
}

void sprintf__s_wc_i_i(wchar_t *s, int len, wchar_t *format, wchar_t *v1, int v2, int v3) {
	try
	{
		swprintf_s(s, len, format, v1, v2, v3);
	}
	catch (...)
	{
		s[0] = '?';
		s[1] = 0;
	};
}


void sprintf__s_wc(wchar_t *s, int len, wchar_t *format, wchar_t *v1) {
	try
	{
		swprintf_s(s, len, format, v1);
	}
	catch (...)
	{
		s[0] = '?';
		s[1] = 0;
	};
}
void sprintf__s_wc_wc(wchar_t *s, int len, wchar_t *format, wchar_t *v1, wchar_t *v2) {
	try
	{
		swprintf_s(s, len, format, v1, v2);
	}
	catch (...)
	{
		s[0] = '?';
		s[1] = 0;
	};
}

bool init_net() {
	WSADATA wsaData;


	if (WSAStartup(0x101, &wsaData)) {
		set_GLOBAL_STOP_true(); // GLOBAL_STOP = true;
		return false;
	};


	return true;

}

void set_GLOBAL_STOP_true() {
	GLOBAL_STOP = true;
}

void get_screen_size(int *scr_w, int *scr_h) {

	*scr_w = 0;
	*scr_h = 0;

	HDC hdc2;
	HDC g_hdcScreen;
	//HGDIOBJ hold_bitmap;

	hdc2 = ::CreateCompatibleDC(NULL); //::DeleteDC(hdc2);
	g_hdcScreen = GetDC(NULL); //ReleaseDC(NULL, g_hdcScreen);

	*scr_w = GetDeviceCaps(g_hdcScreen, HORZRES);
	*scr_h = GetDeviceCaps(g_hdcScreen, VERTRES);

	ReleaseDC(NULL, g_hdcScreen);
	::DeleteDC(hdc2);
}


void load_onetime_screenposition_desktop(int *x, int *y) {

	//*x = -5555;
	//*y = -5555;

	DWORD r;
	HKEY hkey;

	hkey = 0;
	r = RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\VisiatorDesktop", 0, KEY_READ | KEY_WRITE | 0x0100, &hkey);
	if (hkey == 0) {
		r = RegCreateKeyEx(HKEY_CURRENT_USER, L"Software\\VisiatorDesktop", 0, L"", REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS | 0x0100, NULL, &hkey, NULL);
	};
	if (hkey == 0) {
		//send_udp("Load_or_Create_MY_UNIC_ID() error1");
		return;
	}

	DWORD xx, yy;

	DWORD size;
	DWORD type;


	size = 4;
	type = REG_BINARY;
	if (RegQueryValueEx(hkey, L"onetime_desktop_x", 0, &type, (BYTE *)&xx, &size) != 0) {
		xx = -5555;
	};
	size = 8;
	type = REG_BINARY;
	if (RegQueryValueEx(hkey, L"onetime_desktop_y", 0, &type, (BYTE *)&yy, &size) != 0) {
		yy = -5555;
	};
	RegCloseKey(hkey);

	if (xx != -5555 && yy != -5555) {
		*x = xx;
		*y = yy;
	}


}


void load_fonts() {

	HMODULE hMod = GetModuleHandle(nullptr);

	//////////////////////////////////////////////////////////////////////

	font[0] = new FONT();

	HRSRC hRes_font0 = FindResource(hMod, MAKEINTRESOURCE(20000), L"BINARY");
	HGLOBAL hGlob_font0 = LoadResource(hMod, hRes_font0);
	BYTE *lpbArray_font0 = (BYTE*)LockResource(hGlob_font0);
	DWORD sz_font0;
	sz_font0 = SizeofResource(hMod, hRes_font0);

	font[0]->load_from_buffer(lpbArray_font0, sz_font0);

	//////////////////////////////////////////////////////////////////////

	font[1] = new FONT();

	HRSRC hRes_font1 = FindResource(hMod, MAKEINTRESOURCE(20001), L"BINARY");
	HGLOBAL hGlob_font1 = LoadResource(hMod, hRes_font1);
	BYTE *lpbArray_font1 = (BYTE*)LockResource(hGlob_font1);
	DWORD sz_font1;
	sz_font1 = SizeofResource(hMod, hRes_font1);

	font[1]->load_from_buffer(lpbArray_font1, sz_font1);

}

uint32_t transparent_color(uint32_t old_color, uint32_t new_color, int percent) {
	int need_r, need_g, need_b;

	int old_r, old_g, old_b, new_r, new_g, new_b;

	need_r = new_color & 0x0000ff;
	need_g = (new_color & 0x00ff00) >> 8;
	need_b = (new_color & 0xff0000) >> 16;

	old_r = old_color & 0xff;
	old_g = ((old_color & 0xff00) >> 8);
	old_b = ((old_color & 0xff0000) >> 16);



	new_r = (int)(old_r + (float)(need_r - old_r) / (float)255 * (float)percent);
	new_g = (int)(old_g + (float)(need_g - old_g) / (float)255 * (float)percent);
	new_b = (int)(old_b + (float)(need_b - old_b) / (float)255 * (float)percent);

	return new_r | (new_g << 8) | (new_b << 16);

}

int wchar_to_ascii(int p) {

	if (p == 0x401) return '�';
	if (p == 0x451) return '�';
	if (p >= 0x410 && p <= 0x44f) {
		return (0xC0 + (p - 0x410));
	};
	if (p >= 0x20 && p <= 0x7f)
	{
		return p;
	};
	if (p == 8226) return 150;
	return '?';
}

HCURSOR
hCurs_IDC_HAND = 0
, hCurs_IDC_ARROW = 0
, hCurs_IDC_IBEAM = 0
, hCurs_IDC_WAIT = 0
, hCurs_IDC_CROSS = 0
, hCurs_IDC_UPARROW = 0
, hCurs_IDC_SIZE = 0
, hCurs_IDC_ICON = 0
, hCurs_IDC_SIZENWSE = 0
, hCurs_IDC_SIZENESW = 0
, hCurs_IDC_SIZEWE = 0
, hCurs_IDC_SIZENS = 0
, hCurs_IDC_SIZEALL = 0
, hCurs_IDC_NO = 0
, hCurs_IDC_APPSTARTING = 0;

void load_all_standart_cursor() {
	if (hCurs_IDC_HAND == 0) hCurs_IDC_HAND = LoadCursor(NULL, IDC_HAND);
	if (hCurs_IDC_ARROW == 0) hCurs_IDC_ARROW = LoadCursor(NULL, IDC_ARROW);
	if (hCurs_IDC_IBEAM == 0) hCurs_IDC_IBEAM = LoadCursor(NULL, IDC_IBEAM);
	if (hCurs_IDC_WAIT == 0) hCurs_IDC_WAIT = LoadCursor(NULL, IDC_WAIT);
	if (hCurs_IDC_CROSS == 0) hCurs_IDC_CROSS = LoadCursor(NULL, IDC_CROSS);
	if (hCurs_IDC_UPARROW == 0) hCurs_IDC_UPARROW = LoadCursor(NULL, IDC_UPARROW);
	if (hCurs_IDC_SIZE == 0) hCurs_IDC_SIZE = LoadCursor(NULL, IDC_SIZE);
	if (hCurs_IDC_ICON == 0) hCurs_IDC_ICON = LoadCursor(NULL, IDC_ICON);
	if (hCurs_IDC_SIZENWSE == 0) hCurs_IDC_SIZENWSE = LoadCursor(NULL, IDC_SIZENWSE);
	if (hCurs_IDC_SIZENESW == 0) hCurs_IDC_SIZENESW = LoadCursor(NULL, IDC_SIZENESW);
	if (hCurs_IDC_SIZEWE == 0) hCurs_IDC_SIZEWE = LoadCursor(NULL, IDC_SIZEWE);
	if (hCurs_IDC_SIZENS == 0) hCurs_IDC_SIZENS = LoadCursor(NULL, IDC_SIZENS);
	if (hCurs_IDC_SIZEALL == 0) hCurs_IDC_SIZEALL = LoadCursor(NULL, IDC_SIZEALL);
	if (hCurs_IDC_NO == 0) hCurs_IDC_NO = LoadCursor(NULL, IDC_NO);
	if (hCurs_IDC_APPSTARTING == 0) hCurs_IDC_APPSTARTING = LoadCursor(NULL, IDC_APPSTARTING);



}

PAL *new_PAL(int count) {
	int sz;
	sz = sizeof(PAL);
	sz *= count;
	return (PAL *)new unsigned char[sz];
}

void delete_PAL(PAL **q) {
	unsigned char *p;
	p = (unsigned char *)*q;
	delete[] p;
	*q = nullptr;
}

void zero_wchar_t(wchar_t *s, int len) {
	for (int i = 0; i < len; i++) s[i] = 0;
}


void zero_unsigned_char(unsigned char *s, int len) {
	for (int i = 0; i < len; i++) s[i] = 0;
}

void zero_int(unsigned int *v, int sz) {
	for (int i = 0; i < sz; i++) v[i] = 0;
}

void zero_void(void *s, int len) {
	char *q;
	q = (char *)s;
	for (int i = 0; i < len; i++) q[i] = 0;
}


void clean_ENCODED_SCREEN_8bit_header(ENCODED_SCREEN_8bit_header *h) {
	if (h == NULL) return;
	zero_void(h, sizeof(ENCODED_SCREEN_8bit_header));
}

unsigned long long generate_ID(unsigned char *id) {
	if (id == NULL) return 0;

	uint16_t p1, p2, p3;
	unsigned long long ll;
	uint16_t *s;
	s = (uint16_t *)&ll;
	ll = 0;

	if (my_strlen(id) == 11) {

		if (id[0] < '0' || id[0] > '9') return 0;
		if (id[1] < '0' || id[1] > '9') return 0;
		if (id[2] < '0' || id[2] > '9') return 0;

		if (id[3] >= '0' && id[3] <= '9') return 0;

		if (id[4] < '0' || id[4] > '9') return 0;
		if (id[5] < '0' || id[5] > '9') return 0;
		if (id[6] < '0' || id[6] > '9') return 0;

		if (id[7] >= '0' && id[7] <= '9') return 0;

		if (id[8] < '0' || id[8] > '9') return 0;
		if (id[9] < '0' || id[9] > '9') return 0;
		if (id[10] < '0' || id[10] > '9') return 0;

		p1 = (id[0] - '0') * 100 + (id[1] - '0') * 10 + (id[2] - '0');
		p2 = (id[4] - '0') * 100 + (id[5] - '0') * 10 + (id[6] - '0');
		p3 = (id[8] - '0') * 100 + (id[9] - '0') * 10 + (id[10] - '0');

		*s = p1; 	s++;
		*s = p2;	s++;
		*s = p3;	s++;

		return ll;
	}

	if (my_strlen(id) != 9) return 0;




	p1 = (id[0] - '0') * 100 + (id[1] - '0') * 10 + (id[2] - '0');
	p2 = (id[3] - '0') * 100 + (id[4] - '0') * 10 + (id[5] - '0');
	p3 = (id[6] - '0') * 100 + (id[7] - '0') * 10 + (id[8] - '0');

	*s = p1; 	s++;
	*s = p2;	s++;
	*s = p3;	s++;

	return ll;
}

int  my_strlen(unsigned char *v) {
	if (v == NULL || v[0] == 0) return 0;
	int i;
	i = 0;
	while (v[i] != 0) i++;
	return i;
};

int  my_strlen(wchar_t *v) {
	if (v == NULL || v[0] == 0) return 0;
	int i;
	i = 0;
	while (v[i] != 0) i++;
	return i;
};

uint64_t decode_cursor_id(uint32_t p) {

	switch (p)
	{
	case 100: return (unsigned long long)hCurs_IDC_ARROW;
	case 101: return (unsigned long long)hCurs_IDC_HAND;
	case 102: return (unsigned long long)hCurs_IDC_IBEAM;
	case 103: return (unsigned long long)hCurs_IDC_WAIT;
	case 104: return (unsigned long long)hCurs_IDC_CROSS;
	case 105: return (unsigned long long)hCurs_IDC_UPARROW;
	case 106: return (unsigned long long)hCurs_IDC_SIZE;
	case 107: return (unsigned long long)hCurs_IDC_ICON;
	case 108: return (unsigned long long)hCurs_IDC_SIZENWSE;
	case 109: return (unsigned long long)hCurs_IDC_SIZENESW;
	case 110: return (unsigned long long)hCurs_IDC_SIZEWE;
	case 111: return (unsigned long long)hCurs_IDC_SIZENS;
	case 112: return (unsigned long long)hCurs_IDC_SIZEALL;
	case 113: return (unsigned long long)hCurs_IDC_NO;
	case 114: return (unsigned long long)hCurs_IDC_APPSTARTING;

	default:
		break;
	}

	return (unsigned long long)hCurs_IDC_ARROW;
}

void get_display_size_pixel(int *display_w, int *display_h) {

	HDC hdc2;
	HDC g_hdcScreen;

	hdc2 = CreateCompatibleDC(NULL); //::DeleteDC(hdc2);
	g_hdcScreen = GetDC(NULL); //ReleaseDC(NULL, g_hdcScreen);

	*display_w = GetDeviceCaps(g_hdcScreen, DESKTOPHORZRES);
	*display_h = GetDeviceCaps(g_hdcScreen, DESKTOPVERTRES);

	ReleaseDC(NULL, g_hdcScreen);
	::DeleteDC(hdc2);
}

void Load_private_id_and_public_id_from_USER_registry(uint64_t *public_id, uint64_t *private_id) {

	*public_id = 0;
	*private_id = 0;

	LONG r;
	HKEY hkey;

	hkey = 0;
	r = RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\VisiatorDesktop", 0, KEY_READ | KEY_WRITE | 0x0100, &hkey);
	if (hkey == 0) {
		r = RegCreateKeyEx(HKEY_CURRENT_USER, L"Software\\VisiatorDesktop", 0, L"", REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS | 0x0100, NULL, &hkey, NULL);
	};
	if (hkey == 0) {
		//send_udp("Load_or_Create_MY_UNIC_ID() error1");
		return;
	}

	unsigned long long pu, pv;

	DWORD size;
	DWORD type;


	size = 8;
	type = REG_BINARY;
	if (RegQueryValueEx(hkey, L"private_id", 0, &type, (BYTE *)&pv, &size) != 0) {
		pv = 0;
	};
	size = 8;
	type = REG_BINARY;
	if (RegQueryValueEx(hkey, L"public_id", 0, &type, (BYTE *)&pu, &size) != 0) {
		pu = 0;
	};
	RegCloseKey(hkey);

	if (pu != 0 && pv != 0) {
		*public_id = pu;
		*private_id = pv;


		// 2021 09 if (app_attributes.is_service) { app_attributes.set_service_public_id(*public_id); }
		// 2021 09 else { app_attributes.set_desktop_public_id(*public_id); };
	}

}




bool REG_check_and_create_key(HKEY root, wchar_t *reg_key) {
	HKEY hKey;

	LSTATUS nn;
	////////////////////////////////////////////////////////////////////////////////////////
	// ���� ����� ��� - ���� �������
	hKey = 0; // READ_CONTROL OWNER_SECURITY_INFORMATION
	nn = RegOpenKeyEx(root, reg_key, 0, READ_CONTROL | 0x0100, &hKey);
	if (nn == 0) {}; // ���� ����, � ������� ������
	if (nn == 2) {}; // ����� ���, ���� ���������
	if (nn == 5) { // ���� ����, �� ��� ���� ������� 
		return false;
	};
	if (hKey == 0) {

		nn = RegCreateKeyEx(root, reg_key, 0, L"", REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS | 0x0100, NULL, &hKey, NULL);
		if (hKey == 0 || nn != 0) {
			return false;
		}
	}

	RegCloseKey(hKey);

	////////////////////////////////////////////////////////////////////////////////////////
	return true;
}

int  LOAD_PARAM_FOR_SERVICE_check_owner(HKEY root, wchar_t *reg_key) {
	// 1-fatal error , 2-owner is OK , 3-owner is not local_system
	bool need_change_owner;
	HKEY hKey;
	LSTATUS nn;
	SECURITY_DESCRIPTOR *qq;//sd
	qq = (SECURITY_DESCRIPTOR *)new char[100];
	DWORD x = 100;
	LPTSTR name;
	name = NULL;
	BOOL rr;


	PSID pLocalSystemSid = NULL;
	SID_IDENTIFIER_AUTHORITY sia = SECURITY_NT_AUTHORITY;

	if (!AllocateAndInitializeSid(&sia, 1, SECURITY_LOCAL_SYSTEM_RID, 0, 0, 0, 0, 0, 0, 0, &pLocalSystemSid)) { delete[] qq; return 1; }
	// FreeSid(pLocalSystemSid)

	////////////////////////////////////////////////////////////////////////////////////////
// �������� ���������, ���� �� Local_System - ������������ ���������
	//READ_CONTROL
		// KEY_READ | KEY_EXECUTE | KEY_WRITE | WRITE_DAC | WRITE_OWNER | OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION     READ_CONTROL
	nn = RegOpenKeyEx(root, reg_key, 0, READ_CONTROL | 0x0100, &hKey);

	if (nn != ERROR_SUCCESS) { delete[] qq; FreeSid(pLocalSystemSid);  return 1; };
	nn = RegGetKeySecurity(hKey, OWNER_SECURITY_INFORMATION, qq, &x);
	if (nn != ERROR_SUCCESS) { delete[] qq; FreeSid(pLocalSystemSid); return 1; };

	PSID sOwner;
	BOOL deff;
	nn = GetSecurityDescriptorOwner(qq, &sOwner, &deff);
	if (nn == 0) { delete[] qq; FreeSid(pLocalSystemSid); return 1; };

	rr = ConvertSidToStringSid(sOwner, &name); // LocalFree(name);
	if (rr == 0) { delete[] qq; FreeSid(pLocalSystemSid); return 1; };


	//send_udp(L"~~~~~~~>", name);
	if (my_strcmp(name, L"S-1-5-18") != 0) need_change_owner = true;
	else need_change_owner = false;

	LocalFree(name);
	RegCloseKey(hKey);
	delete[] qq;
	FreeSid(pLocalSystemSid);

	if (need_change_owner == true) {
		return 3;
		//if (set_owner_for_key( pLocalSystemSid) == false) { return false; };
	};
	return 2; // owner is OK
}

int  SetPrivilege(HANDLE token, wchar_t *privilege, int enable) {
	TOKEN_PRIVILEGES tp;
	LUID luid;


	//if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &token)) return 0;
	if (!LookupPrivilegeValue(NULL, privilege, &luid)) return 0;

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	if (enable) tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	else tp.Privileges[0].Attributes = 0;

	// Enable the privilege or disable all privileges.
	return AdjustTokenPrivileges(token, FALSE, &tp, NULL, NULL, NULL);
}


bool LOAD_PARAM_FOR_SERVICE_set_owner(HKEY root, wchar_t *reg_key) {

	HKEY hKey;
	BOOL rr;
	int vv;
	LSTATUS nn;
	SECURITY_DESCRIPTOR sd;
	SID_IDENTIFIER_AUTHORITY sia = SECURITY_NT_AUTHORITY;
	PSID pLocalSystemSid = NULL;

	if (!AllocateAndInitializeSid(&sia, 1, SECURITY_LOCAL_SYSTEM_RID, 0, 0, 0, 0, 0, 0, 0, &pLocalSystemSid)) { return false; }
	// FreeSid(pLocalSystemSid)

	/////////////////////////////////////////////
			// ����� ����� ������ ��������� �� local_system, ��� ����������� ���������� SeTakeOwnershipPrivilege

	HANDLE token;
	vv = OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &token);
	if (vv == 0) { FreeSid(pLocalSystemSid); return false; }
	vv = SetPrivilege(token, L"SeTakeOwnershipPrivilege", 1);//�������� ����������
	if (vv == 0) { FreeSid(pLocalSystemSid); return false; }
	// Enable the SE_TAKE_OWNERSHIP_NAME privilege.
	vv = SetPrivilege(token, SE_TAKE_OWNERSHIP_NAME, TRUE);
	if (vv == 0) { FreeSid(pLocalSystemSid); return false; }
	CloseHandle(token);

	/////////////////////////////////////////////

	//READ_CONTROL
		// KEY_READ | KEY_EXECUTE | KEY_WRITE | WRITE_DAC | WRITE_OWNER | OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION     READ_CONTROL
	nn = RegOpenKeyEx(root, reg_key, 0, WRITE_OWNER | 0x0100, &hKey);

	if (nn != ERROR_SUCCESS) { FreeSid(pLocalSystemSid);  return false; };

	if (!InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION)) { FreeSid(pLocalSystemSid); return false; }

	rr = SetSecurityDescriptorOwner(&sd, pLocalSystemSid, TRUE);
	if (rr == 0) { FreeSid(pLocalSystemSid); return false; };

	nn = RegSetKeySecurity(hKey, (SECURITY_INFORMATION)OWNER_SECURITY_INFORMATION, &sd);
	//send_udp(L"RegSetKeySecurity ", nn);
	if (nn != ERROR_SUCCESS) {
		FreeSid(pLocalSystemSid);
		//return false;  TODO 2019+ �� ��� �������������� �������� ������ ��������� ������ �������
	}
	else {
		FreeSid(pLocalSystemSid);
	};
	return true;
}


bool LOAD_PARAM_FOR_SERVICE_set_ACL(HKEY root, wchar_t *reg_key) {

	SID_IDENTIFIER_AUTHORITY sia = SECURITY_NT_AUTHORITY;
	PSID pLocalSystemSid = NULL, pAdministratorsSid = NULL;
	PACL pDacl = NULL;
	DWORD dwAclSize;
	LSTATUS nn;
	SECURITY_DESCRIPTOR sd;

	if (!AllocateAndInitializeSid(&sia, 1, SECURITY_LOCAL_SYSTEM_RID, 0, 0, 0, 0, 0, 0, 0, &pLocalSystemSid)) { return false; }
	// FreeSid(pLocalSystemSid)

	if (!AllocateAndInitializeSid(&sia, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &pAdministratorsSid)) { return false; };


	HKEY hKey;
	////////////////////////////////////////////////////////////////////////////////////////

	nn = RegOpenKeyEx(root, reg_key, 0, KEY_READ | KEY_EXECUTE | KEY_WRITE | WRITE_DAC | WRITE_OWNER | OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION | 0x0100, &hKey);

	if (nn != ERROR_SUCCESS) { FreeSid(pLocalSystemSid);  return false; };


	//dwAclSize = sizeof(ACL) + 1 * (sizeof(ACCESS_ALLOWED_ACE) - sizeof(DWORD)) + GetLengthSid(pLocalSystemSid);

	dwAclSize = sizeof(ACL) + 2 * (sizeof(ACCESS_ALLOWED_ACE) - sizeof(DWORD)) + GetLengthSid(pLocalSystemSid) + GetLengthSid(pAdministratorsSid);

	pDacl = (PACL)HeapAlloc(GetProcessHeap(), 0, dwAclSize);

	if (pDacl == NULL) { RegCloseKey(hKey); FreeSid(pLocalSystemSid);  return false; };

	if (!InitializeAcl(pDacl, dwAclSize, ACL_REVISION)) { RegCloseKey(hKey); FreeSid(pLocalSystemSid); return false; }

	if (!AddAccessAllowedAce(pDacl, ACL_REVISION, KEY_ALL_ACCESS, pLocalSystemSid)) { RegCloseKey(hKey); FreeSid(pLocalSystemSid); return false; }
	if (!AddAccessAllowedAce(pDacl, ACL_REVISION, KEY_ALL_ACCESS, pAdministratorsSid)) { RegCloseKey(hKey); FreeSid(pLocalSystemSid); return false; }


	if (!InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION)) { RegCloseKey(hKey); FreeSid(pLocalSystemSid); return false; }

	if (!SetSecurityDescriptorDacl(&sd, TRUE, pDacl, FALSE)) { RegCloseKey(hKey); FreeSid(pLocalSystemSid); return false; }

	//if (need_change_owner == true) {
	//	rr = SetSecurityDescriptorOwner(&sd, pLocalSystemSid, TRUE);
	//}

	//sd.Owner = pLocalSystemSid;
	nn = RegSetKeySecurity(hKey, (SECURITY_INFORMATION)DACL_SECURITY_INFORMATION, &sd);
	if (nn != 0) {
		//send_udp("E1");
	}
	//nn = RegSetKeySecurity(hKey, (SECURITY_INFORMATION)OWNER_SECURITY_INFORMATION, &sd);



	RegCloseKey(hKey);
	FreeSid(pLocalSystemSid);
	FreeSid(pAdministratorsSid);

	return true;
}

int  LOAD_PARAM_FOR_SERVICE_check_ACL(HKEY root, wchar_t *reg_key) {

	HKEY hKey;
	////////////////////////////////////////////////////////////////////////////////////////
	// 
	hKey = 0;                       // KEY_READ
	RegOpenKeyEx(root, reg_key, 0, READ_CONTROL | 0x0100, &hKey);
	if (hKey == 0) { return 1; };

	//////////////////////////////////
		// ������� ����� ������� �� ������


	//PSECURITY_DESCRIPTOR lpSec;
	PACL pDACL;
	PSID lpOwnerSID;
	SID_NAME_USE eUse;
	TCHAR szName[ACE_NAME_SIZE + 1];
	unsigned long nNameSize = ACE_NAME_SIZE;
	TCHAR szDomain[ACE_NAME_SIZE + 1];
	unsigned long nDomainSize = ACE_NAME_SIZE;
	int i;  ACE_HEADER *lpACEHeader;
	ACCESS_ALLOWED_ACE *lpAllowedACE;
	//LPOBJECTACCESS_MASK lpAccess;

	if (GetSecurityInfo(hKey, SE_REGISTRY_KEY, DACL_SECURITY_INFORMATION, (void**)&lpOwnerSID, NULL, &pDACL,
		NULL, NULL) == ERROR_SUCCESS)
	{

		if (pDACL->AceCount != 1) {
			return 3;
		};

		if ((pDACL != NULL) && (pDACL->AceCount > 0))
		{
			lpACEHeader = (ACE_HEADER*)((char*)pDACL + sizeof(ACL));
			i = 0;
			while (GetAce(pDACL, i, (void**)&lpACEHeader))
			{
				nNameSize = ACE_NAME_SIZE;
				nDomainSize = ACE_NAME_SIZE;
				switch (lpACEHeader->AceType)
				{
				case ACCESS_ALLOWED_OBJECT_ACE_TYPE:
					break;
				case ACCESS_DENIED_ACE_TYPE:
					break;
				case ACCESS_DENIED_OBJECT_ACE_TYPE:
					break;
				case ACCESS_ALLOWED_ACE_TYPE:

					lpAllowedACE = (ACCESS_ALLOWED_ACE*)lpACEHeader;

					if (LookupAccountSid(NULL, (void*)&lpAllowedACE->SidStart,
						szName, &nNameSize, szDomain, &nDomainSize, &eUse))
					{

						//send_udp(szName, L" ", szDomain);
						check_bit(lpAllowedACE->Mask);

						/*
						lpAccess = (LPOBJECTACCESS_MASK)&lpAllowedACE->Mask;
						if (lpAccess->StandardRights == 31)
						{
							// Full Control
						}
						if (lpAccess->StandardRights == 18)
						{
							// Read
						}
						if (lpAccess->StandardRights == 19)
						{
							// Write (change)
						}
						*/
					}
					break;
				}
				i++;
			}
			//LocalFree(lpSec);
		}
	}
	return 3;
}


bool REG_CHECK_EXISTS_KEY_and_check_permissions(HKEY root, wchar_t *reg_key) {

	if (REG_check_and_create_key(root, reg_key) == false) { return false; };

	int result;

	result = LOAD_PARAM_FOR_SERVICE_check_owner(root, reg_key);

	//result = 2;

	if (result == 1) { // ��������� ������
		return false;
	}
	else {
		if (result == 3) { // �������� �� local_system, ���� ��������
			if (LOAD_PARAM_FOR_SERVICE_set_owner(root, reg_key) == false) {
				return false;
			};
		}
		else {
			if (result == 2) { // �������� local_system

			}
		}
	};

	result = LOAD_PARAM_FOR_SERVICE_check_ACL(root, reg_key);

	//result = 2;

	if (result == 1) { // ��������� ������
		return false;
	};
	if (result == 2) { // ����� ��������� ���������, ������������� �� ���������
		//return true;
	};
	if (result == 3) { // ����� ����� �������������
		if (LOAD_PARAM_FOR_SERVICE_set_ACL(root, reg_key) == false) {
			return false;
		};
	};

	return true;
}


bool Save_private_id_and_public_id_from_SERVICE_registry(unsigned long long private_id_, unsigned long long public_id_) {


	//send_udp("Save_private_id_and_public_id_from_SERVICE_registry() begin ");
	if (REG_CHECK_EXISTS_KEY_and_check_permissions(HKEY_LOCAL_MACHINE, L"Software\\VisiatorService") == false) {
		//s_edit_pass->set_label("save error");
	}

	LONG r;
	HKEY hkey;

	hkey = 0;

	r = RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"Software\\VisiatorService", 0, KEY_READ | KEY_WRITE | 0x0100, &hkey);
	if (hkey == 0) {
		r = RegCreateKeyEx(HKEY_LOCAL_MACHINE, L"Software\\VisiatorService", 0, L"", REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS | 0x0100, NULL, &hkey, NULL);
	};
	if (hkey == 0) {
		//send_udp("Save_private_id_and_public_id_from_SERVICE_registry() error 1 ");
		return false;
	}

	DWORD size;
	DWORD type;

	size = 8;
	type = REG_BINARY;


	if (RegSetValueEx(hkey, L"private_id", 0, type, (BYTE *)&private_id_, size) != 0) {
		RegCloseKey(hkey);
		return false;
	};
	if (RegSetValueEx(hkey, L"public_id", 0, type, (BYTE *)&public_id_, size) != 0) {
		RegCloseKey(hkey);
		return false;
	};

	RegCloseKey(hkey);

	//send_udp("Save_private_id_and_public_id_from_SERVICE_registry() finish ");
	return true;
}

bool Save_private_id_and_public_id_from_USER_registry(unsigned long long private_id_, unsigned long long public_id_) {
	LONG r;
	HKEY hkey;
	hkey = 0;

	r = RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\VisiatorDesktop", 0, KEY_READ | KEY_WRITE | 0x0100, &hkey);
	if (hkey == 0) {
		r = RegCreateKeyEx(HKEY_CURRENT_USER, L"Software\\VisiatorDesktop", 0, L"", REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS | 0x0100, NULL, &hkey, NULL);
	};
	if (hkey == 0) {
		//send_udp("Load_or_Create_MY_UNIC_ID() error1");
		return false;
	}

	DWORD size;
	DWORD type;

	size = 8;
	type = REG_BINARY;


	if (RegSetValueEx(hkey, L"private_id", 0, type, (BYTE *)&private_id_, size) != 0) {
		RegCloseKey(hkey);
		return false;
	};
	if (RegSetValueEx(hkey, L"public_id", 0, type, (BYTE *)&public_id_, size) != 0) {

		RegCloseKey(hkey);
		return false;
	};

	RegCloseKey(hkey);
	return true;
}




bool Register_new_partner_on_proxy() {

	uint64_t recv__counter = 0, send__counter = 0;

	//send_udp("Register_new_partner_on_proxy()...");

	if (app_attributes.is_service) {
		//send_udp("Save_private_id_and_public_id_from_SERVICE_registry...");
		if (Save_private_id_and_public_id_from_SERVICE_registry(0, 0) == false) {
			//send_udp("Register_new_partner_on_proxy() registry error");
			my_Slip(1000);
			return false;
		}
	}
	else {
		//send_udp("Save_private_id_and_public_id_from_USER_registry...");
		if (Save_private_id_and_public_id_from_USER_registry(0, 0) == false) {
			//send_udp("Register_new_partner_on_proxy() registry error");
			my_Slip(1000);
			return false;
		}
	}

	unsigned int ip_for_reg_new_partner;
	unsigned int crc32;
	unsigned char bb[500], xx[500];// , zz[500];;
	PACKET_LEVEL0 *r0;
	PACKET_LEVEL1_1002_responce *r1;
	PACKET_LEVEL1_1003_responce *r1003;

	PACKET_LEVEL0 *p0;
	PACKET_LEVEL1_1002_request *p1;
	PACKET_LEVEL1_1003_request *p1003;

	ip_for_reg_new_partner = 0;

	SOCKET sos;
	sockaddr_in dest_addr;

	MY_AES aes;
	MY_CRC crc;
	MY_RSA rsa;
	rsa.init();

	int res, snd;

	if (INVALID_SOCKET == (sos = socket(AF_INET, SOCK_STREAM, 0)))
	{ //3
		//send_udp("Register_new_partner_on_proxy status = create socket error");
		return false;
	} //3

	unsigned int view_ip;
	view_ip = get_ip_view_visiator_com();

	if (view_ip == 0) {
		//send_udp("REQUEST_decodepass view_ip == 0");
		closesocket(sos);
		return false;
	}


	PROXY_LIST *proxy_list; // app_attributes+

	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(SERVER_PORT);
	dest_addr.sin_addr.S_un.S_addr = view_ip;//inet_addr("139.162.182.46");// proxy_address );
	//status = "connect...";
	if (SOCKET_ERROR == connect(sos, (sockaddr*)&dest_addr, sizeof(dest_addr))) // proxy+!
	{//4
		closesocket(sos);
		proxy_list = app_attributes.proxy_list;
		if (proxy_list != NULL) {
			sos = proxy_list->try_connect(view_ip);//(inet_addr("139.162.182.46")); // ������� ������������ ����� ������ +
		};
		if (sos == 0) {
			closesocket(sos);
			//send_udp("Register_new_partner_on_proxy err connect");
			return false;
		};
	}//4

		//status = "connect ok";

	setnonblocking(sos);


	zero_unsigned_char(bb, 128);
	//PACKET_LEVEL0 *p0;
	//PACKET_LEVEL1_1002_request *p1;

	p0 = (PACKET_LEVEL0 *)bb;
	p0->packet_type = 1002;
	p1 = (PACKET_LEVEL1_1002_request *)p0->body;
	p1->sub_type = 101; // 101 - ������ ip ������� ��� ����������� ������ ��������
	p1->im_public_id = 0;
	p1->im_private_id = 0;

	//for (int i = 0; i < 16; i++) p1->AES_pass[i] = '0' + i;
	my_random(p1->AES_pass, 16);

	aes.set_key_16_byte(p1->AES_pass);

	p0->crc32 = crc.calc(&bb[8], 120);

	rsa.encode_128_byte(bb, bb);

	//status = "send_...";
	snd = 0;
	do
	{
		snd = my_send(sos, (unsigned char *)bb, 128, 0, "", &send__counter);//ok
	} while (snd != 128);

	//status = String("read... ");

	do
	{
		res = my_recv(sos, xx, 128, &recv__counter);
	} while (res != 128 && res != -1);
	aes.decrypt_128_byte(xx);

	//2019+ crc = CRC32_short(&xx[4], 124);
	crc32 = crc.calc(&xx[8], 120);
	//PACKET_LEVEL0 *r0;
	//PACKET_LEVEL1_1002_responce *r1;

	r0 = (PACKET_LEVEL0 *)xx;
	if (r0->crc32 == crc32)
	{
		//status = "crc ok";

		r1 = (PACKET_LEVEL1_1002_responce *)r0->body;

		ip_for_reg_new_partner = r1->ip4;
		//status = String(idx) + "read ok pause 3000 " + String(res);

		//::Sleep(300);
		//status = "close...";
	}
	else {
		//status = "crc err";
	};
	struct linger lng = { 1, 0 };
	setsockopt(sos, SOL_SOCKET, SO_LINGER, (char *)(&lng), sizeof(struct linger));

	int ee;
	ee = shutdown(sos, SD_BOTH);
	closesocket(sos);



	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	if (ip_for_reg_new_partner != 0 && ip_for_reg_new_partner != 0xffffffff) { // 3333
		if (INVALID_SOCKET == (sos = socket(AF_INET, SOCK_STREAM, 0)))
		{ //3
			//status = "create socket error";
			//send_udp("Register_new_partner_on_proxy err 4");
			return false;
		} //3




		dest_addr.sin_family = AF_INET;
		dest_addr.sin_port = htons(SERVER_PORT);
		dest_addr.sin_addr.S_un.S_addr = ip_for_reg_new_partner;// inet_addr("139.162.182.46");// proxy_address );
		//status = "connect...";
		if (SOCKET_ERROR == connect(sos, (sockaddr*)&dest_addr, sizeof(dest_addr))) // proxy+
		{//4
			closesocket(sos);
			sos = 0;
			proxy_list = app_attributes.proxy_list;
			if (proxy_list != NULL) {
				sos = proxy_list->try_connect(ip_for_reg_new_partner); // ������� ������������ ����� ������ +
			};
			if (sos == 0) {
				closesocket(sos);
				sos = 0;
				//send_udp("Register_new_partner_on_proxy err 5");
				return false;
			};
		}//4

			//status = "connect ok";

		setnonblocking(sos);


		zero_unsigned_char(bb, 128);
		//PACKET_LEVEL0 *p0;
		//PACKET_LEVEL1_1002_request *p1;

		p0 = (PACKET_LEVEL0 *)bb;
		p0->packet_type = 1003; // ����������� ����������� ������ ��������
		p1003 = (PACKET_LEVEL1_1003_request *)p0->body;


		//for (int i = 0; i < 16; i++) p1003->AES_pass[i] = '0' + i;
		my_random(p1003->AES_pass, 16);

		aes.set_key_16_byte(p1003->AES_pass);

		p0->crc32 = crc.calc(&bb[8], 120);

		rsa.encode_128_byte(bb, bb);

		//status = "send_...";
		snd = 0;
		do
		{
			snd = my_send(sos, (unsigned char *)bb, 128, 0, "", &send__counter);//ok
		} while (snd != 128); // WTF???

		//status = String("read... ");

		do
		{
			res = my_recv(sos, xx, 128, &recv__counter);
		} while (res != 128 && res != -1);
		aes.decrypt_128_byte(xx);

		//2019+ crc = CRC32_short(&xx[4], 124);
		crc32 = crc.calc(&xx[8], 120);
		//PACKET_LEVEL0 *r0;
		//PACKET_LEVEL1_1002_responce *r1;

		r0 = (PACKET_LEVEL0 *)xx;
		if (r0->crc32 == crc32)
		{
			//status = "crc ok";

			r1003 = (PACKET_LEVEL1_1003_responce *)r0->body;

			if (app_attributes.is_service) {
				//send_udp("Save_private_id_and_public_id_from_SERVICE_registry...");
				Save_private_id_and_public_id_from_SERVICE_registry(r1003->private_id, r1003->public_id);
			}
			else {
				//send_udp("Save_private_id_and_public_id_from_USER_registry...");
				Save_private_id_and_public_id_from_USER_registry(r1003->private_id, r1003->public_id);
			}
			//status = String(idx) + "read ok pause 3000 " + String(res);

			//::Sleep(300);
			//status = "close...";
		}
		else {
			//status = "crc err";
		};
		struct linger lng = { 1, 0 };
		setsockopt(sos, SOL_SOCKET, SO_LINGER, (char *)(&lng), sizeof(struct linger));

		int ee;
		ee = shutdown(sos, SD_BOTH);
		closesocket(sos);


	}//3333

	//send_udp("Register_new_partner_on_proxy() finish");
	return true;
}

int my_strcmp(unsigned char *p1, unsigned char *p2) {
	if (p1 == NULL || p2 == NULL) return -1;
	int i;
	i = 0;
	while (p1[i] == p2[i] && p1[i] != 0 && p2[i] != 0) i++;
	if (p1[i] == 0 && p2[i] == 0) return 0;
	return -1;
}
int my_strcmp(char *p1, char *p2) {
	if (p1 == NULL || p2 == NULL) return -1;
	int i;
	i = 0;
	while (p1[i] == p2[i] && p1[i] != 0 && p2[i] != 0) i++;
	if (p1[i] == 0 && p2[i] == 0) return 0;
	return -1;
}
int my_strcmp(wchar_t *p1, wchar_t *p2) {
	if (p1 == NULL || p2 == NULL) return -1;
	int i;
	i = 0;
	while (p1[i] == p2[i] && p1[i] != 0 && p2[i] != 0) i++;
	if (p1[i] == 0 && p2[i] == 0) return 0;
	return -1;
};

int setnonblocking(int sockfd) {
	ULONG          ulB;
	ulB = TRUE; // Set socket to non-blocking mode
	ioctlsocket(sockfd, FIONBIO, &ulB);
	//   fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFD, 0)|O_NONBLOCK);
	int flag = 1;
	int result = setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(int));
	return 0;
}


int my_send(uint32_t s, uint8_t *buf, int len, int flag, char *info, uint64_t *send_couner) {
	//send__udp("send_ ", len);
	int res, er;
	res = send(s, (const char *)buf, len, flag);// ok
	if (res == 0)
	{
		//rstatus = "0";
		return -1;
	};

	if (res > 0) { //rstatus = "+"; 
		*send_couner += res;
		return res;
	};
	er = WSAGetLastError();
	/*
	if (er == WSANOTINITIALISED) { rstatus = "WSANOTINITIALISED"; };   //  A successful WSAStartup call must occur before using this function.
	if (er == WSAENETDOWN) { rstatus = "WSAENETDOWN"; };   //  The network subsystem has failed.
	if (er == WSAEACCES) { rstatus = "WSAEACCES"; };  //     The requested address is a broadcast address, but the appropriate flag was not set. Call setsockopt with the SO_BROADCAST socket option to enable use of the broadcast address.
	if (er == WSAEINTR) { rstatus = "WSAEINTR"; };   //     A blocking Windows Sockets 1.1 call was canceled through WSACancelBlockingCall.
	if (er == WSAEINPROGRESS) { rstatus = "WSAEINPROGRESS"; }; // A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function.
	if (er == WSAEFAULT) { rstatus = "WSAEFAULT"; };  //     The buf parameter is not completely contained in a valid part of the user address space.
	if (er == WSAENETRESET) { rstatus = "WSAENETRESET"; }; //   The connection has been broken due to the keep-alive activity detecting a failure while the operation was in progress.
	if (er == WSAENOBUFS) { rstatus = "WSAENOBUFS"; };  //    No buffer space is available.
	if (er == WSAENOTCONN) { rstatus = "WSAENOTCONN"; }; //    The socket is not connected.
	if (er == WSAENOTSOCK) { rstatus = "WSAENOTSOCK"; };  //   The descriptor is not a socket.
	if (er == WSAEOPNOTSUPP) { rstatus = "WSAEOPNOTSUPP"; }; //  MSG_OOB was specified, but the socket is not stream-style such as type SOCK_STREAM, OOB data is not supported in the communication domain associated with this socket, or the socket is unidirectional and supports only receive operations.
	if (er == WSAESHUTDOWN) { rstatus = "WSAESHUTDOWN"; }; //   The socket has been shut down; it is not possible to send_ on a socket after shutdown has been invoked with how set to SD_SEND or SD_BOTH.
	if (er == WSAEWOULDBLOCK) { rstatus = "WSAEWOULDBLOCK_0"; }; // The socket is marked as nonblocking and the requested operation would block.
	if (er == WSAEMSGSIZE) { rstatus = "WSAEMSGSIZE"; };   //  The socket is message oriented, and the message is larger than the maximum supported by the underlying transport.
	if (er == WSAEHOSTUNREACH) { rstatus = "WSAEHOSTUNREACH"; }; //The remote host cannot be reached from this host at this time.
	if (er == WSAEINVAL) { rstatus = "WSAEINVAL"; };     //  The socket has not been bound with bind, or an unknown flag was specified, or MSG_OOB was specified for a socket with SO_OOBINLINE enabled.
	if (er == WSAECONNABORTED) { rstatus = "WSAECONNABORTED_-1"; };// The virtual circuit was terminated due to a time-out or other failure. The application should close the socket as it is no longer usable.
	if (er == WSAECONNRESET) { rstatus = "WSAECONNRESET_-1"; }; //  The virtual circuit was reset by the remote side executing a hard or abortive close. For UDP sockets, the remote host was unable to deliver a previously sent UDP datagram and responded with a "Port Unreachable" ICMP packet. The application should close the socket as it is no longer usable.
	if (er == WSAETIMEDOUT) { rstatus = "WSAETIMEDOUT"; }; //   The connection has been dropped, because of a network failure or because the system on the other end went down without notice.
	*/
	if (er == WSAEWOULDBLOCK) {
		return 0;
	};
	if (er == WSAECONNRESET || er == WSAECONNABORTED) {
		return -1;
	};
	return -1;
}

int my_recv(uint32_t socket_, uint8_t *buf, int len, uint64_t *recv_couner) {
	//AnsiString status;

	int res, er;
	res = recv(socket_, (char *)buf, len, 0);
	if (res == 0)
	{
		//rstatus = "0";
		//send_udp(L"my_recv() return -1");
		return -1;
	};

	if (res > 0) { //rstatus = "+"; 
		*recv_couner += res;
		return res;
	};
	er = WSAGetLastError();
	/*
	if (er == WSANOTINITIALISED) { rstatus = "WSANOTINITIALISED"; };   //  A successful WSAStartup call must occur before using this function.
	if (er == WSAENETDOWN) { rstatus = "WSAENETDOWN"; };   //  The network subsystem has failed.
	if (er == WSAEACCES) { rstatus = "WSAEACCES"; };  //     The requested address is a broadcast address, but the appropriate flag was not set. Call setsockopt with the SO_BROADCAST socket option to enable use of the broadcast address.
	if (er == WSAEINTR) { rstatus = "WSAEINTR"; };   //     A blocking Windows Sockets 1.1 call was canceled through WSACancelBlockingCall.
	if (er == WSAEINPROGRESS) { rstatus = "WSAEINPROGRESS"; }; // A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function.
	if (er == WSAEFAULT) { rstatus = "WSAEFAULT"; };  //     The buf parameter is not completely contained in a valid part of the user address space.
	if (er == WSAENETRESET) { rstatus = "WSAENETRESET"; }; //   The connection has been broken due to the keep-alive activity detecting a failure while the operation was in progress.
	if (er == WSAENOBUFS) { rstatus = "WSAENOBUFS"; };  //    No buffer space is available.
	if (er == WSAENOTCONN) { rstatus = "WSAENOTCONN"; }; //    The socket is not connected.
	if (er == WSAENOTSOCK) { rstatus = "WSAENOTSOCK"; };  //   The descriptor is not a socket.
	if (er == WSAEOPNOTSUPP) { rstatus = "WSAEOPNOTSUPP"; }; //  MSG_OOB was specified, but the socket is not stream-style such as type SOCK_STREAM, OOB data is not supported in the communication domain associated with this socket, or the socket is unidirectional and supports only receive operations.
	if (er == WSAESHUTDOWN) { rstatus = "WSAESHUTDOWN"; }; //   The socket has been shut down; it is not possible to send_ on a socket after shutdown has been invoked with how set to SD_SEND or SD_BOTH.
	if (er == WSAEWOULDBLOCK) { rstatus = "WSAEWOULDBLOCK_0"; }; // The socket is marked as nonblocking and the requested operation would block.
	if (er == WSAEMSGSIZE) { rstatus = "WSAEMSGSIZE"; };   //  The socket is message oriented, and the message is larger than the maximum supported by the underlying transport.
	if (er == WSAEHOSTUNREACH) { rstatus = "WSAEHOSTUNREACH"; }; //The remote host cannot be reached from this host at this time.
	if (er == WSAEINVAL) { rstatus = "WSAEINVAL"; };     //  The socket has not been bound with bind, or an unknown flag was specified, or MSG_OOB was specified for a socket with SO_OOBINLINE enabled.
	if (er == WSAECONNABORTED) { rstatus = "WSAECONNABORTED_-1"; };// The virtual circuit was terminated due to a time-out or other failure. The application should close the socket as it is no longer usable.
	if (er == WSAECONNRESET) { rstatus = "WSAECONNRESET_-1"; }; //  The virtual circuit was reset by the remote side executing a hard or abortive close. For UDP sockets, the remote host was unable to deliver a previously sent UDP datagram and responded with a "Port Unreachable" ICMP packet. The application should close the socket as it is no longer usable.
	if (er == WSAETIMEDOUT) { rstatus = "WSAETIMEDOUT"; }; //   The connection has been dropped, because of a network failure or because the system on the other end went down without notice.
	*/
	if (er == WSAEWOULDBLOCK) {
		return 0;
	};


	if (er == WSAECONNRESET || er == WSAECONNABORTED) {
		return -1;
	};



	return -1;
};

void my_Slip(DWORD p) {
	p = p / 100;
	while (GLOBAL_STOP == false && p > 0) {
		Sleep(100);
		p--;
	}
}

unsigned int get_ip_view_visiator_com() {

	unsigned int ip;
	PROXY_LIST *proxy_list = app_attributes.proxy_list;

	HOSTENT *hosten;
	hosten = gethostbyname(view_visiator_com);
	if (hosten == NULL) {
		ip = 0;
	}
	else {
		ip = ((in_addr*)hosten->h_addr_list[0])->s_addr;
	};
	//ip = 0;

	if (ip == 0) {
		if (proxy_list != NULL) {
			ip = proxy_list->get_ip_view_visiator_com();
		}
	}


	return ip;
}

//********************************************************************************************************
//**  PROXY_element

PROXY_element::PROXY_element(unsigned int type_, unsigned int ip_, unsigned int port_) {
	type = type_;
	ip = ip_;
	port = port_;
}

//********************************************************************************************************
//**  PROXY_LIST


PROXY_LIST::PROXY_LIST() {
	clean();
}

void PROXY_LIST::clean() {

}

void PROXY_LIST::add(unsigned int type_, unsigned int ip_, unsigned int port_) {
	
	std::list <PROXY_element>::iterator it, end;

	for (it = elements.begin(), end = elements.end(); it != end; ++it) {
		if (it->type == type_ && it->ip == ip_ && it->port == port_) {
			return;
		}
	}

	elements.emplace_back( type_, ip_, port_ );	
}

bool PROXY_LIST::Load_ProxySettings() {

	unsigned int proxy_ip, proxy_port;



	HKEY hkey;

	hkey = 0;    //HKEY_USERS    .DEFAULT\\Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\\Connections
	RegOpenKeyEx(HKEY_USERS, L"", 0, KEY_READ | KEY_ENUMERATE_SUB_KEYS | 0x0100, &hkey);
	if (hkey == 0) {
		//send_udp("error1 Load_ProxySettings");
		return false;
	}

	LONG rr;
	DWORD idx;
	wchar_t name[500];
	DWORD name_size, class_size;


	wchar_t aa[2500];


	idx = 0;
	do
	{
		class_size = 0;
		name_size = 500 - 5;
		rr = RegEnumKeyEx(hkey, idx, name, &name_size, NULL, NULL, &class_size, NULL);
		if (rr == ERROR_SUCCESS) {

			if (my_strlen(name) < 500) {
				wsprintf(aa, L"%s%s", name, L"\\Software\\Visiator");

				load_from_registry(hkey, aa);
				Load_ProxySettings_from_reg(&proxy_ip, &proxy_port, name);
				if (proxy_ip != 0) {
					add( PROXY_TYPE_HTTPS, proxy_ip, proxy_port );
				}
			};
			idx++;
		}
	} while (rr == ERROR_SUCCESS);

	RegCloseKey(hkey);

	return true;
}

void PROXY_LIST::load_from_registry(HKEY key, wchar_t *subkey_name) {
	HKEY hkey;
	LSTATUS rr;

	hkey = 0;    //HKEY_USERS    .DEFAULT\\Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\\Connections
	rr = RegOpenKeyEx(key, subkey_name, 0, KEY_READ | KEY_WRITE | 0x0100, &hkey);
	if (hkey == 0) {
		return;
	}


	unsigned int *ii, type_, ip_, port_;
	unsigned char b[360];
	zero_unsigned_char(b, 360);

	DWORD size;
	DWORD type;

	size = 360;
	type = REG_BINARY;

	rr = RegQueryValueEx(hkey, L"list360", 0, &type, (BYTE *)b, &size);
	if (rr == 0 && size == 240) {
		ii = (unsigned int *)b;
		for (int i = 0; i < 30; i++) {
			type_ = *ii++;
			ip_ = *ii++;
			port_ = *ii++;
			if (ip_ != 0 && port_ != 0) {
				//send_udp(subkey_name);
				add(type_, ip_, port_ );

			}
		}
	}

	RegCloseKey(hkey);
};

void PROXY_LIST::save_in_registry(HKEY key, wchar_t *subkey_name) {

	HKEY hkey;
	LSTATUS rr;

	hkey = 0;    //HKEY_USERS    .DEFAULT\\Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\\Connections
	rr = RegOpenKeyEx(key, subkey_name, 0, KEY_READ | KEY_WRITE | 0x0100, &hkey);
	if (rr == 2) { // ��� �����
		rr = RegCreateKeyEx(key, subkey_name, 0, L"", REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS | 0x0100, NULL, &hkey, NULL);
	};
	if (hkey == 0) {
		return;
	}

	unsigned char b[360];
	zero_unsigned_char(b, 360);

	unsigned int *ii;
	ii = (unsigned int *)b;

	std::list <PROXY_element>::iterator it, end;

	for (it = elements.begin(), end = elements.end(); it != end; ++it) {
		*ii++ = it->type;
		*ii++ = it->ip;
		*ii++ = it->port;
	}


	DWORD size;
	DWORD type;

	size = 360;
	type = REG_BINARY;

	rr = RegSetValueEx(hkey, L"list360", 0, type, (BYTE *)b, size);


	RegCloseKey(hkey);

}

DWORD PROXY_LIST::try_connect(unsigned int ip) {

	int i;
	DWORD sos;
	sos = 0;

	std::list <PROXY_element>::iterator it, end;

	for (it = elements.begin(), end = elements.end(); it != end; ++it) {
		sos = try_connect_proxy(it->ip, it->port, ip);
		if (sos != 0) {
			return sos;
		}
		Sleep(1);
	}

	

	return 0;
}

unsigned int PROXY_element::get_ip_view_visiator_com() {
	/////////////////////////////////////////////////////

	if (ip == 0 || port == 0) return 0;

	//unsigned int proxy_ip, unsigned int proxy_port

	unsigned long long recv__counter_ = 0, send__counter_ = 0;

	unsigned int dns_ip;

	dns_ip = 0;

	unsigned char bb[4004];
	int snd, res;
	SOCKET sos;
	sockaddr_in dest_addr;

	if (INVALID_SOCKET == (sos = socket(AF_INET, SOCK_STREAM, 0)))
	{ //3

		return 0;
	} //3

	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = (u_short)htons(port);
	dest_addr.sin_addr.S_un.S_addr = ip;// proxy_address );
	//status = "connect...";
	if (SOCKET_ERROR == connect(sos, (sockaddr*)&dest_addr, sizeof(dest_addr))) // proxy+!
	{//4
		closesocket(sos);
		sos = 0;
		//status = "err connect";
		//send_udp("get_ip_view_visiator_com err 3");
		return 0;
	}//4

	setnonblocking(sos);
	unsigned char aa[500]; //char buf_ip[100];
	int aa_len;

	my_strcpy((unsigned char *)aa, (unsigned char *)"GET http://ip.visiator.com/ HTTP/1.1\r\nHost: ip.visiator.com\r\n\r\n ");
	//my_strcat((char *)aa, ip_to_char(buf_ip, ip));
	//my_strcat((char *)aa, ":443 HTTP/1.1\r\nProxy-Connection: Keep-Alive\r\n\r\n");

	aa_len = my_strlen(aa);
	snd = my_send(sos, (unsigned char *)aa, aa_len, 0, "", &send__counter_);
	if (snd != aa_len) {
		closesocket(sos);
		sos = 0;
		return 0;
	}
	int cc, r;

	for (int i = 0; i < 4004; i++) bb[i] = 0;

	cc = 0;
	r = 0;
	do {
		res = my_recv(sos, bb + r, 4000, &recv__counter_);
		if (res > 0) r += res;
		if (res == 0) {
			Sleep(10); cc++;
		};
	} while (cc < 500 && res == 0 && GLOBAL_STOP == false);



	if (res == 0 || http_result_code(bb, res) == 200) {

		int i, k, ii[5], jj;// j
		unsigned char *cc;
		cc = (unsigned char *)&jj;
		ii[0] = 0;
		ii[1] = 0;
		ii[2] = 0;
		ii[3] = 0;

		k = 0;
		unsigned char *body;
		unsigned char iip[20];
		for (int i = 0; i < 20; i++) iip[i] = 0;
		body = http_get_body(bb, res);
		if (body != NULL) {
			if (body[0] == 'i' && body[1] == 'p' && body[2] == '=') {
				i = 3;
				while (i < 20 && body[i] != '=' && body[i] != 0) {
					if (body[i] != '.' && (body[i] < '0' || body[i] > '9')) {
						i = 100;
					}
					else {
						if (body[i] == '.') {
							if (k < 3) 							k++;
						}
						else {
							ii[k] *= 10; ii[k] += body[i] - '0';
						}

						iip[i - 3] = body[i];
					}
					i++;
				}
				if (i < 20) {
					cc[3] = ii[3];
					cc[2] = ii[2];
					cc[1] = ii[1];
					cc[0] = ii[0];

					dns_ip = jj;
				}
			}
		}


	}
	closesocket(sos);
	return dns_ip;
}

unsigned int PROXY_LIST::get_ip_view_visiator_com() {

	int i;
	unsigned int ip;
	ip = 0;

	/*
	i = 0;
	while (i < element_max_count) {
		if (element[i].ip != 0 && element[i].port != 0) {
			ip = element[i].get_ip_view_visiator_com();
			if (ip != 0) {
				return ip;
			}
		}
		i++;
	}*/

	std::list <PROXY_element>::iterator it, end;

	for (it = elements.begin(), end = elements.end(); it != end; ++it) {
		
		ip = it->get_ip_view_visiator_com();
		if (ip != 0) {
			return ip;
		}
		/*
		*ii++ = it->type;
		*ii++ = it->ip;
		*ii++ = it->port;
		*/
	}

	return 0;
};

//**********************************************************************************************

DWORD try_connect_proxy(unsigned int proxy_ip, unsigned int proxy_port, unsigned int ip) {

	unsigned long long recv__counter = 0, send__counter = 0;

	unsigned char bb[400];
	int snd, res;
	SOCKET sos;
	sockaddr_in dest_addr;

	if (INVALID_SOCKET == (sos = socket(AF_INET, SOCK_STREAM, 0)))
	{ //3

		return 0;
	} //3

	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons((u_short)proxy_port);
	dest_addr.sin_addr.S_un.S_addr = proxy_ip;// proxy_address );
	//status = "connect...";
	if (SOCKET_ERROR == connect(sos, (sockaddr*)&dest_addr, sizeof(dest_addr))) // proxy+
	{//4
		closesocket(sos);
		sos = 0;
		//status = "err connect";
		//send_udp("try_connect_proxy err 3");
		return 0;
	}//4

	setnonblocking(sos);
	unsigned char aa[500]; char buf_ip[100];
	int aa_len;

	my_strcpy( (unsigned char *)aa, (unsigned char *)"CONNECT ");
	my_strcat((char *)aa, 490, (char *)ip_to_char(buf_ip, ip));
	my_strcat((char *)aa, 490, (char *)":443 HTTP/1.1\r\nProxy-Connection: Keep-Alive\r\n\r\n");

	aa_len = my_strlen(aa);
	snd = my_send(sos, (unsigned char *)aa, aa_len, 0, "", &send__counter);
	if (snd != aa_len) {
		closesocket(sos);
		sos = 0;
		return 0;
	}
	int cc;
	cc = 0;
	do {
		res = my_recv(sos, bb, 400, &recv__counter);
		if (res == 0) {
			Sleep(10); cc++;
		};
	} while (cc < 200 && res == 0 && GLOBAL_STOP == false);



	if (res == 0 || http_result_code(bb, res) != 200) {
		closesocket(sos);
		return 0;
	}

	return sos;
}


void Load_ProxySettings_from_reg(unsigned int *proxy_ip, unsigned int *proxy_port, wchar_t *name) {

	*proxy_ip = 0;
	*proxy_port = 0;

	wchar_t bb[2000];
	my_strcpy(bb, name);
	my_strcat(bb, L"\\Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\\Connections");

	HKEY hkey;
	hkey = 0;
	RegOpenKeyEx(HKEY_USERS, bb, 0, KEY_READ | KEY_ENUMERATE_SUB_KEYS | 0x0100, &hkey);

	if (hkey == 0) {

		return;
	}



	DWORD size;
	DWORD type;

	unsigned char buf[500], buf2[500];
	for (int i = 0; i < 500; i++) {
		buf[i] = 0; buf2[i] = 0;
	};

	size = 500;
	type = REG_BINARY;
	if (RegQueryValueEx(hkey, L"DefaultConnectionSettings", 0, &type, buf, &size) != 0) {
		RegCloseKey(hkey);
		return;
	};
	RegCloseKey(hkey);

	unsigned int *ii;
	ii = (unsigned int *)buf;
	ii += 3; // *ii = str len



	if (*ii != 0) {

		for (unsigned int i = 0; i < *ii; i++) buf2[i] = buf[i + 16];
		//send_udp("bbbbbbbbbbbbb ", (char *)buf2);

		int i, j, k, port, ip[4];
		i = 0;
		j = 0;
		k = 0;
		port = 0;

		ip[0] = 0; ip[1] = 0; ip[2] = 0; ip[3] = 0;

		while (i < 500 - 5 && buf2[i] != 0) {
			if ((buf2[i] >= '0' && buf2[i] <= '9') || (buf2[i] == '.') || (buf2[i] == ':')) {
				if (buf2[i] == ':') {
					if (j > 0) return;
					j++;

				}
				else {
					if (j == 0) {
						if (buf2[i] == '.') {
							if (k >= 3) { return; }
							k++;
						}
						else {
							ip[k] *= 10;
							ip[k] += decode_dig(buf2[i]);
							if (ip[k] > 255) return;
						}
					}
					else {
						port *= 10;
						port += decode_dig(buf2[i]);
					}
				}
			}
			else {
				return;
			}
			i++;
			if (i > 100) return;
		}

		unsigned char *q;

		q = (unsigned char *)proxy_ip;
		q[0] = (unsigned char)ip[0];
		q[1] = (unsigned char)ip[1];
		q[2] = (unsigned char)ip[2];
		q[3] = (unsigned char)ip[3];

		*proxy_port = port;
	};


}


void my_strcpy(wchar_t *dest, wchar_t *source) {
	if (source == NULL && dest != NULL) {
		dest[0] = 0;
		return;
	}
	if (source == NULL || dest == NULL) return;
	int i;
	i = 0;
	while (source[i] != 0) {
		dest[i] = source[i];
		i++;
	};
	dest[i] = 0;
};

void my_strcat(wchar_t *dest, wchar_t *source) {
	if (source == NULL || dest == NULL) return;
	int i, j;
	j = 0;
	while (dest[j] != 0) j++;
	i = 0;
	while (source[i] != 0 && i < 10000 - 5) {
		dest[j + 1] = 0;
		dest[j] = source[i];
		i++;
		j++;
	};
	dest[j] = 0;
}

bool my_random(BYTE *buf, int buf_len) {
	HCRYPTPROV hProv;
	//HCRYPTPROV *phProv = &hProv;
	//BYTE bBuffer[12];			// ������ ��� ��������������� �����
	//BYTE *pbBuffer = bBuffer;		// ��������� �� ������
	//DWORD dwLen = sizeof(bBuffer);
	BOOL retval;

	//send_udp("CryptAcquireContext 1");

	//retval = CryptAcquireContext(&hProv, 0, 0, PROV_RSA_FULL, 0);
	retval = CryptAcquireContext(&hProv, L"Container", NULL, PROV_RSA_FULL, 0);
	if (retval == 0 && GetLastError() == NTE_BAD_KEYSET)
	{
		retval = CryptAcquireContext(&hProv, L"Container", NULL, PROV_RSA_FULL, CRYPT_NEWKEYSET);

		if (!retval != 0)
		{
			// Error ...
			//send_udp("CryptAcquireContext error 2");
			return false;
		}
	}
	/* ���� ������� ����������� ������� */
	if (retval != 0)
	{

		/* ���������� ��� ������ ��������� ����� */
		CryptGenRandom(hProv, buf_len, buf);
		/* ����������� ������������� ������ ������ */
		CryptReleaseContext(hProv, 0);
		//send_udp("CryptAcquireContext ret 2");
		return true;
	}
	else {
		//send_udp("CryptAcquireContext error");
	}
	//send_udp("CryptAcquireContext ret 1");
	return false;
}

int get_hexch_w(wchar_t v) {
	if (v == '0') return 0;
	if (v == '1') return 1;
	if (v == '2') return 2;
	if (v == '3') return 3;
	if (v == '4') return 4;
	if (v == '5') return 5;
	if (v == '6') return 6;
	if (v == '7') return 7;
	if (v == '8') return 8;
	if (v == '9') return 9;
	if (v == 'a' || v == 'A') return 10;
	if (v == 'b' || v == 'B') return 11;
	if (v == 'c' || v == 'C') return 12;
	if (v == 'd' || v == 'D') return 13;
	if (v == 'e' || v == 'E') return 14;
	if (v == 'f' || v == 'F') return 15;
	return 0;
}

unsigned int decode_dig(char v) {

	if (v == '0') return 0;
	if (v == '1') return 1;
	if (v == '2') return 2;
	if (v == '3') return 3;
	if (v == '4') return 4;
	if (v == '5') return 5;
	if (v == '6') return 6;
	if (v == '7') return 7;
	if (v == '8') return 8;
	if (v == '9') return 9;

	return 0;
}

void check_bit(DWORD v) {
	int i = 0;
	DWORD j;
	j = 1;
	while (i < 32) {
		/*if ((v & j) == j) {
			send_udp(L"[ + ] bit ", i);
		}
		else {
			send_udp(L"[   ] bit ", i);
		}*/
		j *= 2;
		i++;
	}
}

void my_strcpy(unsigned char *dest, unsigned char *source) {
	if (source == NULL && dest != NULL) {
		dest[0] = 0;
		return;
	}
	if (source == NULL || dest == NULL) return;
	int i;
	i = 0;
	while (source[i] != 0) {
		dest[i] = source[i];
		i++;
	};
	dest[i] = 0;
};

void my_strcat(char *dest, int max_len, char *source) {
	if (source == NULL || dest == NULL) return;
	int i, j;
	j = 0;
	while (dest[j] != 0) {
		if (j >= max_len) return;
		j++;
	};
	i = 0;
	while (source[i] != 0) {
		if (i >= max_len || j + 1 >= max_len) return;
		dest[j + 1] = 0;
		dest[j] = source[i];
		i++;
		j++;
	};

}

char *ip_to_char(char *buf, unsigned int ip) {
	unsigned char *q;
	q = (unsigned char *)&ip;
	sprintf__s_i_i_i_i(buf, 16, "%d.%d.%d.%d", (int)q[0], (int)q[1], (int)q[2], (int)q[3]);

	return buf;
}

int http_result_code(unsigned char *b, int len) {
	int i, cod, d;
	i = 0;
	cod = 0;
	while (i < 100 && i < len && b[i] != ' ') i++;
	if (i < 100 && i < len && b[i] == ' ') {
		i++;
		while (i < 100 && i < len && b[i] != ' ') {
			d = decode_dig(b[i]);
			cod *= 10;
			cod += d;
			i++;
		};
	};

	return cod;
}

unsigned char *http_get_body(unsigned char *b, int len) {
	int i;
	i = 4;
	while (i < len) {
		if (b[i - 4] == '\r' && b[i - 3] == '\n' && b[i - 2] == '\r' && b[i - 1] == '\n') {
			return &b[i];
		}
		i++;
	}

	return NULL;
}

unsigned int get_IP_for_server_location(unsigned long long partner_id, unsigned long long public_id, unsigned long long private_id) {

	unsigned long long recv__counter = 0, send__counter = 0;

	unsigned int crc32;
	unsigned char bb[500], xx[500]; // , zz[500]

	SOCKET sos;
	sockaddr_in dest_addr;

	unsigned ip_to_server_connect;
	ip_to_server_connect = 0;

	MY_AES aes;
	MY_CRC crc;
	MY_RSA rsa;
	rsa.init();

	int res, snd;

	if (INVALID_SOCKET == (sos = socket(AF_INET, SOCK_STREAM, 0)))
	{ //3
		//status = "create socket error";
		return 1;
	} //3

	unsigned int view_ip;
	view_ip = get_ip_view_visiator_com();

	if (view_ip == 0) {
		//send_udp("view_ip == 0");
		closesocket(sos);
		return 1;
	}

	PROXY_LIST *proxy_list = app_attributes.proxy_list;

	/*HOSTENT *hosten;
	hosten = gethostbyname(view _visiator_com);
	if (hosten == NULL) {
		closesocket(sos);
		sos = 0;
		//status = "err connect";
		return 2;
	}*/

	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(SERVER_PORT);
	//dest_addr.sin_addr.S_un.S_addr = inet_addr("139.162.182.46");// proxy_address );
	dest_addr.sin_addr.S_un.S_addr = view_ip;// ((in_addr*)hosten->h_addr_list[0])->s_addr;







		//status = "connect...";
	if (SOCKET_ERROR == connect(sos, (sockaddr*)&dest_addr, sizeof(dest_addr))) // proxy+!
	{//4
		closesocket(sos);
		sos = 0;

		if (proxy_list != NULL) {
			sos = proxy_list->try_connect(view_ip); //(((in_addr*)hosten->h_addr_list[0])->s_addr); // ������� ������������ ����� ������ +
		};
		if (sos == 0) {
			closesocket(sos);
			return 3;
		};
	}//4

	setnonblocking(sos);


	zero_unsigned_char(bb, 128);

	//***************************************
	PACKET_LEVEL0 *p0;
	PACKET_LEVEL1_1002_request *p1002;


	unsigned char AES_pass[16];
	my_random(AES_pass, 16);

	aes.set_key_16_byte(AES_pass);

	p0 = (PACKET_LEVEL0 *)bb;
	p0->packet_type = 1002;
	p1002 = (PACKET_LEVEL1_1002_request *)p0->body;
	p1002->sub_type = 103;
	p1002->im_public_id = partner_id;
	p1002->im_private_id = public_id;
	for (int i = 0; i < 16; i++) p1002->AES_pass[i] = AES_pass[i];

	p0->crc32 = crc.calc(&bb[8], 120);
	rsa.encode_128_byte(bb, bb);


	//***************************************	

	//status = "send_...";
	snd = 0;
	do
	{
		snd = my_send(sos, (unsigned char *)bb, 128, 0, "", &send__counter); // ok
		Sleep(1);
	} while (snd != 128);

	//status = String("read... ");

	do
	{
		res = my_recv(sos, xx, 128, &recv__counter);
		Sleep(1);
	} while (res != 128 && res != -1 && GLOBAL_STOP == false);
	aes.decrypt_128_byte(xx);

	//2019+ crc = CRC32_short(&xx[4], 124);
	crc32 = crc.calc(&xx[8], 120);
	PACKET_LEVEL0 *r0;
	PACKET_LEVEL1_1002_responce *r1;

	r0 = (PACKET_LEVEL0 *)xx;
	if (r0->crc32 == crc32)
	{
		//status = "crc ok";

		r1 = (PACKET_LEVEL1_1002_responce *)r0->body;

		ip_to_server_connect = r1->ip4;
		//status = String(idx) + "read ok pause 3000 " + String(res);

		//::Sleep(300);
		//status = "close...";
	}
	else {
		//status = "crc err";
	};
	struct linger lng = { 1, 0 };
	setsockopt(sos, SOL_SOCKET, SO_LINGER, (char *)(&lng), sizeof(struct linger));

	int ee;
	ee = shutdown(sos, SD_BOTH);
	closesocket(sos);

	return ip_to_server_connect;


}

BOOL IsElevated() {
	BOOL fRet = FALSE;
	HANDLE hToken = NULL;
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
		TOKEN_ELEVATION Elevation;
		DWORD cbSize = sizeof(TOKEN_ELEVATION);
		if (GetTokenInformation(hToken, TokenElevation, &Elevation, sizeof(Elevation), &cbSize)) {
			fRet = Elevation.TokenIsElevated;
		}
	}
	if (hToken) {
		CloseHandle(hToken);
	}
	return fRet;

}


#define MAX_NAME 256

bool check_admin() {

	BOOL EE;

	EE = IsElevated();
	if (EE == TRUE) { //send_udp("Elevated"); 
	}
	else { //send_udp("NO Elevated"); 
	};

	bool detect = false;
	unsigned int i;
	PTOKEN_GROUPS pGroupInfo;
	SID_IDENTIFIER_AUTHORITY SIDAuth = SECURITY_NT_AUTHORITY;
	PSID pSID = NULL;
	wchar_t lpName[MAX_NAME];
	wchar_t lpDomain[MAX_NAME];
	SID_NAME_USE SidType;
	HANDLE hToken;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
		// error
		return false;
	}
	DWORD dwSize = 0, dwResult = 0;

	if (!GetTokenInformation(hToken, TokenGroups, NULL, dwSize, &dwSize)) {
		dwResult = GetLastError();

		wchar_t Buf[256];



		if (dwResult != ERROR_INSUFFICIENT_BUFFER) {
			//printf("GetTokenInformation Error %u\n", dwResult);
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwResult,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), Buf, sizeof(Buf), NULL);
			return FALSE;
		}

	};

	pGroupInfo = (PTOKEN_GROUPS)GlobalAlloc(GPTR, dwSize);

	if (!GetTokenInformation(hToken, TokenGroups, pGroupInfo, dwSize, &dwSize))
	{
		// error
		return false;
	}

	if (!AllocateAndInitializeSid(&SIDAuth, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &pSID))
	{
		// error
		return false;
	}

	for (i = 0; i < pGroupInfo->GroupCount; i++)
	{
		if (EqualSid(pSID, pGroupInfo->Groups[i].Sid))
		{

			// Lookup the account name and print it.

			dwSize = MAX_NAME;
			if (!LookupAccountSid(NULL, pGroupInfo->Groups[i].Sid, lpName, &dwSize, lpDomain, &dwSize, &SidType))
			{
				dwResult = GetLastError();
				if (dwResult == ERROR_NONE_MAPPED) {
					// strcpy_s(lpName, dwSize, "NONE_MAPPED");

				}
				else
				{
					// error
					//printf("LookupAccountSid Error %u\n", GetLastError());
					return false;
				}
			}
			//printf("Current user is a member of the %s\\%s group\n",	lpDomain, lpName);
			//send_udp(lpDomain, lpName);
			// Find out whether the SID is enabled in the token.
			if (pGroupInfo->Groups[i].Attributes & SE_GROUP_ENABLED) {
				//send_udp("The group SID is enabled.\n");
				detect = true;
			}
			else {
				if (pGroupInfo->Groups[i].Attributes &	SE_GROUP_USE_FOR_DENY_ONLY) {
					//send_udp("The group SID is a deny-only SID.\n");
				}
				else {
					//send_udp("The group SID is not enabled.\n");
				};
			}
		}
	}

	if (pSID)
		FreeSid(pSID);
	if (pGroupInfo)
		GlobalFree(pGroupInfo);

	return detect;


}

int  check_IsWow64(DWORD pid)
{

	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
	if (hProcess == NULL)
		return 0;

	typedef BOOL(WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
	LPFN_ISWOW64PROCESS fnIsWow64Process;
	fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(TEXT("kernel32")), "IsWow64Process");

	BOOL isWow64;
	bool res = fnIsWow64Process != NULL && fnIsWow64Process(hProcess, &isWow64);
	CloseHandle(hProcess);
	if (res == true) {
		if (isWow64 == TRUE) {
			return 64;
		}
		else {
			return 32;
		}
	}

	return 0;
}

unsigned int get_sol() {

	//return 0x7788BBDD;
	unsigned int a;
	my_random((BYTE *)&a, 4);
	return a;
};

bool check_run_as_service() {

	bool rr;
	rr = false;

	//TCHAR filename[MAX_PATH];

	HANDLE h1;
	BOOL Success;
	DWORD current_pid, parent_pid;

	current_pid = GetCurrentProcessId();
	parent_pid = 0;

	PROCESSENTRY32 ProcEntry;
	ProcEntry.dwSize = sizeof(PROCESSENTRY32);

	///////////////////////////////////////////////////////////
	// ��������� Parent PID ��� ������ ��������

	h1 = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (h1 == INVALID_HANDLE_VALUE) return false;


	Success = Process32First(h1, &ProcEntry);
	while (Success == TRUE) {

		if (ProcEntry.th32ProcessID == current_pid) {
			//send_udp(L"FFFFFFFFFFFFFFFF ", ProcEntry.th32ProcessID, ProcEntry.th32ParentProcessID);
			parent_pid = ProcEntry.th32ParentProcessID;

		}

		Success = Process32Next(h1, &ProcEntry);
	}

	CloseHandle(h1);

	if (parent_pid == 0) {
		//send_udp("parent_pid == 0");
		return false;
	};

	/////////////////////////////////////
	// ��������� ��� ����� ��� ������ Parent �

	h1 = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (h1 == INVALID_HANDLE_VALUE) return false;


	Success = Process32First(h1, &ProcEntry);
	while (Success == TRUE) {

		if (ProcEntry.th32ProcessID == parent_pid) {
			//send_udp(L"parent ", ProcEntry.szExeFile);
			wchar_t ss[150];
			int i = 0;
			for (i = 0; i < 150; i++) ss[i] = 0;
			i = 0;
			while (i < 150 && ProcEntry.szExeFile[i] != 0) {
				ss[i] = ProcEntry.szExeFile[i];
				i++;
			}
			_wcslwr_s(ss, 150);
			if (my_strcmp(ss, L"services.exe") == 0) {

				rr = true;
			}
		}

		Success = Process32Next(h1, &ProcEntry);
	}

	CloseHandle(h1);

	if (rr == true) {
		//send_udp("is service");
	}
	else {
		//send_udp("is no service");
	}

	return rr;



}

void format_sz(char *s, unsigned long long sz) {

	s[0] = 0;

	if (sz >= 1000000000L) {
		sprintf__s_ull(s, 490, "%lld Gb", sz / 1000000000L);
	}
	else {
		if (sz >= 1000000L) {
			sprintf__s_ull(s, 490, "%lld Mb", sz / 1000000L);
		}
		else {
			if (sz >= 1000L) {
				sprintf__s_ull(s, 490, "%lld Kb", sz / 1000L);
			}
			else {
				sprintf__s_ull(s, 490, "%lld", sz);
			};
		}
	}

}

void format_sz(wchar_t *s, unsigned long long sz) {

	s[0] = 0;

	if (sz >= 1000000000L) {
		wsprintf(s, L"%I64d GB", sz / 1000000000L);
	}
	else {
		if (sz >= 1000000L) {
			wsprintf(s, L"%I64d MB", sz / 1000000L);
		}
		else {
			if (sz >= 1000L) {
				wsprintf(s, L"%I64d KB", sz / 1000L);
			}
			else {
				wsprintf(s, L"%I64d", sz);
			};
		}
	}

}

int my_big_send(unsigned int socket_, unsigned char *buf, int len, unsigned long long *send_counter) {

	int snd, need_send, send_ok, timeout;
	need_send = len;
	send_ok = 0;
	timeout = 0;
	snd = 0;

	while (need_send > 0 && GLOBAL_STOP == false) {
		snd = my_send(socket_, buf + send_ok, need_send, 0, "", send_counter);
		if (snd < 0) {
			return -1;
		}
		if (snd > 0) {
			timeout = 0;
			send_ok += snd;
			need_send -= snd;
		}
		else {
			Sleep(1);
			timeout++;
			if (timeout > 1000) {
				return -1;
			}
		}
	}
	return send_ok; // WTF?? 2021 // snd;
}

int my_big_recv(unsigned int socket_, unsigned char *buf, int len, unsigned long long *recv_counter) {

	int rcv, need_recv, recv_ok, timeout;
	need_recv = len;
	recv_ok = 0;
	timeout = 0;
	while (need_recv > 0 && GLOBAL_STOP == false) {
		rcv = my_recv(socket_, buf + recv_ok, need_recv, recv_counter);
		if (rcv < 0) {
			return -1;
		}
		if (rcv > 0) {
			timeout = 0;
			recv_ok += rcv;
			need_recv -= rcv;
		}
		else {
			Sleep(1);
			timeout++;
			if (timeout > 10000) {
				return -1;
			}
		}
	}
	return rcv;
}

void hexstr_to_char16_w(wchar_t *hstr, unsigned char *buf) {

	if (hstr == NULL || buf == NULL) return;
	for (int i = 0; i < 16; i++) {

		buf[i] = (get_hexch_w(hstr[i * 2]) << 4) | get_hexch_w(hstr[i * 2 + 1]);
		//sprintf_ s(ss, 290, "[%04X] [%04X] [%04X] ", get_hexch_w(hstr[i * 2]), get_hexch_w(hstr[i * 2 + 1]), buf[i]);
		//send_udp2(ss);
	}
}


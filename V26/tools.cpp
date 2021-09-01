#include "tools.h"
#include "APPLICATION_ATTRIBUTES.h"

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

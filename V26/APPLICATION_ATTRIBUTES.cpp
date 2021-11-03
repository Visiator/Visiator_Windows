#include "stdafx.h"

#include "APPLICATION_ATTRIBUTES.h"


APPLICATION_ATTRIBUTES::APPLICATION_ATTRIBUTES() {
	proxy_list = new PROXY_LIST();
	my_exe_file_name = new wchar_t[2505];
	zero_wchar_t(my_exe_file_name, 2505);
	zero_unsigned_char(startup_parametr_PASS_ENCR, 32);
};

void APPLICATION_ATTRIBUTES::get_all_parametrs(HINSTANCE hInstance_) {
	hInstance = hInstance_;

	global_my_proc_id = GetCurrentProcessId();
	
	
	int bb;
	bb = GetModuleFileName(NULL, my_exe_file_name, 2500);



	im_is_admin = check_admin();
	if (im_is_admin) { //send_udp("is admin"); 
	}
	else { //send_udp("is no admin"); 
	}



	is_32_or_64_bit_system = check_IsWow64(global_my_proc_id);



	is_service = check_run_as_service();



	if (is_service == true) {
		//if (update_exe_service == NULL) {
		//	update_exe_service = neww UPDATE_EXE(); neew( sizeof(UPDATE_EXE), update_exe_service);
		//};
		//update_exe_service->Start_update_service();
	}



	getRealWindowsVersion();



	_SetProcessDPIAware();



	get_programm_version();



	wchar_t pr[5550];// , *q, *qq;
	for (int i = 0; i < 5550; i++) pr[i] = 0;

	GetEnvironmentVariable(L"PR", pr, 5500);
	if (pr[0] != 0) {
		//send_udp2( "PR=", pr);
		set_startup_paramert_pr(pr);
	};
	
	GetEnvironmentVariable(L"RO", pr, 5500);
	if (pr[0] != 0) {
		//send_udp2( "PR=", pr);
		set_startup_paramert_ro(pr);
	};

}

bool APPLICATION_ATTRIBUTES::WINDOWS_is_XP() {
	if (OsMajorVersion <= 5) { // Windows XP
		return true;
	}
	return false;
}


void APPLICATION_ATTRIBUTES::get_programm_version() {
	//UINT vLen, langD;
	//BOOL retVal;

	//LPVOID retbuf = NULL;

	//wchar_t fileEntry[256];

	HRSRC hResInfo;
	DWORD dwSize;
	HGLOBAL hResData;
	LPVOID pRes, pResCopy;
	UINT uLen;
	VS_FIXEDFILEINFO *lpFfi;

	int i1, i2;

	VER = 0;



	HMODULE hInst = GetModuleHandle(NULL);

	if (hInst == NULL) {
		//send_udp("hLib == NULL");
		return;
	}



	hResInfo = FindResource(hInst, MAKEINTRESOURCE(VS_VERSION_INFO), RT_VERSION);
	if (hResInfo == NULL) return;

	dwSize = SizeofResource(hInst, hResInfo);
	hResData = LoadResource(hInst, hResInfo);
	pRes = LockResource(hResData);
	pResCopy = LocalAlloc(LMEM_FIXED, dwSize);
	CopyMemory(pResCopy, pRes, dwSize);
	FreeResource(hResData);

	VerQueryValue(pResCopy, TEXT("\\"), (LPVOID*)&lpFfi, &uLen);


	i1 = (lpFfi->dwFileVersionMS & 0xffff0000) >> 16;
	i2 = (lpFfi->dwFileVersionMS & 0xffff);



	VER = i1 * 1000 + i2;

	LocalFree(pResCopy);

	return;
}

#ifndef DPI_ENUMS_DECLARED
typedef enum PROCESS_DPI_AWARENESS
{
	PROCESS_DPI_UNAWARE = 0,
	PROCESS_SYSTEM_DPI_AWARE = 1,
	PROCESS_PER_MONITOR_DPI_AWARE = 2
} PROCESS_DPI_AWARENESS;
#endif

void APPLICATION_ATTRIBUTES::_SetProcessDPIAware() {

	typedef BOOL(WINAPI * SETPROCESSDPIAWARE_T)(void);
	typedef HRESULT(WINAPI * SETPROCESSDPIAWARENESS_T)(PROCESS_DPI_AWARENESS);

	HMODULE user32 = LoadLibraryA("User32.dll");
	SETPROCESSDPIAWARE_T SetProcessDPIAware_ = NULL;
	if (user32) {
		SetProcessDPIAware_ = (SETPROCESSDPIAWARE_T)GetProcAddress(user32, "SetProcessDPIAware");
		if (SetProcessDPIAware_ != NULL) {
			//send_udp("SetProcessDPIAware FOUND");
			SetProcessDPIAware_();
		}
		else {
			//send_udp("SetProcessDPIAware NOT FOUND");
		}
	}

	if (user32) {
		FreeLibrary(user32);
	}
}

void APPLICATION_ATTRIBUTES::getRealWindowsVersion()
{
	RTL_OSVERSIONINFOEXW *pk_OsVer;
	pk_OsVer = new RTL_OSVERSIONINFOEXW;
	typedef LONG(WINAPI* tRtlGetVersion)(RTL_OSVERSIONINFOEXW*);

	memset(pk_OsVer, 0, sizeof(RTL_OSVERSIONINFOEXW));
	pk_OsVer->dwOSVersionInfoSize = sizeof(RTL_OSVERSIONINFOEXW);

	HMODULE h_NtDll = GetModuleHandleW(L"ntdll.dll");
	tRtlGetVersion f_RtlGetVersion = (tRtlGetVersion)GetProcAddress(h_NtDll, "RtlGetVersion");

	if (!f_RtlGetVersion)
		return; // This will never happen (all processes load ntdll.dll)

	LONG Status = f_RtlGetVersion(pk_OsVer);

	OsMajorVersion = pk_OsVer->dwMajorVersion;
	OsMinorVersion = pk_OsVer->dwMinorVersion;

	//send_udp("WinVer ", OsMajorVersion, OsMinorVersion);

	delete pk_OsVer;
}

void APPLICATION_ATTRIBUTES::set_startup_paramert_pr(wchar_t *p) {
	sudp("set_startup_paramert_pr");
	//send_udp2(p);
	hexstr_to_char16_w(p, startup_parametr_PASS_ENCR);
}
void APPLICATION_ATTRIBUTES::set_service_public_id(unsigned long long val) {

	service_public_id = val;

	char x[500], xx[500];
	generate_ID_to_text( x, service_public_id);
	sprintf_s(xx, 450, "APPLICATION_ATTRIBUTES::set_service_public_id( %s )", x);
	sudp(xx);

	
	if (gui_service_public_id != nullptr) {
		wchar_t ss[500];
		generate_ID_to_text(ss, service_public_id);
		gui_service_public_id->set_text(ss);
	}
}
void APPLICATION_ATTRIBUTES::set_service_private_id(unsigned long long val) {
	service_private_id = val;
}

void APPLICATION_ATTRIBUTES::set_startup_paramert_ro(wchar_t *p) {
	sudp("set_startup_paramert_ro");
	//sudp(p);
	if (p != nullptr && p[0] != 0 && p[0] != '0') {
		startup_parametr_ReadOnly = true;
	};
}

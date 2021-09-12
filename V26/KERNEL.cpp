
#include "KERNEL.h"
#include "tools.h"
#include "Application_Attributes.h"
#include "mem.h"

extern APPLICATION_ATTRIBUTES app_attributes;

//bool GetLogonFromToken(HANDLE hToken, wchar_t *user, wchar_t *domain, USERLOGIN_LIST *userlogin_list);



DWORD KERNEL::scan_explorer_pid_for_active_session(void) {



	wchar_t *pszProcessName;
	DWORD need_sess_id;
	need_sess_id = WTSGetActiveConsoleSessionId();

	/*if (my_FileExists("c:\\1\\s0")) need_sess_id = 0;
	if (my_FileExists("c:\\1\\s1")) need_sess_id = 1;
	if (my_FileExists("c:\\1\\s2")) need_sess_id = 2;
	if (my_FileExists("c:\\1\\s3")) need_sess_id = 3;*/

	NTSTATUS status;

	status = NtQuerySystemInformation(SystemProcessInformation, vv, ll, NULL);
	while (status == STATUS_INFO_LENGTH_MISMATCH)
	{
		delete[] vv;
		ll += 2000;
		vv = new char[ll];
		status = NtQuerySystemInformation(5, vv, ll, NULL);
	};
	pProcesses = (PSYSTEM_PROCESSES)vv;

	for (;;) {
		pszProcessName = pProcesses->ProcessName.Buffer;

		//if (wstrcpy_lower(pszProcessName, L"winlogon.exe") == 0) {
		if (
			my_strcmp(pszProcessName, L"explorer.exe") == 0 //|| my_strcmp(pszProcessName, L"explorer.exe") == 0
			)
		{

			DWORD sess_id;

			sess_id = 11111;
			if (ProcessIdToSessionId(pProcesses->ProcessId, &sess_id) == TRUE)
			{
				if (sess_id != 11111 && sess_id == need_sess_id)
				{
					//send_udp(L"NEED run agent for PID/SESS = ", pProcesses->ProcessId, need_sess_id);
					//send_udp(pszProcessName, pProcesses->ProcessId, sess_id);
					/*FILE *f;
					fopen_s( &f, "c:\\1\\ss.txt", "wb");
					fprintf(f, "-%d-%d-", pProcesses->ProcessId, need_sess_id);
					fclose(f);*/

					return pProcesses->ProcessId;
				};
			}

			//send_udp2("wtF? ", pProcesses->ProcessId);
			//send_udp2("sess_id=", sess_id);
			//send_udp2("need_sess_id=", need_sess_id);
		}

		if (pProcesses->NextEntryDelta == 0)
			break;
		pProcesses = (PSYSTEM_PROCESSES)(((LPBYTE)pProcesses)
			+ pProcesses->NextEntryDelta);
	};
	//send_udp("scan_winlogon_pid_for_active_session() explorer.exe not found!");
	return 0;
}

/*
DWORD KERNEL::scan_explorer_pid_by_user_name(wchar_t *user_name) {

	if (user_name == nullptr) return 0;
	

	wchar_t *pszProcessName;
	DWORD need_sess_id;
	need_sess_id = WTSGetActiveConsoleSessionId();

	NTSTATUS status;

	status = NtQuerySystemInformation(SystemProcessInformation, vv, ll, NULL);
	while (status == STATUS_INFO_LENGTH_MISMATCH)
	{
		delete[] vv;
		ll += 2000;
		vv = new char[ll];
		status = NtQuerySystemInformation(5, vv, ll, NULL);
	};
	pProcesses = (PSYSTEM_PROCESSES)vv;

	wchar_t strUser[1000], strdomain[1000];

	for (;;) {
		pszProcessName = pProcesses->ProcessName.Buffer;

		//if (wstrcpy_lower(pszProcessName, L"winlogon.exe") == 0) {
		if (my_strcmp(pszProcessName, L"explorer.exe") == 0) {

			HANDLE hToken = NULL;
			HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pProcesses->ProcessId);
			if (hProcess != NULL) {
				if (OpenProcessToken(hProcess, TOKEN_QUERY, &hToken))
				{
					strUser[0] = 0;
					strdomain[0] = 0;
					if (GetLogonFromToken(hToken, strUser, strdomain, nullptr) == true) {
						if (my_strcmp(strUser, user_name) == 0) {
							CloseHandle(hToken);
							CloseHandle(hProcess);
							return pProcesses->ProcessId;
						}
					}
					CloseHandle(hToken);

				};
				CloseHandle(hProcess);
			}

			
		}

		if (pProcesses->NextEntryDelta == 0)
			break;
		pProcesses = (PSYSTEM_PROCESSES)(((LPBYTE)pProcesses)
			+ pProcesses->NextEntryDelta);
	};
	
	return 0;

};
*/

DWORD KERNEL::scan_winlogon_pid_for_active_session(void) {



	wchar_t *pszProcessName;
	DWORD need_sess_id;
	need_sess_id = WTSGetActiveConsoleSessionId();

	/*if (my_FileExists("c:\\1\\s0")) need_sess_id = 0;
	if (my_FileExists("c:\\1\\s1")) need_sess_id = 1;
	if (my_FileExists("c:\\1\\s2")) need_sess_id = 2;
	if (my_FileExists("c:\\1\\s3")) need_sess_id = 3;*/

	NTSTATUS status;

	status = NtQuerySystemInformation(SystemProcessInformation, vv, ll, NULL);
	while (status == STATUS_INFO_LENGTH_MISMATCH)
	{
		delete[] vv;
		ll += 2000;
		vv = new char[ll];
		status = NtQuerySystemInformation(5, vv, ll, NULL);
	};
	pProcesses = (PSYSTEM_PROCESSES)vv;

	for (;;) {
		pszProcessName = pProcesses->ProcessName.Buffer;

		//if (wstrcpy_lower(pszProcessName, L"winlogon.exe") == 0) {
		if (
			my_strcmp(pszProcessName, L"winlogon.exe") == 0 //|| my_strcmp(pszProcessName, L"explorer.exe") == 0
			) 
		{

			DWORD sess_id;

			sess_id = 11111;
			if (ProcessIdToSessionId(pProcesses->ProcessId, &sess_id) == TRUE) 
			{
				if (sess_id != 11111 && sess_id == need_sess_id) 
				{
					//send_udp(L"NEED run agent for PID/SESS = ", pProcesses->ProcessId, need_sess_id);
					//send_udp(pszProcessName, pProcesses->ProcessId, sess_id);
					/*FILE *f;
					fopen_s( &f, "c:\\1\\ss.txt", "wb");
					fprintf(f, "-%d-%d-", pProcesses->ProcessId, need_sess_id);
					fclose(f);*/

					return pProcesses->ProcessId;
				};
			}
		}

		if (pProcesses->NextEntryDelta == 0)
			break;
		pProcesses = (PSYSTEM_PROCESSES)(((LPBYTE)pProcesses)
			+ pProcesses->NextEntryDelta);
	};
	send_udp("scan_winlogon_pid_for_active_session() winlogon not found!");
	return 0;
}

void KERNEL::KILL_ALL_AGENTS(void) {

	wchar_t *pszProcessName;
	DWORD need_sess_id;
	need_sess_id = WTSGetActiveConsoleSessionId();

	NTSTATUS status;

	status = NtQuerySystemInformation(SystemProcessInformation, vv, ll, NULL);
	while (status == STATUS_INFO_LENGTH_MISMATCH)
	{
		delete[] vv;
		ll += 2000;
		vv = new char[ll];
		status = NtQuerySystemInformation(5, vv, ll, NULL);
	};
	pProcesses = (PSYSTEM_PROCESSES)vv;

	for (;;) {
		pszProcessName = pProcesses->ProcessName.Buffer;

		if (my_strcmp(pszProcessName, L"visiator.exe") == 0 ||
			my_strcmp(pszProcessName, L"v16.exe") == 0) {

			if (pProcesses->ProcessId != app_attributes.global_my_proc_id) {
				kill_process_by_pid(pProcesses->ProcessId);
			}

		}

		if (pProcesses->NextEntryDelta == 0)
			break;
		pProcesses = (PSYSTEM_PROCESSES)(((LPBYTE)pProcesses)
			+ pProcesses->NextEntryDelta);
	};
	return;

}

bool KERNEL::kill_process_by_pid(DWORD pid) {
	HANDLE h = NULL;
	//send__udp("kill_process_by_pid() ", pid);

	h = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
	if (h == NULL) { return true; }; // процесса уже нет
	CloseHandle(h);

	h = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
	if (h == NULL) {
		//send_udp("OpenProcess err "); 
		return false;
	}; // не удалось убить процесс
	try
	{
		//send_udp("terminate...");
		TerminateProcess(h, 0);
	}
	catch (...) {};
	CloseHandle(h);

	h = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
	if (h == NULL) { return true; }; // процесса уже нет
	CloseHandle(h);
	//send_udp("fail");
	return false; // не удалось убить процесс
}

DWORD KERNEL::my_spawnl(DWORD pid, wchar_t *cmd, wchar_t *p1, wchar_t *p2, wchar_t *p3) {
	wchar_t paramerts[1500];
	my_strcpy(paramerts, p1);
	my_strcat(paramerts, 1450, L" \"");
	my_strcat(paramerts, 1450, p2);
	my_strcat(paramerts, 1450, L"\" \"");
	my_strcat(paramerts, 1450, p3);
	my_strcat(paramerts, 1450, L"\"");
	return my_spawnl(pid, cmd, paramerts);
}

DWORD KERNEL::my_spawnl_2(DWORD pid, wchar_t *cmd, wchar_t *paramerts) {
	if (pid == 0 || cmd == NULL || paramerts == NULL || cmd[0] == 0) return 0;

	//send_udp("my_spawnl_2 ", pid);

	return 0;
};

DWORD KERNEL::my_spawnl(DWORD pid, wchar_t *cmd, wchar_t *paramerts) {
	if (pid == 0 || cmd == NULL || paramerts == NULL || cmd[0] == 0) return 0;

	wchar_t prm[1500];
	my_strcpy(prm, cmd);
	my_strcat(prm, 1450, L" ");
	my_strcat(prm, 1450, paramerts);

	DWORD pp;
	BOOL rr;
	HANDLE hh = NULL;
	PROCESS_INFORMATION	proc_info;
	STARTUPINFO startup_info;
	ZeroMemory(&startup_info, sizeof(STARTUPINFO));
	ZeroMemory(&proc_info, sizeof(PROCESS_INFORMATION));


	send_udp("OpenProcess...", pid);
	hh = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (hh == NULL) {
		send_udp("OpenProcess err "); 
		return 0;
	};

	HANDLE       hToken = NULL;
	send_udp("OpenProcessToken...");
	if (OpenProcessToken(hh, TOKEN_ALL_ACCESS, &hToken) == FALSE) {
		send_udp("OpenProcessToken err");
		return 0;
	}
	if (hToken == NULL) {
		send_udp("hToken == NULL");
		return 0;
	}

	startup_info.cb = sizeof(STARTUPINFO);
	startup_info.wShowWindow = SW_SHOW;
	startup_info.dwFlags = STARTF_USESTDHANDLES;
	if (my_strcmp(paramerts, L"indicator") == 0) {
		startup_info.lpDesktop = L"WinSta0\\Default";
	}
	else {
		//startup_info.lpDesktop = L"WinSta0\\Winlogon";
		startup_info.lpDesktop = L"WinSta0\\Default";
	};
	//send_udp(L"CreateProcessAsUser... {", prm, L"}");



	rr = CreateProcessAsUser(
		hToken,
		//L"c:\\prj\\V12\\Debug\\V12.exe",
		cmd,
		//L"c:\\prj\\V12\\Debug\\V12.exe mode:agent",
		prm,
		NULL,
		NULL,
		FALSE,
		NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE,
		NULL,
		NULL,
		&startup_info,
		&proc_info);
	if (rr == FALSE) {
		send_udp("CreateProcessAsUser err");

		return 0;
	}
	pp = proc_info.dwProcessId;

	CloseHandle(proc_info.hThread);
	CloseHandle(proc_info.hProcess);

	CloseHandle(hToken);

	send_udp( "CreateProcessAsUser OK ", pp);

	return pp;
}



HMODULE sasLib = 0;


void KERNEL::CAD(void) {
	// RpcRT4.lib
	// XP PostMessage (HWND_BROADCAST, WM_HOTKEY, 0, MAKELONG (MOD_ALT | MOD_CONTROL, VK_DELETE));

	if (app_attributes.OsMajorVersion <= 5) {
		//send_udp("CAD XP version");
		PostMessage(HWND_BROADCAST, WM_HOTKEY, 0, MAKELONG(MOD_ALT | MOD_CONTROL, VK_DELETE));
		CAD_XP();
		return;
	}

	//send_udp("CAD WIN7 version");
	/*

	HDESK desktop = OpenDesktopW(L"Winlogon", 0, TRUE,
	DESKTOP_CREATEMENU | DESKTOP_CREATEWINDOW | DESKTOP_ENUMERATE |
	DESKTOP_HOOKCONTROL | DESKTOP_WRITEOBJECTS | DESKTOP_READOBJECTS |
	DESKTOP_SWITCHDESKTOP | GENERIC_WRITE);
int result = SetThreadDesktop(desktop);
if (result)
{
	HMODULE sasdll = LoadLibraryA("sas.dll");
	if (sasdll)
	{
		typedef void(__stdcall * SendSAS_t)(BOOL);
		SendSAS_t sendSAS = (SendSAS_t)GetProcAddress(sasdll, "SendSAS");
		if (sendSAS)
			sendSAS(FALSE);
	}
}
CloseDesktop(desktop);

	*/

	/*

	wmsgapi.dll
	typedef DWORD (WINAPI* lpfnWmsgSendMessage)(DWORD dwSessionId, UINT uMsg, WPARAM wParam, LPARAM lParam);

	*/

	//send_udp(L"KERNEL::CAD()");

	/*if (sendSas == NULL) {
		HMODULE sasLib = LoadLibrary(L"sas.dll");
		sendSas = (SendSas)GetProcAddress(sasLib, "SendSAS");
		if (!sendSas) {
			//send_udp("GetProcAddress(SendSAS) failed");
			return;
		}
	}*/

	if (sasLib == 0) {
		//send_udp(L"load dll sas.dll");
		sasLib = LoadLibrary(L"sas.dll");
		sendSas = (SendSas)GetProcAddress(sasLib, "SendSAS");
		if (!sendSas) {
			//send_udp( "GetProcAddress(SendSAS) failed");
			return;
		}
	};

	HKEY hKey;
	LSTATUS nn;
	////////////////////////////////////////////////////////////////////////////////////////
	// Если ключа нет - надо создать
	hKey = 0; // READ_CONTROL OWNER_SECURITY_INFORMATION
	nn = RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", 0, KEY_READ | KEY_WRITE | 0x0100, &hKey);

	//send__udp("nn==", nn);
	if (nn == 0) {}; // ключ есть, и успешно открыт

	DWORD size;
	DWORD type;
	unsigned int data, q;
	data = 0;

	size = 4;
	type = 0;

	if (RegQueryValueEx(hKey, L"SoftwareSASGeneration", 0, &type, (BYTE *)&data, &size) != 0) {
		//send_udp("read error");
	}
	else {

		//send__udp("read ok ", data);
	}
	if (data != 3) {
		//send_udp("WWWW");
		q = 3;
		RegSetValueEx(hKey, L"SoftwareSASGeneration", 0, REG_DWORD, (BYTE *)&q, 4);
		//RegCloseKey(hkey);

	}
	else {
		//send__udp("rrrr ", data);
	}

	RegCloseKey(hKey);

	// SoftwareSASGeneration
	//send_udp("CAD()");


	sendSas(FALSE);

	/*

	HANDLE hTokenNew = NULL, hTokenDup = NULL,hToken=NULL;
 STARTUPINFO si;
 PROCESS_INFORMATION pi;
 LPFNCREATEENVIRONMENTBLOCK  lpfnCreateEnvironmentBlock  = NULL;
 LPFNDESTROYENVIRONMENTBLOCK lpfnDestroyEnvironmentBlock = NULL;
 HMODULE  hmod = LoadLibrary("kernel32.dll");
 WTSGETACTIVECONSOLESESSIONID lpfnWTSGetActiveConsoleSessionId = (WTSGETACTIVECONSOLESESSIONID)GetProcAddress(hmod,"WTSGetActiveConsoleSessionId");

 DWORD dwSessionId = lpfnWTSGetActiveConsoleSessionId();
 BOOL bret = WTSQueryUserToken(dwSessionId,&hToken);
 if (bret == false)
{
	printf("ERROR: %d", GetLastError());
}

 DuplicateTokenEx(hToken,MAXIMUM_ALLOWED,NULL,SecurityIdentification,TokenPrimary,&hTokenDup);
 ZeroMemory( &si, sizeof( STARTUPINFO ) );
 si.cb = sizeof( STARTUPINFO );
 si.lpDesktop = "winsta0\\default";
 LPVOID  pEnv = NULL;
 DWORD dwCreationFlag = NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE;
 HMODULE hModule = LoadLibrary("Userenv.dll");
 if(hModule )
 {
  LPFNCREATEENVIRONMENTBLOCK lpfnCreateEnvironmentBlock = (LPFNCREATEENVIRONMENTBLOCK)GetProcAddress( hModule, "CreateEnvironmentBlock" );
  if( lpfnCreateEnvironmentBlock != NULL )
  {
   if(lpfnCreateEnvironmentBlock(&pEnv, hTokenDup, FALSE))
   {
	//WriteToLog("CreateEnvironmentBlock Ok");
	dwCreationFlag |= CREATE_UNICODE_ENVIRONMENT;
   }
   else
   {
	pEnv = NULL;
   }
  }
 }
  //
 ZeroMemory( &pi,sizeof(pi));


if(!CreateProcessAsUser( hTokenDup, NULL, _T("c:\\ProgramFiles\\test.exe"),
						 NULL, NULL, FALSE, dwCreationFlag,
						 pEnv, NULL, &si, &pi ))

 {
	if ( NULL != lpfnDestroyEnvironmentBlock )
		lpfnDestroyEnvironmentBlock(pEnv);

	if (NULL != hModule)
FreeLibrary(hModule);
	CloseHandle(hTokenDup);
 }

	*/

	/*
	LPARAM lParam = 0;
	if (WmsgSendMessage != NULL)
		DWORD dwRet = WmsgSendMessage(WTSGetActiveConsoleSessionId(), 0x208, 0, (LPARAM)&lParam);
	*/
	/*

	HWND hwndCtrlAltDel = FindWindowW(L"SAS window class",
		L"SAS window");
	if (hwndCtrlAltDel == NULL)
	{
		send_udp(L"not");
		hwndCtrlAltDel = HWND_BROADCAST;
	}
	else {
		send_udp(L"yes");
	}
	*/
}

BOOL KERNEL::CAD_XP(void)
{
	HDESK hdeskCurrent;
	HDESK hdesk;
	HWINSTA hwinstaCurrent;
	HWINSTA hwinsta;

	// 
	// Save the current Window station 
	//

	//send_udp("CAD_XP()");

	hwinstaCurrent = GetProcessWindowStation();
	if (hwinstaCurrent == NULL) {
		//send_udp("err 1");
		return FALSE;
	};
	// 
	// Save the current desktop 
	// 
	hdeskCurrent = GetThreadDesktop(GetCurrentThreadId());
	if (hdeskCurrent == NULL) {
		//send_udp("err 2");
		return FALSE;
	};
	// 
	// Obtain a handle to WinSta0 - service must be running 
	// in the LocalSystem account 
	// 
	hwinsta = OpenWindowStation(L"winsta0", FALSE,
		WINSTA_ACCESSCLIPBOARD |
		WINSTA_ACCESSGLOBALATOMS |
		WINSTA_CREATEDESKTOP |
		WINSTA_ENUMDESKTOPS |
		WINSTA_ENUMERATE |
		WINSTA_EXITWINDOWS |
		WINSTA_READATTRIBUTES |
		WINSTA_READSCREEN |
		WINSTA_WRITEATTRIBUTES);
	if (hwinsta == NULL) {
		//send_udp("err 3");
		return FALSE;
	};
	// 
	// Set the windowstation to be winsta0 
	// 

	if (!SetProcessWindowStation(hwinsta)) {
		//send_udp("err 4");
		return FALSE;
	};

	// 
	// Get the default desktop on winsta0 
	// 
	hdesk = OpenDesktop(L"Winlogon", 0, FALSE,
		DESKTOP_CREATEMENU |
		DESKTOP_CREATEWINDOW |
		DESKTOP_ENUMERATE |
		DESKTOP_HOOKCONTROL |
		DESKTOP_JOURNALPLAYBACK |
		DESKTOP_JOURNALRECORD |
		DESKTOP_READOBJECTS |
		DESKTOP_SWITCHDESKTOP |
		DESKTOP_WRITEOBJECTS);
	if (hdesk == NULL) {
		//send_udp("err 5");
		return FALSE;
	};

	// 
	// Set the desktop to be "default" 
	// 
	if (!SetThreadDesktop(hdesk)) {
		//send_udp("err 6");
		return FALSE;
	};

	PostMessage(HWND_BROADCAST, WM_HOTKEY, 0, MAKELPARAM(MOD_ALT | MOD_CONTROL, VK_DELETE));


	// 
	// Reset the Window station and desktop 
	// 
	if (!SetProcessWindowStation(hwinstaCurrent)) {
		//send_udp("err 7");
		return FALSE;
	};

	if (!SetThreadDesktop(hdeskCurrent)) {
		//send_udp("err 8");
		return FALSE;
	};

	// 
	// Close the windowstation and desktop handles 
	// 
	if (!CloseWindowStation(hwinsta)) {
		//send_udp("err 9");
		return FALSE;
	};
	if (!CloseDesktop(hdesk)) {
		//send_udp("err 10");
		return FALSE;
	};
	//send_udp("OK");
	return TRUE;
}

KERNEL::~KERNEL() {
	delete[] vv;
}

KERNEL::KERNEL(void) {
	ll = 290000;
	vv = new char[ll];


	HINSTANCE h_wmsgapi = LoadLibraryW(L"wmsgapi.dll");
	WmsgSendMessage = (_lpfnWmsgSendMessage)GetProcAddress(h_wmsgapi, "WmsgSendMessage");

	HMODULE h_ntdll;
	h_ntdll = GetModuleHandle(L"ntdll");
	NtQuerySystemInformation = (_NtQuerySystemInformation)GetProcAddress(h_ntdll, "NtQuerySystemInformation");

	NtQueryInformationProcess = (_NtQueryInformationProcess)GetProcAddress(h_ntdll, "NtQueryInformationProcess");

	sendSas = NULL;
}

void my_dateSYS_diff(unsigned long long t1_h, unsigned long long t1_low, unsigned long long t2_h, unsigned long long t2_low, unsigned long long *result1, unsigned long long *result2)
{
	*result1 = 0;
	*result2 = 0;

	unsigned long long tt1, tt2;
	tt1 = t1_h << 32 | t1_low;
	tt2 = t2_h << 32 | t2_low;

	if (tt1 > tt2) { *result1 = tt1 - tt2; }
	else { *result2 = tt2 - tt1; }

};


DWORD KERNEL::get_conhost_exe_by_parent_pid(DWORD parent_pid) {

	NTSTATUS status;

	status = NtQuerySystemInformation(SystemProcessInformation, vv, ll, NULL);
	while (status == STATUS_INFO_LENGTH_MISMATCH)
	{
		delete[] vv;
		ll += 2000;
		vv = new char[ll];
		status = NtQuerySystemInformation(5, vv, ll, NULL);
	};
	pProcesses = (PSYSTEM_PROCESSES)vv;
	wchar_t *pszProcessName;

	for (;;) {
		pszProcessName = pProcesses->ProcessName.Buffer;

		pszProcessName = pProcesses->ProcessName.Buffer;
		if (my_strcmp(pszProcessName, L"conhost.exe") == 0) {

			if (pProcesses->InheritedFromProcessId == parent_pid) {
				return pProcesses->ProcessId;
			};

		}
		//}
		if (pProcesses->NextEntryDelta == 0) break;
		pProcesses = (PSYSTEM_PROCESSES)(((LPBYTE)pProcesses) + pProcesses->NextEntryDelta);
	};

	return 0;
};

DWORD KERNEL::get_conhost_exe_by_time_create(DWORD parent_pid) {//, DWORD *keyb, HWND hw) {

	HANDLE h1;
	BOOL rr;
	FILETIME lpCreationTime1, lpExitTime1, lpKernelTime1, lpUserTime1;

	// определим время создания процесса parent_pid
	h1 = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, parent_pid);
	rr = GetProcessTimes(h1, &lpCreationTime1, &lpExitTime1, &lpKernelTime1, &lpUserTime1);
	CloseHandle(h1);

	///////////////////////////////////////////////////////////////////////////////////////

	//DWORD k0, k1, k2;

	//*keyb = 0;
	wchar_t *pszProcessName;

	NTSTATUS status;

	status = NtQuerySystemInformation(SystemProcessInformation, vv, ll, NULL);
	while (status == STATUS_INFO_LENGTH_MISMATCH)
	{
		delete[] vv;
		ll += 2000;
		vv = new char[ll];
		status = NtQuerySystemInformation(5, vv, ll, NULL);
	};
	pProcesses = (PSYSTEM_PROCESSES)vv;


	//DWORD conhost_pid;
	unsigned long long old_r2 = 0xffffffffffffffff;
	DWORD it_is = 0, it_k = 0;

	for (;;) {
		pszProcessName = pProcesses->ProcessName.Buffer;



		//if (pProcesses->InheritedFromProcessId == parent_pid) {
		pszProcessName = pProcesses->ProcessName.Buffer;
		if (my_strcmp(pszProcessName, L"conhost.exe") == 0) {

			unsigned long long d1, d2, rr;

			unsigned long long r1, r2; // date_diff, 
			my_dateSYS_diff(lpCreationTime1.dwHighDateTime, lpCreationTime1.dwLowDateTime, pProcesses->CreateTime.HighPart, pProcesses->CreateTime.LowPart, &r1, &r2);

			d1 = lpCreationTime1.dwHighDateTime << 32 | lpCreationTime1.dwLowDateTime;
			d2 = pProcesses->CreateTime.HighPart << 32 | pProcesses->CreateTime.LowPart;

			//char sss[300];


			if (d1 <= d2) {
				rr = d2 - d1;
				//sprintf_ s(sss, 250, " %d --------M------- %llu ", pProcesses->ProcessId, d2 - d1);
				if (old_r2 > rr) {
					old_r2 = rr;
					it_is = pProcesses->ProcessId;

				}
			}
			else {
				//sprintf_ s(sss, 250, " %d --------B------- %llu ", pProcesses->ProcessId, d1 - d2);
			}

			//send_udp(sss);


			/*if (pProcesses->ThreadCount >= 1) { k0 = (DWORD)GetKeyboardLayout(pProcesses->Threads[0].ClientId.UniqueThread); };
			if (pProcesses->ThreadCount >= 2) { k1 = (DWORD)GetKeyboardLayout(pProcesses->Threads[1].ClientId.UniqueThread); };
			if (pProcesses->ThreadCount >= 3) { k2 = (DWORD)GetKeyboardLayout(pProcesses->Threads[2].ClientId.UniqueThread); };

			if (r2 != 0 && old_r2 > r2) {

				//send_udp((wchar_t *)pProcesses->ProcessName.Buffer);
				//send_udp("++", (int)pProcesses->ProcessId );

				old_r2 = r2;

				it_k = k1;
			};*/
		}
		//}
		if (pProcesses->NextEntryDelta == 0) break;
		pProcesses = (PSYSTEM_PROCESSES)(((LPBYTE)pProcesses) + pProcesses->NextEntryDelta);
	};

	//*keyb = it_k;

	//send_udp("it_is = ", it_is, parent_pid);

	return it_is;

}

/*
bool GetLogonFromToken(HANDLE hToken, wchar_t *user, wchar_t *domain, USERLOGIN_LIST *userlogin_list) {

	for (int i = 0; i < 1000; i++) { user[i] = 0; domain[i] = 0; }

	PTOKEN_USER ptu = NULL;
	DWORD dwLength = 0;

	if (!GetTokenInformation(
		hToken,         // handle to the access token
		TokenUser,    // get information about the token's groups 
		(LPVOID)ptu,   // pointer to PTOKEN_USER buffer
		0,              // size of buffer
		&dwLength)) {
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
			ptu = (PTOKEN_USER)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwLength);
		}

		if (!GetTokenInformation(
			hToken,         // handle to the access token
			TokenUser,    // get information about the token's groups 
			(LPVOID)ptu,   // pointer to PTOKEN_USER buffer
			dwLength,       // size of buffer
			&dwLength       // receives required buffer size
		)) {
			if (ptu != NULL) HeapFree(GetProcessHeap(), 0, (LPVOID)ptu);
		}

		SID_NAME_USE SidType;

		DWORD name_size, domain_size;



		name_size = 1000;
		domain_size = 1000;

		if (!LookupAccountSid(NULL, ptu->User.Sid, user, &name_size, domain, &domain_size, &SidType))
		{
			DWORD dwResult = GetLastError();
			if (dwResult == ERROR_NONE_MAPPED) {
				//send_udp(user, L"NONE_MAPPED");
			}
			else
			{
				//send_udp("LookupAccountSid Error ", dwResult);
			}
		}
		else
		{
			if(userlogin_list != nullptr) userlogin_list->add(user, domain);

		}

		if (ptu != NULL) HeapFree(GetProcessHeap(), 0, (LPVOID)ptu);
	}
	return true;
}
*/

/*
void KERNEL::scan_user_list(USERLOGIN_LIST *userlogin_list) {

	wchar_t *pszProcessName;
	DWORD need_sess_id;
	need_sess_id = WTSGetActiveConsoleSessionId();

	NTSTATUS status;

	status = NtQuerySystemInformation(SystemProcessInformation, vv, ll, NULL);
	while (status == STATUS_INFO_LENGTH_MISMATCH)
	{
		delete_(&vv);
		ll += 2000;
		vv = new_char(ll, 366);
		status = NtQuerySystemInformation(5, vv, ll, NULL);
	};
	pProcesses = (PSYSTEM_PROCESSES)vv;

	wchar_t strUser[1000], strdomain[1000];

	for (;;) {
		pszProcessName = pProcesses->ProcessName.Buffer;

		//if (wstrcpy_lower(pszProcessName, L"winlogon.exe") == 0) {
		if (my_strcmp(pszProcessName, L"explorer.exe") == 0) {

			HANDLE hToken = NULL;
			HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pProcesses->ProcessId);
			if (hProcess != NULL) {
				if (OpenProcessToken(hProcess, TOKEN_QUERY, &hToken))
				{
					if (GetLogonFromToken(hToken, strUser, strdomain, userlogin_list) == true) {

					}
					CloseHandle(hToken);

				};
				CloseHandle(hProcess);
			}

			
		}

		if (pProcesses->NextEntryDelta == 0)
			break;
		pProcesses = (PSYSTEM_PROCESSES)(((LPBYTE)pProcesses)
			+ pProcesses->NextEntryDelta);
	};
	return;
}
*/
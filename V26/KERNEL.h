#pragma once

//#pragma comment(lib, "ws2_32.lib")
//#pragma comment(lib, "Wtsapi32.lib")




#define WIN32_LEAN_AND_MEAN
#define WIN32_WINNT 0x0501

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>

#include <commctrl.h>
#include <tlhelp32.h>
#include <wtsapi32.h>
//#include <Winternl.h>



#include "tools.h"


typedef enum _SYSTEM_INFORMATION_CLASS {
	SystemBasicInformation = 0,
	SystemPerformanceInformation = 2,
	SystemTimeOfDayInformation = 3,
	SystemProcessInformation = 5,
	SystemProcessorPerformanceInformation = 8,
	SystemInterruptInformation = 23,
	SystemExceptionInformation = 33,
	SystemRegistryQuotaInformation = 37,
	SystemLookasideInformation = 45
} SYSTEM_INFORMATION_CLASS;

typedef LONG NTSTATUS;

typedef LONG KPRIORITY;

//#define NT_SUCCESS(Status) ((NTSTATUS) (Status) >= 0)

#define STATUS_INFO_LENGTH_MISMATCH ((NTSTATUS) 0xC0000004L)

#define SystemProcessesAndThreadsInformation 5

typedef struct _CLIENT_ID {
	DWORD         UniqueProcess;
	DWORD         UniqueThread;
} CLIENT_ID;
typedef struct _VM_COUNTERS {
	SIZE_T        PeakVirtualSize;
	SIZE_T        VirtualSize;
	ULONG         PageFaultCount;
	SIZE_T        PeakWorkingSetSize;
	SIZE_T        WorkingSetSize;
	SIZE_T        QuotaPeakPagedPoolUsage;
	SIZE_T        QuotaPagedPoolUsage;
	SIZE_T        QuotaPeakNonPagedPoolUsage;
	SIZE_T        QuotaNonPagedPoolUsage;
	SIZE_T        PagefileUsage;
	SIZE_T        PeakPagefileUsage;
} VM_COUNTERS;
typedef struct _SYSTEM_THREADS {
	LARGE_INTEGER KernelTime;
	LARGE_INTEGER UserTime;
	LARGE_INTEGER CreateTime;
	ULONG         WaitTime;
	PVOID         StartAddress;
	CLIENT_ID     ClientId;
	KPRIORITY     Priority;
	KPRIORITY     BasePriority;
	ULONG         ContextSwitchCount;
	LONG          State;
	LONG          WaitReason;
} SYSTEM_THREADS, *PSYSTEM_THREADS;

typedef struct _UNICODE_STRING {
	USHORT Length;
	USHORT MaximumLength;
	PWSTR  Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

typedef struct _SYSTEM_PROCESSES {
	ULONG             NextEntryDelta;
	ULONG             ThreadCount;
	ULONG             Reserved1[6];
	LARGE_INTEGER     CreateTime;
	LARGE_INTEGER     UserTime;
	LARGE_INTEGER     KernelTime;
	UNICODE_STRING    ProcessName;
	KPRIORITY         BasePriority;
	ULONG             ProcessId;
	ULONG             InheritedFromProcessId;
	ULONG             HandleCount;
	ULONG             Reserved2[2];
	VM_COUNTERS       VmCounters;
#if _WIN32_WINNT >= 0x500
	IO_COUNTERS       IoCounters;
#endif
	SYSTEM_THREADS    Threads[1];
} SYSTEM_PROCESSES, *PSYSTEM_PROCESSES;

typedef NTSTATUS(NTAPI *_NtQuerySystemInformation)(ULONG SystemInformationClass, PVOID SystemInformation, ULONG SystemInformationLength, PULONG ReturnLength);
typedef NTSTATUS(NTAPI *_NtQueryInformationProcess)(HANDLE ProcessHandle, DWORD ProcessInformationClass, PVOID ProcessInformation, DWORD ProcessInformationLength, PDWORD ReturnLength);

typedef DWORD(WINAPI* _lpfnWmsgSendMessage)(DWORD dwSessionId, UINT uMsg, WPARAM wParam, LPARAM lParam);
typedef VOID(WINAPI *SendSas)(BOOL asUser);

class KERNEL {
private:
	_NtQuerySystemInformation NtQuerySystemInformation;
	_NtQueryInformationProcess NtQueryInformationProcess;
	char *vv;
	unsigned long ll;
	PSYSTEM_PROCESSES pProcesses;
	_lpfnWmsgSendMessage WmsgSendMessage;
	SendSas sendSas;

public:
	void  KILL_ALL_AGENTS(void);
	DWORD my_spawnl(DWORD pid, wchar_t *cmd, wchar_t *parametrs);
	DWORD my_spawnl_2(DWORD pid, wchar_t *cmd, wchar_t *parametrs);

	DWORD my_spawnl(DWORD pid, wchar_t *cmd, wchar_t *p1, wchar_t *p2, wchar_t *p3);
	bool kill_process_by_pid(DWORD pid);
	void  CAD(void);
	BOOL  CAD_XP(void);

	DWORD scan_explorer_pid_by_user_name(wchar_t *user_name);

	DWORD scan_explorer_pid_for_active_session(void);
	DWORD scan_winlogon_pid_for_active_session(void);
	DWORD get_conhost_exe_by_parent_pid(DWORD parent_pid);
	DWORD get_conhost_exe_by_time_create(DWORD parent_pid);// , DWORD *keyb, HWND hw);
	//void scan_user_list(USERLOGIN_LIST *userlogin_list);
	KERNEL(void);
	~KERNEL();
};

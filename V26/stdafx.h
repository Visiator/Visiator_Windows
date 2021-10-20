// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS

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
#include <io.h>
#include <fcntl.h>
#include <conio.h>
#include <signal.h>
#include <commctrl.h>
#include <tlhelp32.h>
#include <wtsapi32.h>
#include <ShellAPI.h>



# pragma hdrstop



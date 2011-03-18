#ifndef U_PROCESS_MAN_H
#define U_PROCESS_MAN_H

#include "ulib.h"

ULIB_API BOOL isProgramUp(const TCHAR *name);

ULIB_API BOOL startProgram(const TCHAR *cmdline, const TCHAR *wdir, BOOL bWait = FALSE);

ULIB_API BOOL stopProgram(const TCHAR *name);

ULIB_API VOID printProcessName(DWORD processID);

ULIB_API DWORD getWindowPid(HWND hWnd, LPDWORD lpdwProcessId);

ULIB_API DWORD terminatePid(DWORD dwPID, DWORD dwTimeout);

ULIB_API HWND findWindowByPid(DWORD pid);

ULIB_API BOOL EnableDebugPrivilege();

ULIB_API BOOL KillProcess(DWORD pid, BOOL bForced = FALSE);

#endif // U_PROCESS_MAN_H


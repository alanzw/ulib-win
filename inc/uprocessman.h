#ifndef U_PROCESS_MAN_H
#define U_PROCESS_MAN_H

#include "ulib.h"

ULIB_API BOOL isProgramUp(const TCHAR *name);

ULIB_API BOOL startProgram(const TCHAR *cmdline, const TCHAR *wdir, BOOL bWait = FALSE);

ULIB_API BOOL stopProgram(const TCHAR *name);

ULIB_API VOID printProcessName(DWORD processID);

ULIB_API DWORD getWindowPid(HWND hWnd, LPDWORD lpdwProcessId);

#endif // U_PROCESS_MAN_H


#ifndef MSW_HW_APP_H
#define MSW_HW_APP_H

#include <windows.h>
#include <tchar.h>

HW_API int hwWinMain(HINSTANCE hInstance, LPTSTR lpCmdLine, int nCmdShow);

HW_API LPTSTR * hwCommandLineToArgv(LPCTSTR lpCmdLine, int *lpArgc);

#endif // MSW_HW_APP_H

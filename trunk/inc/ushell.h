#ifndef U_SHELL_H
#define U_SHELL_H

#include "ulib.h"

namespace UShell
{

#if (WINVER >= 0x0600)

ULIB_API HRESULT SimpleInvoke(HWND hwnd);

#endif // (WINVER >= 0x0600)


ULIB_API int execute( HWND hwnd,
                      LPCTSTR lpFile,
                      LPCTSTR lpParameters = NULL,
                      LPCTSTR lpDirectory = NULL,
                      INT nShowCmd = SW_SHOWNORMAL
                 );

ULIB_API BOOL executeEx( HWND hwnd,
                         LPCTSTR lpFile,
                         LPCTSTR lpParameters = NULL,
                         LPCTSTR lpDirectory = NULL,
                         INT nShowCmd = SW_SHOWNORMAL
                       );

ULIB_API int msgbox(HWND hWnd, LPCTSTR pszMsg, LPCTSTR pszTitle, UINT fuStyle = MB_OK);

}; // namespace UShell


#endif // U_SHELL_H


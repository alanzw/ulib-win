#ifdef   WINVER
#undef   WINVER
#endif

#ifdef   _WIN32_IE
#undef   _WIN32_IE
#endif

#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif

#define   WINVER       0x0501
#define   _WIN32_IE    0x0501
#define   _WIN32_WINNT 0x0501

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>
#include <stdio.h>

#include "umsg.h"

//extern HINSTANCE g_hInst;

// Message
void showMsg(const TCHAR *szMsg, const TCHAR *szTitle/*=_T("Info")*/, HWND hOwner/*=NULL*/)
{
    ::MessageBox(hOwner, szMsg, szTitle, MB_OK|MB_ICONINFORMATION);
}

//
int showMsgFormat(const TCHAR * szCaption, const TCHAR * szFormat, ...)
{
    TCHAR   szBuffer [1024] ;
    va_list pArgList ;

    // The va_start macro (defined in STDARG.H) is usually equivalent to:
    // pArgList = (char *) &szFormat + sizeof (szFormat) ;

    va_start (pArgList, szFormat) ;

    // The last argument to wvsprintf points to the arguments

    _vsntprintf (szBuffer, sizeof (szBuffer) / sizeof (TCHAR),
        szFormat, pArgList) ;

    // The va_end macro just zeroes out pArgList for no good reason

    va_end (pArgList) ;

    return ::MessageBox (NULL, szBuffer, szCaption, 0) ;
}


//
void showError(const TCHAR *msg)
{
    DWORD eNum;
    TCHAR str[1000];
    TCHAR sysMsg[256];

    eNum = GetLastError( );
    getErrorString(eNum, sysMsg);

    // Display the message
    wsprintf(str, _T("%s failed with error %d (%s)"),
        msg, eNum, sysMsg );
    showMsg(str);
}

//
void showErrorByNum(DWORD dwErrorNum)
{
    TCHAR str[1000];
    TCHAR sysMsg[256];

    getErrorString(dwErrorNum, sysMsg);

    // Display the message
    wsprintf(str, _T("failed with error %d (%s)"),
        dwErrorNum, sysMsg );
    showMsg(str);
}

/*
DWORD WINAPI FormatMessage(
  __in      DWORD dwFlags,
  __in_opt  LPCVOID lpSource,
  __in      DWORD dwMessageId,
  __in      DWORD dwLanguageId,
  __out     LPTSTR lpBuffer,
  __in      DWORD nSize,
  __in_opt  va_list *Arguments
);
*/

void getErrorString(DWORD dwErrorNum, TCHAR *lpBuffer)
{
    TCHAR* p;

    BOOL bOK = ::FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dwErrorNum,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        lpBuffer,
        256,
        NULL );

    if (!bOK)
    {
        HMODULE hModule = ::LoadLibraryEx(_T("netmsg.dll"), NULL, DONT_RESOLVE_DLL_REFERENCES);

        if (NULL == hModule)
        {
            return;
        }

        bOK = ::FormatMessage( FORMAT_MESSAGE_FROM_HMODULE |
            FORMAT_MESSAGE_FROM_SYSTEM,
            hModule,
            dwErrorNum,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            lpBuffer,
            256,
            NULL );
        if (!bOK)
        {
            return;
        }
    }

    // Trim the end of the line and terminate it with a null
    p = lpBuffer;
    while( ( *p > 31 ) || ( *p == 9 ) )
        ++p;
    do { *p-- = 0; } while( ( p >= lpBuffer ) &&
        ( ( *p == '.' ) || ( *p < 33 ) ) );

}

int LoadMessage(DWORD dwMsgId, PTSTR pszBuffer, UINT cchBuffer, ...)
{
    va_list args;
    va_start( args, cchBuffer );

    return FormatMessage(
        FORMAT_MESSAGE_FROM_HMODULE,
        NULL,                   // Module (e.g. DLL) to search for the Message.NULL = own .EXE
        dwMsgId,                // Id of the message to look up (aus"Messages.h")
        LANG_NEUTRAL,           // Language: LANG_NEUTRAL = current thread'slanguage
        pszBuffer,              // Destination buffer
        cchBuffer,              // Character count of destination buffer
        &args                   // Insertion parameters
    );
}

void showCustomMsgBox(const TCHAR *szMsg, const TCHAR *szTitle/*=_T("Info")*/, HWND hOwner/*=NULL*/)
{
    HINSTANCE hInstResource = LoadLibrary( _T("shell32.dll") );

    if (!hInstResource)
    {
        showMsg(_T("Cannot load library!"));
        return;
    }

    MSGBOXPARAMS mbp;
    ZeroMemory( &mbp, sizeof( mbp ) );
    mbp.cbSize       = sizeof( mbp );
    mbp.dwLanguageId = GetSystemDefaultLangID();
    mbp.dwStyle      = MB_USERICON;
    mbp.hInstance    = hInstResource;
    mbp.lpszCaption  = szTitle;
    mbp.lpszIcon     = MAKEINTRESOURCE( 44 );
    mbp.lpszText     = szMsg;
    mbp.hwndOwner    = hOwner;
    MessageBoxIndirect( &mbp );

    ::FreeLibrary( hInstResource );
}

void showCustomInfoMsg(const TCHAR *szMsg, HWND hOwner)
{
    showCustomMsgBox(szMsg, _T("Info"), hOwner);
}

/***********************************************************************
*
*  Global variables
*
***********************************************************************/
static HWND g_hwndTimedOwner;
static BOOL g_bTimedOut;

/***********************************************************************
*
*  MessageBoxTimer
*
*      The timer callback function that posts the fake quit message.
*      This function causes the message box to exit because the message box
*      has determined that the application is exiting.
*
***********************************************************************/
void CALLBACK MessageBoxTimer(HWND hwnd,
                              UINT uiMsg,
                              UINT idEvent,
                              DWORD dwTime)
{
   g_bTimedOut = TRUE;
   if (g_hwndTimedOwner)
      EnableWindow(g_hwndTimedOwner, TRUE);
   PostQuitMessage(0);
}


/***********************************************************************
*
*  TimedMessageBox
*
*      The same as the standard MessageBox, except that TimedMessageBox
*      also accepts a timeout. If the user does not respond within the
*      specified timeout, the value 0 is returned instead of one of the
*      ID* values.
*
***********************************************************************/
int TimedMessageBox(HWND hwndOwner,
                    LPCTSTR pszMessage,
                    LPCTSTR pszTitle,
                    UINT flags,
                    DWORD dwTimeout)
{
   UINT idTimer;
   int iResult;

   g_hwndTimedOwner = NULL;
   g_bTimedOut = FALSE;

   if (hwndOwner && IsWindowEnabled(hwndOwner))
      g_hwndTimedOwner = hwndOwner;

   //
   // Set a timer to dismiss the message box.
   idTimer = SetTimer(NULL, 0, dwTimeout, (TIMERPROC)MessageBoxTimer);

   iResult = MessageBox(hwndOwner, pszMessage, pszTitle, flags);

   //
   // Finished with the timer.
   KillTimer(NULL, idTimer);

   //
   // See if there is a WM_QUIT message in the queue if we timed out.
   // Eat the message so we do not quit the whole application.
   if (g_bTimedOut)
   {
      MSG msg;
      PeekMessage(&msg, NULL, WM_QUIT, WM_QUIT, PM_REMOVE);
      iResult = -1;
   }

   return iResult;
}



extern "C" ULIB_API void __cdecl rdlShowMsg(
   HWND hwnd,        // handle to owner window
   HINSTANCE hinst,  // instance handle for the DLL
   LPTSTR lpCmdLine, // string the DLL will parse
   int nCmdShow      // show state
)
{
    ::MessageBox(0, lpCmdLine, lpCmdLine, MB_OK);
}

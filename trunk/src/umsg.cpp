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

int ULIB_API showYesNoMsgbox(const TCHAR *szMsg, const TCHAR *szTitle/*=_T("Info")*/, HWND hOwner/*=NULL*/)
{
    return ::MessageBox(hOwner, szMsg, szTitle, MB_YESNO|MB_ICONQUESTION);
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
    wsprintf(str, _T("%s failed with error %d (%s)"), msg, eNum, sysMsg );
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
BOOL  ULIB_API g_bTimedOut;
//static HWND g_hwndMsgBox = NULL;
static LPCTSTR g_msgboxTitle = NULL;

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

   g_hwndTimedOwner = hwndOwner;
   g_bTimedOut = FALSE;
   g_msgboxTitle = pszTitle;

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

      g_bTimedOut = FALSE;
   }

   return iResult;
}

/*
 *
 */
static LRESULT CALLBACK cbtProc(int nCode, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK msgBoxSubClassProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

typedef struct {
    HWND hwndMsgBox;
    FARPROC lpMsgBoxSubClassProc;
    BOOL SubClass;

    HHOOK     hhookCBT;     // CBT hook identifier
    HBRUSH  hbrushBkgnd;  // Brush to paint message box background
    COLORREF  clrText;      // Color of message box text
    FARPROC   MsgBoxProc;   // Message box window procedure
    HINSTANCE hinst;        // HINSTANCE of module
} CBTvalues;

// Globals
//__declspec(thread)
CBTvalues cbt;

//****************************************************************************
// Function: cbtProc
//
// Purpose: Callback function of WH_CBT hook
//
// Parameters and return value:
//    See documentation for cbtProc.
//
// Comments: The message box is subclassed on creation and the original
//    window procedure is restored on destruction
//
//****************************************************************************
static LRESULT CALLBACK cbtProc(int nCode, WPARAM wParam, LPARAM lParam)
{
   LPCBT_CREATEWND lpcbtcreate;

   if (nCode < 0)
       return CallNextHookEx(cbt.hhookCBT, nCode, wParam, lParam);

   // Window owned by our task is being created. Since the hook is installed just
   //   before the MessageBox call and removed after the MessageBox call, the window
   //   being created is either the message box or one of its controls.
   if (nCode == HCBT_CREATEWND)
   {
       lpcbtcreate = (LPCBT_CREATEWND)lParam;

       // Check if the window being created is a message box. The class name of
       //   a message box is WC_DIALOG since message boxes are just special dialogs.
       //   We can't subclass the message box right away because the window
       //   procedure of the message box is not set when this hook is called. So
       //   we wait till the hook is called again when one of the message box
       //   controls are created and then we subclass. This will happen because
       //   the message box has at least one control.

       if (WC_DIALOG == lpcbtcreate->lpcs->lpszClass)
       {
           cbt.hwndMsgBox = (HWND)wParam;
           cbt.SubClass = TRUE;      // Remember to subclass when the hook is called next
       }
       else if (cbt.SubClass)
       {
           // Subclass the dialog to change the color of the background and text
           cbt.MsgBoxProc = (FARPROC)SetWindowLong(cbt.hwndMsgBox, GWL_WNDPROC,
                                                  (LONG)msgBoxSubClassProc);
           cbt.SubClass = FALSE;
       }
   }
   else if (nCode == HCBT_DESTROYWND && (HWND)wParam == cbt.hwndMsgBox)
   {
       // Reset the original window procedure when the message box is about to
       //   be destroyed.
       SetWindowLong(cbt.hwndMsgBox, GWL_WNDPROC, (LONG)cbt.MsgBoxProc);
       cbt.hwndMsgBox = NULL;
   }
   return 0;
}


//****************************************************************************
// Function: MsgBoxSubClassProc
//
// Purpose: Subclass procedure for message box to change text and background color
//
// Parameters & return value:
//    Standard. See documentaion for WindowProc
//
//****************************************************************************
static LRESULT CALLBACK msgBoxSubClassProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
     // Change the background and text color of the message box.

     switch(msg)
        { /* msg */
         case WM_CTLCOLORDLG:
                 SetBkMode((HDC)wParam, TRANSPARENT);
                 SetTextColor((HDC)wParam, cbt.clrText);
                 return (LRESULT)cbt.hbrushBkgnd;
         case WM_CTLCOLORSTATIC:
                 SetBkMode((HDC)wParam, TRANSPARENT);
                 SetTextColor((HDC)wParam, cbt.clrText);
                 return (LRESULT)GetStockObject(NULL_BRUSH);
        } /* msg */
     return  CallWindowProc((WNDPROC)cbt.MsgBoxProc, hwnd, msg, wParam, lParam);
}


int ColorMessageBox(HWND hwndOwner,
                    LPCTSTR pszMessage,
                    LPCTSTR pszTitle,
                    UINT flags,
                    COLORREF clrBkgnd,
                    COLORREF clrText)
{
   int iResult;

    cbt.hbrushBkgnd = ::CreateSolidBrush(clrBkgnd);
    cbt.hinst = ::GetModuleHandle(NULL);
    cbt.clrText = clrText;

    // Set a task specific CBT hook before calling MessageBox. The CBT
    //    hook will be called when the message box is created and will
    //    give us access to the window handle of the MessageBox. The
    //    message box can then be subclassed in the CBT hook to change
    //    the color of the text and background. Remove the hook after
    //    the MessageBox is destroyed.

    cbt.hhookCBT = SetWindowsHookEx(WH_CBT, cbtProc, cbt.hinst, GetCurrentThreadId());

    iResult = MessageBox(hwndOwner, pszMessage, pszTitle, flags);

    UnhookWindowsHookEx(cbt.hhookCBT);

   return iResult;
}

//****************************************************************************
// Function: CBTMessageBox
//
// Purpose: Subclass procedure for message box to change button text.
//
//****************************************************************************

HHOOK hhk;

LRESULT CALLBACK CBTProc(INT nCode, WPARAM wParam, LPARAM lParam)
{
    HWND  hChildWnd;    // msgbox is "child"
    // notification that a window is about to be activated
    // window handle is wParam
    if (nCode == HCBT_ACTIVATE)
    {
        // set window handles
        hChildWnd  = (HWND)wParam;
        //to get the text of yes button
        UINT result;
        if(::GetDlgItem(hChildWnd,IDYES)!=NULL)
        {
            //s.LoadString(IDS_Yes);
            result= SetDlgItemText(hChildWnd, IDYES, "Yaa");
        }
        if(GetDlgItem(hChildWnd,IDOK)!=NULL)
        {
            //s.LoadString(IDS_OK);
            result= SetDlgItemText(hChildWnd, IDOK, "Okka");
        }
        // exit CBT hook
        UnhookWindowsHookEx(hhk);
    }
    // otherwise, continue with any possible chained hooks
    else CallNextHookEx(hhk, nCode, wParam, lParam);
    return 0;
}

int CBTMessageBox(HWND hwnd, LPCTSTR lpText, LPCTSTR lpCaption,UINT uType)
{
    hhk = ::SetWindowsHookEx(WH_CBT, &CBTProc, 0, ::GetCurrentThreadId());
    int nRet = MessageBox(hwnd, lpText, lpCaption, uType);
    //::UnhookWindowsHookEx(hhk);
    return nRet;
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

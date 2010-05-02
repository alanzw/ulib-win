#define _WIN32_WINNT 0x501
#define  _WIN32_IE 0x0400


#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "uedit.h"

#if !defined(_MSC_VER) || _MSC_VER <= 1200

#if (_WIN32_WINNT >= 0x501)
#define ECM_FIRST               0x1500      // Edit control messages
#define	EM_SETCUEBANNER	    (ECM_FIRST + 1)		// Set the cue banner with the lParm = LPCWSTR
#define Edit_SetCueBannerText(hwnd, lpcwText) \
	(BOOL)SNDMSG((hwnd), EM_SETCUEBANNER, 0, (LPARAM)(lpcwText))
#define	EM_GETCUEBANNER	    (ECM_FIRST + 2)		// Set the cue banner with the lParm = LPCWSTR
#define Edit_GetCueBannerText(hwnd, lpwText, cchText) \
	(BOOL)SNDMSG((hwnd), EM_GETCUEBANNER, (WPARAM)(lpwText), (LPARAM)(cchText))

typedef struct _tagEDITBALLOONTIP
{
	DWORD   cbStruct;
	LPCWSTR pszTitle;
	LPCWSTR pszText;
	INT     ttiIcon; // From TTI_*
} EDITBALLOONTIP, *PEDITBALLOONTIP;
#define	EM_SHOWBALLOONTIP   (ECM_FIRST + 3)		// Show a balloon tip associated to the edit control
#define Edit_ShowBalloonTip(hwnd, peditballoontip) \
	(BOOL)SNDMSG((hwnd), EM_SHOWBALLOONTIP, 0, (LPARAM)(peditballoontip))
#define EM_HIDEBALLOONTIP   (ECM_FIRST + 4)     // Hide any balloon tip associated with the edit control
#define Edit_HideBalloonTip(hwnd) \
	(BOOL)SNDMSG((hwnd), EM_HIDEBALLOONTIP, 0, 0)
#endif

#endif

UEdit::UEdit(HWND hParent, UINT nResource, HINSTANCE hInst)
:UControl(hParent, nResource, hInst)
{}

UEdit::UEdit(UBaseWindow *pWndParent, UINT nID)
:UControl(pWndParent, nID)
{}

UEdit::~UEdit()
{};

BOOL UEdit::create()
{
    return UControl::create(_T("EDIT")); // EDITCLASS
}

BOOL UEdit::setText(const TCHAR *lpText)
{
    return this->sendMsg(WM_SETTEXT, 0, (LPARAM)lpText);
}

int UEdit::getText(TCHAR *lpText)
{
    return this->sendMsg(WM_GETTEXT, (WPARAM)EDIT_MAX_LENGTH, (LPARAM)lpText);
}

BOOL UEdit::clear()
{
    return this->sendMsg(WM_SETTEXT, 0, (LPARAM)"");
}

int UEdit::getLineCount()
{
    return this->sendMsg(EM_GETLINECOUNT);
}

int UEdit::getLineLength(int n)
{
    return this->sendMsg(EM_LINELENGTH, (WPARAM)n);
}

int UEdit::getLine(int nIndex, LPTSTR lpBuffer, WORD wSize)
{
    *((WORD *)lpBuffer) = wSize;
    return this->sendMsg(EM_GETLINE, (WPARAM)nIndex, (LPARAM)lpBuffer);
}

LONG UEdit::scrollLineDown()
{
    return this->sendMsg(EM_SCROLL, (WPARAM)SB_LINEDOWN);
}

LONG UEdit::scrollLineUp()
{
    return this->sendMsg(EM_SCROLL, (WPARAM)SB_LINEUP);
}

LONG UEdit::scrollPageDown()
{
    return this->sendMsg(EM_SCROLL, (WPARAM)SB_PAGEDOWN);
}

LONG UEdit::scrollPageUp()
{
    return this->sendMsg(EM_SCROLL, (WPARAM)SB_PAGEUP);
}

BOOL UEdit::setLimitText( int n )
{
    return this->sendMsg(EM_SETLIMITTEXT, (WPARAM)n);
}

BOOL UEdit::limitText( int n )
{
    return this->sendMsg(EM_LIMITTEXT, (WPARAM)n);
}

BOOL UEdit::addLine(LPCTSTR sLine)
{
    TCHAR buf[1024] = {0};
    this->getText(buf);
    if ('\0' != buf[0])
    {
        lstrcat(buf, _T("\r\n"));
    }
    lstrcat(buf, sLine);
    return this->setText(buf);
}

BOOL UEdit::showBalloonTip(LPCWSTR pszTitle, LPCWSTR pszText, INT nIcon)
{
	EDITBALLOONTIP ebt;
    ebt.cbStruct = sizeof(EDITBALLOONTIP);
    ebt.pszTitle = pszTitle;
    ebt.pszText = pszText;
    ebt.ttiIcon = nIcon;
   
    return this->sendMsg(EM_SHOWBALLOONTIP, 0, (LPARAM)&ebt);
}

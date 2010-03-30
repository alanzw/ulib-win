/*
 * =====================================================================================
 *
 *       Filename:  usplashwnd.cpp
 *
 *    Description:  USplashWindow implement
 *
 *        Version:  1.0
 *        Created:  2009-8-4 4:10:05
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#include <windows.h>
#include <tchar.h>

#include <cassert>

#include "usplashwnd.h"
#include "uwndclassx.h"
#include "umsg.h"
#include "udllman.h"

LPCTSTR m_cSplashWndClass = _T("USplashWindow_{B7434340-E3F8-4b53-B576-CB607DDEAF9D}");

enum {
    ID_SPLASH_TIMER = 11
};

USplashWindow::USplashWindow(HINSTANCE hInst)
: UBaseWindow(NULL, hInst, m_cSplashWndClass),
  _clrTrans(huys::white)
{
    ::ZeroMemory(m_sFilename, sizeof(m_sFilename));
    //setWndClassName(m_cSplashWndClass);
    setStyles(WS_POPUP|WS_VISIBLE);
    setTitle(_T("splash"));
    setExStyles(WS_EX_TOPMOST | WS_EX_TOOLWINDOW);
}

USplashWindow::USplashWindow( LPCTSTR sFileName )
: UBaseWindow(NULL, ::GetModuleHandle(NULL)),
  _clrTrans(huys::white)
{
    ::ZeroMemory(m_sFilename, sizeof(m_sFilename));
    this->setBmp(sFileName);
}

USplashWindow::~USplashWindow()
{

}

void USplashWindow::setBmp( LPCTSTR sFileName )
{
    lstrcpyn(m_sFilename, sFileName, MAX_PATH);
    m_ubmp.loadFromFile(sFileName);
}

void USplashWindow::setBmp( UINT nID, HINSTANCE hInst )
{
    m_ubmp.loadFromResource(nID, hInst);
}

bool USplashWindow::create()
{
    RECT rc =
    {
        0,
        0,
        m_ubmp.getWidth(),
        m_ubmp.getHeight()
    };
    RECT rcScreen;

    //::GetWindowRect(m_hSelf, &rc);

    //::GetWindowRect(HWND_DESKTOP, &rcScreen);

    ::SystemParametersInfo(SPI_GETWORKAREA, NULL, &rcScreen, NULL);

    LONG dx = (rcScreen.right-rcScreen.left)/2 - (rc.right-rc.left)/2;
    LONG dy = (rcScreen.bottom-rcScreen.top)/2 - (rc.bottom-rc.top)/2;

    rc.left += dx;
    rc.right += dx;
    rc.top += dy;
    rc.bottom += dy;
/*
    ::CreateWindowEx(
        0,                   // dwExStyle
        m_cSplashWndClass,   // lpClassName
        NULL,                // lpWindowName
        WS_POPUP|WS_VISIBLE|DS_CENTER, // dwStyle
        rc.left,                   // x
        rc.top,                   //
        m_ubmp.getWidth(),   // Bitmap Width = Splash Window Width
        m_ubmp.getHeight(),  // Bitmap Height = Splash Window Height
        NULL,                // hParent
        NULL,                // hMenu
        m_hInst,                // hInstance
        this);               // lpParam
*/
    assert(UBaseWindow::create());

    ::MoveWindow( *this,
                  rc.left,
                  rc.top,
                  m_ubmp.getWidth(),
                  m_ubmp.getHeight(), 
                  TRUE
                );
    
    return true;
}

bool USplashWindow::showSplash(DWORD dwDelayedTime)
{
    //assert(this->registerWndClass());
    this->create();
    //centerWindow();
    //CWindow::CenterWindow
    //this->create();
    //showError(_T("Create Error: "));
    //showErrorByNum(1400);
    //::UpdateWindow(m_hSelf);
    //::ShowWindow(m_hSelf, SW_SHOW);
    update();
    show();
    ::SetForegroundWindow(*this);
    Sleep(dwDelayedTime);
    this->killFlash();

    return true;
}

bool USplashWindow::showByTimer(DWORD dwDelayedTime /*= 3000*/)
{
    assert(this->create());
    update();
    show();
    ::SetForegroundWindow(*this);
    setTimer(ID_SPLASH_TIMER, dwDelayedTime);
    return true;
}

bool USplashWindow::showTransparent(huys::Color clr, DWORD dwDelayedTime)
{
    assert(this->create());
    setTransparentColor(clr);
    update();
    show();
    ::SetForegroundWindow(*this);
    setTimer(ID_SPLASH_TIMER, dwDelayedTime);
    return true;
}

void USplashWindow::onDraw(HDC hdc)
{
    RECT rc;
    ::GetClientRect(*this, &rc);
    m_ubmp.showStretch(hdc, rc);
}

BOOL USplashWindow::onTimer(WPARAM wParam, LPARAM lParam)
{
    killTimer(ID_SPLASH_TIMER);
    destroy();
    setHandle(NULL);
	return FALSE;
}

BOOL USplashWindow::filterMessage(UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	if ((uMessage == WM_KEYDOWN	   ||
		 uMessage == WM_SYSKEYDOWN	   ||
		 uMessage == WM_LBUTTONDOWN   ||
		 uMessage == WM_RBUTTONDOWN   ||
		 uMessage == WM_MBUTTONDOWN   ||
		 uMessage == WM_NCLBUTTONDOWN ||
		 uMessage == WM_NCRBUTTONDOWN ||
		 uMessage == WM_NCMBUTTONDOWN))
	{
		this->killFlash();
		killTimer(ID_SPLASH_TIMER);
		return TRUE;
	}

	return UBaseWindow::filterMessage(uMessage, wParam, lParam);
}


void USplashWindow::killFlash()
{
    ::DestroyWindow(*this);
    setHandle(NULL);
}

bool USplashWindow::setTransparentColor(huys::Color clr)
{
    _clrTrans = clr;

#if defined(_MSC_VER) && (_MSC_VER > 1200)
        this->modifyExStyles(WS_EX_LAYERED);
        ::SetLayeredWindowAttributes(*this, _clrTrans, 0, LWA_COLORKEY);
#else

#ifndef LWA_COLORKEY
#define LWA_COLORKEY 1
#endif
        this->modifyExStyles(WS_EX_LAYERED);
        UDllMan dlm(_T("user32.dll"));
        dlm.callFunc<BOOL, HWND, COLORREF, BYTE, DWORD>(_T("SetLayeredWindowAttributes"),
            *this, _clrTrans, 0, LWA_COLORKEY);

#endif // (_MFC_VER > 1200)
}

BOOL USplashWindow::onPreRegisterWindowClass(huys::UWindowClass &uwc)
{
    uwc.setBKBrush((HBRUSH)::GetStockObject(WHITE_BRUSH));
    uwc.setStyles(CS_BYTEALIGNCLIENT | CS_BYTEALIGNWINDOW);
    return FALSE;
}
/*
void USplashWindow::centerWindow()
{
    //RECT rc;
    //RECT rcScreen;

    //::GetWindowRect(m_hSelf, &rc);

    //::GetWindowRect(HWND_DESKTOP, &rcScreen);

    //LONG dx = (rcScreen.right-rcScreen.left)/2 - (rc.right-rc.left)/2;
    //LONG dy = (rcScreen.bottom-rcScreen.top)/2 - (rc.bottom-rc.top)/2;

    //rc.left += dx;
    //rc.right += dx;
    //rc.top += dy;
    //rc.bottom += dy;

    //::MoveWindow(m_hSelf, rc.left, rc.top,
    //    rc.right-rc.left, rc.bottom-rc.top, FALSE);

    HWND hWndCenter = NULL;
    assert(::IsWindow(m_hSelf));

    // determine owner window to center against
    DWORD dwStyle = (DWORD)::GetWindowLong(m_hSelf, GWL_STYLE);
    if(hWndCenter == NULL)
    {
        if(dwStyle & WS_CHILD)
            hWndCenter = ::GetParent(m_hSelf);
        else
            hWndCenter = ::GetWindow(m_hSelf, GW_OWNER);
    }

    // get coordinates of the window relative to its parent
    RECT rcDlg;
    ::GetWindowRect(m_hSelf, &rcDlg);
    RECT rcArea;
    RECT rcCenter;
    HWND hWndParent;
    if(!(dwStyle & WS_CHILD))
    {
        // don't center against invisible or minimized windows
        if(hWndCenter != NULL)
        {
            DWORD dwStyleCenter = ::GetWindowLong(hWndCenter, GWL_STYLE);
            if(!(dwStyleCenter & WS_VISIBLE) || (dwStyleCenter & WS_MINIMIZE))
                hWndCenter = NULL;
        }

        // center within screen coordinates
        ::SystemParametersInfo(SPI_GETWORKAREA, NULL, &rcArea, NULL);
        if(hWndCenter == NULL)
            rcCenter = rcArea;
        else
            ::GetWindowRect(hWndCenter, &rcCenter);
    }
    else
    {
        // center within parent client coordinates
        hWndParent = ::GetParent(m_hSelf);
        assert(::IsWindow(hWndParent));

        ::GetClientRect(hWndParent, &rcArea);
        assert(::IsWindow(hWndCenter));
        ::GetClientRect(hWndCenter, &rcCenter);
        ::MapWindowPoints(hWndCenter, hWndParent, (POINT*)&rcCenter, 2);
    }

    int DlgWidth = rcDlg.right - rcDlg.left;
    int DlgHeight = rcDlg.bottom - rcDlg.top;

    // find dialog's upper left based on rcCenter
    int xLeft = (rcCenter.left + rcCenter.right) / 2 - DlgWidth / 2;
    int yTop = (rcCenter.top + rcCenter.bottom) / 2 - DlgHeight / 2;

    // if the dialog is outside the screen, move it inside
    if(xLeft < rcArea.left)
        xLeft = rcArea.left;
    else if(xLeft + DlgWidth > rcArea.right)
        xLeft = rcArea.right - DlgWidth;

    if(yTop < rcArea.top)
        yTop = rcArea.top;
    else if(yTop + DlgHeight > rcArea.bottom)
        yTop = rcArea.bottom - DlgHeight;

    // map screen coordinates to child coordinates
    ::SetWindowPos(m_hSelf, NULL, xLeft, yTop, -1, -1,
        SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}
*/

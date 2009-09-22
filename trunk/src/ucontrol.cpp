#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <cassert>

#include "ucontrol.h"

UControl::UControl( HWND hParent, UINT nResource, HINSTANCE hInst )
:m_hParent(hParent),
m_nResourceID(nResource),
m_hInstance(hInst),
m_hSelf(0),
m_hPrevParent(0),
m_bManaged(true)
{
    m_rc.left   = 0;
    m_rc.top    = 0;
    m_rc.right  = 0;
    m_rc.bottom = 0;

    m_dwStyles = WS_CHILD | WS_VISIBLE;
}

UControl::UControl()
:m_hParent(0),
m_nResourceID(-1),
m_hInstance(0),
m_hSelf(0),
m_bManaged(false)
{}

BOOL UControl::create(const TCHAR *strCtrlClass, const TCHAR *strWindowText/*=_T("")*/)
{

    assert(NULL == m_hSelf);

    m_hSelf = ::CreateWindow(
            strCtrlClass,           // predefined class
            strWindowText,          // window text
            m_dwStyles,             // styles.
            m_rc.left,              // x position.
            m_rc.top,               // y positio.
            m_rc.right-m_rc.left,   // width
            m_rc.bottom-m_rc.top,   // height
            m_hParent,              // parent window
            (HMENU)m_nResourceID,   // control ID
            m_hInstance,            // instance handle
            0);                     // Pointer not needed.
    this->setLong();
    return TRUE;
}

BOOL UControl::createEx( DWORD dwExStyle,
                         const TCHAR *strCtrlClass /*= _T("")*/,
                         const TCHAR *strWindowText /*= _T("")*/ )
{
    assert(NULL == m_hSelf);

    m_hSelf = ::CreateWindowEx(
            dwExStyle,             // extended window style
            strCtrlClass,          // predefined class
            strWindowText,         // window text
            m_dwStyles,            // styles.
            m_rc.left,             // x position.
            m_rc.top,              // y positio.
            m_rc.right-m_rc.left,  // width
            m_rc.bottom-m_rc.top,  // height
            m_hParent,             // parent window
            (HMENU)m_nResourceID,  // control ID
            m_hInstance,           // instance handle
            0);                    // Pointer not needed.
    this->setLong();
    return TRUE;
}

BOOL UControl::destroy()
{
    ::DestroyWindow(m_hSelf);
    return TRUE;
}

BOOL UControl::setPosition( const RECT *rect )
{
    return ::MoveWindow(m_hSelf, rect->left, rect->top,
        rect->right-rect->left, rect->bottom-rect->top, TRUE);
}

BOOL UControl::setPositionEx(int x, int y, int nWidth, int nHeight, BOOL bRepaint /*= FALSE*/)
{
    return ::MoveWindow(m_hSelf, x, y, nWidth, nHeight, bRepaint);
}

void UControl::setPos(LONG x, LONG y, LONG w, LONG h)
{
    m_rc.left = x;
    m_rc.top = y;
    m_rc.right = x + w;
    m_rc.bottom = y + h;
}

void UControl::setRect(const LPRECT rc)
{
    m_rc = *rc;
}

BOOL UControl::setWindowText(const TCHAR *lpText)
{
    return ::SendMessage(m_hSelf, WM_SETTEXT, (WPARAM)0, (LPARAM)lpText);
}

BOOL UControl::getWindowText( TCHAR *lpText, int nMaxCount)
{
    return ::GetWindowText(m_hSelf, lpText, nMaxCount);
}

BOOL UControl::setStyles( DWORD dwStyle )
{
    DWORD dwNewStyle = m_dwStyles | dwStyle;
    if (dwStyle == dwNewStyle)
    {
        return FALSE;
    } else {
        m_dwStyles = dwNewStyle;
        return TRUE;
    }
}

BOOL UControl::modifyStyles( DWORD dwStyle )
{
    DWORD dwOldStyle = ::GetWindowLong(m_hSelf, GWL_STYLE);
    DWORD dwNewStyle = dwOldStyle | dwStyle;
    if (dwOldStyle == dwNewStyle)
        return FALSE;

//    return ::SetWindowLongPtr(m_hSelf, GWL_STYLE, dwNewStyle);
    return ::SetWindowLong(m_hSelf, GWL_STYLE, dwNewStyle);
    //::SetWindowPos(m_hSelf, NULL, 0, 0, 0, 0,
    //    SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

    //return TRUE;
}

//
BOOL UControl::isVisible() const
{
    return ::IsWindowVisible(m_hSelf);
}

BOOL UControl::show()
{
    return ::ShowWindow(m_hSelf, SW_SHOW);
}

BOOL UControl::hide()
{
    return ::ShowWindow(m_hSelf, SW_HIDE);
}

BOOL UControl::invalidate(BOOL bRedraw)
{
    return ::InvalidateRect(m_hSelf, NULL, bRedraw);
}

BOOL UControl::update()
{
    return ::UpdateWindow(m_hSelf);
}

BOOL UControl::subclassProc()
{
    m_OriginProc =  (WNDPROC) SetWindowLong(m_hSelf, GWL_WNDPROC, (LONG) UControl::newControlProc);
    return TRUE;
}

BOOL UControl::restoreProc()
{
    SetWindowLong(m_hSelf, GWL_WNDPROC, (LONG) m_OriginProc);
    return TRUE;
}

WNDPROC UControl::getOriginProc() const
{
    return m_OriginProc;
}


BOOL UControl::sendMsg(UINT msg, WPARAM wParam, LPARAM lParam)
{
    return ::SendMessage(m_hSelf, (UINT)msg, (WPARAM)wParam, (LPARAM)lParam);
}

BOOL UControl::setLong()
{
    return ::SetWindowLong(m_hSelf, GWL_USERDATA, LONG(this));
}

/*
BOOL UControl::setUserData(DWORD dwUserData)
{
    return ::SetWindowLong(m_hSelf, GWL_USERDATA, dwUserData);
}
*/

BOOL UControl::onMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
        return this->onPaint();
    case WM_LBUTTONDOWN:
        return onLButtonDown(wParam, lParam);
    case WM_RBUTTONDOWN:
        return onRButtonDown(wParam, lParam);
    case WM_LBUTTONUP:
        return this->onLButtonUp(wParam, lParam);
    case WM_RBUTTONUP:
        return this->onRButtonUp(wParam, lParam);
    case WM_MOUSEMOVE:
        return this->onMouseMove(wParam, lParam);
    case WM_CHAR:
        return onChar(wParam, lParam);
    case WM_KEYDOWN:
        return onKeyDown(wParam, lParam);
    case WM_KEYUP:
        return onKeyUp(wParam, lParam);
    case WM_ENABLE:
        return onEnable(wParam, lParam);
    case WM_SHOWWINDOW:
        return onShow(wParam, lParam);
    case WM_SETFOCUS:
        return onSetFocus();
    case WM_KILLFOCUS:
        return onKillFocus();
    case WM_TIMER:
        return onTimer(wParam, lParam);
    case WM_NOTIFY:
        return onNotify(wParam, lParam);
    case WM_NOTIFY + WM_REFLECT_BASE:
        return onNotifyReflect(wParam, lParam);
    case WM_NOTIFY + WM_REFLECT_CTLCOLOR:
        return onCtrlColor(wParam, lParam);
    case WM_NOTIFY + WM_REFLECT_DRAWITEM:
        return onDrawItem(wParam, lParam);
    case WM_NOTIFY + WM_REFLECT_MEASUREITEM:
        return onMeasureItem(wParam, lParam);
    default:
        break;
    }
    return 0;
};

BOOL UControl::getClientRect( RECT *rc )
{
    ::GetClientRect(m_hSelf, rc);
    return TRUE;
}

// TODO: fix me!
UControl * UControl::fromHandle( HWND hwnd )
{
    assert(NULL == m_hSelf);

    if (!::IsWindow(hwnd))
    {
        return FALSE;
    }

    m_hSelf = hwnd;

    m_hParent = ::GetParent(hwnd);

    m_hInstance = ::GetModuleHandle(NULL);

    m_nResourceID = ::GetDlgCtrlID(m_hSelf);

    return this;
}

UControl * UControl::fromID(HWND hParent, UINT nID )
{
    assert(NULL == m_hSelf);

    m_hParent = hParent;
    m_nResourceID = nID;

    m_hSelf = ::GetDlgItem(m_hParent, m_nResourceID);

    m_hInstance = ::GetModuleHandle(NULL);

    return this;
}

LRESULT UControl::newControlProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    UControl *pCtrl = 0;

    //if (uMsg == WM_NCCREATE) {
    //    ::SetWindowLong (hwnd, GWL_USERDATA, long((LPCREATESTRUCT(lParam))->lpCreateParams));
    //}

    pCtrl = reinterpret_cast<UControl *> (::GetWindowLong (hwnd, GWL_USERDATA));

    BOOL bRet = pCtrl->onMessage(uMsg, wParam, lParam);

    if (uMsg == WM_NOTIFY + WM_REFLECT_CTLCOLOR)
    {
        return bRet;
    }

    if (bRet)
    {
        return bRet;
    }

    return CallWindowProc(pCtrl->getOriginProc(), hwnd, uMsg, wParam, lParam);
}

HWND UControl::setParentWnd( HWND hParent )
{
    m_hPrevParent = ::SetParent(m_hSelf, hParent);
    if (NULL != hParent)
    {
        m_hParent = hParent;
    }
    return m_hPrevParent;
}

BOOL UControl::setTimer( UINT_PTR nIDEvent, UINT uElapse )
{
    ::SetTimer(m_hSelf, nIDEvent, uElapse, (TIMERPROC)NULL);
    return TRUE;
}

BOOL UControl::killTimer( UINT_PTR nIDEvent )
{
    ::KillTimer(m_hSelf, nIDEvent);
    return TRUE;
}

void UControl::setFont( HFONT hFont )
{
    this->sendMsg(WM_SETFONT, (WPARAM)hFont, (LPARAM)TRUE);
}

HFONT UControl::getFont()
{
	return (HFONT)this->sendMsg(WM_GETFONT);
}

BOOL UControl::sendMsgParent( UINT uMessage, WPARAM wParam/*=0*/, LPARAM lParam/*=0*/ )
{
	return ::SendMessage(m_hParent, uMessage, wParam, lParam);
}

BOOL UControl::onPaint()
{
 //!! BeginPaint will eat previous control text drawing or other actions
 //   PAINTSTRUCT ps;
 //   HDC hdc;
 //   hdc = BeginPaint(m_hSelf, &ps);
	////hdc = ::GetDC(m_hSelf);
 //   
	//onDraw(hdc);

	////::ReleaseDC(m_hSelf, hdc);
 //   EndPaint(m_hSelf, &ps);
	return FALSE;
}

BOOL UControl::subclass( HWND hCtrl )
{
    this->fromHandle(hCtrl);
    this->setLong();
    this->subclassProc();
    return TRUE;
}

BOOL UControl::subclass( UINT nID, HWND hParent )
{
    this->fromID(hParent, nID);
    this->setLong();
    this->subclassProc();
    return TRUE;
}


/*
 * =====================================================================================
 *
 *       Filename:  ubasewindow.cpp
 *
 *    Description:  UBaseWindow implement
 *
 *        Version:  1.0
 *        Created:  2009-8-9 2:08:59
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

#include "uwndclassx.h"
#include "ubasewindow.h"

#include "umsg.h"

#define UBASEWNDCLASSNAME _T("UBASEWND_{F12499FD-C439-4be3-9AB8-8208DD6BD934}")

LRESULT CALLBACK DefaultUBaseWindowProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{

    UBaseWindow *pBaseWnd = 0;

    if (nMsg == WM_NCCREATE) {
        // if this nMessage gets sent then a new window has just been created,
        // so we'll asociate its handle with its AbstractWindow instance pointer
        ::SetWindowLong (hWnd, GWL_USERDATA, long((LPCREATESTRUCT(lParam))->lpCreateParams));
    }

    pBaseWnd = reinterpret_cast<UBaseWindow *>(::GetWindowLong(hWnd, GWL_USERDATA));

    if (pBaseWnd)
    {
        if (NULL == pBaseWnd->getHandle())
            pBaseWnd->setHandle(hWnd);

        return pBaseWnd->WindowProc(nMsg, wParam, lParam);
    }
    return ::DefWindowProc(hWnd, nMsg, wParam, lParam);
}


UBaseWindow::UBaseWindow( HWND hParent /*= NULL*/,
                          HINSTANCE hInst /*= NULL*/,
                          LPCTSTR lpWindowClass /* = NULL */ )
: m_hParent(hParent), m_hInst(hInst), m_hSelf(NULL), m_lpMenuName(NULL),
  m_dwStyles(WS_VISIBLE|WS_SYSMENU),
  m_dwExStyles(0),
  m_lpWindowTitle(NULL),
  m_lpWindowClass(lpWindowClass)
{
    m_rcWindow.left = 0;
    m_rcWindow.top = 0;
    m_rcWindow.right = 0;
    m_rcWindow.bottom = 0;
}

UBaseWindow::~UBaseWindow()
{

}

bool UBaseWindow::create()
{
    if (NULL == m_lpWindowClass)
    {
        m_lpWindowClass = UBASEWNDCLASSNAME;
    }
    
    if (!huys::UWindowClass::isWndClass(m_lpWindowClass, NULL))
    {
        if (!registerWndClass())
        {
            return false;
        }
    }


    if (NULL == m_lpWindowTitle)
    {
        m_lpWindowTitle = _T("ubasewindow");
    }


    ::CreateWindowEx(
        m_dwExStyles,                     // dwExStyle
        m_lpWindowClass,       // lpClassName
        m_lpWindowTitle,       // lpWindowName
        m_dwStyles,            // dwStyle
        CW_USEDEFAULT,         // x
        CW_USEDEFAULT,         // y
        CW_USEDEFAULT,         // Window Width
        CW_USEDEFAULT,         // Height
        m_hParent,             // hParent
        NULL,                  // hMenu
        m_hInst,               // hInstance
        this);                 // lpParam

    return (NULL != m_hSelf);
}

bool UBaseWindow::registerWndClass()
{
    huys::UWindowClass uwc(m_lpWindowClass, m_hInst);

    //
    uwc.setProcdure(&DefaultUBaseWindowProc);

    //
    onPreRegisterWindowClass(uwc);
    
    //
    if (NULL != m_lpMenuName)
    {
        uwc.setMenu(m_lpMenuName);
    }

    if (!uwc.registerIt())
    {
        //TODO: Error Handling
        LOG_STRING("Erorr register wndclassex!");
        return false;
    }

    return true;
}

BOOL UBaseWindow::defaultMessageHandler(UINT uMessage, WPARAM wParam, LPARAM lParam)
{
    return ::DefWindowProc(m_hSelf, uMessage, wParam, lParam);
}

BOOL UBaseWindow::onMessage(UINT uMessage, WPARAM wParam, LPARAM lParam)
{
    switch (uMessage)
    {
    // FIXME: WM_NCCREATE doesn't work here.
    //case WM_NCCREATE:
    //    return this->onNCCreate();
    case WM_CREATE:
        return this->onCreate();
    case WM_PAINT:
        return this->onPaint();
    case WM_CHAR:
        return this->onChar(wParam, lParam);
    case WM_COMMAND:
        return this->onCommand(wParam, lParam);
    case WM_LBUTTONDOWN:
        return this->onLButtonDown(wParam, lParam);
    case WM_CLOSE:
        return onClose();
    case WM_DESTROY:
        return onDestroy();
    case WM_TIMER:
        return onTimer(wParam, lParam);
    case WM_ERASEBKGND:
        {
        if (FALSE != onEraseBkgnd((HDC)wParam)) return TRUE;
        return defaultMessageHandler(uMessage, wParam, lParam);
        }
    default:
        return defaultMessageHandler(uMessage, wParam, lParam);
    }
}

LRESULT UBaseWindow::WindowProc( UINT uMessage, WPARAM wParam, LPARAM lParam )
{
    return onMessage(uMessage, wParam, lParam);
}

BOOL UBaseWindow::onPaint()
{

    PAINTSTRUCT ps;
    HDC hdc;
    hdc = BeginPaint(m_hSelf, &ps);

    onDraw(hdc);

    EndPaint(m_hSelf, &ps);

    return FALSE;
}


BOOL UBaseWindow::show()
{
    return ::ShowWindow(m_hSelf, SW_NORMAL);
}

BOOL UBaseWindow::update()
{
    return ::UpdateWindow(m_hSelf);
}

BOOL UBaseWindow::destroy()
{
    return ::DestroyWindow(m_hSelf);
}

BOOL UBaseWindow::onClose()
{
    ::PostQuitMessage(0);
    return 0;
}

BOOL UBaseWindow::setTimer( UINT_PTR nIDEvent, UINT uElapse )
{
    ::SetTimer(m_hSelf, nIDEvent, uElapse, (TIMERPROC)NULL);
    return TRUE;
}

BOOL UBaseWindow::killTimer( UINT_PTR nIDEvent )
{
    ::KillTimer(m_hSelf, nIDEvent);
    return TRUE;
}

LPCTSTR UBaseWindow::getMenu() const
{
    return m_lpMenuName;
}

bool UBaseWindow::isChild(HWND hParent)
{
    return (TRUE == ::IsChild(hParent, m_hSelf));
}

bool UBaseWindow::isIconic()
{
    return (TRUE == ::IsIconic(m_hSelf));
}

bool UBaseWindow::isVisible()
{
    return (TRUE == ::IsWindowVisible(m_hSelf));
}

bool UBaseWindow::isZoomed()
{
    return (TRUE == ::IsZoomed(m_hSelf));
}

LONG UBaseWindow::sendMsg( UINT message, WPARAM wParam /*=0*/, LPARAM lParam /*=0*/ )
{
    return ::SendMessage(m_hSelf, message, wParam, lParam);
}

LONG UBaseWindow::postMsg( UINT message, WPARAM wParam /*=0*/, LPARAM lParam /*=0*/ )
{
    return ::PostMessage(m_hSelf, message, wParam, lParam);
}

bool UBaseWindow::setIconBig(HICON hIcon)
{
    return TRUE == this->sendMsg(WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hIcon);
}

bool UBaseWindow::setIconBig(UINT nID)
{
    return this->setIconBig(::LoadIcon(::GetModuleHandle(NULL), MAKEINTRESOURCE(nID)));
}

void UBaseWindow::showMsg(LPCTSTR sMsg, LPCTSTR sTitle /*= _T("Info")*/)
{
    ::showMsg(sMsg, sTitle, *this);
}

BOOL UBaseWindow::setWindowText(LPCTSTR lpText)
{
    return this->sendMsg(WM_SETTEXT, 0, (LPARAM)lpText);
}

HWND UBaseWindow::setActive()
{
    return ::SetActiveWindow(m_hSelf);
}

BOOL UBaseWindow::isActive()
{
    return (::GetActiveWindow() == m_hSelf);
}


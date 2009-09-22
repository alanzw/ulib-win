#include <windows.h>
#include <tchar.h>

#include "uwindowx.h"

namespace huys
{

MESSAGE_MAP_BEGIN(UWindow)
  MESSAGE(WM_PAINT, UWindow::onPaint)
  MESSAGE(WM_CLOSE, UWindow::onClose)
  MESSAGE(WM_CHAR, UWindow::onChar)
MESSAGE_MAP_END()


UWindowClass::UWindowClass(LPCTSTR lpszClassName, HINSTANCE hInstance)
{
    ::ZeroMemory(&m_wcx, sizeof(WNDCLASSEX));
    m_wcx.cbSize = sizeof(WNDCLASSEX);
    m_wcx.style = CS_HREDRAW | CS_VREDRAW | CS_GLOBALCLASS;
    m_wcx.lpfnWndProc = WindowProcedure;
    m_wcx.hInstance = hInstance;
    m_wcx.hbrBackground = (HBRUSH)(COLOR_WINDOW);
    m_wcx.lpszClassName = lpszClassName;
    //
    m_wcx.lpszMenuName = NULL;
    m_wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
    /* Use default icon and mouse-pointer */
    m_wcx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    m_wcx.hIconSm =  LoadIcon(NULL, IDI_APPLICATION); //small class icon

}

BOOL UWindowClass::registerIt()
{
    return ::RegisterClassEx(&m_wcx);
}

LPCTSTR UWindowClass::name() const
{
    return m_wcx.lpszClassName;
}

void UWindowClass::setStyles( DWORD dwStyle )
{
    DWORD dwNewStyle = m_wcx.style | dwStyle;
    if (dwStyle == dwNewStyle)
    {
        //
    } else {
        m_wcx.style = dwNewStyle;
    }
}

void UWindowClass::setProcdure(WNDPROC lpfnWndProc)
{
    if (m_wcx.lpfnWndProc != lpfnWndProc)
    {
        m_wcx.lpfnWndProc = lpfnWndProc;
    }
}

bool UWindowClass::isWndClass(LPCTSTR lpWndClassName, HINSTANCE hInstance)
{
    WNDCLASS wndcls;
    ::ZeroMemory(&wndcls, sizeof(WNDCLASS));
    if (! GetClassInfo(hInstance, lpWndClassName, &wndcls))
    {
        return false;
    }
    return true;
}

UWindow::UWindow()
: _hInst(NULL), _hParent(NULL), _hSelf(NULL)
{
    this->lpszCaption = "UWindow";
}

BOOL UWindow::create(HINSTANCE hInstance)
{
    UWindowClass uwc("huys_window", hInstance);

    if (!uwc.registerIt())
    {
        return FALSE;
    }


    _hSelf = CreateWindowEx(
        0,
        "huys_window",
        this->lpszCaption,
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        0,
        0,
        200,
        240,
        NULL,
        NULL,
        hInstance,
        NULL);
    if (!_hSelf)
    {
        return FALSE;
    }

    return TRUE;
}

LRESULT UWindow::WindowProc(UINT nMsg, WPARAM wParam, LPARAM lParam)
{
    int count = 0;
    while((MessageEntry[count].pfn) != 0)
    {
        if(MessageEntry[count].nMessage == nMsg)
        {
            (this->*MessageEntry[count].pfn)();
            return 0;
        }
        count++;
    }
    return DefWindowProc(_hSelf, nMsg, wParam, lParam);
}

void UWindow::onPaint()
{
    PAINTSTRUCT ps;
    HDC hdc;
    //HBRUSH hbrush;
    //HPEN hpen;
    hdc = BeginPaint(_hSelf, &ps);
    RECT rt;
    GetClientRect(_hSelf, &rt);
    rt.top = 5;
    DrawText(hdc, "Hello World!", strlen("Hello World!"), &rt, DT_SINGLELINE|DT_CENTER|DT_VCENTER );

    MoveToEx(hdc, 0, 0, NULL);
    LineTo(hdc, rt.right, rt.bottom);
    MoveToEx(hdc, rt.left, rt.bottom, NULL);
    LineTo(hdc, rt.right, rt.top);

    EndPaint(_hSelf, &ps);
}

//
void UWindow::onClose()
{
    PostQuitMessage(0);
}

//
void UWindow::onChar()
{
    ::MessageBox(_hSelf, "xx", "xx", MB_OK);
}


LRESULT CALLBACK WindowProcedure( HWND hWnd,
                                  UINT nMsg,
                                  WPARAM wParam,
                                  LPARAM lParam )
{
    UWindow *pWnd = theApp->GetWnd();


    if(pWnd->getHWnd() == 0)
    {
        return DefWindowProc(hWnd, nMsg, wParam, lParam);
    }

    return pWnd->WindowProc(nMsg, wParam, lParam);
}

UWindowApp::~UWindowApp()
{
    CHECK_PTR(m_pWnd);
}

BOOL UWindowApp::initInstance(HINSTANCE hInstance)
{
    m_pWnd = new UWindow();
    return m_pWnd->create(hInstance);
};

int UWindowApp::run()
{
    MSG  msg;
    while(GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (msg.wParam);
}

huys::UWindowApp *theApp;

}; // namespace huys

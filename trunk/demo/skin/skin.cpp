#include <windows.h>
#include "skin.h"

HHOOK    globalWndHookEx;
/*
 * Button Procduce Routine
 */
#define STATUS_BUTTON_NORMAL    0x00000000
#define STATUS_BUTTON_HOVER     0x00000001
#define STATUS_BUTTON_DOWN      0x00000002

class CButtonExt
{
public:
    CButtonExt() {}
    ~CButtonExt() {}

    static UINT        m_nStatus;
    static WNDPROC    m_cWndProc;

    static LRESULT DefWindowProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
    {
        POINT    point;

        switch( message )
        {
        case WM_PAINT:
            return OnPaint( hWnd );
            break;

        case WM_LBUTTONDOWN:
            point.x = LOWORD(lParam);
            point.y = HIWORD(lParam);
            return OnLButtonDown( hWnd, 0, point );
            break;

        case WM_LBUTTONUP:
            point.x = LOWORD(lParam);
            point.y = HIWORD(lParam);
            return OnLButtonUp( hWnd, 0, point );
            break;

        case WM_LBUTTONDBLCLK:
            point.x = LOWORD(lParam);
            point.y = HIWORD(lParam);
            return OnLButtonDblClk( hWnd, 0, point );
            break;

        case WM_MOUSEMOVE:
            point.x = LOWORD(lParam);
            point.y = HIWORD(lParam);
            return OnMouseMove( hWnd, 0, point );
            break;

        default:
            break;
        }

        return CallWindowProc( m_cWndProc, hWnd, message, wParam, lParam );
    }

    static LRESULT OnLButtonDown( HWND hWnd, UINT nFlags, POINT point ) {
        m_nStatus = STATUS_BUTTON_DOWN;
        ::InvalidateRect(hWnd, 0, TRUE);
        ::UpdateWindow(hWnd);

        return TRUE;
    }

    static LRESULT OnLButtonUp( HWND hWnd, UINT nFlags, POINT point ) {
        if( m_nStatus != STATUS_BUTTON_NORMAL ) {
            m_nStatus = STATUS_BUTTON_NORMAL;
            ::InvalidateRect(hWnd, 0, TRUE);
            ::UpdateWindow(hWnd);
            SendMessage( ::GetParent(hWnd), WM_COMMAND, GetDlgCtrlID(hWnd), (LPARAM) (hWnd) );
        }

        return TRUE;
    }

    static LRESULT OnLButtonDblClk( HWND hWnd, UINT nFlags, POINT point ) {
        return TRUE;
    }

    static LRESULT OnMouseMove( HWND hWnd, UINT nFlags, POINT point ) {
        HRGN hRgn = ::CreateRectRgn( 0, 0, 0, 0 );
        GetWindowRgn( hWnd, hRgn );

        BOOL bIn = PtInRegion( hRgn, point.x, point.y );
        if( bIn ) {
            if( m_nStatus == STATUS_BUTTON_DOWN ) return TRUE;
            if( m_nStatus != STATUS_BUTTON_HOVER ) {
                m_nStatus = STATUS_BUTTON_HOVER;
                    InvalidateRect(hWnd, 0, TRUE);
                    UpdateWindow(hWnd);
                    SetCapture(hWnd);
            }
        } else {
            if ( m_nStatus == STATUS_BUTTON_HOVER ) {
                m_nStatus = STATUS_BUTTON_NORMAL;
                InvalidateRect(hWnd, 0, TRUE);
                UpdateWindow(hWnd);
                ReleaseCapture();
            }
        }
        DeleteObject( hRgn );

        return TRUE;
    }

    static LRESULT OnPaint( HWND hWnd ) {
        RECT        rc;
        POINT        pt;
        pt.x = 2;
        pt.y = 2;
        char cs[256];

        PAINTSTRUCT ps;
        HDC hdc;
        HBRUSH hbrush;
        HBRUSH holdBrush;
        HPEN hpen;
        HPEN holdPen;
        HFONT hfont;
        HFONT holdFont;
        hdc = BeginPaint(hWnd, &ps);

        SetBkMode(hdc, TRANSPARENT);

        hfont = ::CreateFont( 12, 0, 0, 0, FW_HEAVY, 0, 0, 0, ANSI_CHARSET, \
            OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, \
            VARIABLE_PITCH | FF_SWISS, "MS Sans Serif" );

        holdFont = (HFONT)::SelectObject(hdc, hfont);
        if( m_nStatus == STATUS_BUTTON_DOWN ) {
            hbrush = ::CreateSolidBrush( RGB( 160, 160, 160 ) );
            hpen = ::CreatePen( PS_SOLID, 1, RGB( 100, 100, 100 ) );
            SetTextColor( hdc, RGB( 50, 50, 250 ) );
        } else if( m_nStatus == STATUS_BUTTON_HOVER ) {
            hbrush = ::CreateSolidBrush( RGB( 60, 60, 180 ) );
            hpen = ::CreatePen( PS_SOLID, 1, RGB( 0, 0, 0 ) );
            SetTextColor(hdc, RGB( 250, 250, 50 ) );
        } else if( m_nStatus == STATUS_BUTTON_NORMAL ) {
            hbrush = ::CreateSolidBrush( RGB( 240, 240, 240 ) );
            hpen = ::CreatePen( PS_SOLID, 1, RGB( 120, 120, 120 ) );
            SetTextColor(hdc, RGB( 50, 50, 50 ) );
        }
        holdBrush = (HBRUSH)SelectObject( hdc, hbrush );
        holdPen = (HPEN)SelectObject( hdc, hpen );
        GetClientRect( hWnd, &rc );
        RoundRect( hdc, rc.left, rc.top, rc.right, rc.bottom, pt.x, pt.y );

        HRGN hRgn = ::CreateRectRgn( rc.left, rc.top, rc.right, rc.bottom );
        SetWindowRgn(hWnd, hRgn, TRUE );
        DeleteObject( hRgn );

        GetWindowText(hWnd, cs, 255);
        DrawText(hdc, cs, strlen(cs), &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE );

        SelectObject(hdc, holdFont );
        SelectObject( hdc, holdBrush );
        SelectObject( hdc, holdPen );


        EndPaint(hWnd, &ps);

        return TRUE;
    }

    static LRESULT OnEraseBkgnd( HWND hWnd, HDC hDC ) {
        return TRUE;
    }
};

UINT    CButtonExt::m_nStatus = STATUS_BUTTON_NORMAL;
WNDPROC    CButtonExt::m_cWndProc = NULL;


/*
 * Windows Hook Procedure Routine
 * nCode    - hook code
 * wParam    - current-process flag
 * lParam    - message data
 */
LRESULT CALLBACK ICallWndProc( int nCode, WPARAM wParam, LPARAM lParam )
{
    PCWPSTRUCT    pcs = (PCWPSTRUCT) lParam;
    HWND        hWnd = pcs->hwnd;

    if( hWnd )
    {
        char sClassName[201] = "\0";

        ::GetClassName( hWnd, sClassName, 200 );
        if( strcmp( sClassName, "Button" ) == 0 )
        {
            DWORD    dwStyle = (DWORD)GetWindowLong(hWnd, GWL_STYLE);
            //const DWORD hookStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP;
            if ( dwStyle == 0x50010000 ) {
            //DWORD dwSkined = (DWORD)GetWindowLong(hWnd, GWL_USERDATA);
            //if ( SKBS_SKINED == dwSkined ) {
                WNDPROC    WndProc;
                WndProc = (WNDPROC) GetWindowLong( hWnd, GWL_WNDPROC );
                if( CButtonExt::m_cWndProc != NULL && \
                    WndProc != CButtonExt::m_cWndProc )
                {
                    return CallNextHookEx( globalWndHookEx, nCode, wParam, lParam );
                }
                if( WndProc != (WNDPROC) CButtonExt::DefWindowProc ) {
                    WndProc = (WNDPROC) SetWindowLong( hWnd, GWL_WNDPROC, (LONG) CButtonExt::DefWindowProc );
                    CButtonExt::m_cWndProc = WndProc;
                }
            }
        }
    }

    return CallNextHookEx( globalWndHookEx, nCode, wParam, lParam );
}

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif


BOOL IStart( HINSTANCE hModule, DWORD dwThreadID )
{
    globalWndHookEx = SetWindowsHookEx(
        WH_CALLWNDPROC, (HOOKPROC) ICallWndProc, hModule, dwThreadID );

    return TRUE;
}

BOOL IStop( void )
{
    UnhookWindowsHookEx( globalWndHookEx );

    return TRUE;
}

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif


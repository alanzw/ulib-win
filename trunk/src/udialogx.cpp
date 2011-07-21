#if defined(_MSC_VER) && _MSC_VER <= 1200
#define _WIN32_WINNT 0x0400
#endif

#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <cassert>
#include "udialogx.h"
#include "umsg.h"

namespace huys
{

UDialogBox::UDialogBox(HINSTANCE hInst, UINT nID, DLGPROC lpDialogFunc, HWND hParent)
{
    m_hInst = hInst;
    m_nDialogID = nID;
    m_lpDialogFunc = lpDialogFunc;
    m_hParent = hParent;

    //
    bInitialized = FALSE;

    //
    m_mode = U_DLG_MODALESS;

    //
    m_bShowAnimation = FALSE;
}

UDialogBox::~UDialogBox()
{
    // EndDialog(m_hDlg, 0);

}

BOOL UDialogBox::create()
{

    //m_hDlg = CreateDialog(m_hInst, MAKEINTRESOURCE(m_nDialogID), m_hParent, m_lpDialogFunc);
    //this->setLong();
    ::CreateDialogParam(m_hInst, MAKEINTRESOURCE(m_nDialogID), m_hParent, m_lpDialogFunc, (LPARAM)this);

    assert(NULL != m_hDlg);

    if (m_bShowAnimation)
    {
#if (WINVER >= 0x0500)
        AnimateWindow(m_hDlg, 1000, AW_SLIDE|AW_HOR_POSITIVE);
        RECT rc;
        GetClientRect(m_hDlg, &rc);
        ::InvalidateRect(m_hDlg, &rc, FALSE);
#else
        ShowWindow(m_hDlg, SW_SHOW);
#endif
    }
    else
    {
        ShowWindow(m_hDlg, SW_SHOW);
    }


    if (!m_hParent)
    {
        return this->go();
    }
    else
    {
        return 0;
    }
}

BOOL UDialogBox::hide()
{
    return ShowWindow(m_hDlg, SW_HIDE);
}

BOOL UDialogBox::show()
{
    return ShowWindow(m_hDlg, SW_SHOW);
}

HWND UDialogBox::getHWND()
{
    return m_hDlg;
}

BOOL UDialogBox::onCancel()
{
    if (m_bShowAnimation)
    {
#if (WINVER >= 0x0500)
        AnimateWindow(m_hDlg, 1000, AW_BLEND|AW_HIDE);
#endif
    }

    if (U_DLG_MODAL == m_mode)
    {
        EndDialog (m_hDlg, 0) ;
    }
    else
    {
        if (m_hDlg)
        {
            DestroyWindow(m_hDlg);
            m_hDlg = NULL;
        }

        if (NULL == m_hParent)
        {
            ::PostQuitMessage (0);
        }
    }

    return 0;
}

BOOL UDialogBox::onClose()
{
    return this->onCancel();
}

BOOL UDialogBox::modifyStyles(DWORD dwStyle)
{
    DWORD dwOldStyle = ::GetWindowLongPtr(m_hDlg, GWL_STYLE);
    DWORD dwNewStyle = dwOldStyle | dwStyle;
    if (dwOldStyle == dwNewStyle)
        return FALSE;

//    return ::SetWindowLongPtr(m_hSelf, GWL_STYLE, dwNewStyle);
    return ::SetWindowLongPtr(m_hDlg, GWL_STYLE, dwNewStyle);
}

BOOL UDialogBox::modifyExStyles(DWORD dwStyle)
{
    DWORD dwOldStyle = ::GetWindowLongPtr(m_hDlg, GWL_EXSTYLE);
    DWORD dwNewStyle = dwOldStyle | dwStyle;
    if (dwOldStyle == dwNewStyle)
        return FALSE;

//    return ::SetWindowLongPtr(m_hSelf, GWL_STYLE, dwNewStyle);
    return ::SetWindowLongPtr(m_hDlg, GWL_EXSTYLE, dwNewStyle);
}

BOOL UDialogBox::DialogProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    BOOL bRet;
    if ( message == WM_CTLCOLORSTATIC  || 
         message == WM_CTLCOLOREDIT    || 
         message == WM_CTLCOLORLISTBOX ||
         message == WM_CTLCOLORBTN     ||
         message == WM_CTLCOLORSCROLLBAR
       )
    {
        if ((bRet=UDialogBox::onCtrlColor(wParam, lParam)) != FALSE)
        {
            return bRet;
        }
    }

    if (message == WM_DRAWITEM)
    {
        if ((bRet=UDialogBox::onDrawItem(wParam, lParam)) != FALSE)
        {
            return bRet;
        }
    }


    switch (message)
    {
    case WM_INITDIALOG:
        return this->onInit();
    case WM_PAINT:
        return this->onPaint();
    case WM_MOUSEMOVE:
        return this->onMouseMove(wParam, lParam);
    case WM_LBUTTONDOWN:
        return this->onLButtonDown(wParam, lParam);
    case WM_RBUTTONDOWN:
        return this->onRButtonDown(wParam, lParam);
    case WM_LBUTTONUP:
        return this->onLButtonUp(wParam, lParam);
    case WM_RBUTTONUP:
        return this->onRButtonUp(wParam, lParam);
	case WM_MBUTTONDOWN:
		return this->onMButtonDown(wParam, lParam);
	case WM_MBUTTONUP:
		return this->onMButtonUp(wParam, lParam);
	case WM_MOUSEWHEEL:
		return this->onMouseWheel(wParam, lParam);
    case WM_CHAR: // ? !!!DialogBox never get this!!
        return this->onChar(wParam, lParam);
    case WM_CLOSE:
        return this->onClose();
    case WM_DESTROY:
        return this->onDestroy();
    case WM_NCDESTROY:
        return this->onNCDestroy();
    case WM_COMMAND:
        return this->onCommand(wParam, lParam);
    case WM_SYSCOMMAND:
        return this->onSysCommand(wParam, lParam);
    case WM_NOTIFY:
        return this->onNotify(wParam, lParam);
    case WM_TIMER:
        return this->onTimer(wParam, lParam);
    case WM_ERASEBKGND:
        return this->onEraseBkgnd((HDC)wParam);
    case WM_CTLCOLORDLG:
        return this->onCtrlColorDlg((HDC)wParam);
    case WM_DROPFILES:
        return this->onDropFiles(wParam, lParam);
    }
    //!!! return ::DefWindowProc(m_hDlg, message, wParam, lParam); !!! Never do it!
    return FALSE;
}

BOOL UDialogBox::onCommand(WPARAM wParam, LPARAM lParam)
{
    switch (LOWORD (wParam))
    {
    case IDOK:
    case IDCANCEL:
        return this->onCancel();
    }
    return FALSE;
}

BOOL UDialogBox::onNotify(WPARAM wParam, LPARAM lParam)
{
    UINT code = ((LPNMHDR)lParam)->code;
    switch(code)
    {
    //case NM_SETCURSOR:
    //    break;
    case 0:
    default:
        if ((0 != lParam) && (NULL != ((LPNMHDR)lParam)->hwndFrom))
        {
            BOOL bRet = ::SendMessage(((LPNMHDR)lParam)->hwndFrom, WM_NOTIFY + WM_REFLECT_BASE, wParam, lParam);
            if(bRet)
            {
                return bRet;
            }
        };
    };
    return FALSE;
}

/**
 *  wParam : Handle to the device context for the control.
 *  lParam : Handle to the control.
 */

BOOL UDialogBox::onCtrlColor(WPARAM wParam, LPARAM lParam)
{
    BOOL bRet = ::SendMessage((HWND)lParam, WM_NOTIFY + WM_REFLECT_CTLCOLOR, wParam, lParam);
    return bRet;
}

// Message Reflection
BOOL UDialogBox::onDrawItem( WPARAM wParam, LPARAM lParam )
{
    HWND hwnd =  ((LPDRAWITEMSTRUCT) lParam)->hwndItem;
    BOOL bRet = ::SendMessage(hwnd, WM_NOTIFY + WM_REFLECT_DRAWITEM, wParam, lParam);
    return bRet;
}


BOOL UDialogBox::onMeasureItem( HWND hDlg, WPARAM wParam, LPARAM lParam )
{
    LPMEASUREITEMSTRUCT lpm = (LPMEASUREITEMSTRUCT) lParam;
    BOOL bRet;

    switch (lpm->CtlType)
    {
    case ODT_COMBOBOX:
    case ODT_LISTBOX:
        bRet = ::SendMessage(::GetDlgItem(hDlg, lpm->CtlID), WM_NOTIFY + WM_REFLECT_MEASUREITEM, wParam, lParam);
        break;
    }
    return bRet;
}

BOOL UDialogBox::sendMsg(UINT message, WPARAM wParam/*=0*/, LPARAM lParam/*=lParam*/)
{
    return ::SendMessage(m_hDlg, message, wParam, lParam);
}

BOOL UDialogBox::postMsg(UINT message, WPARAM wParam/*=0*/, LPARAM lParam/*=lParam*/)
{
    return ::PostMessage(m_hDlg, message, wParam, lParam);
}

BOOL UDialogBox::setLong()
{
    return ::SetWindowLongPtr(m_hDlg, GWLP_USERDATA, (LONG_PTR)this);
}

INT_PTR CALLBACK UDialogBox::DefaultDlgProc(HWND hDlg, UINT message,
                             WPARAM wParam, LPARAM lParam)
{
    BOOL bRet;
    if (message == WM_CTLCOLORSTATIC || message == WM_CTLCOLOREDIT)
     {
         if ((bRet=UDialogBox::onCtrlColor(wParam, lParam)) != FALSE)
         {
            return bRet;
         }
    }

    if (message == WM_DRAWITEM)
    {
        if ((bRet=UDialogBox::onDrawItem(wParam, lParam)) != FALSE)
         {
            return bRet;
         }
    }

    if (message == WM_MEASUREITEM)
    {
        if ((bRet=UDialogBox::onMeasureItem(hDlg, wParam, lParam)) != FALSE)
        {
            return bRet;
        }
    }


    //static BOOL bInit = FALSE;
    UDialogBox *pDlg = 0;


    if (message == WM_INITDIALOG)
    {
        // if this nMessage gets sent then a new window has just been created,
        // so we'll asociate its handle with its AbstractWindow instance pointer
        ::SetWindowLongPtr (hDlg, GWLP_USERDATA, (LONG_PTR)lParam);
        pDlg = (UDialogBox *)(lParam);
        pDlg->setHWND(hDlg);
    }

    pDlg = reinterpret_cast<UDialogBox *>(::GetWindowLongPtr(hDlg, GWLP_USERDATA));


    if (!pDlg)
        return ::DefWindowProc(hDlg, message, wParam, lParam);

    //if (!pDlg->bInitialized)
    //{
    //    pDlg->bInitialized = TRUE;
    //    if (!pDlg->onInit())
    //    {
    //        pDlg->onCancel();
    //    }
    //    //pDlg->onCtrlColor(wParam, lParam);
    //}

    return pDlg->DialogProc(message, wParam, lParam);
}

BOOL UDialogBox::setParentH( HWND hParent )
{
    m_hParent = hParent;
    return TRUE;
}

BOOL UDialogBox::destroy()
{
#if (WINVER >= 0x0500)
    AnimateWindow(m_hDlg, 1000, AW_BLEND|AW_HIDE);
#endif

    if (U_DLG_MODAL == m_mode)
    {
        EndDialog (m_hDlg, 0) ;
    }
    else
    {
        if (m_hDlg)
        {
            DestroyWindow(m_hDlg);
            m_hDlg = NULL;
        }

        if (NULL == m_hParent)
        {
            ::PostQuitMessage (0);
        }

    }
    //DestroyWindow(m_hDlg);
    //m_hDlg = NULL;
    //::PostQuitMessage (0);
    //m_hDlg = NULL;
    //delete this;
    return 0;
}

BOOL UDialogBox::onDestroy()
{
    return destroy();
}

BOOL UDialogBox::onNCDestroy()
{
    //delete this;
    return FALSE;
}

BOOL UDialogBox::onPreTranslateMessage(LPMSG msg)
{
    return FALSE;
}

BOOL UDialogBox::go()
{
    BOOL bRet;
    MSG msg;

    while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
    {
        if ( (msg.hwnd == m_hDlg || ::GetParent(msg.hwnd) == m_hDlg)&& onPreTranslateMessage(&msg))
        {
            continue;
        }
    
        if (bRet == -1)
        {
            // Handle the error and possibly exit
        }
        else if (!IsWindow(m_hDlg) || !IsDialogMessage(m_hDlg, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return msg.wParam;
}

BOOL UDialogBox::setTimer( UINT_PTR nIDEvent, UINT uElapse )
{
    ::SetTimer(m_hDlg, nIDEvent, uElapse, (TIMERPROC)NULL);
    return TRUE;
}

BOOL UDialogBox::killTimer( UINT_PTR nIDEvent )
{
    ::KillTimer(m_hDlg, nIDEvent);
    return TRUE;
}

BOOL UDialogBox::sendChildMsg( UINT nID, UINT nMessage, WPARAM wParam/*=0*/, LPARAM lParam/*=0*/ )
{
    return ::SendMessage(::GetDlgItem(m_hDlg, nID), nMessage, wParam, lParam);
}

//
BOOL UDialogBox::setDlgIconBig(HICON hIcon)
{
    return this->sendMsg(WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hIcon);
}

BOOL UDialogBox::setDlgIconBig(UINT nID)
{
    return this->setDlgIconBig(::LoadIcon(m_hInst, MAKEINTRESOURCE(nID)));
}
//
BOOL UDialogBox::setDlgIconSmall(HICON hIcon)
{
    return this->sendMsg(WM_SETICON, (WPARAM)ICON_SMALL, (LPARAM)hIcon);
}

BOOL UDialogBox::setDlgIconSmall(UINT nID)
{
    return this->setDlgIconSmall(::LoadIcon(m_hInst, MAKEINTRESOURCE(nID)));
}

BOOL UDialogBox::onPaint()
{
    PAINTSTRUCT ps;
    HDC hdc;
    hdc = BeginPaint(m_hDlg, &ps);

    onDraw(hdc);

    EndPaint(m_hDlg, &ps);
    return FALSE;
}

BOOL UDialogBox::setDlgItemText( UINT uID, LPCTSTR lpString )
{
    return ::SetDlgItemText(m_hDlg, uID, lpString);
}

BOOL UDialogBox::setDlgItemInt( UINT uID, UINT uValue, BOOL bSigned /*= FALSE*/ )
{
    return ::SetDlgItemInt(m_hDlg, uID, uValue, bSigned);
}

BOOL UDialogBox::sendDlgItemMessage( UINT uID, UINT uMessage, WPARAM wParam, LPARAM lParam )
{
    return ::SendDlgItemMessage(m_hDlg, uID, uMessage, wParam, lParam);
}

HWND UDialogBox::getDlgItem( UINT uID )
{
    return ::GetDlgItem(m_hDlg, uID);
}

UINT UDialogBox::getDlgItemText( UINT uID, LPTSTR lpString, int nMaxCount )
{
    return ::GetDlgItemText(m_hDlg, uID, lpString, nMaxCount);
}

BOOL UDialogBox::getDlgItemInt( UINT uID, int *lpValue, BOOL bSigned /*= FALSE*/ )
{
    BOOL bTranslated = FALSE;
    *lpValue = ::GetDlgItemInt(m_hDlg, uID, &bTranslated, bSigned);
    return bTranslated;
}

int UDialogBox::getDlgCtrlID( HWND hCtrl )
{
    return ::GetDlgCtrlID(hCtrl);
}

BOOL UDialogBox::setWindowRgn(HRGN hRgn, BOOL bRedraw /* = TRUE */)
{
    return ::SetWindowRgn(m_hDlg, hRgn, bRedraw);
}

BOOL UDialogBox::setTopMost()
{
    return ::SetWindowPos(m_hDlg, HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE|SWP_NOSIZE);
}

BOOL UDialogBox::setNoTopMost()
{
    return ::SetWindowPos(m_hDlg, HWND_NOTOPMOST, 0,0,0,0, SWP_NOMOVE|SWP_NOSIZE);
}

BOOL UDialogBox::bringToTop()
{
    return ::BringWindowToTop(m_hDlg);
}

BOOL UDialogBox::redraw(LPRECT lpRect)
{
    return ::InvalidateRect(m_hDlg, lpRect, TRUE);
}

BOOL UDialogBox::update()
{
    return ::UpdateWindow(m_hDlg);
}

}; // namespace huys


#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT  0x0500
#include "resource.h"

#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <shellapi.h>

#include "udialogx.h"
#include "udlgapp.h"
#include "ubitmap.h"
#include "ubutton.h"
#include "umsg.h"
#include "udllman.h"

using huys::UDialogBox;

class UDialogExt : public UDialogBox
{
    enum {
        ID_TMEVT_RT = 22
    };

    enum {
        IDC_BN_OK      = 2000,
        IDM_SYS_ABOUT  = 1111,
        IDM_SYS_HELP   = 1112,
        IDM_SYS_REMOVE = 1113
    };
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
    : UDialogBox(hInst, nID) {}

    BOOL onInit()
    {
#if defined(_MSC_VER) && (_MSC_VER > 1200)
        this->modifyExStyles(WS_EX_LAYERED);
        ::SetLayeredWindowAttributes(m_hDlg, 0, 129,LWA_ALPHA);
#else

#ifndef WS_EX_LAYERED
#define WS_EX_LAYERED 1
#endif

#ifndef LWA_ALPHA
#define LWA_ALPHA 2
#endif

        this->modifyExStyles(WS_EX_LAYERED);
        UDllMan dlm(_T("user32.dll"));
        //dlm.callFunc<BOOL, HWND, COLORREF, BYTE, DWORD>(_T("SetLayeredWindowAttributes"),
        //    m_hDlg, 0, 129, LWA_ALPHA);
#endif // _MSC_VER
        HMENU hMenu = GetSystemMenu (m_hDlg, FALSE) ;

        AppendMenu (hMenu, MF_SEPARATOR, 0,           NULL) ;
        AppendMenu (hMenu, MF_STRING, IDM_SYS_ABOUT,  TEXT ("About...")) ;
        AppendMenu (hMenu, MF_STRING, IDM_SYS_HELP,   TEXT ("Help...")) ;
        AppendMenu (hMenu, MF_STRING, IDM_SYS_REMOVE, TEXT ("Remove Additions")) ;

        UButton ubn(m_hDlg, IDC_BN_OK, m_hInst);
        ubn.setPos(100, 150, 100, 100);
        ubn.create();
        ubn.setWindowText(_T("OK"));

        this->setDlgIconBig(::LoadIcon(NULL, IDI_QUESTION));

        return TRUE;
    }

    BOOL onMouseMove(WPARAM wParam, LPARAM lParam)
    {
        long xPos = GET_X_LPARAM(lParam);
        long yPos = GET_Y_LPARAM(lParam);

        HCURSOR hCur = ::LoadCursor(NULL, IDC_WAIT);

        if (xPos < 100 && yPos < 100)
        {
            ::SetCursor(hCur);
        }

        return FALSE;
    }

    BOOL onLButtonDown(WPARAM wParam, LPARAM lParam)
    {
        this->killTimer(ID_TMEVT_RT);
        return FALSE;
    }

    BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
            switch(LOWORD (wParam))
            {
            case IDC_BN_OK:
                return onBnOK();
            default:
                return UDialogBox::onCommand(wParam, lParam);
            }
    }

    BOOL onSysCommand(WPARAM wParam, LPARAM lParam)
    {
        switch (LOWORD (wParam))
        {
        case IDM_SYS_ABOUT:
            ::MessageBox( m_hDlg,
                TEXT ("A Poor-Person's Menu Program\n"),
                TEXT ("Help not yet implemented!"),
                MB_OK | MB_ICONINFORMATION) ;
            break;

        case IDM_SYS_HELP:
            MessageBox (m_hDlg, TEXT ("Help not yet implemented!"),
                TEXT ("Help not yet implemented!"), MB_OK | MB_ICONEXCLAMATION) ;
            break ;

        case IDM_SYS_REMOVE:
            GetSystemMenu (m_hDlg, TRUE) ;
            break ;
        }
        return FALSE;
    }

    virtual BOOL onPaint()
    {
        PAINTSTRUCT ps;
        HDC hdc;
        //static UBitmap ubm(_T("bk.bmp"));
        hdc = BeginPaint(m_hDlg, &ps);
        RECT rt;
        GetClientRect(m_hDlg, &rt);
        //ubm.show(hdc, rt);
        rt.top = 5;
        DrawText(hdc, "Hello World!", strlen("Hello World!"), &rt, DT_SINGLELINE|DT_CENTER|DT_VCENTER );
        RECT rcRect = {250, 20, 350, 120};
        MoveToEx(hdc, rcRect.left, rcRect.top, NULL);
        LineTo(hdc, rcRect.right, rcRect.top);
        LineTo(hdc, rcRect.right, rcRect.bottom);
        LineTo(hdc, rcRect.left, rcRect.bottom);
        LineTo(hdc, rcRect.left, rcRect.top);
        EndPaint(m_hDlg, &ps);
        return FALSE;
    }

    BOOL onEraseBkgnd(HDC hdc)
    {
        //UBitmap ubm(_T("bk.bmp"));
        UBitmap ubm(IDB_BACKGROUND, m_hInst);
        //HDC hdc = (HDC)wParam;
        RECT rc;
        ::GetClientRect(m_hDlg, &rc);
        //hdc = ::GetWindowDC(m_hDlg);
        ubm.showStretch(hdc, rc);
        //ReleaseDC(m_hDlg, hdc);
        return TRUE;
    }

    BOOL onBnOK()
    {
        //::MessageBox(m_hDlg, "You Press it!", "INFO", MB_OK);
        this->setTimer(ID_TMEVT_RT, 500);
        return 0;
    }

    BOOL onTimer(WPARAM wParam, LPARAM lParam)
    {
        switch (wParam)
        {
        case ID_TMEVT_RT:
            return rotateCaption();
        default:
            return FALSE;
        }
    }

    BOOL onDropFiles(WPARAM wParam, LPARAM lParam)
    {
        HDROP hDropInfo = (HDROP)wParam;
        WORD wNumFilesDropped = ::DragQueryFile(hDropInfo, -1, NULL, 0);
        showMsgFormat(_T("dropfile"), _T("%u"), wNumFilesDropped);
        return FALSE;
    }

    BOOL rotateCaption()
    {
        char sCaption[256];
        ::GetWindowText(m_hDlg, sCaption, 256);
        int nLength = lstrlen(sCaption);
        sCaption[nLength] = sCaption[0];
        sCaption[nLength+1] = '\0';
        ::SetWindowText(m_hDlg, &sCaption[1]);
        return FALSE;
    }
};

UDLGAPP_T(UDialogExt, IDD_TEST);


#include "resource.h"

#include <windows.h>
#include <commctrl.h>
#include <tchar.h>
#include <shellapi.h>

#include "udialogx.h"
#include "udlgapp.h"
#include "ubutton.h"
#include "uedit.h"
#include "colors.h"

#include "utrayicon.h"

#include "adt/uautoptr.h"

#define  WM_SHELLNOTIFY  WM_USER+5

NOTIFYICONDATA ntd;

using huys::UDialogBox;

WNDPROC oldWndProc;

HINSTANCE u_hInst;

HMENU hPopupMenu;

const TCHAR *lpSuperClass = _T("SUPEREDITCLASS");

LRESULT APIENTRY EditWndProc( HWND hWnd,
                              UINT uMsg,
                              WPARAM wParam,
                              LPARAM lParam )
{
    switch (uMsg)
    {
        case WM_CHAR:
            if (wParam>='0' && wParam<='9' || wParam == VK_BACK)
                return ::CallWindowProc(oldWndProc, hWnd, uMsg, wParam, lParam);
            break;
        case WM_KEYDOWN:
            switch(wParam)
            {
            case VK_RETURN:
            case VK_TAB:
            default:
                break;
            }
        default:
            return ::CallWindowProc(oldWndProc, hWnd, uMsg, wParam, lParam);
    }

	return 0;
}


class UEditSuper : public UEdit
{
public:
    UEditSuper(HWND hParent, UINT nID, HINSTANCE hInst)
    : UEdit(hParent, nID, hInst)
    {}
    BOOL create()
    {
        return UControl::create(lpSuperClass);
    }
};

class UDialogExt : public UDialogBox
{
    enum {
        ID_BTN_OK     = 5555,
        ID_BTN_CANCEL = 5556,
        ID_BTN_MIN    = 5557,
        IDM_RESTORE   = 5558,
        IDM_EXIT      = 5559
    };
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
    : UDialogBox(hInst, nID) {}

    virtual BOOL onInit()
    {
        WNDCLASSEX wcx = {0};
        wcx.cbSize = sizeof(WNDCLASSEX);
        ::GetClassInfoEx(NULL, WC_EDIT, &wcx);

        oldWndProc = wcx.lpfnWndProc;
        wcx.lpfnWndProc = EditWndProc;
        wcx.hInstance = m_hInst;

        wcx.lpszClassName = lpSuperClass;

        ::RegisterClassEx(&wcx);


        UEditSuper ueEdit(m_hDlg, 2222, m_hInst);
        ueEdit.setPos(20, 20, 500, 200);
        ueEdit.create();

        m_pUBtnOK = new UPushButton(m_hDlg, ID_BTN_OK, m_hInst);
        m_pUBtnOK->setPos(250, 260, 70, 40);
        m_pUBtnOK->create();
        m_pUBtnOK->setWindowText(_T("OK"));

        m_pUBtnCancel = new UPushButton(m_hDlg, ID_BTN_CANCEL, m_hInst);
        m_pUBtnCancel->setPos(330, 260, 70, 40);        
        m_pUBtnCancel->create();
        m_pUBtnCancel->setWindowText(_T("Cancel"));

        //
        UPushButton ubn_min(m_hDlg, ID_BTN_MIN, m_hInst);
        ubn_min.setPos(410, 260, 80, 40);
        ubn_min.create();
        ubn_min.setWindowText(_T("&Minimize"));

        ntd.cbSize = sizeof(NOTIFYICONDATA);
        ntd.hWnd = m_hDlg;
        ntd.uID = IDI_APP;
        ntd.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
        ntd.uCallbackMessage = WM_SHELLNOTIFY;
        ntd.hIcon = ::LoadIcon(m_hInst, MAKEINTRESOURCE(IDI_APP));
        lstrcpy( ntd.szTip, "hello");

        hPopupMenu = CreatePopupMenu();
        AppendMenu(hPopupMenu, MF_STRING, IDM_RESTORE, "Restore");
        AppendMenu(hPopupMenu, MF_STRING, IDM_EXIT, "Exit");
        return TRUE;
    }

    virtual BOOL DialogProc(UINT message, WPARAM wParam, LPARAM lParam)
    {
        BOOL result = UDialogBox::DialogProc(message, wParam, lParam);

        if (message == WM_COMMAND)
        {
            switch (wParam) {
                case ID_BTN_OK:
                    break;
                case ID_BTN_CANCEL:
                    this->onCancel();
                    break;
                case ID_BTN_MIN:
                    ::ShowWindow(m_hDlg, SW_MINIMIZE);
                    break;
                default:
                    break;
            }

            if (0 == lParam)
            {
                Shell_NotifyIcon(NIM_DELETE, &ntd);
                switch (wParam)
                {
                case IDM_RESTORE:
                    ShowWindow(m_hDlg, SW_RESTORE);
                    break;
                case IDM_EXIT:
                    return this->onDestroy();
                }

            }

        }

        if (message == WM_SIZE)
        {
            if (wParam == SIZE_MINIMIZED)
            {
                this->hide();
                Shell_NotifyIcon(NIM_ADD, &ntd);
            }
        }

        if (message == WM_SHELLNOTIFY)
        {
            if (wParam==IDI_APP)
            {
                if (lParam==WM_RBUTTONDOWN)
                {
                    POINT pt;
                    ::GetCursorPos(&pt);
                    ::SetForegroundWindow(m_hDlg);
                    ::TrackPopupMenu(hPopupMenu, TPM_RIGHTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, 0, m_hDlg, NULL);
                    ::PostMessage(m_hDlg, WM_NULL, 0, 0);
                }
                else if (lParam==WM_LBUTTONDBLCLK)
                {
                    //::SendMessage(m_hDlg, WM_COMMAND, IDM_RESTORE, 0);
                    this->sendMsg(WM_COMMAND, IDM_RESTORE);
                    ::SetForegroundWindow(m_hDlg);
                }
            }
        }



        return result;
    }

    virtual BOOL onPaint()
    {
        PAINTSTRUCT ps;
        HDC hdc;
        hdc = BeginPaint(m_hDlg, &ps);
        EndPaint(m_hDlg, &ps);
        return FALSE;
    }

    virtual BOOL onDestroy()
    {
        ::DestroyMenu(hPopupMenu);
        return UDialogBox::onDestroy();
    }
private:
    huys::ADT::UAutoPtr<UPushButton> m_pUBtnOK;
    huys::ADT::UAutoPtr<UPushButton> m_pUBtnCancel;
};

UDLGAPP_T(UDialogExt, IDD_TEST);


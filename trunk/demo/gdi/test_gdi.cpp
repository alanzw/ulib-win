#include "resource.h"

#include <windows.h>
#include <commctrl.h>
#include <tchar.h>
#include <shellapi.h>

#include "udialogx.h"
#include "udlgapp.h"
#include "ubutton.h"
#include "colors.h"
#include "ugdi.h"
#include "uregion.h"

#include "adt/uautoptr.h"

#define  WM_SHELLNOTIFY  WM_USER+5

NOTIFYICONDATA ntd;

using huys::UDialogBox;

HMENU hPopupMenu;

class UDialogExt : public UDialogBox
{
    enum {
        ID_BN_OK     = 5555,
        ID_BN_CANCEL = 5556,
        ID_BN_MIN    = 5557,
        IDM_RESTORE  = 5558,
        IDM_EXIT     = 5559
    };
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
    : UDialogBox(hInst, nID) {}

    virtual BOOL onInit()
    {
        m_pUBtnOK = new UPushButton(m_hDlg, ID_BN_OK, m_hInst);
        m_pUBtnOK->create();
        RECT rcBtn = {250, 260, 320, 300};
        m_pUBtnOK->setPosition(&rcBtn);
        m_pUBtnOK->setWindowText(_T("OK"));

        m_pUBtnCancel = new UPushButton(m_hDlg, ID_BN_CANCEL, m_hInst);
        m_pUBtnCancel->create();
        rcBtn.left += 90;
        rcBtn.right += 90;
        m_pUBtnCancel->setPosition(&rcBtn);
        m_pUBtnCancel->setWindowText(_T("Cancel"));

        //
        UPushButton ubn_min(m_hDlg, ID_BN_MIN, m_hInst);
        rcBtn.left += 90;
        rcBtn.right += 110;
        ubn_min.create();
        ubn_min.setWindowText(_T("&Minimize"));
        ubn_min.setPosition(&rcBtn);

        ntd.cbSize = sizeof(NOTIFYICONDATA);
        ntd.hWnd = m_hDlg;
        ntd.uID = IDI_APP;
        ntd.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
        ntd.uCallbackMessage = WM_SHELLNOTIFY;
        ntd.hIcon = ::LoadIcon(m_hInst, MAKEINTRESOURCE(IDI_HELP));
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
                case ID_BN_OK:
                    return onBnOk();
                    break;
                case ID_BN_CANCEL:
                    this->onCancel();
                    break;
                case ID_BN_MIN:
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
                ::TrackPopupMenu(hPopupMenu, TPM_RIGHTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, NULL, m_hDlg, NULL);
                ::PostMessage(m_hDlg, WM_NULL, 0, 0);
                }
                else if (lParam==WM_LBUTTONDBLCLK)
                {
                    ::SendMessage(m_hDlg, WM_COMMAND, IDM_RESTORE, 0);
                }
            }
        }



        return result;
    }

    virtual BOOL onPaint()
    {
        PAINTSTRUCT ps;
        HDC hdc;
        RECT rc;
        ::GetClientRect(m_hDlg, &rc);
        hdc = BeginPaint(m_hDlg, &ps);
        this->draw(hdc, rc);
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
private:
    void draw(HDC hdc, RECT &rc)
    {
        huys::ULine ul(rc.left, rc.top, rc.right, rc.bottom-100);
        ul.setLineColor(huys::blue);
        ul.setLineWidth(2);
        ul.Draw(hdc);

        huys::URectangle ur(50, 50, 200, 200);
        ur.setLineColor(huys::red);
        ur.setLineWidth(2);
        ur.setFilledColor(huys::orange);
        ur.setFilledStyle(BS_SOLID);
        ur.Draw(hdc);

        ur.moveCenterTo(300, 125);
        ur.Draw(hdc);

        ur.moveCenterTo(500, 125);
        ur.shrinkTo(40, 40);
        ur.Draw(hdc);

        ur.offset(0, 60);
        ur.Draw(hdc);

        huys::UTriangle utri(450, 180, 370, 240, 500, 250);
        utri.setLineColor(huys::babyblue);
        utri.setFilledColor(huys::babyblue);
        utri.setFilledStyle(BS_SOLID);
        utri.Draw(hdc);
    }

    BOOL onBnOk()
    {
        URegion ur;
        ur.createRect(20, 260, 80, 300);

        HDC hdc = GetDC(m_hDlg);
        HBRUSH hbr = (HBRUSH)::GetStockObject(BLACK_BRUSH);
        //ur.fill(hdc, hbr);

        URegion ur2;
        ur2.createRect(30, 270, 60, 280);
        ur2.combine(ur, RGN_XOR);
        //ur2.fill(hdc, hbr);
        ur2.frame(hdc, hbr, 2, 2);

        return FALSE;
    }
};

UDLGAPP_T(UDialogExt, IDD_TEST);

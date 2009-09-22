#include "resource.h"

#include <windows.h>
#include <commctrl.h>
#include <tchar.h>

#include "ulib.h"
#include "udialogx.h"
#include "udlgapp.h"
#include "uprogressbar.h"
#include "ubutton.h"

#include "colors.h"

//HINSTANCE g_hInst = 0;
const UINT ID_BTN_OK = 5555;
const UINT ID_BTN_CANCEL = 5556;

using huys::UDialogBox;

class UDialogExt : public UDialogBox
{
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
    : UDialogBox(hInst, nID) {}

    ~UDialogExt()
    {
        if (m_pUPB)
        {
            delete m_pUPB;
            m_pUPB = NULL;
        }

        if (m_pUBtnOK)
        {
            delete m_pUBtnOK;
            m_pUBtnOK = NULL;
        }

        if (m_pUBtnCancel)
        {
            delete m_pUBtnCancel;
            m_pUBtnCancel = NULL;
        }
    }

    virtual BOOL onInit()
    {
        m_pUPB = new UProgressBar(m_hDlg, 3333, m_hInst);
        m_pUPB->setStyles(PBS_SMOOTH);
        m_pUPB->create();
        RECT rc = {20, 20, 420, 120};
        m_pUPB->setPosition(&rc);

        m_pUPB->setRange(0, 200);
        m_pUPB->setStep(10);
 
        m_pUPB->setBKColor(huys::red);
        m_pUPB->setBarColor(huys::white);

        m_pUBtnOK = new UPushButton(m_hDlg, ID_BTN_OK, m_hInst);
        m_pUBtnOK->create();
        RECT rcBtn = {250, 260, 320, 300};
        m_pUBtnOK->setPosition(&rcBtn);
        m_pUBtnOK->setWindowText(_T("OK"));

        m_pUBtnCancel = new UPushButton(m_hDlg, ID_BTN_CANCEL, m_hInst);
        m_pUBtnCancel->create();
        rcBtn.left += 90;
        rcBtn.right += 90;
        m_pUBtnCancel->setPosition(&rcBtn);
        m_pUBtnCancel->setWindowText(_T("Cancel"));
        return TRUE;
    }

    virtual BOOL DialogProc(UINT message, WPARAM wParam, LPARAM lParam)
    {
        BOOL result = UDialogBox::DialogProc(message, wParam, lParam);

        if (message == WM_LBUTTONDOWN)
        {
            m_pUPB->stepIt();
            ::InvalidateRect(m_hDlg, NULL, TRUE);
        }

        if (message == WM_COMMAND)
        {
            switch (wParam) {
                case ID_BTN_OK:
                    m_pUPB->setPos(50);
                    ::InvalidateRect(m_hDlg, NULL, TRUE);
                    break;
                case ID_BTN_CANCEL:
                    this->onCancel();
                    break;
                default:
                    break;
            }
        }

        return result;
    }

    virtual BOOL onPaint()
    {
        PAINTSTRUCT ps;
        HDC hdc;
        char str[100] = "";
        int pos = 0;
        if (m_pUPB)
            pos = m_pUPB->getPos();
        wsprintf(str, "Current Pos: %d", pos);
        hdc = BeginPaint(m_hDlg, &ps);
        RECT rt;
        GetClientRect(m_hDlg, &rt);
        rt.top = 5;
        DrawText(hdc, str, strlen(str), &rt, DT_SINGLELINE|DT_CENTER|DT_VCENTER );
        EndPaint(m_hDlg, &ps);
        return FALSE;
    }
private:
    UProgressBar *m_pUPB;
    UPushButton *m_pUBtnOK;
    UPushButton *m_pUBtnCancel;
};

BEGIN_DLGAPP
  UDialogExt dlg(hInstance, IDD_TEST);
  dlg.create();
END_DLGAPP


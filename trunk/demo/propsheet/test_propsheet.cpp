#include "resource.h"

#include <windows.h>
#include <commctrl.h>
#include <tchar.h>

#include "ulib.h"
#include "udialogx.h"
#include "udlgapp.h"
#include "upropsheet.h"
#include "ubutton.h"
#include "udc.h"
#include "colors.h"

#include "adt/uautoptr.h"

const UINT ID_BTN_OK = 5555;
const UINT ID_BTN_CANCEL = 5556;

using huys::UDialogBox;

class UDialogExt : public UDialogBox
{
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
    : UDialogBox(hInst, nID) {}

    virtual BOOL onInit()
    {
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

        if (message == WM_COMMAND)
        {
            switch (wParam) {
                case ID_BTN_OK:
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
        UPaintDC dc(m_hDlg);


        return FALSE;
    }
private:
    huys::ADT::UAutoPtr<UPushButton> m_pUBtnOK;
    huys::ADT::UAutoPtr<UPushButton> m_pUBtnCancel;
};

UDLGAPP_T(UDialogExt, IDD_TEST);


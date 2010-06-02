#include "resource.h"

#include <windows.h>
#include <commctrl.h>
#include <tchar.h>
#include <prsht.h>
#include "ulib.h"
#include "udialogx.h"
#include "udlgapp.h"
#include "upropsheet.h"
#include "ubutton.h"
#include "udc.h"
#include "colors.h"

#include "adt/uautoptr.h"

#ifdef _MSC_VER

#pragma comment(lib, "comctl32.lib")

#endif // _MSC_VER

using huys::UDialogBox;

class UDialogExt : public UDialogBox
{
    enum {
        ID_BTN_OK = 5555,
        ID_BTN_CANCEL = 5556
    };
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
                    return this->onBnOK();
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
private:
    BOOL onBnOK()
    {

        PROPSHEETPAGE psp[2];
        PROPSHEETHEADER psh;

        psp[1].dwSize = sizeof(PROPSHEETPAGE);
        psp[1].dwFlags = PSP_USEICONID | PSP_USETITLE;

        psp[1].hInstance = m_hInst;
        psp[1].pszTemplate = (LPCTSTR)IDD_PROP;
        psp[1].pszIcon = MAKEINTRESOURCE(IDI_HELP);
        psp[1].pfnDlgProc = NULL;
        psp[1].pszTitle = "ºÏ²¢";
        psp[1].lParam =0;
        psp[1].pfnCallback = NULL;

        psp[0].dwSize = sizeof(PROPSHEETPAGE);
        psp[0].dwFlags = PSP_USEICONID | PSP_USETITLE|PSP_HASHELP;
        psp[0].hInstance =m_hInst;
        psp[0].pszTemplate = (LPCTSTR)IDD_PROP2;
        psp[0].pszIcon = MAKEINTRESOURCE(IDI_HELP);
        psp[0].pfnDlgProc =NULL;
        psp[0].pszTitle = "·Ö¸î";
        psp[0].lParam = 0;
        psp[0].pfnCallback = NULL;

        psh.dwSize = sizeof(PROPSHEETHEADER);
        psh.dwFlags = PSH_USEICONID | PSH_PROPSHEETPAGE;
        psh.hwndParent =m_hDlg;
        psh.hInstance = m_hInst;
        psh.pszIcon = MAKEINTRESOURCE(IDI_HELP);
        psh.pszCaption = (LPSTR)"split";
        psh.nPages = sizeof(psp) / sizeof(PROPSHEETPAGE);
        psh.nStartPage = 0;
        psh.ppsp = (LPCPROPSHEETPAGE) &psp;
        psh.pfnCallback =0;

        PropertySheet(&psh);

        return FALSE;
    }
};

UDLGAPP_T(UDialogExt, IDD_TEST);


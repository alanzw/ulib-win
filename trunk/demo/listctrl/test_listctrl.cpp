#include "resource.h"

#define _WIN32_IE 0x400

#include <windows.h>
#include <commctrl.h>
#include <tchar.h>

#include "ulistview.h"
#include "udialogx.h"
#include "udlgapp.h"
#include "uimagelist.h"
#include "umsg.h"

using huys::UDialogBox;

class UDialogExt : public UDialogBox
{
    enum {
        ID_LISTCTRL      = 13333,
        ID_LISTCTRL_ICON = 13334
    };
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
        : UDialogBox(hInst, nID),
        m_pListCtrl(0),
        m_pListCtrlIcon(0)
    {}

    ~UDialogExt()
    {
        CHECK_PTR(m_pListCtrl);
        CHECK_PTR(m_pListCtrlIcon);
    }

    virtual BOOL onInit()
    {
        m_pListCtrl = new UListView(m_hDlg, ID_LISTCTRL, m_hInst);
        m_pListCtrl->setStyles(LVS_REPORT | LVS_EDITLABELS);
        m_pListCtrl->create();
        RECT rc;
        ::GetClientRect(m_hDlg, &rc);
        rc.right = rc.left + 100;
        m_pListCtrl->setPosition(&rc);

        static UImageList uil(IDR_TOOLBAR1, m_hInst);
        HIMAGELIST himl = uil.getHandle();
        m_pListCtrl->setImageListNormal(himl);

        char str[] = "0_ÎÒ°®Äã";
        m_pListCtrl->addColTextWidth(0, str, 100);
        str[0]='1';
        m_pListCtrl->addItemTextImage(0, str, 1);

        //
        m_pListCtrlIcon = new UListView(m_hDlg, ID_LISTCTRL_ICON, m_hInst);
        m_pListCtrlIcon->setStyles( LVS_ICON );
        m_pListCtrlIcon->setPos(150, rc.top+10, 100, rc.bottom-rc.top-20);
        m_pListCtrlIcon->create();

        m_pListCtrlIcon->setImageListNormal(himl);
        m_pListCtrlIcon->addItemTextImage(0, str, 1);
        m_pListCtrlIcon->addItemTextImage(0, str, 1);
        return TRUE;
    }

    virtual BOOL DialogProc(UINT message, WPARAM wParam, LPARAM lParam)
    {
        BOOL result = UDialogBox::DialogProc(message, wParam, lParam);

        if (message == WM_NOTIFY)
        {
            LPNMHDR lpnmh = (LPNMHDR) lParam;
            char buffer[256];
            ZeroMemory(buffer, sizeof(buffer));
            switch (lpnmh->code)
            {
            case NM_CLICK:
                if (lpnmh->idFrom == ID_LISTCTRL_ICON)
                {
                    //showMsgFormat("Notify", "From ID: %d --- %s", lpnmh->idFrom, m_pListCtrl->getHitText(buffer));
                    //showCustomInfoMsg(m_pListCtrl->getHitText(buffer), m_hDlg);
                    //m_pListCtrl->getSelectedItem(buffer);
                    //showMsgFormat("Notify", "From ID: %d -- %s", lpnmh->idFrom, buffer);
                    LPNMITEMACTIVATE lpnmitem = (LPNMITEMACTIVATE) lParam;
                    showMsgFormat("Notify", "From nIndex %d", lpnmitem->iItem);
                }
                break;
            }
        }


        return result;
    }

private:
    UListView *m_pListCtrl;
    UListView *m_pListCtrlIcon;
};

UDLGAPP_T(UDialogExt, IDD_TEST);


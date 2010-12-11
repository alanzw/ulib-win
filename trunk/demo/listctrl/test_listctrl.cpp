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

#include "adt/uautoptr.h"

#include "custom_listctrl.h"

using huys::UDialogBox;

class UDialogExt : public UDialogBox
{
    enum {
        ID_LISTCTRL      = 13333,
        ID_LISTCTRL_ICON = 13334,
        ID_LIST_CUSTOM   = 13335
    };
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
        : UDialogBox(hInst, nID)
    {}

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


        m_pListCustom = new UCustomListView(m_hDlg, ID_LIST_CUSTOM, m_hInst);
        m_pListCustom->setPos(280, rc.top+10, 450, 450);
        m_pListCustom->setStyles(LVS_REPORT);
        m_pListCustom->create();
		m_pListCustom->setExStylesListView(LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES|LVS_EX_FULLROWSELECT);

        m_pListCustom->addColTextWidth(0, str, 100);
        m_pListCustom->addColTextWidth(1, str, 100);
        m_pListCustom->addColTextWidth(2, str, 100);
        m_pListCustom->addColTextWidth(3, str, 100);
        m_pListCustom->addItemTextImage(0, str, 1);
        m_pListCustom->addItemTextImage(1, str, 1);
        m_pListCustom->addItemTextImage(2, str, 1);
        m_pListCustom->addItemTextImage(3, str, 1);
        m_pListCustom->addItemTextImage(4, str, 1);
		m_pListCustom->addItemTextImage(5, str, 1);
        m_pListCustom->setItemText(0, 1, str);
        m_pListCustom->setItemText(0, 2, str);
        m_pListCustom->setItemText(0, 3, str);
		m_pListCustom->setItemText(1, 1, str);
		m_pListCustom->setItemText(1, 2, str);
		m_pListCustom->setItemText(1, 3, "<=");

        //TCHAR buf[512];
        //::GetCurrentDirectory(511, buf);
        //lstrcat(buf, _T("\\back.jpg"));
        //m_pListCustom->setBKImage(buf);

        return TRUE;
    }

    virtual BOOL onNotify(WPARAM wParam, LPARAM lParam)
    {
        LPNMHDR lpnmh = (LPNMHDR) lParam;
        //char buffer[256];
        //::ZeroMemory(buffer, sizeof(buffer));
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
        return UDialogBox::onNotify(wParam, lParam);
    }

	BOOL onCommand(WPARAM wParam, LPARAM lParam)
	{
		switch (LOWORD (wParam))
		{
		case IDOK:
            {
                this->m_pListCustom->endEdit();
                return FALSE;
            }
		case IDCANCEL:
			{
				::SendMessage((HWND)lParam, WM_CHAR, VK_RETURN,0);
				return FALSE;
			}
		default:
			return UDialogBox::onCommand(wParam, lParam);
		}
	}
private:
    huys::ADT::UAutoPtr<UListView> m_pListCtrl;
    huys::ADT::UAutoPtr<UListView> m_pListCtrlIcon;

    huys::ADT::UAutoPtr<UCustomListView> m_pListCustom;
};

UDLGAPP_T(UDialogExt, IDD_TEST);


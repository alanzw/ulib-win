#include "resource.h"

#define _WIN32_IE 0x0300

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "utreeview.h"
#include "udialogx.h"
#include "udlgapp.h"
#include "uimagelist.h"
#include "umsg.h"

#include "adt/uautoptr.h"

using huys::UDialogBox;

class UDialogExt : public UDialogBox
{
    enum {
        ID_TREECTRL = 13333
    };
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
     : UDialogBox(hInst, nID)
    {}

    virtual BOOL onInit()
    {
        RECT rc;
        ::GetClientRect(m_hDlg, &rc);

        rc.right = rc.left + 200;

        m_pTreeCtrl = new UTreeView(m_hDlg, ID_TREECTRL, m_hInst);
        m_pTreeCtrl->setStyles(WS_BORDER|TVS_HASLINES| TVS_LINESATROOT | TVS_HASBUTTONS|TVS_CHECKBOXES);
        m_pTreeCtrl->setRect(&rc);
        m_pTreeCtrl->create();

        m_uil = new UImageList(IDR_TOOLBAR1, m_hInst);
        m_pTreeCtrl->setNormalImageList(m_uil);

        char str[] = "0_ÎÒ°®Äã";

        HTREEITEM item = m_pTreeCtrl->addTextRoot(str);

        str[0] = '1';
        item = m_pTreeCtrl->addTextChild(item, str);
        str[0] = '2';
        item = m_pTreeCtrl->addTextChild(item, str);

        str[0] = '0';
        item = m_pTreeCtrl->addTextRoot(str);
        item = m_pTreeCtrl->addTextRoot(str);

        m_pTreeCtrl->subclassProc();

        m_pChildDlg = new UDialogBox(m_hInst, IDD_CHILD, UDialogBox::DefaultDlgProc, m_hDlg);
        m_pChildDlg->create();
        m_pChildDlg->hide();

        return TRUE;
    }

    BOOL onNotify(WPARAM wParam, LPARAM lParam)
    {
        LPNMHDR lpnmh = (LPNMHDR) lParam;
        char buffer[256];
        ::ZeroMemory(buffer, sizeof(buffer));
        if (lpnmh->idFrom == ID_TREECTRL)
        {
            switch (lpnmh->code)
            {
            case NM_CLICK:
                this->onTreeCtrlClick(buffer);
                break;
            case TVN_BEGINDRAG:
                this->onBeginDrag((LPNMTREEVIEW)lParam);
                break;
            }
        }
        return UDialogBox::onNotify(wParam, lParam);
    }

protected:
    BOOL onTreeCtrlClick(char *pszText)
    {
        //showMsgFormat("Notify", "From ID: %d --- %s", lpnmh->idFrom, m_pTreeCtrl->getHitText(buffer));
        showCustomInfoMsg(m_pTreeCtrl->getSelectedItem(pszText), m_hDlg);
        //m_pTreeCtrl->getSelectedItem(buffer);
        //showMsgFormat("Notify", "From ID: %d -- %s", lpnmh->idFrom, buffer);
        RECT rc;
        ::GetClientRect(m_hDlg, &rc);
        rc.left += 200;

        if (m_pChildDlg != 0)
        {
            ::MoveWindow(m_pChildDlg->getHWND(), rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, TRUE);
            m_pChildDlg->show();
        }

        return 0;
    }

    BOOL onBeginDrag(LPNMTREEVIEW lpntv)
    {
/*
        HIMAGELIST himl;    // handle to image list
        RECT rcItem;        // bounding rectangle of item
        DWORD dwLevel;      // heading level of item
        DWORD dwIndent;     // amount that child items are indented

        // Tell the tree-view control to create an image to use
        // for dragging.
        himl = TreeView_CreateDragImage(hwndTV, lpnmtv->itemNew.hItem);

        // Get the bounding rectangle of the item being dragged.
        TreeView_GetItemRect(hwndTV, lpnmtv->itemNew.hItem, &rcItem, TRUE);

        // Get the heading level and the amount that the child items are
        // indented.
        dwLevel = lpnmtv->itemNew.lParam;
        dwIndent = (DWORD)SendMessage(hwndTV, TVM_GETINDENT, 0, 0);

        // Start the drag operation.
        ImageList_BeginDrag(himl, 0, 0, 0);
        ImageList_DragEnter(hwndTV, lpnmtv->pt.x, lpnmtv->pt.x);

        // Hide the mouse pointer, and direct mouse input to the
        // parent window.
        ShowCursor(FALSE);
        SetCapture(GetParent(hwndTV));
        g_fDragging = TRUE;
*/
        return 0;
    }
private:
    huys::ADT::UAutoPtr<UTreeView> m_pTreeCtrl;
    huys::ADT::UAutoPtr<UDialogBox> m_pChildDlg;
    huys::ADT::UAutoPtr<UImageList> m_uil;
};

UDLGAPP_T(UDialogExt, IDD_TEST);

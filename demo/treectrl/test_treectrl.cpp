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

using huys::UDialogBox;

const UINT ID_TREECTRL = 13333;

class UDialogExt : public UDialogBox
{
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
        : UDialogBox(hInst, nID),
        m_pTreeCtrl(0)
    {}

    ~UDialogExt()
    {
        CHECK_PTR(m_pTreeCtrl);
        CHECK_PTR(m_pChildDlg);
    }

    virtual BOOL onInit()
    {

            m_pTreeCtrl = new UTreeView(m_hDlg, ID_TREECTRL, m_hInst);
            m_pTreeCtrl->setStyles(WS_BORDER|TVS_HASLINES| TVS_LINESATROOT | TVS_HASBUTTONS|TVS_CHECKBOXES);
            m_pTreeCtrl->create();
            RECT rc;
            ::GetClientRect(m_hDlg, &rc);
            rc.right = rc.left + 200;
            m_pTreeCtrl->setPosition(&rc);
            static UImageList uil(IDR_TOOLBAR1, m_hInst);
            HIMAGELIST himl = uil.getHandle();
            m_pTreeCtrl->setNormalImageList(himl);

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
    }

    virtual BOOL DialogProc(UINT message, WPARAM wParam, LPARAM lParam)
    {
        BOOL result = UDialogBox::DialogProc(message, wParam, lParam);

        if (message == WM_NOTIFY)
        {
            LPNMHDR lpnmh = (LPNMHDR) lParam;
            char buffer[256];
            ZeroMemory(buffer, sizeof(buffer));
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
        }


        return result;
    }

    virtual BOOL onPaint()
    {
        PAINTSTRUCT ps;
        HDC hdc;
        hdc = BeginPaint(m_hDlg, &ps);
        RECT rt;
        GetClientRect(m_hDlg, &rt);
        rt.top = 5;
        //DrawText(hdc, "Hello World!", strlen("Hello World!"), &rt, DT_SINGLELINE|DT_CENTER|DT_VCENTER );
        EndPaint(m_hDlg, &ps);
        return FALSE;
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

        if (m_pChildDlg)
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
    UTreeView *m_pTreeCtrl;

    UDialogBox *m_pChildDlg;
};

UDLGAPP_T(UDialogExt, IDD_TEST);

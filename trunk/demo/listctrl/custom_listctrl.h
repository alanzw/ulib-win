#ifndef U_CUSTOM_LISTCTRL_H
#define U_CUSTOM_LISTCTRL_H

#include "ulistview.h"
#include "colors.h"

class UCustomListView : public UListView
{
public:
    UCustomListView(HWND hParent, UINT nID, HINSTANCE hInst=::GetModuleHandle(NULL))
        : UListView(hParent, nID, hInst)
    {

    }

    ~UCustomListView()
    {

    }

    BOOL create()
    {
        return  (UListView::create() && subclassProc());
    }

    BOOL doCustomDraw(LPNMLVCUSTOMDRAW lplvcd)
    {
        if (NULL == lplvcd)
        {
            return FALSE;
        }
        // 1
        if (lplvcd->nmcd.dwDrawStage == CDDS_PREPAINT)
        {
            ::SetWindowLong(m_hParent, DWL_MSGRESULT, (LONG)(CDRF_NOTIFYITEMDRAW));
        }

        // 2
        else if (lplvcd->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
        {
            ::SetWindowLong(m_hParent, DWL_MSGRESULT, (LONG)(CDRF_NOTIFYSUBITEMDRAW));
        }
        // 3
        else if (lplvcd->nmcd.dwDrawStage &  (CDDS_SUBITEM | CDDS_PREPAINT))
        {
            // row colors
            if (lplvcd->nmcd.dwItemSpec % 2 == 0)
                lplvcd->clrTextBk = huys::orange;
            else
                lplvcd->clrTextBk = huys::pink;

            // subitem 0
            if (lplvcd->iSubItem == 0)
                lplvcd->clrTextBk = RGB(0xA6, 0xCA, 0xF0);

            // subitem colors
            if (lplvcd->iSubItem % 3 == 0)
                lplvcd->clrText = huys::red;
            else
                lplvcd->clrText = huys::white;

            ::SetWindowLong(m_hParent, DWL_MSGRESULT, (LONG)(CDRF_NEWFONT));
        }
        else
        {
            ::SetWindowLong(m_hParent, DWL_MSGRESULT, (LONG)(CDRF_DODEFAULT));
        }

        return TRUE;
    }

    BOOL onNotifyReflect(WPARAM wParam, LPARAM lParam)
    {
        if ((UINT)NM_CUSTOMDRAW == ((LPNMHDR)lParam)->code)
        {
            return doCustomDraw((LPNMLVCUSTOMDRAW) lParam);
        }
        return UListView::onNotifyReflect(wParam, lParam);
    }
private:

};

#endif // U_CUSTOM_LISTCTRL_H

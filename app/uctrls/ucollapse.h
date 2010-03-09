#ifndef U_COLLAPSE_H
#define U_COLLAPSE_H

#include "ubutton.h"
#include "ubasewindow.h"
#include "colors.h"
#include "umsg.h"

#include "adt/uvector.h"

class UCollapseGroupBox : public UOwnerDrawnButton
{
    typedef huys::ADT::UVector<HWND> USubCtrls;
public:
    UCollapseGroupBox(HWND hParent, UINT nID, HINSTANCE hInst)
    : UOwnerDrawnButton(hParent, nID, hInst), m_bExpand(TRUE), m_bMoveMarked(FALSE)
    {
        setStyles(SWP_NOREPOSITION|WS_CLIPSIBLINGS);
    }

    UCollapseGroupBox(UBaseWindow *pWndParent, UINT nID)
    : UOwnerDrawnButton(pWndParent, nID), m_bExpand(TRUE), m_bMoveMarked(FALSE)
    {
        setStyles(SWP_NOREPOSITION|WS_CLIPSIBLINGS);
    }

    UCollapseGroupBox()
        : m_bExpand(TRUE), m_bMoveMarked(FALSE)
    {}

    ~UCollapseGroupBox()
    {}


    virtual BOOL create()
    {
        BOOL bRet = UControl::create(_T("BUTTON"), m_sTitle);
        bRet &= this->subclassProc();
        ::GetWindowRect(m_hSelf, &m_rcWindow);
        ::ScreenToClient(m_hParent, (LPPOINT)&m_rcWindow);
        ::ScreenToClient(m_hParent, ((LPPOINT)&m_rcWindow)+1);
        return bRet;
    }

    void setTitle(LPCTSTR lpTitle)
    {
        lstrcpy(m_sTitle, lpTitle);
    }

    virtual BOOL onSize(WPARAM wParam, LPARAM lParam)
    {
        //::GetWindowRect(m_hSelf, &m_rcWindow);
        return FALSE;
    }

    virtual BOOL onMove(WPARAM wParam, LPARAM lParam)
    {
        //::GetWindowRect(m_hSelf, &m_rcWindow);
        return FALSE;
    }

    virtual BOOL onDrawItem(WPARAM wParam, LPARAM lParam)
    {
        LPDRAWITEMSTRUCT lpDrawItem;
        HDC hdc;
        HBRUSH hbrush = ::CreateSolidBrush(huys::aliceblue);
        HBRUSH hbTitle = ::CreateSolidBrush(huys::blue);

        lpDrawItem = (LPDRAWITEMSTRUCT) lParam;
        hdc = lpDrawItem->hDC;

        HPEN hPen = ::CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_3DSHADOW));
        ::SetTextColor( hdc, ::GetSysColor( COLOR_HIGHLIGHTTEXT ) );
        ::SetBkColor( hdc, huys::orange );
        HPEN hOldPen = (HPEN)::SelectObject(hdc, hPen);
        HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdc, hbrush);
        //::FillRect( hdc, &lpDrawItem->rcItem, hbrush );
        ::Rectangle(hdc, lpDrawItem->rcItem.left, lpDrawItem->rcItem.top, lpDrawItem->rcItem.right, lpDrawItem->rcItem.bottom);
        ::SelectObject(hdc, hOldPen);
        ::SelectObject(hdc, hOldBrush);

        SIZE size;
        ::GetTextExtentPoint32(hdc, m_sTitle, lstrlen(m_sTitle), &size);

        ::SetTextColor( hdc, huys::white );
        ::SetBkColor( hdc, huys::blue );
        RECT rcTitle = {
            lpDrawItem->rcItem.left,
            lpDrawItem->rcItem.top,
            lpDrawItem->rcItem.right,
            lpDrawItem->rcItem.top + size.cy + 4};

        m_rcTitle = rcTitle;
        m_rcTitleSelf = rcTitle;
        ::ClientToScreen(m_hSelf, (LPPOINT)&m_rcTitle);
        ::ClientToScreen(m_hSelf, ((LPPOINT)&m_rcTitle)+1);
        ::ScreenToClient(m_hParent, (LPPOINT)&m_rcTitle);
        ::ScreenToClient(m_hParent, ((LPPOINT)&m_rcTitle)+1);

        ::FillRect( hdc, &rcTitle, hbTitle);
        ::TextOut( hdc,
                   lpDrawItem->rcItem.left+5,
                   lpDrawItem->rcItem.top+2,
                   m_sTitle,
                   lstrlen(m_sTitle)
                 );

        ::SetTextColor( hdc, huys::darkmagenta );
        ::SetBkColor( hdc, huys::aliceblue );
        ::TextOut( hdc,
                   lpDrawItem->rcItem.left+5,
                   lpDrawItem->rcItem.top+size.cy + 4+5,
                   _T("OwnerDraw"),
                   _tcslen(_T("OwnerDraw"))
                 );

        //InvalidateRect(m_hParent, &m_rcWindow, TRUE);
        UpdateWindow(m_hParent);

        return TRUE; //

    }

    virtual BOOL subclass(UINT nID, HWND hParent)
    {
        BOOL bRet = UControl::subclass(nID, hParent);
        bRet &= this->modifyStyles(BS_OWNERDRAW|SWP_NOREPOSITION|WS_CLIPSIBLINGS);
        this->invalidate(TRUE);
        ::GetWindowRect(m_hSelf, &m_rcWindow);
        ::ScreenToClient(m_hParent, (LPPOINT)&m_rcWindow);
        ::ScreenToClient(m_hParent, ((LPPOINT)&m_rcWindow)+1);
        return bRet;
    }

    BOOL onLButtonDown(WPARAM wParam, LPARAM lParam)
    {
        //RECT rc;
        //GetWindowRect(m_hSelf, &rc);
        POINT pt = {
            GET_X_LPARAM(lParam),
            GET_Y_LPARAM(lParam)
        };

        if (PtInRect(&m_rcTitleSelf, pt))
        {
        /*
            if (m_bExpand)
            {
                this->setPosition(&m_rcTitle);
                m_bExpand = FALSE;
            }
            else
            {
                this->setPosition(&m_rcWindow);
                m_bExpand = TRUE;
            }
        */
            m_bMoveMarked = TRUE;
            //::SetCapture(m_hSelf);

            m_ptStart.x = pt.x;
            m_ptStart.y = pt.y;

            updateSubCtrl();
        }
        else
        {
            ::ClientToScreen(m_hSelf, (LPPOINT)&pt);
            ::ScreenToClient(m_hParent, (LPPOINT)&pt);
            ::SendMessage(m_hParent, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(pt.x, pt.y));
        }


        return TRUE;
    }

    BOOL onLButtonUp(WPARAM wParam, LPARAM lParam)
    {
        if (m_bMoveMarked)
        {
            m_bMoveMarked = FALSE;
            //::ReleaseCapture();
        }
    }

    BOOL onMouseMove(WPARAM wParam, LPARAM lParam)
    {
        RECT rc;
        TRACKMOUSEEVENT tme;

        this->getClientRect(&rc);

        POINT pt = {
            GET_X_LPARAM(lParam),
            GET_Y_LPARAM(lParam)
        };

        if (m_bMoveMarked && MK_LBUTTON == wParam)
        {
            rc.left += pt.x - m_ptStart.x;
            rc.right += pt.x - m_ptStart.x;
            rc.top += pt.y - m_ptStart.y;
            rc.bottom += pt.y - m_ptStart.y;

            clientToDialog(m_hParent, &rc);

            this->setPosition(&rc);
        }

    }


    void addSubCtrl(HWND hWnd)
    {
        m_subctrls.push_back(hWnd);
    }

    void updateSubCtrl()
    {
        if(m_bExpand)
        {
            for (USubCtrls::const_iterator it = m_subctrls.begin(); it != m_subctrls.end(); ++it)
            {
                ::EnableWindow(*it, TRUE);
                ::ShowWindow(*it, TRUE);
            }
        }
        else
        {
            for (USubCtrls::const_iterator it = m_subctrls.begin(); it != m_subctrls.end(); ++it)
            {
                ::EnableWindow(*it, FALSE);
                ::ShowWindow(*it, FALSE);
            }
        }
    }

    void updateWindowSize()
    {
        ::GetWindowRect(m_hSelf, &m_rcWindow);
        ::ScreenToClient(m_hParent, (LPPOINT)&m_rcWindow);
        ::ScreenToClient(m_hParent, ((LPPOINT)&m_rcWindow)+1);
    }
private:
    TCHAR m_sTitle[256];
    RECT m_rcTitle;
    RECT m_rcWindow;
    RECT m_rcTitleSelf;
    BOOL m_bExpand;

    USubCtrls m_subctrls;

    BOOL m_bMoveMarked;
    POINT m_ptStart;
};

#endif // U_COLLAPSE_H

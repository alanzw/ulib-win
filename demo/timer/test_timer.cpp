#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include <cstdio>
#include <climits>

#include "udialogx.h"
#include "udlgapp.h"
#include "uicon.h"
#include "ubutton.h"
#include "ustatic.h"
#include "utimer.h"
#include "umsg.h"
#include "colors.h"

#include "adt/uautoptr.h"

class UColorStatic : public UStatic
{
    enum {
        ID_TIMER_INTERNAL = 22
    };
public:
    UColorStatic(HWND hParent, UINT nResource, HINSTANCE hInst)
        : UStatic(hParent, nResource, hInst)
    {
        m_dwStyles &= ~SS_SIMPLE;
        m_dwStyles |= SS_CENTER | SS_CENTERIMAGE;

        m_clr = huys::red;
        m_hbrush = ::CreateSolidBrush(m_clr);
        m_bFlag = TRUE;
    }

    ~UColorStatic()
    {
        this->killTimer(ID_TIMER_INTERNAL);
    }

    virtual BOOL create()
    {
        BOOL bRet = UStatic::create();
        this->subclassProc();
        this->setTimer(ID_TIMER_INTERNAL, 500);
        return  bRet;
    }

    virtual BOOL onTimer(WPARAM wParam, LPARAM lParam)
    {
        switch (wParam)
        {
        case ID_TIMER_INTERNAL:
            {
                if (m_bFlag)
                {
                    m_clr = huys::red;
                }
                else
                {
                    m_clr = huys::blue;
                }
                m_bFlag = !m_bFlag;
                ::DeleteObject(m_hbrush);
                m_hbrush = ::CreateSolidBrush(m_clr);
                this->invalidate();
                return FALSE;
            }
        default:
            ;
        }
        return FALSE;
    }

    virtual BOOL onCtrlColor(WPARAM wParam, LPARAM lParam)
    {
        HDC hdc = (HDC)wParam;
        //    HWND hwnd = (HWND)lParam;
        //    RECT rc;

        //::SelectObject(hdc, m_hbrush);
        ::SetTextColor(hdc, huys::white);
        ::SetBkColor(hdc, m_clr);

        return (BOOL)m_hbrush;
    }

    BOOL onMessage( UINT message, WPARAM wParam, LPARAM lParam )
    {
        switch(message)
        {
        case WM_ERASEBKGND:
            return this->onEraseBackground(wParam, lParam);
        default:
            return UStatic::onMessage(message, wParam, lParam);
        }
    }

    BOOL onEraseBackground(WPARAM wParam, LPARAM lParam)
    {
        HDC hdc = (HDC)wParam;
        RECT rc;
        this->getClientRect(&rc);
        HBRUSH holdbrush = (HBRUSH)::SelectObject(hdc, m_hbrush);
        ::Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
        ::SelectObject(hdc, holdbrush);

        return TRUE;
    }
private:
    HBRUSH m_hbrush;
    huys::Color m_clr;
    BOOL m_bFlag;
};

using huys::UDialogBox;

class UDialogExt : public UDialogBox
{
    enum {
        IDC_BN_GO    = 1021,
        IDC_ST_TIMER = 1022,
        IDC_BN_TIMER = 1023,
        ID_TIMER_EVENT = 1333
    };
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
        : UDialogBox(hInst, nID),
          m_nCount(0)
    {}

    ~UDialogExt()
    {}

    virtual BOOL onInit()
    {
        ::InvalidateRect(m_hDlg, NULL, TRUE);

        m_pBnGo = new UButton(m_hDlg, IDC_BN_GO, m_hInst);
        m_pBnGo->create();
        m_pBnGo->setWindowText(_T("GO"));

        RECT rc = { 200, 100, 380, 150};
        m_pBnGo->setPosition(&rc);


        m_pStTimer = new UColorStatic(m_hDlg, IDC_ST_TIMER, m_hInst);
        m_pStTimer->create();
        rc.bottom -= 70;
        rc.top -= 70;
        m_pStTimer->setPosition(&rc);
        m_pStTimer->setWindowText(_T("0.00"));

        m_pBnTimer = new UButton(m_hDlg, IDC_BN_TIMER, m_hInst);
        m_pBnTimer->create();
        m_pBnTimer->setWindowText(_T("Timer"));
        rc.top += 150;
        rc.bottom += 150;
        m_pBnTimer->setPosition(&rc);

        return TRUE;
    }

    virtual BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch (LOWORD(wParam))
        {
        case IDC_BN_GO:
            return onBnGo();
        case IDC_BN_TIMER:
            return onBnTimer();
        default:
            return UDialogBox::onCommand(wParam, lParam);
        }
    }

    BOOL onTimer(WPARAM wParam, LPARAM lParam)
    {
        char title[128] = {0};
        switch (wParam)
        {
        case ID_TIMER_EVENT:
            {
                if (m_nCount < INT_MAX)
                {
                    ++m_nCount;
                }
                sprintf(title, "%d", m_nCount);
                ::SetWindowText(m_hDlg, title);
            }
        default:
            ;
        }

        return FALSE;
    }

protected:
private:
    huys::ADT::UAutoPtr<UButton> m_pBnGo;
    huys::ADT::UAutoPtr<UButton> m_pBnTimer;
    huys::ADT::UAutoPtr<UStatic> m_pStTimer;

    UTimer m_utr;

    int m_nCount;

    BOOL onBnGo()
    {
        m_pBnGo->disable();
        m_utr.begin();

        Sleep(1999);

        m_utr.stop();

        char buf[128] = {0};
        double ms =  m_utr.getSeconds();
        sprintf(buf, "%.2lf", ms);
        //showMsg(buf, "xx", m_hDlg);
        m_pStTimer->setWindowText(buf);
        //m_pStTimer->invalidate(TRUE);
        m_pBnGo->enable();

        this->killTimer(ID_TIMER_EVENT);
        m_pBnTimer->enable();

        return FALSE;
    }

    BOOL onBnTimer()
    {
        this->setTimer(ID_TIMER_EVENT, 500);
        m_pBnTimer->disable();
        return FALSE;
    }
};

UDLGAPP_T(UDialogExt, IDD_TEST);


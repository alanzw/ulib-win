#ifndef PULSE_BUTTON_H_INCLUDED
#define PULSE_BUTTON_H_INCLUDED

#include "ubutton.h"
#include "utimer.h"

class UPulseButton : public UOwnerDrawnButton
{
    enum {
		ID_TIMER_INTERNAL = 22
	};
public:
    UPulseButton(HWND hParent, UINT nResource, HINSTANCE hInst)
    : UOwnerDrawnButton(hParent, nResource, hInst)
    {
    }

    ~UPulseButton()
    {
        this->killTimer(ID_TIMER_INTERNAL);
    }

    virtual BOOL create()
	{
		BOOL bRet = UOwnerDrawnButton::create();
		this->setTimer(ID_TIMER_INTERNAL, 500);
		return  bRet;
	}

    virtual BOOL onDrawItem(WPARAM wParam, LPARAM lParam)
    {
        LPDRAWITEMSTRUCT lpDrawItem;
        HDC hdc;

        lpDrawItem = (LPDRAWITEMSTRUCT) lParam;
        hdc = lpDrawItem->hDC;

        RECT rc = lpDrawItem->rcItem;

        return TRUE; // Message is handled.
    }

    virtual BOOL onLButtonDown(WPARAM wParam, LPARAM lParam)
    {
        //this->setStatus(BNST_DOWN);
        ::SetCapture(m_hSelf);
        ::InvalidateRect(m_hSelf, NULL, TRUE);
        return TRUE;
    }

    virtual BOOL onLButtonUp(WPARAM wParam, LPARAM lParam)
    {
        //this->setStatus(BNST_NORMAL);
        ::ReleaseCapture();
        ::InvalidateRect(m_hSelf, NULL, TRUE);

        UOwnerDrawnButton::onLButtonUp(wParam, lParam);

        RECT rc;
        this->getClientRect(&rc);

        POINT pt = {
            GET_X_LPARAM(lParam),
            GET_Y_LPARAM(lParam)
        };

        //if ( m_bsPrev == BNST_DOWN && PtInRect(&rc, pt))
        //    ::SendMessage(::GetParent(m_hSelf), WM_COMMAND, MAKEWPARAM(::GetDlgCtrlID(m_hSelf), 0), (LPARAM) m_hSelf);

        return TRUE;
    }

    virtual BOOL onMouseMove(WPARAM wParam, LPARAM lParam)
    {
        RECT rc;
        TRACKMOUSEEVENT tme;

        this->getClientRect(&rc);

        POINT pt = {
            GET_X_LPARAM(lParam),
            GET_Y_LPARAM(lParam)
        };

        //if (m_bsNow == BNST_DOWN)
        {
            if ( PtInRect(&rc, pt) )
            {
                //SetStatus(DOWN);
                //this->setStatus(BNST_DOWN);
                ::InvalidateRect(m_hSelf, NULL, TRUE);
            }
            else
            {
                //this->setStatus(BNST_NORMAL);
                ::InvalidateRect(m_hSelf, NULL, TRUE);
            }
        }
        //else if (m_bsPrev == BNST_DOWN && m_bsNow == BNST_HOVER)
        {
            //this->setStatus(BNST_DOWN);
            ::InvalidateRect(m_hSelf, NULL, TRUE);
        }
        //else
        {
            if (PtInRect(&rc, pt))
            {
                //GetParent()->SetWindowText("END HOVER");
                //this->setStatus(BNST_HOVER);
                ::InvalidateRect(m_hSelf, NULL, TRUE);
            }
            else
            {
                //GetParent()->SetWindowText("END NORMAL");
                //this->setStatus(BNST_NORMAL);
                //Invalidate();
            }

        }


        //UOwnerDrawnButton::onMouseMove(wParam, lParam);


        tme.cbSize = sizeof(TRACKMOUSEEVENT);
        tme.dwFlags = TME_LEAVE;
        tme.hwndTrack = m_hSelf;

        TrackMouseEvent(&tme);

        return TRUE;
    }

    virtual BOOL onMessage(UINT nMessage, WPARAM wParam, LPARAM lParam)
    {
        if (WM_MOUSELEAVE == nMessage)
            return this->onMouseLeave(wParam, lParam);
        else
            return UOwnerDrawnButton::onMessage(nMessage, wParam, lParam);
    }

    BOOL onMouseLeave(WPARAM wParam, LPARAM lParam)
    {
        //this->setStatus(BNST_NORMAL);
        ::InvalidateRect(m_hSelf, NULL, TRUE);

        return TRUE;
    }

    BOOL onTimer(WPARAM wParam, LPARAM lParam)
	{
		switch (wParam)
		{
		case ID_TIMER_INTERNAL:
			{
				return FALSE;
			}
		default:
			return FALSE;
		}
	}
private:
    void drawText()
    {

    }

    void drawPulse()
    {

    }

    void drawCenter()
    {

    }

    void drawBorder()
    {

    }
};

#endif // PULSE_BUTTON_H_INCLUDED

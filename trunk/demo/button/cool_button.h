#ifndef U_COOL_BUTTON_H
#define U_COOL_BUTTON_H

class UCoolButton : public UOwnerDrawnButton
{
public:
    typedef enum tagButtonStatus {
        BNST_NORMAL,
        BNST_HOVER,
        BNST_DOWN
    } ButtonStatus;
public:
    UCoolButton(HWND hParent, UINT nResource, HINSTANCE hInst)
    : UOwnerDrawnButton(hParent, nResource, hInst),
      m_bsNow(BNST_NORMAL), m_bsPrev(BNST_NORMAL)
    {
    }

    virtual ~UCoolButton()
    {
        CHECK_PTR(m_pBmpNormal);
        CHECK_PTR(m_pBmpHover);
        CHECK_PTR(m_pBmpDown);
    }

    void attachBitmap()
    {
        m_pBmpNormal = new UBitmap(IDB_NORMAL, m_hInstance);
        m_pBmpHover = new UBitmap(IDB_HOVER, m_hInstance);
        m_pBmpDown = new UBitmap(IDB_DOWN, m_hInstance);
    }

	void setBtnPos(int x, int y)
	{
		RECT rc = {
			x, 
			y,
			x + m_pBmpNormal->getWidth(),
			y + m_pBmpNormal->getHeight()
		};
		this->setPosition(&rc);
	}

    virtual BOOL onDrawItem(WPARAM wParam, LPARAM lParam)
    {
        LPDRAWITEMSTRUCT lpDrawItem;
        HDC hdc;

        lpDrawItem = (LPDRAWITEMSTRUCT) lParam;
        hdc = lpDrawItem->hDC;

        RECT rc = lpDrawItem->rcItem;

        switch (m_bsNow)
        {
        case BNST_HOVER:
            m_pBmpHover->show(hdc, rc);
            break;
        case BNST_DOWN:
            m_pBmpDown->show(hdc, rc);
            break;
        case BNST_NORMAL:
        default:
            m_pBmpNormal->show(hdc, rc);
        }

        //m_pBmpNormal->show(hdc, rc);

        return TRUE; // Message is handled.
    }

    virtual BOOL onLButtonDown(WPARAM wParam, LPARAM lParam)
    {
        this->setStatus(BNST_DOWN);
        ::SetCapture(m_hSelf);

        return TRUE;
    }

    virtual BOOL onLButtonUp(WPARAM wParam, LPARAM lParam)
    {
        this->setStatus(BNST_NORMAL);
        ::ReleaseCapture();
        ::InvalidateRect(m_hSelf, NULL, TRUE);

        UOwnerDrawnButton::onLButtonUp(wParam, lParam);

        RECT rc;
        this->getClientRect(&rc);

        POINT pt = {
            GET_X_LPARAM(lParam),
            GET_Y_LPARAM(lParam)
        };

        if ( m_bsPrev == BNST_DOWN && PtInRect(&rc, pt))
            ::SendMessage(::GetParent(m_hSelf), WM_COMMAND, MAKEWPARAM(::GetDlgCtrlID(m_hSelf), 0), (LPARAM) m_hSelf);

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

        if (m_bsNow == BNST_DOWN)
        {
            if ( PtInRect(&rc, pt) )
            {
                //SetStatus(DOWN);
            }
            else
            {
                this->setStatus(BNST_HOVER);
                ::InvalidateRect(m_hSelf, NULL, TRUE);
            }
        }
        else if (m_bsPrev == BNST_DOWN && m_bsNow == BNST_HOVER)
        {
            this->setStatus(BNST_DOWN);
        }
        else
        {
            if (PtInRect(&rc, pt))
            {
                //GetParent()->SetWindowText("END HOVER");
                this->setStatus(BNST_HOVER);
                ::InvalidateRect(m_hSelf, NULL, TRUE);
            }
            else
            {
                //GetParent()->SetWindowText("END NORMAL");
                this->setStatus(BNST_NORMAL);
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

    void setStatus(ButtonStatus status)
    {
        m_bsPrev = m_bsNow;
        m_bsNow = status;
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
        this->setStatus(BNST_NORMAL);
        ::InvalidateRect(m_hSelf, NULL, TRUE);

        return TRUE;
    }

private:
    ButtonStatus m_bsNow;
    ButtonStatus m_bsPrev;
    UBitmap *m_pBmpNormal;
    UBitmap *m_pBmpHover;
    UBitmap *m_pBmpDown;
};

#endif // U_COOL_BUTTON_H

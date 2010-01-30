#ifndef PULSE_BUTTON_H_INCLUDED
#define PULSE_BUTTON_H_INCLUDED

#include "ubutton.h"
#include "utimer.h"
#include "colors.h"

class UPulseButton : public UOwnerDrawnButton
{
    enum {
		ID_TIMER_INTERNAL = 22
	};

    typedef enum tagButtonShape {
        BNSP_RECTANGLE,
        BNSP_ROUND
    } ButtonShape;
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

        UOwnerDrawnButton::onDrawItem(wParam, lParam);

        return TRUE; // Message is handled.
    }

    virtual BOOL onLButtonDown(WPARAM wParam, LPARAM lParam)
    {
        ::SetCapture(m_hSelf);
        _bPressed = true;
        ::InvalidateRect(m_hSelf, NULL, TRUE);
        return TRUE;
    }

    virtual BOOL onLButtonUp(WPARAM wParam, LPARAM lParam)
    {
        ::ReleaseCapture();
        _bPressed = false;
        ::InvalidateRect(m_hSelf, NULL, TRUE);

        UOwnerDrawnButton::onLButtonUp(wParam, lParam);

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


        _bMouseOver = PtInRect(&rc, pt);

        ::InvalidateRect(m_hSelf, NULL, TRUE);

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
        _bMouseOver = false;
        _bPressed = false;
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
    int _nPulseWidth;
    bool _bMouseOver;
    bool _bPressed;
    float _fPulseSpeed;

    huys::Color _clrTop;
    huys::Color _clrBottom;
    huys::Color _clrPulse;
    huys::Color _clrFocus;
    huys::Color _clrFore;

    ButtonShape _shape;

    int _nCornerRadius;

    int _nInterval;
private:
    void drawText(HDC hdc, LPRECT lpRect)
    {

    }

    void drawPulse(HDC hdc, LPRECT lpRect)
    {

    }

    void drawCenter(HDC hdc, LPRECT lpRect)
    {

    }

    void drawBorder(HDC hdc, LPRECT lpRect)
    {

    }

    void drawReflex(HDC hdc, LPRECT lpRect)
    {
    }

    void drawHighlight(HDC hdc, LPRECT lpRect)
    {

    }


};

#endif // PULSE_BUTTON_H_INCLUDED

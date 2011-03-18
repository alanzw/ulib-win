/*
* =====================================================================================
*
*       Filename:  uhyperlink.h
*
*    Description:  
*
*        Version:  1.0
*        Created:  2010-9-28 19:06:37
*       Revision:  none
*       Compiler:  gcc
*
*         Author:  huys (hys), huys03@gmail.com
*        Company:  hu
*
* =====================================================================================
*/

#ifndef U_HYPERLINK_H
#define U_HYPERLINK_H

#include "ustatic.h"
#include "umsg.h"
#include "ubrush.h"
#include "udc.h"
#include "ushell.h"

class UHyperLink : public UStatic
{
    enum {
        ID_TIMER_DELAY = 111
    };
public:
    UHyperLink(HWND hParent, UINT nID, HINSTANCE hInst=GetModuleHandle(NULL))
    : UStatic(hParent, nID, hInst)
    {
        m_dwStyles &= ~SS_SIMPLE;
        m_dwStyles |= SS_NOTIFY;
        
        _brBackgroud.createSolidBrush(::GetSysColor(COLOR_3DFACE));
        
        m_crLink = huys::blue;
        m_crVisited = huys::purple;
        m_crHover = huys::red;
        
        m_bOverControl = FALSE;			// Cursor not yet over control
        m_bVisited = FALSE;			// Hasn't been visited yet.
        
        m_bAdjustToFit = FALSE;

		m_sUrl = "http://blog.csdn.net/fox000002";
    }

    BOOL create()
    {
        return UStatic::create() &&  this->subclassProc() && setTimer(ID_TIMER_DELAY, 100);
    }

    virtual BOOL onMouseMove(WPARAM wParam, LPARAM lParam)
    {
        if (!m_bOverControl)		// Cursor has just moved over control
        {
            m_bOverControl = TRUE;

            //if (m_nUnderline == ulHover)
            //    SetFont(&m_UnderlineFont);
            invalidate();

            setTimer(ID_TIMER_DELAY, 100);
        }
        return TRUE;
    }
    
    virtual BOOL onCtrlColor(WPARAM wParam, LPARAM lParam)
    {
        HDC hdc = (HDC)wParam;
        
        USmartDC dc(hdc);
        
        if (m_bOverControl)
            dc.setTextColor(m_crHover);
        else if (m_bVisited)
            dc.setTextColor(m_crVisited);
        else
            dc.setTextColor(m_crLink);
        
        dc.setBKMode(TRANSPARENT);
        
        return (BOOL)(HBRUSH)GetStockObject(NULL_BRUSH);
    }
    /*   
    virtual BOOL onEraseBkgnd(HDC hdc)
    {
        USmartDC udc(hdc);
        huys::URectL rect;
        
        this->getClientRect(rect);
        
        udc.fillRect(rect, _brBackgroud);
        
        return TRUE;
    }
*/

    /* virtual */ BOOL onTimer(WPARAM wParam, LPARAM lParam)
    {
        if (wParam == ID_TIMER_DELAY)
        {
			huys::UPointL p(GetMessagePos());
			::ScreenToClient(m_hSelf, p);

            huys::URectL rect;
            this->getClientRect(rect);
            if (!rect.PtrInRect(p))
            {
                m_bOverControl = FALSE;
                killTimer(ID_TIMER_DELAY);

                //if (m_nUnderline != ulAlways)
                //    SetFont(&m_StdFont);
                rect.offsetY(0, 10);
				::InvalidateRect(m_hSelf, rect, TRUE);
            }
	
            //CStatic::OnTimer(nIDEvent);    

            return TRUE;
        }
        return FALSE;
    }

	/* virtual */ BOOL onNotifyReflect(WPARAM wParam, LPARAM lParam)
	{
		UINT code = ((LPNMHDR)lParam)->code;;
		if (STN_CLICKED == code)
		{
			return this->onClicked();
		}
		return UStatic::onNotifyReflect(wParam, lParam);
	}

	/* virtual */ BOOL onLButtonDown(WPARAM wParam, LPARAM lParam)
	{

		//return this->onClicked();

		this->goURL(m_sUrl);

		return FALSE;
	}
public:
    void setBackground(huys::Color clr)
    {
        if (!_brBackgroud.isNull())
        {
            _brBackgroud.destroy();
        }
        
        _brBackgroud.createSolidBrush(clr);
    }
    
    void setColors(huys::Color crLink, huys::Color crVisited,
                   huys::Color crHover = -1)
    {
        m_crLink = crLink;
        m_crVisited = crVisited;
        m_crHover = crHover;
    }
private:
    huys::Color m_crLink;		// Hyperlink colours
    huys::Color m_crVisited;
    huys::Color m_crHover;	// Hover colour

    BOOL m_bOverControl;		// cursor over control?
    BOOL m_bVisited;			// Has it been visited?
    
    UBrush _brBackgroud;
    
    BOOL m_bAdjustToFit;		// Adjust window size to fit text?

	LPCTSTR m_sUrl;
private:
	BOOL onClicked()
	{
		showMsg("clicked!");
		return TRUE;
	}

	void goURL(const char * url)
	{
		UShell::open(url);		
	}

    ///////////////////////////////////////////////////////////////////////////////
    // PositionWindow

    // Move and resize the window so that the window is the same size
    // as the hyperlink text. This stops the hyperlink cursor being active
    // when it is not directly over the text. If the text is left justified
    // then the window is merely shrunk, but if it is centred or right
    // justified then the window will have to be moved as well.
    //
    // Suggested by Pål K. Tønder 
    //
    void positionWindow()
    {
        if (!::IsWindow(m_hSelf) || !m_bAdjustToFit) 
        return;

        // Get the current window position
        huys::URectL WndRect, ClientRect;
        this->getWindowRect(WndRect);
        this->getClientRect(ClientRect);

        //this->clientToScreen(m_hSelf, ClientRect);

        HWND hParent = ::GetParent(m_hSelf);
        if (hParent)
        {
            this->screenToDialog(hParent, WndRect);
            this->clientToDialog(hParent, ClientRect);
        }

        // Get the size of the window text
        TCHAR strWndText[512];
        this->getWindowText(strWndText, 512);
/*
        UPrivate dc(m_hSelf);
        HANDLE hOldFont = dc.selectObj(m_UnderlineFont);
        huys::USizeL Extent;
        pDC->GetTextExtent(strWndText);
        dc.selectObj(hOldFont);
        ReleaseDC(pDC);

        // Adjust for window borders
        Extent.cx += WndRect.Width() - ClientRect.Width(); 
        Extent.cy += WndRect.Height() - ClientRect.Height(); 

        // Get the text justification via the window style
        DWORD dwStyle = GetStyle();

        // Recalc the window size and position based on the text justification
        if (dwStyle & SS_CENTERIMAGE)
        WndRect.DeflateRect(0, (WndRect.Height() - Extent.cy)/2);
        else
        WndRect.bottom = WndRect.top + Extent.cy;

        if (dwStyle & SS_CENTER) 
        WndRect.DeflateRect((WndRect.Width() - Extent.cx)/2, 0);
        else if (dwStyle & SS_RIGHT) 
        WndRect.left = WndRect.right - Extent.cx;
        else // SS_LEFT = 0, so we can't test for it explicitly 
        WndRect.right = WndRect.left + Extent.cx;

        // Move the window
        SetWindowPos(NULL, 
            WndRect.left, WndRect.top, 
            WndRect.Width(), WndRect.Height(), 
            SWP_NOZORDER);
*/
    }
};

#endif // U_HYPERLINK_H


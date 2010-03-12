#ifndef U_HISTOGRAM_H
#define U_HISTOGRAM_H

#include "ugdi.h"
#include "colors.h"
#include "udc.h"
#include "adt/uvector.h"
#include "adt/uautoptr.h"

class UHistogramCtrl : public UStatic
{
    enum {
        ID_TIMER_UPDATE
    };
public:
    UHistogramCtrl(HWND hParent)
        : UStatic(hParent, IDC_STATIC, ::GetModuleHandle(NULL))
    {
        m_dwStyles &= ~SS_SIMPLE;
        _counter = 0;
    }

    virtual BOOL create()
    {
        BOOL bRet = UStatic::create();
        this->subclassProc();
        
        setTimer(ID_TIMER_UPDATE, 1000);
        return  bRet;
    }

    virtual BOOL onPaint()
	{
		PAINTSTRUCT ps;
		HDC hdc;
		hdc = BeginPaint(m_hSelf, &ps);

        RECT rc;

        this->getClientRect(&rc);

        onDraw(hdc, &rc);

        EndPaint(m_hSelf, &ps);

		return FALSE;

    }

    virtual BOOL onTimer(WPARAM wParam, LPARAM lParam)
    {
        if (wParam == ID_TIMER_UPDATE)
        {
            if (3600 > _counter)
            {
                ++_counter;
            }
        
            this->go();
            invalidate(FALSE);
        }
        

    
        return FALSE;
    }

    void addPos(LPPOINT pts, int n)
    {
        for (int i=0; i<n; ++i)
        {
            m_pts.push_back(pts[i]);
        }
    }
private:
    huys::ADT::UAutoPtr<UMemDC> m_pMemDC;
    UBitmap m_bmp;
    typedef huys::ADT::UVector<POINT> PTList;
    PTList m_pts;
    
    int _counter;
private:
    void onDraw(HDC hdc, LPRECT lpRect)
    {
        if (m_pMemDC == NULL)
        {
            m_pMemDC = new UMemDC(hdc);
            m_bmp.createCompatibleBitmap(hdc, lpRect->right-lpRect->left,
                lpRect->bottom-lpRect->top);
            m_pMemDC->selectObj(m_bmp);
            
        
        long i;
        // background
        huys::URectangle ur(*lpRect);
        ur.setLineColor(huys::black);
        ur.setFilledColor(huys::black);
        ur.setFilledStyle(BS_SOLID);
        ur.Draw(m_pMemDC);

        // grid
        huys::ULine ul(lpRect->left, lpRect->top, lpRect->right, lpRect->top);
        ul.setLineColor(huys::seagreen);
        for (i=lpRect->top; i < lpRect->bottom; i+=10)
        {
            ul.offsetY(10);
            ul.Draw(m_pMemDC);
        }

        ul.SetStartPnt(lpRect->left, lpRect->top);
        ul.SetEndPnt(lpRect->left, lpRect->bottom);
        for (i=lpRect->left; i < lpRect->right; i+=10)
        {
            ul.offsetX(10);
            ul.Draw(m_pMemDC);
        }

        // line
        ul.setLineColor(huys::green);
        for (PTList::iterator it = m_pts.begin() + 1; it != m_pts.end() && m_pts.size()>1; ++it)
        {
            ul.SetEndPnt(*it);
            ul.SetStartPnt(*(it-1));
            ul.Draw(m_pMemDC);
        }
        }
    
        ::BitBlt(hdc, 0, 0, lpRect->right-lpRect->left, lpRect->bottom-lpRect->top, m_pMemDC, 0, 0, SRCCOPY);
    }
    
    void go()
    {
        if (m_pMemDC != NULL)
        {
        RECT rc;

        this->getClientRect(&rc);
        
        LPRECT lpRect = &rc;
        
        UPrivateDC dc(m_hSelf);
        
        m_pMemDC->bitBlt(0, 0, lpRect->right-lpRect->left, lpRect->bottom-lpRect->top, dc, 20, 0, SRCCOPY);
    

        // background
        lpRect->left = lpRect->right - 20;
        huys::URectangle ur(*lpRect);
        ur.setLineColor(huys::black);
        ur.setFilledColor(huys::black);
        ur.setFilledStyle(BS_SOLID);
        ur.Draw(m_pMemDC);
        
        
        long i;
        
        // grid
        huys::ULine ul(lpRect->left, lpRect->top, lpRect->right, lpRect->top);
        ul.setLineColor(huys::seagreen);
        for (i=lpRect->top; i < lpRect->bottom; i+=10)
        {
            ul.offsetY(10);
            ul.Draw(m_pMemDC);
        }

        //lpRect->left = lpRect->right - lpRect->right%10;
        ul.SetStartPnt(lpRect->left, lpRect->top);
        ul.SetEndPnt(lpRect->left, lpRect->bottom);
        for (i=lpRect->left; i < lpRect->right; i+=10)
        {
            ul.Draw(m_pMemDC);
            ul.offsetX(10);
        }
        
        //
        char buf[256];
        wsprintf(buf, "%d", _counter);
        m_pMemDC->setBKColor(huys::black);
        m_pMemDC->setTextColor(huys::pink);
        m_pMemDC->textOut(100, 20, buf, lstrlen(buf));
        
        }
    }
};

#endif // U_HISTOGRAM_H

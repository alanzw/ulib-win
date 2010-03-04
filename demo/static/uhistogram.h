#ifndef U_HISTOGRAM_H
#define U_HISTOGRAM_H

#include "ugdi.h"
#include "colors.h"

#include "adt/uvector.h"

class UHistogramCtrl : public UStatic
{
public:
    UHistogramCtrl(HWND hParent)
        : UStatic(hParent, IDC_STATIC, ::GetModuleHandle(NULL))
    {
        m_dwStyles &= ~SS_SIMPLE;
    }

    virtual BOOL create()
    {
        BOOL bRet = UStatic::create();
        this->subclassProc();
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
    HDC m_hMemDC;
    typedef huys::ADT::UVector<POINT> PTList;
    PTList m_pts;
private:
    void onDraw(HDC hdc, LPRECT lpRect)
    {
        long i;
        // background
        huys::URectangle ur(*lpRect);
        ur.setLineColor(huys::black);
        ur.setFilledColor(huys::black);
        ur.setFilledStyle(BS_SOLID);
        ur.Draw(hdc);

        // grid
        huys::ULine ul(lpRect->left, lpRect->top, lpRect->right, lpRect->top);
        ul.setLineColor(huys::seagreen);
        for (i=lpRect->top; i < lpRect->bottom; i+=10)
        {
            ul.offsetY(10);
            ul.Draw(hdc);
        }

        ul.SetStartPnt(lpRect->left, lpRect->top);
        ul.SetEndPnt(lpRect->left, lpRect->bottom);
        for (i=lpRect->left; i < lpRect->right; i+=10)
        {
            ul.offsetX(10);
            ul.Draw(hdc);
        }

        // line
        ul.setLineColor(huys::green);
        for (PTList::iterator it = m_pts.begin() + 1; it != m_pts.end() && m_pts.size()>1; ++it)
        {
            ul.SetEndPnt(*it);
            ul.SetStartPnt(*(it-1));
            ul.Draw(hdc);
        }
    }
};

#endif // U_HISTOGRAM_H

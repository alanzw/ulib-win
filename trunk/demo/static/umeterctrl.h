#ifndef U_METER_CTRL_H
#define U_METER_CTRL_H

#include <cmath>

#include "ugdi.h"
#include "colors.h"
#include "udc.h"
#include "uregion.h"
#include "adt/uvector.h"
#include "adt/uautoptr.h"

#ifndef ROUND
#define ROUND(x) (int)((x) + 0.5 - (double)((x)<0))
#endif // ROUND

class UMeterCtrl : public UStatic
{
    enum {
        ID_TIMER_UPDATE = 1222
    };
    
    enum {
        BOUNDARY_POINTS = 48
    };
public:
    UMeterCtrl(HWND hParent)
    : UStatic(hParent, IDC_STATIC, ::GetModuleHandle(NULL)),
      m_clrBackground(huys::skyblue)
    {
        m_dwStyles &= ~SS_SIMPLE;
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
            go();
            invalidate(FALSE);
        }
        return FALSE;
    }
private:
    huys::ADT::UAutoPtr<UMemDC> m_pMemDC;
    UBitmap m_bmp;
    
    huys::Color m_clrBackground;
    
    URegion m_rgn;
    
    int m_nBottomCX;
    int m_nBottomCY;
    int m_nTopRadius;
    int m_nBottomRadius;
    
    POINT m_ptBoundary[BOUNDARY_POINTS];
private:
    void onDraw(HDC hdc, LPRECT lpRect)
    {
        if (m_pMemDC == NULL)
        {
            m_pMemDC = new UMemDC(hdc);
            m_bmp.createCompatibleBitmap(hdc, lpRect->right-lpRect->left, lpRect->bottom-lpRect->top);
            m_pMemDC->selectObj(m_bmp);

            // Background
            m_pMemDC->setBrushColor(m_clrBackground);
            m_pMemDC->rectangle(lpRect);
            
            //
            RECT rcLabel = {
                lpRect->left + (lpRect->right-lpRect->left)/20,
                lpRect->bottom - 10,
                lpRect->right- (lpRect->right-lpRect->left)/20,
                lpRect->bottom - (lpRect->right-lpRect->left)/20
            };
            
            //
            m_nBottomCX = (lpRect->left + lpRect->right)/2;
            m_nBottomCY = lpRect->bottom - (lpRect->bottom-lpRect->top)/16;
            
            m_nTopRadius = (lpRect->bottom-lpRect->top)*6/8;
            m_nBottomRadius = m_nTopRadius/2;
            //
            double radPerdeg = 4.0 * atan(1.0)/180.;
            int nStartAngleDeg = 60;
            int nEndAngleDeg = 120;
            int nTickDeg = 15;
            
            int nAngleIncrementDeg = 5;
            
            double leftAngleRad = nEndAngleDeg * radPerdeg;
            double rightAngleRad = nStartAngleDeg * radPerdeg;
            
            // meter face
            int nRef = 0;
            double angleRad;
            int nAngleDeg;
            int temp;
            for (nAngleDeg = nStartAngleDeg; nAngleDeg<=nEndAngleDeg; nAngleDeg+=nAngleIncrementDeg)
            {
                angleRad = nAngleDeg * radPerdeg;
                
                //
                temp = m_nBottomCX + m_nTopRadius * cos(angleRad);
                m_ptBoundary[nRef].x = ROUND(temp);
                
                temp = m_nBottomCY - m_nTopRadius * sin(angleRad);
                m_ptBoundary[nRef].y = ROUND(temp);
            
                ++nRef;
            }
            
            int nHalfPoints = nRef;
            
            for (nAngleDeg = nEndAngleDeg; nAngleDeg>=nStartAngleDeg; nAngleDeg-=nAngleIncrementDeg)
            {
                angleRad = nAngleDeg * radPerdeg;
                
                //
                temp = m_nBottomCX + m_nBottomRadius * cos(angleRad);
                m_ptBoundary[nRef].x = ROUND(temp);
                
                temp = m_nBottomCY - m_nBottomRadius * sin(angleRad);
                m_ptBoundary[nRef].y = ROUND(temp);
            
                ++nRef;
            }
            
            POINT ptRecess[BOUNDARY_POINTS];
            
            for (int i=0; i<nRef; ++i)
            {
                ptRecess[i].x = m_ptBoundary[i].x;
                ptRecess[i].y = m_ptBoundary[i].y - 1;
            }

            
            if (!m_rgn.isNull())
            {
                m_rgn.destroy();
            }
            m_rgn.createPolygon(m_ptBoundary, nRef, ALTERNATE);
            
            huys::UPolyLine poly(ptRecess, nHalfPoints+1);
            poly.Draw(m_pMemDC);
            
            poly.setPoints(&ptRecess[nHalfPoints], nHalfPoints+1);
            poly.Draw(m_pMemDC);
 
        }

        ::BitBlt(hdc, 0, 0, lpRect->right-lpRect->left, lpRect->bottom-lpRect->top, m_pMemDC, 0, 0, SRCCOPY);
    }

    void go()
    {

    }
};

#endif // U_METER_CTRL_H

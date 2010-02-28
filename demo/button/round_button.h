#ifndef U_ROUND_BUTTON_H
#define U_ROUND_BUTTON_H

#include <cassert>
#include <cmath>
#include <climits>
#include <cstdlib>

#include "uregion.h"
#include "colors.h"

#ifndef min
#define min(X,Y) ((X) < (Y) ?  (X) : (Y))
#endif // min

class URoundButton : public UOwnerDrawnButton
{
public:
    URoundButton(HWND hParent, UINT nResource, HINSTANCE hInst)
    : UOwnerDrawnButton(hParent, nResource, hInst),
      m_clrFocusCircle(huys::black),
      m_clrBackground(::GetSysColor(COLOR_BTNFACE))
    {
    }

    virtual ~URoundButton()
    {}

    void resize()
    {
        this->modifyStyles(BS_OWNERDRAW);

        RECT rect;
        this->getClientRect(&rect);

        // Resize the window to make it square
        rect.bottom = rect.right = min(rect.bottom, rect.right);

        // Get the vital statistics of the window
        m_ptCenter.x = (rect.left + rect.right)/2;
        m_ptCenter.y = (rect.top + rect.bottom)/2;
        m_nRadius  = rect.bottom/2-1;

        // Set the window region so mouse clicks only activate the round section
        // of the button
        m_rgn.destroy();
        ::SetWindowRgn(m_hSelf, NULL, FALSE);
        m_rgn.createEllipticIndirect(&rect);
        ::SetWindowRgn(m_hSelf, m_rgn, TRUE);

        // Convert client coords to the parents client coords
        ::ClientToScreen(m_hSelf, (LPPOINT)&rect);
        ::ClientToScreen(m_hSelf, ((LPPOINT)&rect)+1);
        ::ScreenToClient(m_hParent, (LPPOINT)&rect);
        ::ScreenToClient(m_hParent, ((LPPOINT)&rect)+1);
        // Resize the window
        this->setPosition(&rect);
    }

    virtual BOOL onDrawItem(WPARAM wParam, LPARAM lParam)
    {
        LPDRAWITEMSTRUCT lpDrawItem;
        HDC hdc;

        lpDrawItem = (LPDRAWITEMSTRUCT) lParam;
        hdc = lpDrawItem->hDC;

        RECT rc = lpDrawItem->rcItem;

        UINT state = lpDrawItem->itemState;
        UINT nStyle = GetWindowLong(m_hSelf, GWL_STYLE);
        LONG nRadius = m_nRadius;

        int nSavedDC = ::SaveDC(hdc);

        HANDLE hObject = ::GetStockObject(NULL_BRUSH);
        HANDLE hOldObject = ::SelectObject(hdc, hObject);
        ::SetBkColor(hdc, m_clrBackground);
        ::ExtTextOut(hdc, 0, 0, ETO_OPAQUE, &rc, NULL, 0, NULL);

        // Draw the focus circle around the button
        if ((state & ODS_FOCUS) && m_bDrawDashedFocusCircle)
            DrawCircle(hdc, m_ptCenter, nRadius--, m_clrFocusCircle);

        // Draw the raised/sunken edges of the button (unless flat)
        if (nStyle & BS_FLAT)
        {
            DrawCircle(hdc, m_ptCenter, nRadius--, RGB(0,0,0));
            DrawCircle(hdc, m_ptCenter, nRadius--, ::GetSysColor(COLOR_3DHIGHLIGHT));
        }
        else
        {
            if ((state & ODS_SELECTED))
            {
                DrawCircle(hdc, m_ptCenter, nRadius--,
                       ::GetSysColor(COLOR_3DDKSHADOW), ::GetSysColor(COLOR_3DHIGHLIGHT));
                DrawCircle(hdc, m_ptCenter, nRadius--,
                       ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DLIGHT));
            }
            else
            {
                DrawCircle(hdc, m_ptCenter, nRadius--,
                       ::GetSysColor(COLOR_3DHIGHLIGHT), ::GetSysColor(COLOR_3DDKSHADOW));
                DrawCircle(hdc, m_ptCenter, nRadius--,
                       ::GetSysColor(COLOR_3DLIGHT), ::GetSysColor(COLOR_3DSHADOW));
            }
        }

        // draw the text if there is any
        TCHAR strText[512];
        ::GetWindowText(m_hSelf, strText, 511);

        if (lstrcmp(strText, "") != 0)
        {
            URegion rgn;
            rgn.createElliptic(m_ptCenter.x-nRadius, m_ptCenter.y-nRadius,
                              m_ptCenter.x+nRadius, m_ptCenter.y+nRadius);
            ::SelectClipRgn(hdc, rgn);

            SIZE Extent = {0};
            ::GetTextExtentPoint32(hdc, strText, lstrlen(strText), &Extent);
            POINT pt = { m_ptCenter.x - Extent.cx/2, m_ptCenter.x - Extent.cy/2 };

            if (state & ODS_SELECTED)
            {
                pt.x += 1;
                pt.y += 1;
            }
            ::SetBkMode(hdc, TRANSPARENT);

            if (state & ODS_DISABLED)
                ::DrawState(hdc, (HBRUSH)NULL, 0, (WPARAM)strText, (LPARAM)lstrlen(strText), pt.x, pt.y, Extent.cx, Extent.cy, DSS_DISABLED);
            else
                ::TextOut(hdc, pt.x, pt.y, strText, lstrlen(strText));

            ::SelectClipRgn(hdc, NULL);
            rgn.destroy();
        }

        // Draw the focus circle on the inside of the button
        if ((state & ODS_FOCUS) && m_bDrawDashedFocusCircle)
            DrawCircle(hdc, m_ptCenter, nRadius-2, m_clrFocusCircle, TRUE);

        ::RestoreDC(hdc, nSavedDC);
        return TRUE;
        UNREFERENCED_LOCAL_VARIABLE(hOldObject);
    }

    //
    void setColorFocusCircle(huys::Color clr)
    {
        m_clrFocusCircle = clr;
    }
private:
    URegion m_rgn;
    POINT m_ptCenter;
    LONG m_nRadius;
    BOOL   m_bDrawDashedFocusCircle;

    huys::Color m_clrFocusCircle;
    huys::Color m_clrBackground;
private:


    // Calculate colour for a point at the given angle by performing a linear
    // interpolation between the colours crBright and crDark based on the cosine
    // of the angle between the light source and the point.
    //
    // Angles are measured from the +ve x-axis (i.e. (1,0) = 0 degrees, (0,1) = 90 degrees )
    // But remember: +y points down!

    COLORREF GetColour(double dAngle, COLORREF crBright, COLORREF crDark)
    {
        #define Rad2Deg    180.0/3.1415
        #define LIGHT_SOURCE_ANGLE    -2.356        // -2.356 radians = -135 degrees, i.e. From top left

        assert(dAngle > -3.1416 && dAngle < 3.1416);
        double dAngleDifference = LIGHT_SOURCE_ANGLE - dAngle;

        if (dAngleDifference < -3.1415) dAngleDifference = 6.293 + dAngleDifference;
        else if (dAngleDifference > 3.1415) dAngleDifference = 6.293 - dAngleDifference;

        double Weight = 0.5*(cos(dAngleDifference)+1.0);

        BYTE Red   = (BYTE) (Weight*GetRValue(crBright) + (1.0-Weight)*GetRValue(crDark));
        BYTE Green = (BYTE) (Weight*GetGValue(crBright) + (1.0-Weight)*GetGValue(crDark));
        BYTE Blue  = (BYTE) (Weight*GetBValue(crBright) + (1.0-Weight)*GetBValue(crDark));

        //TRACE("LightAngle = %0.0f, Angle = %3.0f, Diff = %3.0f, Weight = %0.2f, RGB %3d,%3d,%3d\n",
        //      LIGHT_SOURCE_ANGLE*Rad2Deg, dAngle*Rad2Deg, dAngleDifference*Rad2Deg, Weight,Red,Green,Blue);

        return RGB(Red, Green, Blue);
    }

    void DrawCircle(HDC hdc, POINT p, LONG lRadius, COLORREF crColour, BOOL bDashed  = FALSE)
    {
        const int nDashLength = 1;
        LONG lError, lXoffset, lYoffset;
        int  nDash = 0;
        BOOL bDashOn = TRUE;

        //Check to see that the coordinates are valid
        assert( (p.x + lRadius <= LONG_MAX) && (p.y + lRadius <= LONG_MAX) );
        assert( (p.x - lRadius >= LONG_MIN) && (p.y - lRadius >= LONG_MIN) );

        //Set starting values
        lXoffset = lRadius;
        lYoffset = 0;
        lError = -lRadius;

        do {
            if (bDashOn) {
                SetPixelV(hdc, p.x + lXoffset, p.y + lYoffset, crColour);
                SetPixelV(hdc, p.x + lXoffset, p.y - lYoffset, crColour);
                SetPixelV(hdc, p.x + lYoffset, p.y + lXoffset, crColour);
                SetPixelV(hdc, p.x + lYoffset, p.y - lXoffset, crColour);
                SetPixelV(hdc, p.x - lYoffset, p.y + lXoffset, crColour);
                SetPixelV(hdc, p.x - lYoffset, p.y - lXoffset, crColour);
                SetPixelV(hdc, p.x - lXoffset, p.y + lYoffset, crColour);
                SetPixelV(hdc, p.x - lXoffset, p.y - lYoffset, crColour);
            }

            //Advance the error term and the constant X axis step
            lError += lYoffset++;

            //Check to see if error term has overflowed
            if ((lError += lYoffset) >= 0)
                lError -= --lXoffset * 2;

            if (bDashed && (++nDash == nDashLength)) {
                nDash = 0;
                bDashOn = !bDashOn;
            }

        } while (lYoffset <= lXoffset);    //Continue until halfway point
    }

    void DrawCircle(HDC hdc, POINT p, LONG lRadius, COLORREF crBright, COLORREF crDark)
    {
        LONG lError, lXoffset, lYoffset;

        //Check to see that the coordinates are valid
        assert( (p.x + lRadius <= LONG_MAX) && (p.y + lRadius <= LONG_MAX) );
        assert( (p.x - lRadius >= LONG_MIN) && (p.y - lRadius >= LONG_MIN) );

        //Set starting values
        lXoffset = lRadius;
        lYoffset = 0;
        lError   = -lRadius;

        do {
            const double Pi = 3.141592654;
            const double Pi_on_2 = Pi * 0.5;
            //         Three_Pi_on_2 = Pi * 1.5;
            COLORREF crColour;
            double   dAngle = atan2((double)lYoffset, (double)lXoffset);

            //Draw the current pixel, reflected across all eight arcs
            crColour = GetColour(dAngle, crBright, crDark);
            SetPixelV(hdc, p.x + lXoffset, p.y + lYoffset, crColour);

            crColour = GetColour(Pi_on_2 - dAngle, crBright, crDark);
            SetPixelV(hdc, p.x + lYoffset, p.y + lXoffset, crColour);

            crColour = GetColour(Pi_on_2 + dAngle, crBright, crDark);
            SetPixelV(hdc, p.x - lYoffset, p.y + lXoffset, crColour);

            crColour = GetColour(Pi - dAngle, crBright, crDark);
            SetPixelV(hdc, p.x - lXoffset, p.y + lYoffset, crColour);

            crColour = GetColour(-Pi + dAngle, crBright, crDark);
            SetPixelV(hdc, p.x - lXoffset, p.y - lYoffset, crColour);

            crColour = GetColour(-Pi_on_2 - dAngle, crBright, crDark);
            SetPixelV(hdc, p.x - lYoffset, p.y - lXoffset, crColour);

            crColour = GetColour(-Pi_on_2 + dAngle, crBright, crDark);
            SetPixelV(hdc, p.x + lYoffset, p.y - lXoffset, crColour);

            crColour = GetColour(-dAngle, crBright, crDark);
            SetPixelV(hdc, p.x + lXoffset, p.y - lYoffset, crColour);

            //Advance the error term and the constant X axis step
            lError += lYoffset++;

            //Check to see if error term has overflowed
            if ((lError += lYoffset) >= 0)
                lError -= --lXoffset * 2;

        } while (lYoffset <= lXoffset);    //Continue until halfway point
    }
};

#endif // U_ROUND_BUTTON_H

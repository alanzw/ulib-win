/*
 * =====================================================================================
 *
 *       Filename:  uled.h
 *
 *    Description:  Class ULed
 *
 *        Version:  1.0
 *        Created:  2009-8-7 18:26:16
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#ifndef U_LED_H
#define U_LED_H

#include "ugdi.h"

class ULEDCtrl : public UStatic
{
public:
    ULEDCtrl(HWND hParent, UINT nID)
    : UStatic(hParent, nID, ::GetModuleHandle(NULL))
    {

    }

    virtual BOOL create()
    {
        BOOL bRet = UStatic::create();
        this->subclassProc();
        return bRet;
    }

    virtual BOOL onPaint()
    {
        HDC hdc;
        PAINTSTRUCT ps;
        RECT rc;

        this->getClientRect(&rc);

        hdc = BeginPaint(m_hSelf, &ps);

        draw(hdc, rc);

        EndPaint(m_hSelf, &ps);
        return FALSE;
    }

    void setColor(huys::Color clrMain, huys::Color clrShade)
    {
        m_clrMain = clrMain;
        m_clrShade = clrShade;
    }
private:
    huys::Color m_clrMain;
    huys::Color m_clrShade;

    void draw(HDC hdc, RECT &rect)
    {
        huys::URectangle urc(rect);
        urc.setLineColor(huys::gray);
        urc.setFilledColor(huys::blue);
        urc.setFilledStyle(BS_SOLID);
        urc.Draw(hdc);
    }
};

#endif // U_LED_H


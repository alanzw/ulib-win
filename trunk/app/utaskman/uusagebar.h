/*
 * =====================================================================================
 *
 *       Filename:  uusagebar.h
 *
 *    Description:  Class UUsageBar
 *
 *        Version:  1.0
 *        Created:  2009/9/19 22:34:29
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#ifndef U_USAGE_BAR_H
#define U_USAGE_BAR_H

#include "ustatic.h"
#include "ugdi.h"

class UUsageBar : public UStatic
{
public:
    UUsageBar(HWND hParent, UINT nResource, HINSTANCE hInst)
        : UStatic(hParent, nResource, hInst)
    {
        m_dwStyles &= ~SS_SIMPLE;
        //m_dwStyles |= SS_NOTIFY;
    }

    UUsageBar() {}

    virtual ~UUsageBar()
    {

    }
    virtual BOOL create()
    {
        BOOL bRet = UControl::create(_T("STATIC"), NULL);
        this->subclassProc();
        return  bRet;
    }

    virtual BOOL onPaint()
    {
        PAINTSTRUCT ps;
        HDC hdc;
        hdc = BeginPaint(m_hSelf, &ps);

        RECT rcClient = {0};
        this->getClientRect(&rcClient);

        drawBack(hdc, &rcClient);

        RECT rcBar = {
           rcClient.left += 10,
           rcClient.top += 10,
           rcClient.right -= 10,
           rcClient.bottom -= 10
        };

        drawBar(hdc, &rcBar);

        EndPaint(m_hSelf, &ps);

        return FALSE;
    }

    void updateUsage()
    {
        this->update();
    }
private:
    void drawBack(HDC hdc, LPRECT lpRect)
    {
        huys::URectangle ur(*lpRect);
        ur.setFilledColor(huys::black);
        ur.setFilledStyle(BS_SOLID);
        ur.Draw(hdc);
    }

    void drawBar(HDC hdc, LPRECT lpRect)
    {
        huys::URectangle ur(*lpRect);
        ur.setFilledColor(huys::green);
        ur.setFilledStyle(BS_SOLID);
        ur.Draw(hdc);

        huys::ULine ul(lpRect->left, lpRect->bottom, lpRect->right, lpRect->bottom);
        ul.setLineColor(huys::black);

        while (ul.getStartY() > lpRect->top)
        {
            ul.offsetY(-5);
            ul.Draw(hdc);
        }
    }
};

#endif // U_USAGE_BAR_H


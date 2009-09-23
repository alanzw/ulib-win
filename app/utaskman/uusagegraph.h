/*
 * =====================================================================================
 *
 *       Filename:  uusagegraph.h
 *
 *    Description:  UUsage Graph Class
 *
 *        Version:  1.0
 *        Created:  2009-9-20 20:02:24
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#ifndef U_USAGE_GRAPH_H
#define U_USAGE_GRAPH_H

#include "ustatic.h"

class UUsageGraph : public UStatic
{
public:
    UUsageGraph(HWND hParent, UINT nResource, HINSTANCE hInst)
        : UStatic(hParent, nResource, hInst)
    {
        m_dwStyles &= ~SS_SIMPLE;
        m_dwStyles |= SS_NOTIFY;
    }

    UUsageGraph()
    {}

    virtual ~UUsageGraph()
    {

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

        RECT rcClient = {0};
        this->getClientRect(&rcClient);

        drawBack(hdc, &rcClient);

        drawGrid(hdc, &rcClient);

        RECT rcBar = {
           rcClient.left += 10,
           rcClient.top += 10,
           rcClient.right -= 10,
           rcClient.bottom -= 10
        };

        drawGraph(hdc, &rcBar);

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

    void drawGrid(HDC hdc, LPRECT lpRect)
    {
        int w = lpRect->right - lpRect->left;
        int h = lpRect->bottom - lpRect->top;

        huys::ULine ul(lpRect->left, lpRect->top+10, lpRect->right, lpRect->top+10);
        ul.setLineColor(huys::blueviolet);
        //ul.setLineWidth(1);
        ul.Draw(hdc);

        while (ul.getStartY() <= lpRect->bottom - 10)
        {
            ul.offsetY(10);
            ul.Draw(hdc);
        }
    }

    void drawGraph(HDC hdc, LPRECT lpRect)
    {

    }
};

#endif // U_USAGE_GRAPH_H


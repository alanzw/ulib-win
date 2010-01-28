#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "colors.h"
#include "uchart.h"
#include "ugdi.h"

UChartControl::UChartControl(HWND hParent, UINT nResource, HINSTANCE hInst)
    : UStatic(hParent, nResource, hInst)
{
    m_dwStyles &= ~SS_SIMPLE;
    m_dwStyles |= SS_CENTER;

    m_hbrush = ::CreateSolidBrush(huys::red);
}

UChartControl::~UChartControl()
{}

BOOL UChartControl::onCtrlColor(WPARAM wParam, LPARAM lParam)
{

    //::MessageBox(NULL, "this is in reflected message function.", "Hi", MB_OK);

//    HDC hdc = (HDC)wParam;
//    HWND hwnd = (HWND)lParam;
//    RECT rc;

    //::SelectObject(hdc, hBrushBK);

    return (BOOL)m_hbrush;
}

BOOL UChartControl::onPaint()
{

     HDC hdc;
     PAINTSTRUCT ps;
     RECT rc;

     hdc = BeginPaint(m_hSelf, &ps);

     this->getClientRect(&rc);
     //ClientToScreen(hwnd, (LPPOINT)&rc);
     //HBRUSH holdbrush = (HBRUSH)SelectObject(hdc, m_hbrush);
     //Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
     //SelectObject(hdc, holdbrush);


     rc.left += 40;
     rc.right -= 40;
     rc.top += 40;
     rc.bottom -= 40;

     HBRUSH hNewBrush = ::CreateSolidBrush(huys::blueviolet);
     HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdc, hNewBrush);
     ::Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
     ::SelectObject(hdc, hOldBrush);

    huys::ULine uline(rc.left+20, rc.top, rc.left+20, rc.bottom);
    uline.setLineColor(huys::gray);
    //uline.setFilledColor(huys::blueviolet);
    uline.setLineStyle(PS_DOT);
    // Filling in the Gaps.
    ::SetBkMode(hdc, TRANSPARENT);

    uline.Draw(hdc);
    for (int i=0; i<rc.right-rc.left-60; i+=20)
    {
        uline.offsetX(20);
        uline.Draw(hdc);
    }

    uline.SetStartPnt(rc.left, rc.top+20);
    uline.SetEndPnt(rc.right, rc.top+20);
    uline.Draw(hdc);
    for (int j=0; j<rc.bottom-rc.top-60; j+=20)
    {
        uline.offsetY(20);
        uline.Draw(hdc);
    }

     MoveToEx(hdc, rc.left, rc.top, NULL);
     LineTo(hdc, rc.right, rc.bottom);

     EndPaint(m_hSelf, &ps);

    return FALSE;
}

BOOL UChartControl::onEraseBkgnd(HDC hdc)
{
    RECT rc;
    this->getClientRect(&rc);
    HBRUSH holdbrush = (HBRUSH)::SelectObject(hdc, m_hbrush);
    ::Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
    ::SelectObject(hdc, holdbrush);

    return TRUE;
}

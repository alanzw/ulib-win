#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "colors.h"
#include "udc.h"
#include "ugdi.h"

#include "ulife.h"

ULifeCtrl::ULifeCtrl(HWND hParent, UINT nID, HINSTANCE hInst)
: UStatic(hParent, nID, hInst)
{
}

ULifeCtrl::ULifeCtrl()
{
}

ULifeCtrl::ULifeCtrl(UBaseWindow *pWndParent, UINT nID)
: UStatic(pWndParent, "")
{
    m_nResourceID = nID;
    m_dwStyles &= ~SS_SIMPLE;
    m_dwStyles |= SS_NOTIFY;
    m_hbrush = ::CreateSolidBrush(huys::red);
    
   _board.initialize();
}

ULifeCtrl::~ULifeCtrl()
{}

BOOL ULifeCtrl::create()
{
    BOOL bRet = UStatic::create();
    this->subclassProc();
    return  bRet;
}

BOOL ULifeCtrl::onPaint()
{
    PAINTSTRUCT ps;
    HDC hdc;
    hdc = BeginPaint(m_hSelf, &ps);

    RECT rc;

    this->getClientRect(&rc);

    HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdc, m_hbrush);
    ::Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
    ::SelectObject(hdc, hOldBrush);
    
    int dx = (rc.right - rc.left) / 10;
    int dy = (rc.bottom - rc.top) / 10;
    _dx = dx;
    _dy = dy;
    
    huys::ULine line(rc.left, rc.top + dy, rc.right, rc.top + dy);
    line.setLineColor(huys::white);
    line.setLineWidth(2);
    
    for (int j = rc.top + dy; j<rc.bottom; j+= dy)
    {
        line.Draw(hdc);
        line.offsetY(dy);
    }
    
    line.SetStartPnt(rc.left + dx, rc.top);
    line.SetEndPnt(rc.left + dx, rc.bottom);
    for (int i = rc.left + dx; i<rc.right; i+= dx)
    {
        line.Draw(hdc);
        line.offsetX(dx);
    }
   
    huys::UAngleArc circ;
    circ.setLineColor(huys::blue);
    circ.setFilledColor(huys::blue);
    circ.setFilledStyle(BS_SOLID);
    circ.setRadius(5);
    circ.setStartAngle(0.0f);
    circ.setSweepAngle(360.f);
    
    for (int k = 1; k<=ULifeBoard::maxrow; ++k)
    {
        for (int m=1; m<=ULifeBoard::maxcol; ++m )
        {
            if (1 == _board.getState(k, m))
            {
                circ.setCenter(rc.left + (2 * k - 1) * dx / 2, rc.top + (2 * m - 1) * dx / 2);
                circ.Draw(hdc);
            }
        }
    }

    EndPaint(m_hSelf, &ps);

    return FALSE;

}

BOOL ULifeCtrl::onLButtonDown(WPARAM wParam, LPARAM lParam)
{
    RECT rc;
    this->getClientRect(&rc);

    POINT pt = {
        GET_X_LPARAM(lParam),
        GET_Y_LPARAM(lParam)
    };
    
    int i = (pt.x - rc.left)/_dx+1;
    int j = (pt.y - rc.top)/_dy+1;
    
    _board.invertState(i, j);
    invalidate();
    
    return FALSE;
}

void ULifeCtrl::updateState()
{
    _board.update();
    invalidate();
}

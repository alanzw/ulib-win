#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "colors.h"
#include "udc.h"
#include "ugdi.h"

#include "ugo.h"

UGoCtrl::UGoCtrl(HWND hParent, UINT nID, HINSTANCE hInst)
: UStatic(hParent, nID, hInst),
  _nStep(0)
{
}

UGoCtrl::UGoCtrl()
: _nStep(0)
{
}

UGoCtrl::UGoCtrl(UBaseWindow *pWndParent, UINT nID)
: UStatic(pWndParent, ""),
  _nStep(0)
{
    m_nResourceID = nID;
    m_dwStyles &= ~SS_SIMPLE;
    m_dwStyles |= SS_NOTIFY;
    m_hbrush = ::CreateSolidBrush(huys::xpblue);

   _board.initialize();
}

UGoCtrl::~UGoCtrl()
{}

BOOL UGoCtrl::create()
{
    BOOL bRet = UStatic::create();
    this->subclassProc();
    return  bRet;
}

BOOL UGoCtrl::onPaint()
{
    PAINTSTRUCT ps;
    HDC hdc;
    hdc = BeginPaint(m_hSelf, &ps);

    RECT rc;

    this->getClientRect(&rc);

    HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdc, m_hbrush);
    ::Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
    ::SelectObject(hdc, hOldBrush);

    int cols = UGoBoard::maxcol + 1;
    int rows = UGoBoard::maxrow + 1;

    int dx = (rc.right - rc.left) / cols;
    int dy = (rc.bottom - rc.top) / rows;
    _dx = dx;
    _dy = dy;

    huys::ULine line(rc.left+dx, rc.top + dy, rc.right-dx, rc.top + dy);
    line.setLineColor(huys::white);
    line.setLineWidth(2);

    for (int j = rc.top + dy; j<rc.bottom-2; j+= dy)
    {
        line.Draw(hdc);
        line.offsetY(dy);
    }

    line.SetStartPnt(rc.left + dx, rc.top+dy);
    line.SetEndPnt(rc.left + dx, rc.bottom-dy);
    for (int i = rc.left + dx; i<rc.right-2; i+= dx)
    {
        line.Draw(hdc);
        line.offsetX(dx);
    }

    huys::UAngleArc circ;
    circ.setFilledStyle(BS_SOLID);
    circ.setRadius(8);
    circ.setStartAngle(0.0f);
    circ.setSweepAngle(360.f);

    huys::UCircle star;
    star.setFilledStyle(BS_SOLID);
    star.setFilledColor(huys::black);
    star.setRadius(3);

    for (int k = 1; k<=UGoBoard::maxrow; ++k)
    {
        for (int m=1; m<=UGoBoard::maxcol; ++m )
        {
            if ((k == 4 || k==10 || k == 16) && (m == 4 || m == 10 || m == 16))
            {
                star.setCenter(rc.left + k * dx, rc.top + m*dy);
                star.Draw(hdc);
            }

            if (1 == _board.getState(k, m))
            {
                circ.setLineColor(huys::blue);
                circ.setFilledColor(huys::blue);
                circ.setCenter(rc.left + k  * dx, rc.top +  m * dy);
                circ.Draw(hdc);
            }
            if (2 == _board.getState(k, m))
            {
                circ.setLineColor(huys::red);
                circ.setFilledColor(huys::red);
                circ.setCenter(rc.left + k  * dx, rc.top +  m * dy);
                circ.Draw(hdc);
            }
        }
    }

    EndPaint(m_hSelf, &ps);

    return FALSE;

}

BOOL UGoCtrl::onLButtonDown(WPARAM wParam, LPARAM lParam)
{
    RECT rc;
    this->getClientRect(&rc);

    POINT pt = {
        GET_X_LPARAM(lParam),
        GET_Y_LPARAM(lParam)
    };

    int i = (pt.x - rc.left - _dx/2)/_dx+1;
    int j = (pt.y - rc.top - _dy/2)/_dy+1;

    int state = (_nStep%2==0 ? 1 : 2);

    if (i>=1 && i<=UGoBoard::maxcol && j>=1 && j<=UGoBoard::maxrow)
    {
        _board.setState(state, i, j);
        ++_nStep;
        invalidate();
    }
    return FALSE;
}

void UGoCtrl::updateState()
{
    _board.update();
    invalidate();
}

void UGoCtrl::clear()
{
    for (int k = 1; k<=UGoBoard::maxrow; ++k)
    {
        for (int m=1; m<=UGoBoard::maxcol; ++m )
        {
            _board.setState(0, k, m);
        }
    }

    _nStep = 0;
    invalidate();
}


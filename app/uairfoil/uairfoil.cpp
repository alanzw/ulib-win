#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "udc.h"

#include "uairfoil.h"

UAirfoil::UAirfoil(HWND hParent, UINT nID, HINSTANCE hInst)
: UTransLabel(hParent, nID, hInst)
{

}

UAirfoil::UAirfoil()
: UTransLabel(NULL)
{}

UAirfoil::~UAirfoil()
{}

UAirfoil::UAirfoil(UBaseWindow *pWndParent, UINT nID)
: UTransLabel(pWndParent, nID)
{
    m_dwStyles &= ~SS_SIMPLE;
    m_dwStyles |= SS_NOTIFY;
    m_nResourceID = nID;
}

/*
BOOL UAirfoil::onPaint()
{
    UPaintDC dc(*this);

    huys::URectL rect;
    this->getClientRect(rect);

    //dc.setBk
    //dc.textOutEx(10, 10, _T("hello"));

    return FALSE;
}

*/
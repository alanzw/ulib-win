#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "colors.h"

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
    m_hbrush = ::CreateSolidBrush(huys::red);
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


    EndPaint(m_hSelf, &ps);

    return FALSE;

}

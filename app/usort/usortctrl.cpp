#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "colors.h"
#include "udc.h"
#include "ugdi.h"

#include "usortctrl.h"

USortCtrl::USortCtrl(HWND hParent, UINT nID, HINSTANCE hInst)
: UStatic(hParent, nID, hInst)
{
}

USortCtrl::USortCtrl(UBaseWindow *pWndParent, UINT nID)
: UStatic(pWndParent, "")
{
}

USortCtrl::USortCtrl()
{
}

USortCtrl::~USortCtrl()
{}

BOOL USortCtrl::create()
{
    BOOL bRet = UStatic::create();
    this->subclassProc();
    return  bRet;
}

BOOL USortCtrl::onPaint()
{
    PAINTSTRUCT ps;
    HDC hdc;
    hdc = BeginPaint(m_hSelf, &ps);

    RECT rc;

    this->getClientRect(&rc);

    EndPaint(m_hSelf, &ps);

    return FALSE;

}

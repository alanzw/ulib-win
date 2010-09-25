#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "udc.h"

#include "ubooks.h"

UBooks::UBooks(HWND hParent, UINT nID, HINSTANCE hInst)
: UStatic(hParent, nID, hInst)
{

}

UBooks::UBooks()
{}

UBooks::~UBooks()
{}

UBooks::UBooks(UBaseWindow *pWndParent, UINT nID)
: UStatic(pWndParent, nID)
{
    m_dwStyles &= ~SS_SIMPLE;
    m_dwStyles |= SS_NOTIFY;
    m_nResourceID = nID;
}

BOOL UBooks::create()
{
    return UStatic::create() && this->subclassProc();
}

BOOL UBooks::onPaint()
{
    UPaintDC dc(*this);

    RECT rc;
    this->getClientRect(&rc);
 
    return FALSE;
}


void UBooks::Start()
{
    this->setTimer(ID_TIMER_UPDATE, 300);
}

void UBooks::Pause()
{
}

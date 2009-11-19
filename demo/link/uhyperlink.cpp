#include <windows.h>
#include <tchar.h>

#include "uhyperlink.h"

UHyperLink::UHyperLink()
{
}

UHyperLink::UHyperLink(HWND hParent, UINT nResource, HINSTANCE hInst)
: UStatic(hParent, nResource, hInst)
{
    m_dwStyles &= ~SS_SIMPLE;
    m_dwStyles |= SS_NOTIFY;
}

UHyperLink::~UHyperLink()
{
}

BOOL UHyperLink::create()
{
    BOOL bRet = UStatic::create();
    this->subclassProc();
    return  bRet;
}

BOOL UHyperLink::onCtrlColor(WPARAM wParam, LPARAM lParam)
{

}

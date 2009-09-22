#define _WIN32_IE  0x0400

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "udatetimepick.h"

UDateTimePick::UDateTimePick( HWND hParent,
                              UINT nResource,
                              HINSTANCE hInst )
: UControl(hParent, nResource, hInst)
{}

UDateTimePick::~UDateTimePick()
{}

BOOL UDateTimePick::create()
{
    INITCOMMONCONTROLSEX icc;
    icc.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icc.dwICC = ICC_BAR_CLASSES | ICC_TAB_CLASSES | ICC_WIN95_CLASSES ;
    InitCommonControlsEx(&icc);

    return UControl::create(_T("SysDateTimePick32"));
}

BOOL UDateTimePick::setFormat(LPCTSTR sFormat)
{
    return this->sendMsg(DTM_SETFORMAT, 0, (LPARAM)sFormat);
}

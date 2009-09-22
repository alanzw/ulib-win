#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "utrackbar.h"

UTrackBar::UTrackBar(HWND hParent, UINT nResource, HINSTANCE hInst)
: UControl(hParent, nResource, hInst)
{
    m_dwStyles |= TBS_AUTOTICKS | TBS_ENABLESELRANGE;
}

UTrackBar::~UTrackBar()
{}

BOOL UTrackBar::create()
{
    ::InitCommonControls(); // loads common control's DLL
    return UControl::create(_T("msctls_trackbar32"));
}

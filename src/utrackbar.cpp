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


BOOL UTrackBar::setRange(int min, int max, BOOL bRedraw)
{
    return this->sendMsg(TBM_SETRANGE, (WPARAM)bRedraw, (LPARAM)MAKELPARAM(min, max));
}

BOOL UTrackBar::setRangeSize(int nSize)
{
    return this->sendMsg(TBM_SETPAGESIZE, 0, (LPARAM)nSize);
}

BOOL UTrackBar::setTicFreq(int nFreq)
{
    return this->sendMsg(TBM_SETTICFREQ, (WPARAM)nFreq, 0);
}

BOOL UTrackBar::setTPos(int nNewPos, BOOL bRedraw)
{
    return this->sendMsg(TBM_SETPOS, (WPARAM)bRedraw, (LPARAM)nNewPos);
}

int UTrackBar::getRangeMin()
{
    return (int)this->sendMsg(TBM_GETRANGEMIN);    
}

int UTrackBar::getRangeMax()
{
    return (int)this->sendMsg(TBM_GETRANGEMAX);    
}

void UTrackBar::getSelection( int &nMin, int &nMax )
{
    nMin = (int)this->sendMsg(TBM_GETSELSTART);
	nMax = (int)this->sendMsg(TBM_GETSELEND);   
}

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>
#include <assert.h>
#include "uprogressbar.h"

UProgressBar::UProgressBar( HWND hParent,
                            UINT nResource,
                            HINSTANCE hInst )
: UControl(hParent, nResource, hInst)
{}

UProgressBar::~UProgressBar()
{}

BOOL UProgressBar::create()
{
    InitCommonControls();
    return UControl::create(_T("msctls_progress32")); // PROGRESS_CLASS
}

BOOL UProgressBar::setRange(int min, int max)
{
    return this->sendMsg(PBM_SETRANGE, 0,  (LPARAM)MAKELPARAM(min, max));
}

int UProgressBar::getLowLimit()
{
    return this->sendMsg(PBM_GETRANGE, TRUE);
}

int UProgressBar::getHighLimit()
{
    return this->sendMsg(PBM_GETRANGE, FALSE);
}

BOOL UProgressBar::stepIt()
{
    return this->sendMsg(PBM_STEPIT, 0, 0);
}

BOOL UProgressBar::setStep(int nStepSize)
{
    return this->sendMsg(PBM_SETSTEP, nStepSize, 0);
}

int UProgressBar::getStep()
{
#ifdef PBM_GETSTEP
    return this->sendMsg(PBM_GETSTEP);
#endif // PBM_GETSTEP
    return 0;
}

BOOL UProgressBar::setPBPos(int nNewPos)
{

#if (_WIN32_WINNT >= 0x0501)
    assert(!(m_dwStyles & PBS_MARQUEE ));
#endif // _WIN32_WINNT >= 0x0501

    return this->sendMsg(PBM_SETPOS, nNewPos, 0);
}

BOOL UProgressBar::deltaPBPos(int nIncrement)
{

#if (_WIN32_WINNT >= 0x0501)
    assert(!(m_dwStyles & PBS_MARQUEE ));
#endif // _WIN32_WINNT >= 0x0501

    return this->sendMsg(PBM_DELTAPOS, nIncrement, 0);
}

UINT UProgressBar::getPBPos()
{
    return this->sendMsg(PBM_GETPOS);
}

#if (_WIN32_WINNT >= 0x0501)
BOOL UProgressBar::setMarquee(BOOL bMarqueeModeOn, UINT nUpdateTimeInMs)
{
    return this->sendMsg(PBM_SETMARQUEE, bMarqueeModeOn, nUpdateTimeInMs);
}
#endif // _WIN32_WINNT >= 0x0501

BOOL UProgressBar::setState(int iState)
{
#ifdef PBM_SETSTATE
    return this->sendMsg(PBM_SETSTATE, iState);
#endif // PBM_SETSTATE
    return 0;
}

int UProgressBar::getState()
{
#ifdef PBM_GETSTATE
    return this->sendMsg(PBM_GETSTATE);
#endif // PBM_GETSTATE
    return 0;
}

BOOL UProgressBar::setBKColor(huys::Color clr)
{
    return this->sendMsg(PBM_SETBKCOLOR, 0, clr);
}

BOOL UProgressBar::setBarColor(huys::Color clr)
{
    return this->sendMsg(PBM_SETBARCOLOR, 0, clr);
}

huys::Color UProgressBar::getBKColor()
{
#ifdef PBM_GETBKCOLOR
    return (huys::Color)this->sendMsg(PBM_GETBKCOLOR);
#endif // PBM_GETBKCOLOR
    return 0;
}

huys::Color UProgressBar::getBarColor()
{
#ifdef PBM_GETBARCOLOR
    return (huys::Color)this->sendMsg(PBM_GETBARCOLOR);
#endif // PBM_GETBARCOLOR
    return 0;
}


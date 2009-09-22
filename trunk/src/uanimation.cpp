#define _WIN32_WINNT 0x0501
#define _WIN32_IE    0x0500

#include <windows.h>
#include <commctrl.h>
#include <tchar.h>

#include "uanimation.h"

UAnimation::UAnimation(HWND hParent, UINT nResource, HINSTANCE hInst)
: UControl(hParent, nResource, hInst),
state(AS_CLOSED)
{};

UAnimation::~UAnimation()
{};

BOOL UAnimation::create()
{
    INITCOMMONCONTROLSEX icc;
    icc.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icc.dwICC = ICC_BAR_CLASSES | ICC_TAB_CLASSES | ICC_WIN95_CLASSES ;
    InitCommonControlsEx(&icc);

    return UControl::create(_T("SysAnimate32"));
}

BOOL UAnimation::open(const TCHAR *filename)
{
    return this->sendMsg(ACM_OPEN, 0, (LPARAM)filename);
}

BOOL UAnimation::open( const UINT nResID )
{
    return this->sendMsg(ACM_OPEN, 0, (LPARAM)nResID);
}

BOOL UAnimation::play()
{
    // Repeat playing from the first frame to the last frame
    return this->sendMsg(ACM_PLAY, (WPARAM)-1, (LPARAM) MAKELONG(0, -1));
}

BOOL UAnimation::playEx(int nFrom, int nTo, int nReplay)
{
    return this->sendMsg(ACM_PLAY, (WPARAM)nReplay, (LPARAM) MAKELONG(nFrom, nTo));
}

BOOL UAnimation::stop()
{
    this->pause();
    this->play();
    return this->sendMsg(ACM_STOP);
}

BOOL UAnimation::pause()
{
    return this->sendMsg(ACM_STOP);
}

BOOL UAnimation::close()
{
    return this->sendMsg(ACM_OPEN, 0, 0L);
}

BOOL UAnimation::resume()
{
    return this->play();
}

#if WINVER >=  0x0600
BOOL UAnimation::isPlaying()
{
    return this->sendMsg(ACM_ISPLAYING);
}
#endif

void UAnimation::setTransparent()
{
    this->setStyles(ACS_TRANSPARENT);
}


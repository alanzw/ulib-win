#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "uupdownctrl.h"

UUpDownCtrl::UUpDownCtrl( HWND hParent, UINT nResource, HINSTANCE hInst )
: UControl(hParent, nResource, hInst)
{}

UUpDownCtrl::~UUpDownCtrl()
{}

BOOL UUpDownCtrl::create()
{
    InitCommonControls();

    m_hSelf = ::CreateWindow(
        _T("msctls_updown32"), // UPDOWN_CLASS
        _T(""),                //
        m_dwStyles,
        0,
        0,
        0,
        0,
        m_hParent,
        (HMENU)m_nResourceID,
        m_hInstance,
        0);
    this->setLong();
    return TRUE;
}

BOOL UUpDownCtrl::setBase(int nBase)
{
    return this->sendMsg(UDM_SETBASE, nBase, 0);
}

BOOL UUpDownCtrl::setBuddy(HWND hwndBuddy)
{
    return this->sendMsg(UDM_SETBUDDY, (WPARAM)hwndBuddy, 0);
}

BOOL UUpDownCtrl::setPos(int nPos)
{
    return this->sendMsg(UDM_SETPOS, 0, MAKELONG ((short) nPos, 0) );
}

BOOL UUpDownCtrl::setRange(int nUpper, int nLower)
{
    return this->sendMsg(UDM_SETRANGE, 0, MAKELONG ((short) nUpper, (short) nLower) );
}

#if (_WIN32_IE >= 0x0400)
BOOL UUpDownCtrl::setPos32(int nPos)
{
    return this->sendMsg(UDM_SETPOS32, 0, nPos);
}

BOOL UUpDownCtrl::setRange32(int iLow, int iHigh)
{
    return this->sendMsg(UDM_SETRANGE32, iLow, iHigh);
}
#endif // _WIN32_IE >= 0x0400

#if (_WIN32_IE >= 0x0500)
BOOL UUpDownCtrl::setUnicodeFormat(BOOL fUnicode)
{
    return this->sendMsg(UDM_SETUNICODEFORMAT, fUnicode, 0);
}
#endif //  (_WIN32_IE >= 0x0500)

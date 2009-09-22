#define _WIN32_IE    0x0500

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "uipaddress.h"
#include "umsg.h"

UIPAddress::UIPAddress(HWND hParent, UINT nID, HINSTANCE hInst)
: UControl(hParent, nID, hInst)
{
    m_rc.left = 100;
    m_rc.right = m_rc.left + 300;
    m_rc.top = 100;
    m_rc.bottom = m_rc.top + 100;
}

UIPAddress::~UIPAddress()
{}

BOOL UIPAddress::create()
{
//#if (_WIN32_IE >= 0x0400)
    INITCOMMONCONTROLSEX icc;
    icc.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icc.dwICC = ICC_INTERNET_CLASSES;
    BOOL bRet = InitCommonControlsEx(&icc);

    if (!bRet)
    {
        showMsg(_T("Error InitCommonControlsEx with ICC_INTERNET_CLASSES"));
    }

    //return UControl::create(_T("SysIPAddress32"));
//#else
//    return FALSE;
//#endif // (_WIN32_IE >= 0x0400)

    m_hSelf = ::CreateWindow(
        _T("SysIPAddress32"),   // predefined class
        0,  // window text
        WS_VISIBLE | WS_CHILD,     // styles.
        m_rc.left,              // x position.
        m_rc.top,              // y positio.
        m_rc.right-m_rc.left,              // width
        m_rc.bottom-m_rc.top,              // height
        m_hParent,      // parent window
        (HMENU)m_nResourceID,   // control ID
        m_hInstance,
        0);      // Pointer not needed.
        return TRUE;
}

BOOL UIPAddress::setAddress( BYTE first, BYTE second, BYTE third, BYTE fourth )
{
    return this->sendMsg(IPM_SETADDRESS, 0, MAKEIPADDRESS(first, second, third, fourth));
}

BOOL UIPAddress::isBlank()
{
    return this->sendMsg(IPM_ISBLANK);
}

void UIPAddress::setFocus(int nField)
{
    this->sendMsg(IPM_SETFOCUS, (WPARAM) (int) nField);
}

BOOL UIPAddress::setRange(int nField, BYTE low, BYTE high)
{
    return this->sendMsg(IPM_SETRANGE, (WPARAM) (int) nField, 
        (LPARAM) (WORD)MAKEIPRANGE(low, high));
}

DWORD UIPAddress::getAddress()
{
    DWORD dwAddr;
    this->sendMsg(IPM_GETADDRESS, 0, (LPARAM) &dwAddr);
    return dwAddr;
}

BYTE UIPAddress::first()
{
	DWORD dwAddr;
	this->sendMsg(IPM_GETADDRESS, 0, (LPARAM) &dwAddr);
	return FIRST_IPADDRESS(dwAddr);
}

BYTE UIPAddress::second()
{
	DWORD dwAddr;
	this->sendMsg(IPM_GETADDRESS, 0, (LPARAM) &dwAddr);
	return SECOND_IPADDRESS(dwAddr);
}

BYTE UIPAddress::third()
{
	DWORD dwAddr;
	this->sendMsg(IPM_GETADDRESS, 0, (LPARAM) &dwAddr);
	return THIRD_IPADDRESS(dwAddr);
}

BYTE UIPAddress::fourth()
{
	DWORD dwAddr;
	this->sendMsg(IPM_GETADDRESS, 0, (LPARAM) &dwAddr);
	return FOURTH_IPADDRESS(dwAddr);
}

void UIPAddress::clear()
{
	this->sendMsg(IPM_CLEARADDRESS);
}

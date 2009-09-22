#ifdef   WINVER
#undef   WINVER
#endif

#ifdef   _WIN32_IE
#undef   _WIN32_IE
#endif

#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif

#define   WINVER       0x0501
#define   _WIN32_IE    0x0501
#define   _WIN32_WINNT 0x0501

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "ulink.h"

ULink::ULink( HWND hParent, UINT nResource, HINSTANCE hInst)
:UControl(hParent, nResource, hInst)
{
    m_rc.left   = 10;
    m_rc.top    = 10;
    m_rc.right  = 110;
    m_rc.bottom = 40;
};

ULink::~ULink()
{
    ::DestroyWindow(m_hSelf);
};

BOOL ULink::create(const TCHAR *lpText)
{
    //
    //INITCOMMONCONTROLSEX icc;
    //icc.dwSize =    sizeof(INITCOMMONCONTROLSEX);
    //icc.dwICC =ICC_BAR_CLASSES | ICC_TAB_CLASSES | ICC_WIN95_CLASSES | ICC_LINK_CLASS ;
    //InitCommonControlsEx(&icc);

    // **Error in calling SetWindowLong()
    //return UControl::create(_T("SysLink"), lpText); // WC_LINK
        m_hSelf = ::CreateWindow(
            _T("SysLink"),   // predefined class
            lpText,  // window text
            WS_CHILD | WS_VISIBLE,     // styles.
            m_rc.left,
            m_rc.top,
            m_rc.right-m_rc.left,
            m_rc.bottom-m_rc.top,
            m_hParent,      // parent window
            (HMENU)m_nResourceID,   // control ID
            m_hInstance, //::GetModuleHandle ( NULL ),
            0);      // Pointer not needed.
            return TRUE;
}


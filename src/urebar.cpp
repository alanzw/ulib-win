#define _WIN32_IE  0x0400

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "urebar.h"

URebar::URebar(HWND hParent, UINT nResource, HINSTANCE hInst)
: UControl(hParent, nResource, hInst)
{}

URebar::~URebar()
{}

BOOL URebar::create()
{
    // Initialize common controls.
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC   = ICC_COOL_CLASSES | ICC_BAR_CLASSES;
    InitCommonControlsEx(&icex);
    return UControl::createEx( WS_EX_TOOLWINDOW, _T("ReBarWindow32"));
}

/*
typedef struct tagREBARBANDINFO{
    UINT             cbSize;
    UINT             fMask;
    UINT             fStyle;
    COLORREF    clrFore;
    COLORREF    clrBack;
    LPTSTR         lpText;
    UINT             cch;
    int                 iImage;
    HWND           hwndChild;
    UINT             cxMinChild;
    UINT             cyMinChild;
    UINT             cx;
    HBITMAP       hbmBack;
    UINT             wID;
#if (_WIN32_IE >= 0x0400)
    UINT            cyChild;
    UINT            cyMaxChild;
    UINT            cyIntegral;
    UINT            cxIdeal;
    LPARAM        lParam;
    UINT            cxHeader;
#endif
#if (_WIN32_WINNT >= 0x0600)
    RECT        rcChevronLocation;
    UINT        uChevronState;
#endif
} REBARBANDINFO, *LPREBARBANDINFO;
*/

BOOL URebar::initBandInfo()
{
        // Initialize band info used by both bands.
        m_bandinfo.cbSize = sizeof(REBARBANDINFO);
        m_bandinfo.fMask = RBBIM_STYLE       // fStyle is valid
                     | RBBIM_TEXT        // lpText is valid.
                     | RBBIM_CHILD       // hwndChild is valid.
                     | RBBIM_CHILDSIZE   // child size members are valid.
                     | RBBIM_SIZE;       // cx is valid
        m_bandinfo.fStyle = RBBS_CHILDEDGE | RBBS_GRIPPERALWAYS; // | RBBS_FIXEDSIZE;
    return TRUE;
}

BOOL URebar::addChild(HWND hChild, LPCTSTR lpText, UINT cxMinChild, UINT cyMinChild, UINT cx)
{
    m_bandinfo.hwndChild = hChild,
    m_bandinfo.lpText = (LPTSTR)lpText;
    m_bandinfo.cxMinChild = cxMinChild;
    m_bandinfo.cyMinChild = cyMinChild;
    m_bandinfo.cx = cx;

    this->sendMsg(RB_INSERTBAND, (WPARAM)-1, (LPARAM)&m_bandinfo);

    return TRUE;
}

huys::Color URebar::getBkColor()
{
    return (huys::Color)this->sendMsg(RB_GETBKCOLOR);
}

huys::Color URebar::getTextColor()
{
    return (huys::Color)this->sendMsg(RB_GETTEXTCOLOR);
}

huys::Color URebar::setBkColor( huys::Color clr )
{
    return (huys::Color)this->sendMsg(RB_SETBKCOLOR, 0, (LPARAM)clr);
}

huys::Color URebar::setTextColor( huys::Color clr )
{
    return (huys::Color)this->sendMsg(RB_SETTEXTCOLOR, 0, (LPARAM)clr);
}


/*
 * =====================================================================================
 *
 *       Filename:  utooltip.cpp
 *
 *    Description:  Implementation for Class UToolTip
 *
 *        Version:  1.0
 *        Created:  2009-2-15 3:37:05
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys03@gmail.com
 *        Company:  none
 *
 * =====================================================================================
 */

#define _WIN32_IE 0x0500

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "utooltip.h"

#if defined(_MSC_VER) && (_MSC_VER <= 0x1200)
  #if (_WIN32_IE >= 0x0500)
    #define TTM_SETTITLEA           (WM_USER + 32)  // wParam = TTI_*, lParam = char* szTitle
    #define TTM_SETTITLEW           (WM_USER + 33)  // wParam = TTI_*, lParam = wchar* szTitle
  #endif

  #define TTS_BALLOON	0x40 
#endif

  #ifndef TTM_SETTITLE
    #ifdef _UNICODE
      #define TTM_SETTITLE            TTM_SETTITLEW
    #else
      #define TTM_SETTITLE            TTM_SETTITLEA
    #endif
  #endif

#ifndef TTI_INFO
#define TTI_INFO                1
#endif

UToolTip::UToolTip(HWND hParent, UINT nID, HINSTANCE hInst)
: UControl(hParent, nID, hInst),
  m_clrBk(huys::white),
  m_clrText(huys::black),
  m_nMaxWidth(DEFAULT_MAX_WIDTH),
  m_wX(0),
  m_wY(0)
{
    m_dwStyles = TTS_NOPREFIX | TTS_ALWAYSTIP | TTS_BALLOON;
    //m_dwStyles = TTS_NOPREFIX | TTS_ALWAYSTIP;

    ::ZeroMemory(m_sTitle, sizeof(m_sTitle));
    ::ZeroMemory(m_sText, sizeof(m_sText));

    ::ZeroMemory((char *)&m_ti, sizeof(m_ti));

    m_ti.cbSize = sizeof(TOOLINFO);
    //m_ti.uFlags = TTF_TRACK|TTF_CENTERTIP;
    //m_ti.uFlags = TTF_CENTERTIP;
    m_ti.uFlags = TTF_TRACK | TTF_ABSOLUTE;
    m_ti.hwnd = m_hParent;
    m_ti.hinst = m_hInstance;
    m_ti.uId = m_nResourceID;
//    m_ti.lpszText = LPSTR_TEXTCALLBACK;
    m_ti.lpszText = m_sText;

    // ToolTip control will cover the whole window
    m_ti.rect.left = 0;
    m_ti.rect.top = 0;
    m_ti.rect.right = 0;
    m_ti.rect.bottom = 0;
}

UToolTip::~UToolTip()
{}

BOOL UToolTip::create()
{
    //return UControl::create(_T("tooltips_class32"));
    return UControl::createEx(WS_EX_TOPMOST, _T("tooltips_class32"));
}

BOOL UToolTip::activate()
{
//#if (_WIN32_IE >= 0x0300)
    return this->sendMsg(TTM_TRACKACTIVATE, TRUE, (LPARAM)(LPTOOLINFO)&m_ti);
//#else
//    return FALSE;
//#endif
}

BOOL UToolTip::deactivate()
{
//#if (_WIN32_IE >= 0x0300)
    return this->sendMsg(TTM_TRACKACTIVATE, FALSE, (LPARAM)(LPTOOLINFO)&m_ti);
//#else
//    return FALSE;
//#endif
}

BOOL UToolTip::addTool()
{
    return this->sendMsg(TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&m_ti);
}

BOOL UToolTip::setBKColor( huys::Color color )
{
    m_clrBk = color;
    return TRUE;
}

BOOL UToolTip::setTextColor( huys::Color color )
{
    m_clrText = color;
    return TRUE;
}

BOOL UToolTip::setTitle( const TCHAR *sTitle )
{
    lstrcpyn(m_sTitle, sTitle, MAX_TITLE_LENGTH);
    return TRUE;
}

BOOL UToolTip::setMaxWidth( int nWidth )
{
    m_nMaxWidth = nWidth;
    return TRUE;
}

BOOL UToolTip::setTipBkColor( huys::Color clr )
{
//#if (_WIN32_IE >= 0x0300)
     return  this->sendMsg(TTM_SETTIPBKCOLOR, (WPARAM)m_clrBk);
//#else
//    return FALSE;
//#endif
}

BOOL UToolTip::setTipTextColor( huys::Color clr )
{
//#if (_WIN32_IE >= 0x0300)
    return  this->sendMsg(TTM_SETTIPTEXTCOLOR, (WPARAM)m_clrText);
//#else
//    return FALSE;
//#endif
}

BOOL UToolTip::setTipTitle(const TCHAR *sTitle)
{
//#if (_WIN32_IE >= 0x0500)
    return this->sendMsg(TTM_SETTITLE, (WPARAM)TTI_INFO, (LPARAM) (LPCTSTR)sTitle);
//#else
//    return FALSE;
//#endif
}

BOOL UToolTip::setTipMaxWidth( int nWidth )
{
//#if (_WIN32_IE >= 0x0300)
    return this->sendMsg(TTM_SETMAXTIPWIDTH, 0, (LPARAM)nWidth);
//#else
//    return FALSE;
//#endif
}

BOOL UToolTip::showTip()
{
    this->setTipBkColor(m_clrBk);
    this->setTipTextColor(m_clrText);
    this->setTipTitle(m_sTitle);
    this->addTool();
    this->setTipMaxWidth(m_nMaxWidth);
    this->trackPosition();
    this->activate();

    //SetWindowPos(m_hSelf, HWND_TOPMOST,
    //    0, 0, 0, 0,
    //    SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

    return TRUE;
}

BOOL UToolTip::trackPosition()
{
//#if (_WIN32_IE >= 0x0300)
    return this->sendMsg(TTM_TRACKPOSITION, 0, (LPARAM)(DWORD) MAKELONG(m_wX, m_wY));
//#else
//    return FALSE;
//#endif
}

BOOL UToolTip::setText( const TCHAR *sText )
{
    lstrcpyn(m_sText, sText, MAX_TEXT_LENGTH);
    return TRUE;
}

void UToolTip::setXY( WORD x, WORD y )
{
    m_wX = x;
    m_wY = y;
}


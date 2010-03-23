// uTooltip.cpp: implementation of the CMyTooltip class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uTooltip.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMyTooltip::CMyTooltip()
{
	//
	m_bShow = FALSE;

    //
    m_uID = 0;

    //
	m_x = 0;
	m_y = 0;

    // Default text and title.
	m_strText = _T("This is a customized tooltip!");
    m_strTitle = _T("Info");

    // 
	m_nWidth = 100;

    // 
	m_clrBackground = huys::green;
    m_clrText = huys::black;

    //
	m_hTT = NULL;
	m_hParent = NULL;

}

CMyTooltip::~CMyTooltip()
{

}

BOOL CMyTooltip::create(HWND hParent/*=NULL*/)
{

	m_hParent = hParent;
    m_hTT = ::CreateWindowEx(
        WS_EX_TOPMOST,
        TOOLTIPS_CLASS,
        NULL,
        TTS_NOPREFIX | TTS_ALWAYSTIP | TTS_BALLOON ,		
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        m_hParent,
        NULL,
        NULL,
        NULL);

    // INITIALIZE MEMBERS OF THE TOOLINFO STRUCTURE
    m_ti.cbSize = sizeof(TOOLINFO);
    m_ti.uFlags = TTF_TRACK|TTF_CENTERTIP;
    m_ti.hwnd = m_hParent;
    m_ti.hinst = NULL;
    m_ti.uId = m_uID;
    m_ti.lpszText = LPSTR_TEXTCALLBACK;

    // ToolTip control will cover the whole window
    m_ti.rect.left = 0;
    m_ti.rect.top = 0;
    m_ti.rect.right = 0;
    m_ti.rect.bottom = 0;

    // SEND AN ADDTOOL MESSAGE TO THE TOOLTIP CONTROL WINDOW
    ::SendMessage(m_hTT, TTM_TRACKACTIVATE, false, (LPARAM)(LPTOOLINFO) &m_ti);

    return TRUE;
}

BOOL CMyTooltip::show()
{

    //MessageBox(NULL, "xx", "rr", MB_OK);
	if (!m_bShow)
	{
		::SendMessage(m_hTT, TTM_SETTIPBKCOLOR, (WPARAM)m_clrBackground, 0);
        ::SendMessage(m_hTT, TTM_SETTIPTEXTCOLOR, (WPARAM)m_clrText, 0);
#if (_WIN32_IE >= 0x0500)
        ::SendMessage(m_hTT, TTM_SETTITLE, (WPARAM)TTI_INFO, (LPARAM) (LPCTSTR)m_strTitle);
#endif
		::SendMessage(m_hTT, TTM_ADDTOOL, 0, (LPARAM) (LPTOOLINFO) &m_ti);
		::SendMessage(m_hTT, TTM_SETMAXTIPWIDTH, 0, (LPARAM)m_nWidth);
		::SendMessage(m_hTT, TTM_TRACKPOSITION, 0, (LPARAM)(DWORD) MAKELONG(m_x, m_y));
		::SendMessage(m_hTT, TTM_TRACKACTIVATE, true, (LPARAM)(LPTOOLINFO) &m_ti);
		m_bShow = true;
	}
	else
	{
		::SendMessage(m_hTT, TTM_TRACKACTIVATE, false, (LPARAM)(LPTOOLINFO) &m_ti);
		m_bShow = false;
	}


	return true;
}

BOOL CMyTooltip::setXY( int x, int y )
{
	m_x = x;
	m_y = y;

	return TRUE;
}

BOOL CMyTooltip::setText( const TCHAR *szText )
{
	m_strText = szText;	
	return TRUE;
}

BOOL CMyTooltip::setMaxWidth( int width )
{
	m_nWidth = width;
	return TRUE;
}

BOOL CMyTooltip::setBKColor( huys::Color color )
{
	m_clrBackground = color;
	return TRUE;	
}

BOOL CMyTooltip::setTextColor( huys::Color color )
{
    m_clrText = color;
    return TRUE;
}

BOOL CMyTooltip::setTitle( const TCHAR *szTitle )
{
    m_strTitle = szTitle;
    return TRUE;
}

UINT CMyTooltip::getUID() const
{
    return m_uID;
}

LPSTR CMyTooltip::getText()
{
    return (LPSTR)m_strText.GetBuffer(200);
}

BOOL CMyTooltip::isVisible()
{
    return this->m_bShow;
}

HWND CMyTooltip::getParent()
{
    return this->m_hParent;
}

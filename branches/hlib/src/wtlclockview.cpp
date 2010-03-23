// WTLClockView.cpp : implementation of the CWTLClockView class

#include "wtlclock.h"

#include "resource.h"
#include "wtlclockview.h"

BOOL CWTLClockView::PreTranslateMessage(MSG* pMsg)
{
    pMsg;
    return FALSE;
}

LRESULT CWTLClockView::OnPaint(UINT /*uMsg*/,
                               WPARAM /*wParam*/,
                               LPARAM /*lParam*/,
                               BOOL& /*bHandled*/)
{
    CPaintDC   dc(m_hWnd);
    CRect      rc;
    CString    sTime;

    // Get our window's client area.
    GetClientRect ( rc );

    // Build the string to show in the window.
    if ( m_bClockRunning )
        {
        sTime.Format ( _T("The time is %d:%02d:%02d"),
                       m_stLastTime.wHour, m_stLastTime.wMinute, m_stLastTime.wSecond );
        }
    else
        {
        sTime.Format ( _T("Clock stopped on %d:%02d:%02d"),
                       m_stLastTime.wHour, m_stLastTime.wMinute, m_stLastTime.wSecond );
        }

    // Set up the DC and draw the text.
    dc.SaveDC();

    dc.SetBkColor ( RGB(255,153,0) );
    dc.SetTextColor ( RGB(0,0,0) );
    dc.ExtTextOut ( 0, 0, ETO_OPAQUE, rc, sTime, sTime.GetLength(), NULL );

    // Restore the DC.
    dc.RestoreDC(-1);
    return 0;
}

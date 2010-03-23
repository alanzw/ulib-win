#ifndef H_WTLBASE_H
#define H_WTLBASE_H

#include "resource.h"

#define  _WIN32_IE 0x0400

#define WIN32_LEAN_AND_MEAN     // Exclude rarely-used stuff from Windows headers
#define STRICT
#define _WTL_USE_CSTRING

#include <atlbase.h>
#include <atlapp.h>
extern CAppModule _Module;
#include <atlwin.h>
#include <atlframe.h>
#include <atlmisc.h>
#include <atlcrack.h>

class CMyWindow : public CFrameWindowImpl<CMyWindow>
{
public:
    DECLARE_FRAME_WND_CLASS ( _T("First WTL window"), IDR_MAINFRAME );

    BEGIN_MSG_MAP_EX(CMyWindow)
        MSG_WM_CREATE(OnCreate)
        MSG_WM_DESTROY(OnDestroy)
        MSG_WM_ERASEBKGND(OnEraseBkgnd)
        MSG_WM_TIMER(OnTimer)
        COMMAND_ID_HANDLER_EX(IDC_EXIT, OnFileExit)
        COMMAND_ID_HANDLER_EX(IDC_ABOUT, OnAbout)
        CHAIN_MSG_MAP(CFrameWindowImpl<CMyWindow>)
    END_MSG_MAP()
        
    LRESULT OnCreate ( LPCREATESTRUCT lpcs )
    {
        SetTimer ( 1, 1000 );
        SetMsgHandled(false);
        return 0;
    }
    
    void OnDestroy()
    {
        KillTimer(1);
        SetMsgHandled(false);
    }
    
    LRESULT OnEraseBkgnd ( HDC hdc )
    {
        CDCHandle  dc(hdc);
        CRect      rc;
        SYSTEMTIME st;
        CString    sTime;
        
        // Get our window's client area.
        GetClientRect ( rc );
        
        // Build the string to show in the window.
        GetLocalTime ( &st );
        sTime.Format ( _T("The time is %d:%02d:%02d"), st.wHour, st.wMinute, st.wSecond );
        
        // Set up the DC and draw the text.
        dc.SaveDC();
        
        dc.SetBkColor ( RGB(255,153,0) );
        dc.SetTextColor ( RGB(0,0,0) );
        dc.ExtTextOut ( 0, 0, ETO_OPAQUE, rc, sTime, sTime.GetLength(), NULL );
        
        // Restore the DC.
        dc.RestoreDC(-1);
        return 1;                       // We erased the background (ExtTextOut did it)
    }
    
    void OnTimer ( UINT uTimerID, TIMERPROC pTimerProc )
    {
        if ( 1 != uTimerID )
            SetMsgHandled(false);
        else
            RedrawWindow();
    }
    
    void OnFileExit(UINT uCode, int nID, HWND hwndCtrl)
    {
        DestroyWindow();
        return;
    }
    
    void OnAbout(UINT uCode, int nID, HWND hwndCtrl)
    {
        CSimpleDialog<IDD_ABOUT> dlg;
        
        dlg.DoModal();
        return;
    }
};


#endif // H_WTLBASE_H

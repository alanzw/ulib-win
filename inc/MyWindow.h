#ifndef MYWINDOW_H_INCLUDED
#define MYWINDOW_H_INCLUDED

#include "resource.h"   // resource IDs
#define STRICT
#define WIN32_LEAN_AND_MEAN     // Exclude rarely-used stuff from Windows headers

#include <atlbase.h>        // Base ATL classes
extern CComModule _Module;  // Global ATL module object
#include <atlwin.h>         // ATL windowing classes

#include "colors.h"


//////////////////////////////////////////////////////////////////////////
// The About dialog

class CAboutDlg : public CDialogImpl<CAboutDlg>
{
public:
    enum { IDD = IDD_ABOUT };

    BEGIN_MSG_MAP(CAboutDlg)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        MESSAGE_HANDLER(WM_CLOSE, OnClose)
        COMMAND_ID_HANDLER(IDOK, OnOKCancel)
        COMMAND_ID_HANDLER(IDCANCEL, OnOKCancel)
    END_MSG_MAP()

    LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        CenterWindow();
        return TRUE;    // let the system set the focus
    }

    LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        EndDialog(IDCANCEL);
        return 0;
    }

    LRESULT OnOKCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
    {
        EndDialog(wID);
        return 0;
    }
};

//////////////////////////////////////////////////////////////////////////
// Mix-in class that handles WM_ERASEBKGND

template <class T, COLORREF t_crBrushColor>
class CPaintBkgnd
{
public:
    CPaintBkgnd() 
    { 
        m_hbrBkgnd = CreateSolidBrush(t_crBrushColor); 
    }
    ~CPaintBkgnd() 
    { 
        DeleteObject ( m_hbrBkgnd ); 
    }
 
    BEGIN_MSG_MAP(CPaintBkgnd)
        MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
    END_MSG_MAP()
 
    LRESULT OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        T* pT = static_cast<T*>(this);
        HDC dc = (HDC) wParam;
        RECT rcClient;
 
        pT->GetClientRect ( &rcClient );
        FillRect ( dc, &rcClient, m_hbrBkgnd );
        return 1;    // we painted the background
    }
 
protected:
    HBRUSH m_hbrBkgnd;
};

//////////////////////////////////////////////////////////////////////////
// Our main window

class CMyWindow : public CWindowImpl<CMyWindow, CWindow, CFrameWinTraits>,
                  public CPaintBkgnd<CMyWindow, huys::green>
{
private:
    typedef CPaintBkgnd<CMyWindow, huys::green> CPaintBkgndBase;

public:
    DECLARE_WND_CLASS(_T("My Window Class"))

    BEGIN_MSG_MAP(CMyWindow)
        MESSAGE_HANDLER(WM_CREATE, OnCreate)
        MESSAGE_HANDLER(WM_CLOSE, OnClose)
        MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        COMMAND_ID_HANDLER(IDC_ABOUT, OnAbout)
        CHAIN_MSG_MAP(CPaintBkgndBase)
    END_MSG_MAP()
 
    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        HMENU hm = LoadMenu ( _Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MENU1) );

        SetMenu ( hm );
        return 0;
    }

    LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        DestroyWindow();
        return 0;
    }
 
    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        PostQuitMessage(0);
        return 0;
    }
 
    LRESULT OnAbout(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
    {
		CAboutDlg dlg;
        dlg.DoModal();
        return 0;
    }
};

#endif  // ndef MYWINDOW_H_INCLUDED

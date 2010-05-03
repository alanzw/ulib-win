#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "udialogx.h"
#include "udlgapp.h"
#include "ulayout.h"
#include "ubutton.h"
#include "umsg.h"

#include "adt/uautoptr.h"

using huys::UDialogBox;

using huys::layout::UHBoxLayout;

class UDialogExt : public UDialogBox
{
    enum {
        IDC_PANNEL_GO = 1021,
        IDC_BN_FIRST  = 1022,
        IDC_BN_SECOND  = 1023,
        IDC_BN_SEARCH  = 1024
    };
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
        : UDialogBox(hInst, nID)
    {}
    
    virtual BOOL onInit()
    {
        m_pBtnFirst = new UButton(m_hDlg, IDC_BN_FIRST, m_hInst);
        m_pBtnFirst->create();
        m_pBtnFirst->setWindowText(_T("first"));

        m_pBtnSecond = new UButton(m_hDlg, IDC_BN_SECOND, m_hInst);
        m_pBtnSecond->create();
        m_pBtnSecond->setWindowText(_T("second"));

        UHBoxLayout layout;
        layout.setRect(50, 50, 200, 200);
        layout.addControl(m_pBtnFirst);
        layout.addControl(m_pBtnSecond);
        layout.go();
        
        
        m_pBtnSearch = new UButton(m_hDlg, IDC_BN_SEARCH, m_hInst);
        m_pBtnSearch->setPos(300, 200, 100, 100);
        m_pBtnSearch->create();
        m_pBtnSearch->setWindowText(_T("search"));

        return TRUE;
    }

    virtual BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch (LOWORD (wParam))
        {
        case IDC_BN_FIRST:
            return onBnFirst();
        case IDC_BN_SECOND:
            return onBnSecond();
        case IDC_BN_SEARCH:
            return onBnSearch();
        default:
            return UDialogBox::onCommand(wParam, lParam);
        }
    }

private:
    huys::ADT::UAutoPtr<UButton> m_pBtnFirst;
    huys::ADT::UAutoPtr<UButton> m_pBtnSecond;
    
    huys::ADT::UAutoPtr<UButton> m_pBtnSearch;
private:
    BOOL onBnFirst()
    {
        showMsg(_T("first chance"), _T("command"), m_hDlg);
        return FALSE;
    }

    BOOL onBnSecond()
    {
        showMsg(_T("second chance"),_T("command"), m_hDlg);
        return FALSE;
    }
    
    BOOL onBnSearch()
    {
        DWORD pid = ::GetCurrentProcessId();
        
        HWND hwnd = NULL;
    
        while (NULL != (hwnd = ::FindWindowEx(NULL, hwnd, NULL,  _T("Test_layout"))))
        {
            DWORD pidWnd;
            ::GetWindowThreadProcessId(hwnd, &pidWnd);
            if (pid == pidWnd)
            {
                showMsg("It is myself!!!!!");
            }
            else
            {
                showMsg("Another one!!!!!");
            }
        }
    
        return FALSE;
    }
};

UDLGAPP_T(UDialogExt, IDD_TEST);


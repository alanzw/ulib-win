#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "udialogx.h"
#include "udlgapp.h"
#include "ulayout.h"
#include "ubutton.h"
#include "ustatic.h"
#include "umsg.h"

#include "adt/uautoptr.h"

using huys::UDialogBox;

using huys::layout::UHBoxLayout;
using huys::layout::UGridLayout;

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
        
        m_label[0] = new UStatic(m_hDlg, -1, m_hInst);
        m_label[0]->create();
        m_label[0]->setWindowText(_T("1"));

        m_label[1] = new UStatic(m_hDlg, -1, m_hInst);
        m_label[1]->create();
        m_label[1]->setWindowText(_T("2"));

        m_label[2] = new UStatic(m_hDlg, -1, m_hInst);
        m_label[2]->create();
        m_label[2]->setWindowText(_T("3"));

        UGridLayout layout2;
        layout2.setRect(300, 100, 100, 100);
        layout2.addControls(m_label[0], 3);
        //layout2.addControl(m_label[1]);
        //layout2.addControl(m_label[2]);

        layout2.go();

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
    huys::ADT::UAutoPtr<UStatic> m_label[3];
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


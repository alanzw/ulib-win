#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <cassert>

#include "udialogx.h"
#include "ustatusbar.h"
#include "udlgapp.h"
#include "uicon.h"

using huys::UDialogBox;

class UDialogExt : public UDialogBox
{
    enum {
        IDC_STATUSBAR = 10001
    };
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
    : UDialogBox(hInst, nID)
    {}

    ~UDialogExt()
    {
        CHECK_PTR(m_pStatusBar);
    }

    virtual BOOL onInit()
    {
        m_pStatusBar = new UStatusBar(m_hDlg, IDC_STATUSBAR, m_hInst);
        m_pStatusBar->setPos(100, 100, 100, 100);
        m_pStatusBar->create();
        
        m_pStatusBar->setMinHeight(32);
        
        int aWidths[] = { 100, 200 };
        
        m_pStatusBar->setParts(2, aWidths);
        
        m_pStatusBar->setText(0, _T("hello"));
        m_pStatusBar->setText(1, _T("hello"));
        
        m_ico.loadIconEx(m_hInst, IDI_APP);
        
        m_pStatusBar->setIcon(1, m_ico);
        
        //if (!m_pStatusBar->isSimple())
        //{
        //    m_pStatusBar->simplify(TRUE);
        //    m_pStatusBar->setText(0, _T("Simple"));
        //}
        
        return TRUE;
    }
protected:
private:
    UStatusBar *m_pStatusBar;
    UIcon m_ico;
};

UDLGAPP_T(UDialogExt, IDD_DIALOG1);

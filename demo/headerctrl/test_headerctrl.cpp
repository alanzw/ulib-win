#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include "udialogx.h"
#include "uheader.h"
#include "udlgapp.h"

using huys::UDialogBox;

class MyDialog : public UDialogBox
{
    enum {
        ID_UHEADER = 11
    };
public:
    MyDialog(HINSTANCE hInst, UINT nID)
        : UDialogBox(hInst, nID){}

    ~MyDialog()
    {
        CHECK_PTR(m_pHeadCtrl);
    }

    virtual BOOL onInit()
    {
        m_pHeadCtrl = new UHeaderCtrl(m_hDlg, ID_UHEADER, m_hInst);
        m_pHeadCtrl->setPos(20, 20, 140, 140);
        m_pHeadCtrl->create();
        m_pHeadCtrl->layout();
        m_pHeadCtrl->insertItem(0, 199, "bbbbb");

        return TRUE;
    }
private:
    UHeaderCtrl *m_pHeadCtrl;
};

UDLGAPP_T(MyDialog, IDD_TEST);

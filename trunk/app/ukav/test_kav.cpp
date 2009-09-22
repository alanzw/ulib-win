#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include "colors.h"
#include "udialogx.h"
#include "udlgapp.h"
#include "ubutton.h"
#include "ukav_infobar.h"
#include "ukav_tipbar.h"

using huys::UDialogBox;

class UDialogKav6 : public UDialogBox
{
    enum {
        IDC_INFOBAR = 10001,
        IDC_TIPBAR  = 10002,
        IDC_SLINKS  = 10003
    };
public:
    UDialogKav6(HINSTANCE hInst, UINT nID)
    : UDialogBox(hInst, nID),
      m_pInfoBar(0),
      m_pUBmp(0)
    {}

    ~UDialogKav6()
    {
        CHECK_PTR(m_pInfoBar);
        CHECK_PTR(m_pUBmp);
        CHECK_PTR(m_pTipBar);
    }

    virtual BOOL onInit()
    {
        //::InvalidateRect(m_hDlg, NULL, TRUE);

        //m_pUBmp = new UBitmap();

        m_pInfoBar = new UInfoBar(m_hDlg, IDC_INFOBAR);

        m_pInfoBar->setInfo(_T("!Info Here"));

        m_pInfoBar->create();

        RECT rc;
        ::GetClientRect(m_hDlg, &rc);

        rc.left += 20;
        rc.right -= 20;
        rc.top += 5;
        rc.bottom = rc.top + 100;

        m_pInfoBar->setPosition(&rc);


        m_pTipBar = new UTipBar(m_hDlg, IDC_TIPBAR);
        m_pTipBar->setInfo(_T("!Tip Here"));
        m_pTipBar->create();

        rc.left += 200;
        rc.top += 150;
        rc.bottom = rc.top + 70;

        m_pTipBar->setPosition(&rc);


        UGroupBox uss(m_hDlg, IDC_SLINKS, m_hInst);
        //uss.setStyles(SS_BLACKRECT);
        //uss.setText(_T("ccccc"));
        uss.create();

        rc.top = rc.bottom;
        rc.bottom = rc.top + 200;
        uss.setPosition(&rc);
//        uss.setWindowText(_T(""));

        return TRUE;
    }
private:
    UInfoBar *m_pInfoBar;
    UTipBar *m_pTipBar;
    UBitmap *m_pUBmp;
};

UDLGAPP_T(UDialogKav6, IDD_KAV6);


#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "udialogx.h"
#include "udlgapp.h"
#include "udatetimepick.h"
#include "ucombobox.h"

#include "adt/uautoptr.h"

using huys::UDialogBox;

class UDialogExt : public UDialogBox
{
    enum {
        IDC_DATETIMEPICK = 1021,
        IDC_CB_FORMAT    = 1022
    };
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
    : UDialogBox(hInst, nID)
    {}

    virtual BOOL onInit()
    {
        m_pComboFormat = new UComboBox(m_hDlg, IDC_CB_FORMAT, m_hInst);
        m_pComboFormat->setPos(50, 50, 250, 100);
        m_pComboFormat->create();

        m_pDateTimePick = new UDateTimePick(m_hDlg, IDC_DATETIMEPICK, m_hInst);
        m_pDateTimePick->setStyles(DTS_TIMEFORMAT);
        m_pDateTimePick->setPos(50, 100, 180, 30);
        m_pDateTimePick->create();

        m_pComboFormat->addText(_T("hh':'m':'s'"));
        m_pComboFormat->addText(_T(" 'Now : 'hh'-'m'-'s"));
        m_pComboFormat->setCurSel();
        return TRUE;
    }

    virtual BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        if ( CBN_SELCHANGE == HIWORD(wParam))
        {
            if ( IDC_CB_FORMAT == LOWORD(wParam) )
            {
                return onCbFmtChange();
            }
        }

        return UDialogBox::onCommand(wParam, lParam);
    }

protected:
private:
    huys::ADT::UAutoPtr<UDateTimePick> m_pDateTimePick;
    huys::ADT::UAutoPtr<UComboBox> m_pComboFormat;
private:
    BOOL onCbFmtChange()
    {
        TCHAR buf[256];
        m_pComboFormat->getText(buf);
        m_pDateTimePick->setFormat(buf);
        return TRUE;
    }
};

UDLGAPP_T(UDialogExt, IDD_TEST);


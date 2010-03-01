#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include "colors.h"
#include "umsg.h"

#include "udialogx.h"
#include "udlgapp.h"

#include "uedit.h"
#include "ubutton.h"

#include "adt/uautoptr.h"

#include "uchart.h"

using huys::UDialogBox;

class UDialogChart : public UDialogBox
{
    enum {
        IDC_CHART_CTRL = 1111,
        IDC_EDIT_CTRL  = 1112,
        IDC_BTN_PRINT  = 1113,
        IDC_BTN_SAVE   = 1114,
		IDC_BTN_HELP   = 1115
    };
public:
    UDialogChart(HINSTANCE hInst, UINT nID)
    : UDialogBox(hInst, nID)
    {}

    virtual BOOL onInit()
    {
        ::InvalidateRect(m_hDlg, NULL, TRUE);

        RECT rc;
        ::GetClientRect(m_hDlg, &rc);

        m_btnPrint = new UButton(m_hDlg, IDC_BTN_PRINT, m_hInst);
        m_btnPrint->setPos(20, 5, 100, 40);
        m_btnPrint->create();
        m_btnPrint->setWindowText(_T("Print"));

        m_btnSave = new UButton(m_hDlg, IDC_BTN_SAVE, m_hInst);
        m_btnSave->setPos(150, 5, 100, 40);
        m_btnSave->create();
        m_btnSave->setWindowText(_T("Save"));

		m_btnHelp = new UButton(m_hDlg, IDC_BTN_HELP, m_hInst);
		m_btnHelp->setPos(350, 5, 100, 40);
		m_btnHelp->create();
		m_btnHelp->setWindowText(_T("Help"));

        rc.top += 50;
        rc.bottom -= 120;

        m_chart = new UChartControl(m_hDlg, IDC_CHART_CTRL, m_hInst);
        m_chart->setRect(&rc);
        m_chart->setText(_T(""));
        m_chart->create();

        //::InvalidateRect(m_hDlg, NULL, TRUE);
        rc.bottom += 120;
        rc.top = rc.bottom - 120;

        m_edit = new UEdit(m_hDlg, IDC_EDIT_CTRL, m_hInst);
        m_edit->setRect(&rc);
        m_edit->setStyles(ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN);
        m_edit->create();

        return TRUE;
    }

    virtual BOOL DialogProc(UINT message, WPARAM wParam, LPARAM lParam)
    {
        BOOL result = UDialogBox::DialogProc(message, wParam, lParam);
        HBRUSH hBrushBK = ::CreateSolidBrush(huys::aliceblue);
        switch (message)
        {
        case WM_CTLCOLORDLG:
            return (BOOL)hBrushBK;
        }
        return result;
    }


    BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch(LOWORD(wParam))
        {
        case IDC_BTN_PRINT:
            return onBnPrint();
        case IDC_BTN_SAVE:
            return onBnSave();
		case IDC_BTN_HELP:
			return onBnHelp();
        default:
            return UDialogBox::onCommand(wParam, lParam);
        }
    }
private:
    huys::ADT::UAutoPtr<UChartControl> m_chart;
    huys::ADT::UAutoPtr<UEdit> m_edit;
    huys::ADT::UAutoPtr<UButton> m_btnPrint;
    huys::ADT::UAutoPtr<UButton> m_btnSave;
	huys::ADT::UAutoPtr<UButton> m_btnHelp;
private:
    BOOL onBnPrint()
    {
        showMsg(_T("Not Implemented yet!"), _T("Info"), m_hDlg);
        return FALSE;
    }

    BOOL onBnSave()
    {
        showMsg(_T("Not Implemented yet!"), _T("Info"), m_hDlg);
        return FALSE;
    }

	BOOL onBnHelp()
	{
		showMsg(_T("Not Implemented yet!"), _T("Info"), m_hDlg);
		return FALSE;
	}
};

UDLGAPP_T(UDialogChart, IDD_CHART);


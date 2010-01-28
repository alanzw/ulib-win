#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include "colors.h"
#include "udialogx.h"
#include "udlgapp.h"

#include "uedit.h"

#include "adt/uautoptr.h"

#include "uchart.h"

using huys::UDialogBox;

class UDialogChart : public UDialogBox
{
    enum {
        IDC_CHART_CTRL = 1111,
        IDC_EDIT_CTRL  = 1112
    };
public:
    UDialogChart(HINSTANCE hInst, UINT nID)
    : UDialogBox(hInst, nID) {}

    virtual BOOL onInit()
    {
        ::InvalidateRect(m_hDlg, NULL, TRUE);

        m_chart = new UChartControl(m_hDlg, IDC_CHART_CTRL, m_hInst);
        m_chart->create();
        m_chart->subclassProc();

        RECT rc;
        ::GetClientRect(m_hDlg, &rc);
        rc.bottom -= 120;
        m_chart->setPosition(&rc);

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
private:
    huys::ADT::UAutoPtr<UChartControl> m_chart;
    huys::ADT::UAutoPtr<UEdit> m_edit;
};

UDLGAPP_T(UDialogChart, IDD_CHART);


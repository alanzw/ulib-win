#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include "colors.h"
#include "udialogx.h"
#include "udlgapp.h"

#include "uchart.h"

using huys::UDialogBox;

class UDialogChart : public UDialogBox
{
    enum {
        IDC_CHART_CTRL = 1111
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
        //case WM_CTLCOLORSTATIC:
            //return UDialogBox::onCtrlColor(wParam, lParam);
        //    return (BOOL)hBrushBK;
        }
        return result;
    }
private:
    UChartControl *m_chart;
};

UDLGAPP_T(UDialogChart, IDD_CHART);


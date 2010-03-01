#ifndef U_CHART_H
#define U_CHART_H

#include "ustatic.h"
#include "adt/uautoptr.h"

class UChartControl : public UStatic
{
public:
    UChartControl(HWND hParent, UINT nID, HINSTANCE hInst);
    virtual ~UChartControl();

    BOOL create();
    //
    BOOL onEraseBkgnd(HDC hdc);

    //virtual BOOL onNotifyReflect(WPARAM wParam, LPARAM lParam);
    virtual BOOL onCtrlColor(WPARAM wParam, LPARAM lParam);

    virtual BOOL onPaint();

    BOOL onCommand(WPARAM wParam, LPARAM lParam);
    BOOL onRButtonDown(WPARAM wParam, LPARAM lParam);
private:
    HBRUSH m_hbrush;

    enum {
        IDM_HELP = 1999,
        IDM_SHOW
    };

    huys::ADT::UAutoPtr<UBaseWindow> win;
private:
    BOOL onMenuHelp();
};

#endif // U_CHART_H


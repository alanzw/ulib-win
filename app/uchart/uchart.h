#ifndef U_CHART_H
#define U_CHART_H

#include "ustatic.h"

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
protected:
private:
    HBRUSH m_hbrush;
};

#endif // U_CHART_H


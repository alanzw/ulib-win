#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "colors.h"
#include "uchart.h"
#include "ugdi.h"
#include "udc.h"
#include "umenu.h"
#include "uicon.h"
#include "ubutton.h"

UChartControl::UChartControl(HWND hParent, UINT nResource, HINSTANCE hInst)
    : UStatic(hParent, nResource, hInst)
{
    m_dwStyles &= ~SS_SIMPLE;
    m_dwStyles |= SS_CENTER | SS_NOTIFY;

    m_hbrush = ::CreateSolidBrush(huys::red);
}

UChartControl::~UChartControl()
{}

BOOL UChartControl::create()
{
    return UStatic::create() && subclassProc();
}

BOOL UChartControl::onCtrlColor(WPARAM wParam, LPARAM lParam)
{

    //::MessageBox(NULL, "this is in reflected message function.", "Hi", MB_OK);

//    HDC hdc = (HDC)wParam;
//    HWND hwnd = (HWND)lParam;
//    RECT rc;

    //::SelectObject(hdc, hBrushBK);

    return (BOOL)m_hbrush;
}

BOOL UChartControl::onPaint()
{
     RECT rc;

     UPaintDC dc(m_hSelf);

     this->getClientRect(&rc);
     //ClientToScreen(hwnd, (LPPOINT)&rc);
     //HBRUSH holdbrush = (HBRUSH)SelectObject(hdc, m_hbrush);
     //Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
     //SelectObject(hdc, holdbrush);

     rc.left += 40;
     rc.right -= 40;
     rc.top += 40;
     rc.bottom -= 40;

     HBRUSH hNewBrush = ::CreateSolidBrush(huys::blueviolet);
     HBRUSH hOldBrush = (HBRUSH)dc.selectObj(hNewBrush);
     dc.rectangle(&rc);
     dc.selectObj(hOldBrush);

    huys::ULine uline(rc.left+20, rc.top, rc.left+20, rc.bottom);
    uline.setLineColor(huys::gray);
    //uline.setFilledColor(huys::blueviolet);
    uline.setLineStyle(PS_DOT);
    // Filling in the Gaps.
    dc.setBKMode(TRANSPARENT);

    uline.Draw(dc);
    for (int i=0; i<rc.right-rc.left-60; i+=20)
    {
        uline.offsetX(20);
        uline.Draw(dc);
    }

    uline.SetStartPnt(rc.left, rc.top+20);
    uline.SetEndPnt(rc.right, rc.top+20);
    uline.Draw(dc);
    for (int j=0; j<rc.bottom-rc.top-60; j+=20)
    {
        uline.offsetY(20);
        uline.Draw(dc);
    }

    //
    dc.drawLine(rc.left, rc.top, rc.right, rc.bottom);

    //
    dc.setTextColor(huys::white);
    dc.textOut(300, 18, _T("Chart Demo"), 10);
    dc.textOut(380, 380, _T("x"), 1);
    dc.textOut(13, 200, _T("y"), 1);
    return FALSE;
}

BOOL UChartControl::onEraseBkgnd(HDC hdc)
{
    RECT rc;
    this->getClientRect(&rc);
    HBRUSH holdbrush = (HBRUSH)::SelectObject(hdc, m_hbrush);
    ::Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
    ::SelectObject(hdc, holdbrush);

    return TRUE;
}

BOOL UChartControl::onRButtonDown( WPARAM wParam, LPARAM lParam )
{
    UMenu umn;
    UMenu upop;

    umn.createPopup();
    umn.append(IDM_HELP, _T("&help"));
    umn.addSeparator();
    umn.append(IDM_SHOW, _T("&show"));

    POINT pt;
    ::GetCursorPos(&pt);
    umn.attach(m_hSelf);
    //umn.trackPopup(TPM_RIGHTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y);
    umn.trackPopup(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y);

    return FALSE;
}

BOOL UChartControl::onCommand( WPARAM wParam, LPARAM lParam )
{
    switch(LOWORD(wParam))
    {
    case IDM_HELP:
        return onMenuHelp();
    default:
        return UStatic::onCommand(wParam, lParam);
    }
}

class UHelpWindow : public UBaseWindow
{
public:
    UHelpWindow(HWND hWnd)
        : UBaseWindow(hWnd)
    {
        RECT rc;
        ::GetWindowRect(hWnd, &rc);
        ::InflateRect(&rc, -200, -50);
        setRect(&rc);
        setTitle(_T("Help"));
    }

    BOOL onCreate()
    {
        setIconBig(IDI_CHART);

        UGroupBox groupbox(this, -1);
        groupbox.setPos(30, 150, 330, 100);
        groupbox.create();

        return TRUE;
    }

    BOOL onClose()
    {
        return this->hide();
    }

    void onDraw(HDC hdc)
    {
        UIcon icon;
        icon.loadImage(::GetModuleHandle(NULL), IDI_CHART, 128, 128);
        icon.drawEx(hdc, 20, 20, 128, 128);
        ::SetBkMode(hdc, TRANSPARENT);
        ::TextOut(hdc, 200, 100, _T("UChart Demo"), 11);

        LPCTSTR desc = _T("UChart is written by huys03@gmail.com");

        ::TextOut(hdc, 60, 200, desc, lstrlen(desc));
    }

    BOOL onPreRegisterWindowClass(huys::UWindowClass &uwc)
    {
        uwc.setBKBrush((HBRUSH)::GetStockObject(LTGRAY_BRUSH));
        return UBaseWindow::onPreRegisterWindowClass(uwc);
    }
};

BOOL UChartControl::onMenuHelp()
{
    showHelp();

    return FALSE;
}

void UChartControl::showHelp()
{
    if (win == 0)
    {
        win = new UHelpWindow(m_hSelf);
        win->create();
    }
    else
    {
        win->show();
    }
}


#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include "colors.h"
#include "udialogx.h"
#include "udlgapp.h"
#include "umsg.h"

#include "uedit.h"
#include "ustatic.h"
#include "ubutton.h"

#include "upython.h"

class UTransparentStatic : public UStatic
{
public:
    UTransparentStatic(HWND hParent, UINT nID = IDC_STATIC, HINSTANCE hInst = (HINSTANCE)::GetModuleHandle(NULL))
    : UStatic(hParent, nID, hInst)
    {
        m_dwStyles &= ~SS_SIMPLE;
        m_dwStyles |= SS_CENTER;

        m_hbrush = ::CreateSolidBrush(huys::red);
    }

    virtual ~UTransparentStatic(){};

    //
    virtual BOOL create()
    {
        BOOL bRet = UStatic::create();
        this->subclassProc();
        return  bRet;
    }

    //
    BOOL setBrush(huys::Color color)
    {
        m_hbrush = ::CreateSolidBrush(color);
        return TRUE;
    }

    virtual BOOL onCtrlColor(WPARAM wParam, LPARAM lParam)
    {
        HDC hdc = (HDC)wParam;
        HWND hwnd = (HWND)lParam;

        ::SetBkMode(hdc, TRANSPARENT);

        return (BOOL) (HBRUSH)GetStockObject(NULL_BRUSH);
    }

    //
    virtual BOOL onMessage(UINT message, WPARAM wParam, LPARAM lParam)
    {
        switch(message)
        {
        case WM_SETTEXT:
            return this->onSetText(wParam, lParam);
        default:
            return UStatic::onMessage(message, wParam, lParam);
        };
    }

    BOOL onSetText(WPARAM wParam, LPARAM lParam)
    {
        RECT Rect;
        ::GetWindowRect(m_hParent, &Rect);
        ::ScreenToClient(m_hParent, (LPPOINT)&Rect);
        ::InvalidateRect(m_hParent, &Rect, TRUE);
        return FALSE;
    }
private:
    HBRUSH m_hbrush;
};

using huys::UDialogBox;

class UDialogScript : public UDialogBox
{
    enum {
        IDC_EDIT_CMD     = 10001,
        IDC_BUTTON_CHECK = 10002
    };
public:
    UDialogScript(HINSTANCE hInst, UINT nID)
    : UDialogBox(hInst, nID),
      m_pEditCmd(NULL)
    {}

    ~UDialogScript()
    {
        CHECK_PTR(m_pEditCmd);
    }

    virtual BOOL onInit()
    {

        RECT rcTitle = {50, 10, 250, 35};
        m_pStaticTitle = new UTransparentStatic(m_hDlg);
        m_pStaticTitle->setText(_T("Python Command"));
        m_pStaticTitle->create();
        //us.modifyStyles(WS_EX_TRANSPARENT);
        m_pStaticTitle->setPosition(&rcTitle);
        //us.hide();
        //us.show();

        m_pEditCmd = new UEdit(m_hDlg, IDC_EDIT_CMD, m_hInst);
        m_pEditCmd->create();

        RECT rc;
        ::GetClientRect(m_hDlg, &rc);
        rc.left += 40;
        rc.right -= 40;
        rc.top += 40;
        rc.bottom = rc.top + 200;
        m_pEditCmd->setPosition(&rc);

        m_pButtonCheck = new UPushButton(m_hDlg, IDC_BUTTON_CHECK, m_hInst);
        m_pButtonCheck->create();
        m_pButtonCheck->setWindowText(_T("check"));
        rc.left = 450;
        rc.right = rc.left + 100;
        rc.top += 260;
        rc.bottom = rc.top + 60;
        m_pButtonCheck->setPosition(&rc);

        return TRUE;
    }

    virtual BOOL onLButtonDown(WPARAM wParam, LPARAM lParam)
    {
        return FALSE;
    }

    virtual BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch(wParam)
        {
        case IDC_BUTTON_CHECK:
            return onBnCheck();
        default:
            return FALSE;
        }
    }

    BOOL onBnCheck()
    {
        huys::UPython upy;

        if (upy.isReady())
        {
            showMsg(_T("Python is OK"), _T("Info"), m_hDlg);
        }

        return FALSE;
    }

    virtual BOOL DialogProc(UINT message, WPARAM wParam, LPARAM lParam)
    {
        BOOL result = UDialogBox::DialogProc(message, wParam, lParam);
        HBRUSH hBrushBK = ::CreateSolidBrush(huys::aliceblue);
        switch (message)
        {
        case WM_CTLCOLORDLG:
            return (BOOL)hBrushBK;
        case WM_CTLCOLORSTATIC:
            //return UDialogBox::onCtrlColor(wParam, lParam);
            return (BOOL)hBrushBK;
        }
        return result;
    }
private:
    UEdit *m_pEditCmd;
    UTransparentStatic *m_pStaticTitle;
    UPushButton *m_pButtonCheck;

};

UDLGAPP_T(UDialogScript, IDD_SCRIPT);

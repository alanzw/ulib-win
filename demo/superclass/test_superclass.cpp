#include "resource.h"

#include <windows.h>
#include <commctrl.h>
#include <tchar.h>

#include "ulib.h"
#include "udialogx.h"
#include "udlgapp.h"
#include "ubutton.h"
#include "uedit.h"
#include "colors.h"

//HINSTANCE g_hInst = 0;
const UINT ID_BTN_OK = 5555;
const UINT ID_BTN_CANCEL = 5556;

using huys::UDialogBox;

WNDPROC oldWndProc;

HINSTANCE u_hInst;

const TCHAR *lpSuperClass = _T("SUPEREDITCLASS");

LRESULT APIENTRY EditWndProc( HWND hWnd,
                              UINT uMsg,
                              WPARAM wParam,
                              LPARAM lParam )
{
    switch (uMsg)
    {
        case WM_CHAR:
            if (wParam>='0' && wParam<='9' || wParam == VK_BACK)
                return ::CallWindowProc(oldWndProc, hWnd, uMsg, wParam, lParam);
            return FALSE;
        case WM_KEYDOWN:
            switch(wParam)
            {
            case VK_RETURN:
            case VK_TAB:
            default:
                break;
            }
			return FALSE;
        default:
            return ::CallWindowProc(oldWndProc, hWnd, uMsg, wParam, lParam);
    }
}


class UEditSuper : public UEdit
{
public:
    UEditSuper(HWND hParent, UINT nID, HINSTANCE hInst)
    : UEdit(hParent, nID, hInst)
    {}
    BOOL create()
    {
        return UControl::create(lpSuperClass);
    }
};

class UDialogExt : public UDialogBox
{
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
    : UDialogBox(hInst, nID) {}

    ~UDialogExt()
    {
        if (m_pUBtnOK)
        {
            delete m_pUBtnOK;
            m_pUBtnOK = NULL;
        }

        if (m_pUBtnCancel)
        {
            delete m_pUBtnCancel;
            m_pUBtnCancel = NULL;
        }
    }

    virtual BOOL onInit()
    {
        WNDCLASSEX wcx = {0};
        wcx.cbSize = sizeof(WNDCLASSEX);
        ::GetClassInfoEx(NULL, WC_EDIT, &wcx);

        oldWndProc = wcx.lpfnWndProc;
        wcx.lpfnWndProc = EditWndProc;
        wcx.hInstance = m_hInst;

        wcx.lpszClassName = lpSuperClass;

        ::RegisterClassEx(&wcx);


        UEditSuper ueEdit(m_hDlg, 2222, m_hInst);
        ueEdit.create();
        RECT rcEdit = {20, 20, 500, 200};
        ueEdit.setPosition(&rcEdit);

        m_pUBtnOK = new UPushButton(m_hDlg, ID_BTN_OK, m_hInst);
        m_pUBtnOK->create();
        RECT rcBtn = {250, 260, 320, 300};
        m_pUBtnOK->setPosition(&rcBtn);
        m_pUBtnOK->setWindowText(_T("OK"));

        m_pUBtnCancel = new UPushButton(m_hDlg, ID_BTN_CANCEL, m_hInst);
        m_pUBtnCancel->create();
        rcBtn.left += 90;
        rcBtn.right += 90;
        m_pUBtnCancel->setPosition(&rcBtn);
        m_pUBtnCancel->setWindowText(_T("Cancel"));
        return TRUE;
    }

    virtual BOOL DialogProc(UINT message, WPARAM wParam, LPARAM lParam)
    {
        BOOL result = UDialogBox::DialogProc(message, wParam, lParam);

        if (message == WM_COMMAND)
        {
            switch (wParam) {
                case ID_BTN_OK:
                    break;
                case ID_BTN_CANCEL:
                    this->onCancel();
                    break;
                default:
                    break;
            }
        }

        return result;
    }

    virtual BOOL onPaint()
    {
        PAINTSTRUCT ps;
        HDC hdc;
        hdc = BeginPaint(m_hDlg, &ps);
        EndPaint(m_hDlg, &ps);
        return FALSE;
    }
private:
    UPushButton *m_pUBtnOK;
    UPushButton *m_pUBtnCancel;
};

UDLGAPP_T(UDialogExt, IDD_TEST);


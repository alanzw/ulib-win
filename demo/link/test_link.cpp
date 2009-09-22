#ifdef   WINVER
#undef   WINVER
#endif

#ifdef   _WIN32_IE
#undef   _WIN32_IE
#endif

#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif

#define   WINVER       0x0501
#define   _WIN32_IE    0x0501
#define   _WIN32_WINNT 0x0501

#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "ulink.h"
#include "udialogx.h"
#include "udlgapp.h"

using huys::UDialogBox;

TCHAR szText[] =  _T("For more information, ")
                  _T("<A HREF=\"http://www.microsoft.com\">click here</A> ")
                  _T(" or <A ID=\"idInfo\">here</A>.");


class UDialogExt : public UDialogBox
{
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
    : UDialogBox(hInst, nID),
      m_pLink(0)
    {}

    virtual ~UDialogExt()
    {
        CHECK_PTR(m_pLink);
    }

    virtual BOOL onInit()
    {
           m_pLink = new ULink(m_hDlg, 0, m_hInst);
           m_pLink->create(szText);
           return TRUE;
    }

    virtual BOOL DialogProc(UINT message, WPARAM wParam, LPARAM lParam)
    {
        BOOL result = UDialogBox::DialogProc(message, wParam, lParam);

        RECT rc = {200, 200, 300, 240};
        if (message == WM_NOTIFY)
        {
            if (((LPNMHDR)lParam)->hwndFrom == m_pLink->getHWND())
            {
                switch (((LPNMHDR)lParam)->code)
                {
                case NM_CLICK:
                 ::MessageBoxW(m_hDlg, ((PNMLINK)lParam)->item.szUrl, ((PNMLINK)lParam)->item.szID, MB_OK);
                 m_pLink->setPosition(&rc);
                 break;
                }
            }
        }

        return result;
    }

private:
    ULink *m_pLink;
    //ULink xx;
};

UDLGAPP_T(UDialogExt, IDD_TEST);


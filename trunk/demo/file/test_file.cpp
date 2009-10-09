#include "resource.h"

#define _WIN32_IE 0x0300

#include <windows.h>
#include <commctrl.h>
#include <tchar.h>
#include <assert.h>
#include <stdio.h>

#include "udialogx.h"
#include "udlgapp.h"
#include "uedit.h"
#include "ufile.h"

using huys::UDialogBox;

const TCHAR *g_Filename = _T("test.file");
const TCHAR *g_TestString = _T("Hi, I am here!");

class UDialogExt : public UDialogBox
{
    enum {
        IDC_ED_CONTENT = 11111
    };
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
        : UDialogBox(hInst, nID),
        m_pEditContent(0)
    {}

    ~UDialogExt()
    {
        CHECK_PTR(m_pEditContent);
    }

    virtual BOOL onInit()
    {
        m_pEditContent = new UEdit(m_hDlg, IDC_ED_CONTENT, m_hInst);
        m_pEditContent->create();

        RECT rcClient;
        ::GetClientRect(m_hDlg, &rcClient);
        RECT rc = { 5, 5, rcClient.right-5, 150 };
        m_pEditContent->setPosition(&rc);

        return TRUE;
    }

    BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch (LOWORD (wParam))
        {
        case IDC_BN_READ:
            return onBnRead();
        case IDC_BN_WRITE:
            return onBnWrite();
        defalut:
            return UDialogBox::onCommand(wParam, lParam);
        }
    }

private:
    UEdit *m_pEditContent;
private:
    BOOL onBnRead()
    {
        UFile tfile;
        tfile.open(g_Filename);
        DWORD dwRead;
        TCHAR buf[512] = {0};
        tfile.read((LPBYTE)buf, 512, &dwRead);
        tfile.close();

        m_pEditContent->setWindowText(buf);
    }

    BOOL onBnWrite()
    {
        UFile tfile;
        tfile.create(g_Filename);
        TCHAR buf[512] = {0};
        m_pEditContent->getWindowText(buf, 511);
        tfile.write((LPBYTE)buf, lstrlen(buf)*sizeof(TCHAR));
        tfile.close();
    }
};

UDLGAPP_T(UDialogExt, IDD_TEST);


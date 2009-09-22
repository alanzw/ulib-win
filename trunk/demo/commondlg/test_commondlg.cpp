#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "udialogx.h"
#include "udlgapp.h"
#include "ucommondialog.h"
#include "ubutton.h"

using huys::UDialogBox;

class UDialogExt : public UDialogBox
{
    enum {
        IDC_BUTTON_COLOR = 1021,
        IDC_BUTTON_FILE  = 1022,
        IDC_BUTTON_FONT  = 1023
    };
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
        : UDialogBox(hInst, nID),
          m_pButtonColor(0),
          m_pButtonFile(0),
          m_pButtonFont(0)
    {}

    ~UDialogExt()
    {
        CHECK_PTR(m_pButtonColor);
        CHECK_PTR(m_pButtonFile);
        CHECK_PTR(m_pButtonFont);
    }

    virtual BOOL onInit()
    {
        m_pButtonColor = new UButton(m_hDlg, IDC_BUTTON_COLOR, m_hInst);
        m_pButtonColor->create();

        RECT rc = { 200, 20, 380, 70};
        m_pButtonColor->setPosition(&rc);
        m_pButtonColor->setWindowText(_T("Choose Color"));

        m_pButtonFile = new UButton(m_hDlg, IDC_BUTTON_FILE, m_hInst);
        m_pButtonFile->create();

        rc.top += 100;
        rc.bottom += 100;
        m_pButtonFile->setPosition(&rc);
        m_pButtonFile->setWindowText(_T("Choose File"));

        m_pButtonFont = new UButton(m_hDlg, IDC_BUTTON_FONT, m_hInst);
        m_pButtonFont->create();

        rc.top += 100;
        rc.bottom += 100;
        m_pButtonFont->setPosition(&rc);
        m_pButtonFont->setWindowText(_T("Choose Font"));
    }

    virtual BOOL DialogProc(UINT message, WPARAM wParam, LPARAM lParam)
    {
        BOOL result = UDialogBox::DialogProc(message, wParam, lParam);

        return result;
    }

    virtual BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch (LOWORD (wParam))
        {
        case IDC_BUTTON_COLOR:
            return onBnColor();
        case IDC_BUTTON_FILE:
            return onBnFile();
        case IDC_BUTTON_FONT:
            return onBnFont();
        default:
            return UDialogBox::onCommand(wParam, lParam);
        }
    }
protected:
    BOOL onBnColor()
    {
        UCommonDialog::UColorDialog colordlg(m_hDlg);
        return colordlg.choose();
    }
    BOOL onBnFile()
    {
        UCommonDialog::UFileDialog filedlg(m_hDlg);
        return filedlg.open();
    }

    BOOL onBnFont()
    {
        UCommonDialog::UFontDialog fontdlg(m_hDlg);
        return fontdlg.choose();
    }
private:
    UButton *m_pButtonColor;
    UButton *m_pButtonFile;
    UButton *m_pButtonFont;
};

UDLGAPP_T(UDialogExt, IDD_TEST);


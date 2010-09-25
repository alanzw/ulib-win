#include "resource.h"

#include <windows.h>
#include <windowsx.h>
#include <tchar.h>

#include "udialogx.h"
#include "ucombobox.h"
#include "udlgapp.h"
#include "ustatic.h"

#include "adt/uautoptr.h"

class UFlatComboBox : public UComboBox
{
public:
    UFlatComboBox(HWND hParent, UINT nID, HINSTANCE hInstance)
    : UComboBox(hParent, nID, hInstance)
    {
    }

    void enableAutoCompletion(bool bEnable = true)
    {
        _bAutoCompletion = bEnable;
    }

private:
    bool _bAutoCompletion;
private:
    void drawCombo()
    {

    }

};

using huys::UDialogBox;

class UDialogExt : public UDialogBox
{
    enum {
        ID_UCOMBOBOX = 11,
        ID_USTATIC   = 12
    };
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
        : UDialogBox(hInst, nID)
    {}

    BOOL onInit()
    {
        m_pCombo = new UComboBox(m_hDlg, ID_UCOMBOBOX, m_hInst);
        m_pCombo->setPos(20, 20, 140, 140);
        m_pCombo->create();
        m_pCombo->addText("hello");
        m_pCombo->addText("hello2");
        m_pCombo->setCurSel(0);

        m_pStatic = new UStatic(m_hDlg, ID_USTATIC, m_hInst);
        m_pStatic->setPos(20, 80, 120, 30);
        m_pStatic->setStyles(SS_SUNKEN);
        m_pStatic->create();

        TCHAR buf[256];
        m_pCombo->getText(buf);
        m_pStatic->setWindowText(buf);

        return TRUE;
    }

    BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        if ( HIWORD(wParam) == CBN_SELCHANGE && LOWORD(wParam) == ID_UCOMBOBOX)
        {
            TCHAR buf[256];
            int nSel = m_pCombo->getCurSel();
            m_pCombo->getLBText(nSel, buf);
            //m_pStatic->setWindowText("");
            //m_pStatic->invalidate(TRUE);
            m_pStatic->setWindowText(buf);
            m_pStatic->hide();
            m_pStatic->show();
            return TRUE;
        }
        return UDialogBox::onCommand(wParam, lParam);
    }

private:
    huys::ADT::UAutoPtr<UComboBox> m_pCombo;
    huys::ADT::UAutoPtr<UStatic> m_pStatic;
};


UDLGAPP_T(UDialogExt, IDD_TEST);



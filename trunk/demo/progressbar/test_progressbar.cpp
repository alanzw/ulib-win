#include "resource.h"

#include <windows.h>
#include <commctrl.h>
#include <tchar.h>

#include "ulib.h"
#include "udialogx.h"
#include "udlgapp.h"
#include "uprogressbar.h"
#include "ubutton.h"

#include "udc.h"
#include "colors.h"

#include "adt/uautoptr.h"
#include "adt/ustring.h"

#include "mac_progressbar.h"

using huys::UDialogBox;

class UDialogExt : public UDialogBox
{
    enum
    {
        ID_BTN_OK = 5555,
        ID_BTN_CANCEL = 5556
    };
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
        : UDialogBox(hInst, nID) {}

    virtual BOOL onInit()
    {
        m_pUPB = new UProgressBar(m_hDlg, 3333, m_hInst);
        m_pUPB->setStyles(PBS_SMOOTH);
        m_pUPB->setPos(20,20,400,100);
        m_pUPB->create();

        m_pUPB->setRange(0, 200);
        m_pUPB->setStep(10);

        m_pUPB->setBKColor(huys::red);
        m_pUPB->setBarColor(huys::white);
        
        m_pUPB2 = new UProgressBar(m_hDlg, 3334, m_hInst);
        m_pUPB2->setStyles(PBS_SMOOTH|PBS_VERTICAL);
        m_pUPB2->setPos(20,200,50,150);
        m_pUPB2->create();

        m_pUPB2->setRange(0, 200);
        m_pUPB2->setStep(10);

        m_pUPB2->setBKColor(huys::red);
        m_pUPB2->setBarColor(huys::white);

        m_pUPBCustom = new UMacProgressBar(m_hDlg, 3335, m_hInst);
        //m_pUPBCustom->setStyles(PBS_SMOOTH|PBS_VERTICAL);
        m_pUPBCustom->setPos(120,260,250,50);
        m_pUPBCustom->setColors(huys::blue);
        m_pUPBCustom->create();
        
        m_pUPBCustom->setRange(0, 200);
        m_pUPBCustom->setStep(10);        

        
        m_pUBtnOK = new UPushButton(m_hDlg, ID_BTN_OK, m_hInst);
        m_pUBtnOK->setPos(250, 160, 70, 40);
        m_pUBtnOK->create();
        m_pUBtnOK->setWindowText(_T("OK"));

        m_pUBtnCancel = new UPushButton(m_hDlg, ID_BTN_CANCEL, m_hInst);
        m_pUBtnCancel->setPos(350, 160, 70, 40);
        m_pUBtnCancel->create();
        m_pUBtnCancel->setWindowText(_T("Cancel"));
        return TRUE;
    }

    BOOL onLButtonDown(WPARAM wParam, LPARAM lParam)
    {
        m_pUPB->stepIt();
        m_pUPB2->stepIt();
        m_pUPBCustom->stepIt();
        m_pUPBCustom->setIndeterminate(!m_pUPBCustom->getIndeterminate());
        RECT rc = {20,20,420,120};
        ::InvalidateRect(m_hDlg, &rc, TRUE);
        return FALSE;
    }

    BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch (wParam)
        {
        case ID_BTN_OK:
            m_pUPB->setPBPos(50);
            ::InvalidateRect(m_hDlg, NULL, TRUE);
            break;
        case ID_BTN_CANCEL:
            this->onCancel();
            break;
        default:
            break;
        }
        return UDialogBox::onCommand(wParam, lParam);
    }

    virtual BOOL onPaint()
    {
        UPaintDC dc(m_hDlg);

        huys::ADT::UStringAnsi str;

        int pos = 0;

        if (m_pUPB)
        {
            pos = m_pUPB->getPBPos();
        }
        str.format("Current Pos: %d", pos);

        RECT rt;
        GetClientRect(m_hDlg, &rt);
        rt.top = 5;

        dc.setBKMode(TRANSPARENT);
        dc.drawText(str, strlen(str), &rt, DT_SINGLELINE|DT_CENTER|DT_VCENTER );

        return FALSE;
    }
private:
    huys::ADT::UAutoPtr<UProgressBar> m_pUPB;
    huys::ADT::UAutoPtr<UProgressBar> m_pUPB2;
    huys::ADT::UAutoPtr<UMacProgressBar> m_pUPBCustom;
    huys::ADT::UAutoPtr<UPushButton> m_pUBtnOK;
    huys::ADT::UAutoPtr<UPushButton> m_pUBtnCancel;
};

UDLGAPP_T(UDialogExt, IDD_TEST);

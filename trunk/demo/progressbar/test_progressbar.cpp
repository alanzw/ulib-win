#include "resource.h"

#include <windows.h>
#include <commctrl.h>
#include <tchar.h>

#include "ulib.h"
#include "udialogx.h"
#include "udlgapp.h"
#include "uprogressbar.h"
#include "ubutton.h"
#include "ucommondialog.h"
#include "ubrush.h"
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
        ID_BTN_CANCEL = 5556,
        ID_BTN_PBCOLOR = 5557,
        ID_BTN_PBINDET = 5558,
        ID_BTN_PBCOLORV = 5559,
        ID_BTN_PBINDETV = 5560
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
        m_pUPBCustom->setPos(120,260,250,20);
        m_pUPBCustom->setColors(huys::blue);
        m_pUPBCustom->create();
        
        m_pUPBCustom->setRange(0, 200);
        m_pUPBCustom->setStep(10);        
        m_pUPBCustom->setPBPos(0);
        
        m_pUBtnOK = new UPushButton(m_hDlg, ID_BTN_OK, m_hInst);
        m_pUBtnOK->setPos(250, 160, 70, 40);
        m_pUBtnOK->create();
        m_pUBtnOK->setWindowText(_T("OK"));

        m_pUBtnCancel = new UPushButton(m_hDlg, ID_BTN_CANCEL, m_hInst);
        m_pUBtnCancel->setPos(350, 160, 70, 40);
        m_pUBtnCancel->create();
        m_pUBtnCancel->setWindowText(_T("Cancel"));


        UGroupBox gb(m_hDlg, -1, m_hInst);
        gb.setPos(100, 300, 400, 150);
        gb.create();
        gb.setWindowText(_T("Progress Bar"));

        m_pPBMac = new UMacProgressBar(m_hDlg, 3336, m_hInst);
        m_pPBMac->setPos(120, 340, 150, 15);
        m_pPBMac->setStyles(PBS_SMOOTH);
        m_pPBMac->create();
        m_pPBMac->setRange(0, 100);
        m_pPBMac->setPBPos(50);

        m_pBnPBColor = new UButton(m_hDlg, ID_BTN_PBCOLOR, m_hInst);
        m_pBnPBColor->setPos(130, 370, 120, 30);
        m_pBnPBColor->create();
        m_pBnPBColor->setWindowText(_T("Set Fill Color"));

        m_pBnPBIndet = new UCheckButton(m_hDlg, ID_BTN_PBINDET, m_hInst);
        m_pBnPBIndet->setPos(130, 410, 120, 30);
        m_pBnPBIndet->create();
        m_pBnPBIndet->setWindowText(_T("Indeterminate"));
        m_pBnPBIndet->uncheck();

        m_pPBMacV = new UMacProgressBar(m_hDlg, 3337, m_hInst);
        m_pPBMacV->setPos(320, 320, 15, 100);
        m_pPBMacV->setStyles(PBS_SMOOTH|PBS_VERTICAL);
        m_pPBMacV->create();
        m_pPBMacV->setRange(0, 100);
        m_pPBMacV->setPBPos(50);

        m_pBnPBColorV = new UButton(m_hDlg, ID_BTN_PBCOLORV, m_hInst);
        m_pBnPBColorV->setPos(370, 370, 120, 30);
        m_pBnPBColorV->create();
        m_pBnPBColorV->setWindowText(_T("Set Fill Color"));
        
        m_pBnPBIndetV = new UCheckButton(m_hDlg, ID_BTN_PBINDETV, m_hInst);
        m_pBnPBIndetV->setPos(370, 410, 120, 30);
        m_pBnPBIndetV->create();
        m_pBnPBIndetV->setWindowText(_T("Indeterminate"));
        m_pBnPBIndetV->uncheck();

        return TRUE;
    }

    BOOL onLButtonDown(WPARAM wParam, LPARAM lParam)
    {
        m_pUPB->stepIt();
        m_pUPB2->stepIt();
        m_pUPBCustom->stepIt();
        m_pUPBCustom->setIndeterminate(!m_pUPBCustom->getIndeterminate());
//         RECT rc = {20,20,420,120};
//         ::InvalidateRect(m_hDlg, &rc, TRUE);
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
        case ID_BTN_PBCOLOR:
            return onBnPBColor();
        case ID_BTN_PBINDET:
            return onBnPBIndet();
        case ID_BTN_PBCOLORV:
            return onBnPBColorV();
        case ID_BTN_PBINDETV:
            return onBnPBIndetV();
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

    huys::ADT::UAutoPtr<UMacProgressBar> m_pPBMac;
    huys::ADT::UAutoPtr<UButton> m_pBnPBColor;
    huys::ADT::UAutoPtr<UCheckButton> m_pBnPBIndet;

    huys::ADT::UAutoPtr<UMacProgressBar> m_pPBMacV;
    huys::ADT::UAutoPtr<UButton> m_pBnPBColorV;
    huys::ADT::UAutoPtr<UCheckButton> m_pBnPBIndetV;
private:
    BOOL onBnPBColor()
    {
        UCommonDialog::UColorDialog colordlg(m_hDlg);
        if(colordlg.choose())
        {
            m_pPBMac->setColors(colordlg.resultColor());
            m_pPBMac->invalidate();
        }
        return FALSE;
    }

    BOOL onBnPBIndet()
    {
        if (m_pBnPBIndet->isChecked())
        {
            m_pPBMac->setIndeterminate(TRUE);
        }
        else
        {
            m_pPBMac->setIndeterminate(FALSE);
        }
        m_pPBMac->invalidate();

        return FALSE;
    }
    BOOL onBnPBColorV()
    {
        UCommonDialog::UColorDialog colordlg(m_hDlg);
        if(colordlg.choose())
        {
            m_pPBMacV->setColors(colordlg.resultColor());
            m_pPBMacV->invalidate();
        }
        return FALSE;
    }
    
    BOOL onBnPBIndetV()
    {
        if (m_pBnPBIndetV->isChecked())
        {
            m_pPBMacV->setIndeterminate(TRUE);
        }
        else
        {
            m_pPBMacV->setIndeterminate(FALSE);
        }
        m_pPBMacV->invalidate();
        
        return FALSE;
    }
};

UDLGAPP_T(UDialogExt, IDD_TEST);

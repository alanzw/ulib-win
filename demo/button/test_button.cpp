#include "resource.h"

#define _WIN32_WINNT 0x0400

#include <windows.h>
#include <tchar.h>
#include <windowsx.h>
#include <commctrl.h>

#include "udialogx.h"
#include "ubutton.h"
#include "udlgapp.h"
#include "colors.h"
#include "umsg.h"
#include "ubitmap.h"
#include "uicon.h"
#include "ustatic.h"

#include "cool_button.h"
#include "imagebutton.h"
#include "round_button.h"
#include "pulse_button.h"

#include "adt/uautoptr.h"

class USkinStatic : public UStatic
{
public:
    USkinStatic(HWND hParent, UINT nBitmap)
        : UStatic(hParent, IDC_STATIC, ::GetModuleHandle(NULL))
    {
        m_dwStyles &= ~SS_SIMPLE;
        _ubm.loadFromResource(nBitmap, m_hInstance);
    }

    virtual BOOL create()
    {
        BOOL bRet = UStatic::create();
        this->subclassProc();
        return  bRet;
    }

    BOOL onPaint()
    {
    //!! BeginPaint will eat previous control text drawing or other actions
        PAINTSTRUCT ps;
        HDC hdc;
        hdc = BeginPaint(m_hSelf, &ps);
    ////hdc = ::GetDC(m_hSelf);
        onDraw(hdc);

    ////::ReleaseDC(m_hSelf, hdc);
        EndPaint(m_hSelf, &ps);
        return FALSE;
    }
//private:
protected:
    virtual void onDraw(HDC hdc)
    {
        RECT rc;
        this->getClientRect(&rc);

        _ubm.showStretch(hdc, rc);
    }

private:
    UBitmap _ubm;
};

class USkinStatusBar : public USkinStatic
{
public:
    USkinStatusBar(HWND hParent, UINT nBitmap)
    : USkinStatic(hParent, nBitmap)
    {}

    BOOL loadBmpSB(UINT nBitmap, HINSTANCE hInst = ::GetModuleHandle(NULL))
    {
        return _ubmSB.loadFromResource(nBitmap, hInst);
    }

private:
    UBitmap _ubmSB;
private:
    void onDraw(HDC hdc)
    {
        USkinStatic::onDraw(hdc);

        if (!_ubmSB.isNull())
        {
            _ubmSB.drawImage(hdc, 5, 5, 0, 0, 4, 3);
            _ubmSB.drawImage(hdc, 25, 5, 0, 1, 4, 3);
            _ubmSB.drawImage(hdc, 45, 5, 0, 2, 4, 3);
        }
    }
};

using huys::UDialogBox;

class UDialogExt : public UDialogBox
{
    enum {
        IDC_BUTTON_UE    = 3333,
        IDC_BUTTON_CHECK = 3334,
        IDC_BUTTON_RA    = 3335,
        IDC_BUTTON_RA2   = 3336,
        IDC_GROUPBOX     = 3337,
        IDC_ICONBUTTON   = 3338,
        IDC_BN_OWNERDRAWN= 4111,
        IDC_BN_ICONTEXT  = 4333,
        IDC_BN_COOL      = 4335,
        IDC_BN_BITMAP    = 4336,
        IDC_BN_ROUND     = 4337,
        IDC_BN_PULSE     = 4338
    };
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
    : UDialogBox(hInst, nID)
    {}

    ~UDialogExt()
    {}

    virtual BOOL onInit()
    {

        m_Icon.loadImage(m_hInst, IDI_APP, 32, 32);

        RECT rcRa = {250, 190, 350, 290};
        RECT rcRa2 = {420, 190, 520, 290};
        RECT rcIco = {5, 200, 115, 300};
        RECT rcOD = {460, 70, 560, 150};
        RECT rcGB = {220, 20, 650, 300};
        RECT rcCH = {250, 50, 350, 150};
        //RECT rcIT = {10, 310, 210, 410};
        RECT rcTest = {10, 20, 110, 160};

        RECT rcCool = {250, 350, 450, 410};

        m_pBtn = new UButton(m_hDlg, IDC_BUTTON_UE, m_hInst);
        m_pBtn->create();
        m_pBtn->setWindowText(_T("help"));
        m_pBtn->setPosition(&rcTest);

        m_pCheckBtn = new UCheckButton(m_hDlg, IDC_BUTTON_CHECK, m_hInst);
        m_pCheckBtn->create();
        m_pCheckBtn->setWindowText(_T("Check me"));
        m_pCheckBtn->setPosition(&rcCH);

        m_pRadioBtn1 = new URadioButton(m_hDlg, IDC_BUTTON_RA, m_hInst);
        m_pRadioBtn1->create();
        m_pRadioBtn1->setWindowText(_T("Radio me"));
        m_pRadioBtn1->setPosition(&rcRa);

        m_pRadioBtn2 = new URadioButton(m_hDlg, IDC_BUTTON_RA2, m_hInst);
        m_pRadioBtn2->create();
        m_pRadioBtn2->setWindowText(_T("Radio you"));
        m_pRadioBtn2->setPosition(&rcRa2);

        m_pGroupBox = new UGroupBox(m_hDlg, IDC_GROUPBOX, m_hInst);
        m_pGroupBox->create();
        m_pGroupBox->setPosition(&rcGB);

        m_pIconBtn = new UIconButton(m_hDlg, IDC_ICONBUTTON, m_hInst);
        m_pIconBtn->create();
        m_pIconBtn->setWindowText(_T("Icon"));
        m_pIconBtn->setIcon(m_Icon);
        m_pIconBtn->setPosition(&rcIco);


        m_pOwnerDrawnBtn = new UOwnerDrawnButton(m_hDlg, IDC_BN_OWNERDRAWN, m_hInst);
        m_pOwnerDrawnBtn->create();
        m_pOwnerDrawnBtn->setPosition(&rcOD);
        m_pOwnerDrawnBtn->setWindowText(_T("OwnerDraw"));


        m_pIconTextBtn = new UIconTextButton(m_hDlg, IDC_BN_ICONTEXT, m_hInst);

        m_pCoolBtn = new UCoolButton(m_hDlg, IDC_BN_COOL, m_hInst);
        m_pCoolBtn->attachBitmap();
        m_pCoolBtn->create();
        m_pCoolBtn->setBtnPos(rcCool.left, rcCool.top);

        m_pImageBtn = new UImageButton(m_hDlg, IDC_BN_BITMAP, m_hInst);
        m_pImageBtn->setPos(380, 310, 360, 100);
        m_pImageBtn->setTextColor(huys::blue);
        m_pImageBtn->setAlignMode(UImageButton::TEXT_INCLUDE | UImageButton::IMAGE_RIGHT | UImageButton::IMAGE_VCENTER);
        m_pImageBtn->create();
        m_pImageBtn->setWindowText(_T("ÎÒ°®Äã"));
        m_pImageBtn->setButtomImage(IDB_IMAGE, huys::magenta);

        m_pRoundBtn = new URoundButton(m_hDlg, IDC_BN_ROUND, m_hInst);
        m_pRoundBtn->setPos(680, 250, 50, 50);
        m_pRoundBtn->setColorFocusCircle(huys::blue);
        m_pRoundBtn->create();
        m_pRoundBtn->resize();
        m_pRoundBtn->setWindowText(_T("P"));

        m_pPulseBtn = new UPulseButton(m_hDlg, IDC_BN_PULSE, m_hInst);
        m_pPulseBtn->setPos(680, 100, 50, 50);
        m_pPulseBtn->create();
        m_pPulseBtn->setWindowText(_T("CC"));

        m_skinStatic = new USkinStatic(m_hDlg, IDB_STATUSBAR);
        m_skinStatic->setPos(20, 450, 100, 27);
        m_skinStatic->create();

        m_skinStatusBar = new USkinStatusBar(m_hDlg, IDB_STATUSBAR);
        m_skinStatusBar->setPos(150, 450, 100, 27);
        m_skinStatusBar->loadBmpSB(IDB_STATUSTOOLBAR);
        m_skinStatusBar->create();
        return TRUE;
    }

    virtual BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        RECT rcIT = {10, 310, 210, 410};
        switch (LOWORD (wParam))
        {
            case IDC_BUTTON_UE:
                showCustomInfoMsg("You Press me!", m_hDlg);
                //::MessageBox(hDlg, "xx", "xx", MB_OK);
                return TRUE;
            case IDC_BUTTON_CHECK:
                showCustomInfoMsg("Check me!", m_hDlg);
                if (IsDlgButtonChecked(m_hDlg, IDC_BUTTON_CHECK))
                {
                    m_pBtn->disable();
                }
                else
                {
                    m_pBtn->enable();
                }
                return TRUE;
            case IDC_BUTTON_RA:
                {
                    m_pImageBtn->enable();
                }
                return TRUE;
            case IDC_BUTTON_RA2:
                {
                    m_pImageBtn->disable();
                }
                return TRUE;
            case IDC_ICONBUTTON:
                //showCustomInfoMsg("Icon here!", m_hDlg);
                return TRUE;
            case IDC_BN_OWNERDRAWN:
                //showCustomInfoMsg("Hello Owner!", m_hDlg);
                {
                    if (NULL == m_pIconTextBtn->getHWND())
                    {
                        m_pIconTextBtn->create();
                        m_pIconTextBtn->setPosition(&rcIT);
                        m_pIconTextBtn->setIcon(m_Icon);
                        m_pIconTextBtn->setWindowText(_T("IT"));
                    //m_pIconTextBtn->subclassProc();
                    }
                }
                return TRUE;
            case IDC_BN_COOL:
                //showMsg("Cool!", "hi", m_hDlg);
                return TRUE;
            case IDC_BN_BITMAP:
                return this->onBnImage();
            case IDOK :
                m_pBtn->setWindowText(_T("New Text"));
                return TRUE;
        default:
            return UDialogBox::onCommand(wParam, lParam);
        }
    }
private:
    huys::ADT::UAutoPtr<UButton> m_pBtn;
    huys::ADT::UAutoPtr<UCheckButton> m_pCheckBtn;
    huys::ADT::UAutoPtr<URadioButton> m_pRadioBtn1;
    huys::ADT::UAutoPtr<URadioButton> m_pRadioBtn2;
    huys::ADT::UAutoPtr<UGroupBox> m_pGroupBox;
    huys::ADT::UAutoPtr<UIconButton> m_pIconBtn;
    huys::ADT::UAutoPtr<UOwnerDrawnButton> m_pOwnerDrawnBtn;
    huys::ADT::UAutoPtr<UIconTextButton> m_pIconTextBtn;

    UIcon m_Icon;

    huys::ADT::UAutoPtr<UCoolButton> m_pCoolBtn;
    huys::ADT::UAutoPtr<UImageButton> m_pImageBtn;
    huys::ADT::UAutoPtr<URoundButton> m_pRoundBtn;
    huys::ADT::UAutoPtr<UPulseButton> m_pPulseBtn;

    huys::ADT::UAutoPtr<USkinStatic> m_skinStatic;
    huys::ADT::UAutoPtr<USkinStatusBar> m_skinStatusBar;
private:
    BOOL onBnImage()
    {
        static BOOL bFlag = FALSE;
        if (!bFlag)
        {
            m_pImageBtn->setAlignMode(UImageButton::TEXT_INCLUDE | UImageButton::IMAGE_VCENTER);
        }
        else
        {
            m_pImageBtn->setAlignMode(UImageButton::TEXT_INCLUDE |
                UImageButton::IMAGE_RIGHT | UImageButton::IMAGE_VCENTER);
        }
        bFlag = !bFlag;
        m_pImageBtn->reset();
        return FALSE;
    }
};

UDLGAPP_T(UDialogExt, IDD_TEST);


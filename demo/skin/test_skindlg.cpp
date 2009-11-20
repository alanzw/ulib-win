#define  WINVER 0x0501

#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "udialogx.h"
#include "ubutton.h"
#include "umsg.h"
#include "udlgapp.h"

#include "skin.h"

using huys::UDialogBox;

class USkinnedDialog : public UDialogBox
{
    enum {
        IDC_BUTTON_UE = 3333,
        IDC_BUTTON_CHECK = 3334,
        IDC_BUTTON_RA = 3335,
        IDC_BUTTON_RA2 = 3336,
        IDC_GROUPBOX = 3337,
        IDC_ICONBUTTON = 3338
    };
public:
    USkinnedDialog(HINSTANCE hInst, UINT nID)
    : UDialogBox(hInst, nID),
      m_hDll(NULL),
      m_pDlgTest(NULL)
    {
        IStart( m_hInst, ::GetCurrentThreadId() );
    }

    ~USkinnedDialog()
    {
        CHECK_PTR(m_pBnTest);
        CHECK_PTR(m_pCKTest);
        CHECK_PTR(m_pRATest);
        CHECK_PTR(m_pRASecond);
        CHECK_PTR(m_pGBTest);
        CHECK_PTR(m_pIconTest);
        CHECK_PTR(m_pDlgTest);
    }

    virtual BOOL onInit()
    {
        m_pBnTest = new UButton(m_hDlg, IDC_BUTTON_UE, m_hInst);
        m_pBnTest->setStyles(WS_TABSTOP);
        m_pBnTest->setPos(20, 60, 90, 100);
        m_pBnTest->create();
        m_pBnTest->setWindowText(_T("help"));

        m_pCKTest = new  UCheckButton(m_hDlg, IDC_BUTTON_CHECK, m_hInst);
        m_pCKTest->setPos(150, 50, 100, 100);
        m_pCKTest->create();
        m_pCKTest->setWindowText("Check me");

        m_pRATest = new URadioButton(m_hDlg, IDC_BUTTON_RA, m_hInst);
        m_pRATest->setPos(150, 190, 100, 100);
        m_pRATest->create();
        m_pRATest->setWindowText("Radio me");

        m_pRASecond = new URadioButton(m_hDlg, IDC_BUTTON_RA2, m_hInst);
        m_pRASecond->setPos(320, 190, 100, 100);
        m_pRASecond->create();
        m_pRASecond->setWindowText("Radio you");

        m_pGBTest = new UGroupBox(m_hDlg, IDC_GROUPBOX, m_hInst);
        m_pGBTest->setPos(120, 40, 430, 360);
        m_pGBTest->create();

        m_pIconTest = new UIconButton(m_hDlg, IDC_ICONBUTTON, m_hInst);
        m_pIconTest->setPos(20, 200, 100, 100);
        m_pIconTest->create();
        HICON hIcon = (HICON)LoadImage( m_hInst, // small class icon
            MAKEINTRESOURCE(IDI_APP),
            IMAGE_ICON,
            GetSystemMetrics(SM_CXSMICON)*2,
            GetSystemMetrics(SM_CYSMICON)*2,
            LR_DEFAULTCOLOR );
        m_pIconTest->setIcon(hIcon);
        m_pIconTest->setWindowText("hhh");

        return TRUE;
    }

    virtual BOOL onClose()
    {
        IStop();

        if (m_hDll) ::FreeLibrary(m_hDll);
        m_hDll = NULL;

        return UDialogBox::onClose();
    }

    virtual BOOL onLButtonDown(WPARAM wParam, LPARAM lParam)
    {
        ::PostMessage(m_hDlg, WM_NCLBUTTONDOWN, (WPARAM)HTCAPTION, (LPARAM)lParam);
        return UDialogBox::onLButtonDown(wParam, lParam);
    }

    virtual BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch (LOWORD(wParam))
        {
        case IDC_BUTTON_UE:
            {
            showMsg("You Press me!", "info", m_hDlg);
            return FALSE;
            }
        case IDC_BUTTON_CHECK:
            showMsg("Check me!", "info");
            if (IsDlgButtonChecked(m_hDlg, IDC_BUTTON_CHECK))
            {
                m_pBnTest->disable();
            }
            else
            {
                m_pBnTest->enable();
            }
            return TRUE;
        case IDC_BUTTON_RA:
            showCustomInfoMsg("Radio me!", m_hDlg);
            return TRUE;
        case IDC_BUTTON_RA2:
            if (!m_pDlgTest)
            {
                m_hDll = ::LoadLibrary("libskin.dll");
                if (NULL == m_hDll)
                {
                    showCustomInfoMsg("Cannot load dll", m_hDlg);
                    return FALSE;
                }
                m_hRes = ::FindResource(m_hDll, MAKEINTRESOURCE(IDD_DIALOG1), RT_DIALOG);
                if (m_hRes == NULL)
                {
                    showCustomInfoMsg("Could not locate dialog box.", m_hDlg);
                    return FALSE;
                }
                m_pDlgTest = new UDialogBox(m_hDll, IDD_DIALOG1, UDialogBox::DefaultDlgProc, m_hDlg);
                m_pDlgTest->create();
            }
            //::FreeLibrary(hdll);
            return TRUE;
        case IDC_ICONBUTTON:
            showCustomInfoMsg("Icon here!", m_hDlg);
            return TRUE;
        case IDOK :
            m_pBnTest->setWindowText("New Text");
            return TRUE;
        default:
            return UDialogBox::onCommand(wParam, lParam);
        }
    }

    virtual BOOL DialogProc(UINT message, WPARAM wParam, LPARAM lParam)
    {
        BOOL bResult = UDialogBox::DialogProc(message, wParam, lParam);

        switch (message)
        {
        case WM_MOVE:
        case WM_PAINT:
        case WM_NCPAINT:
        case WM_NOTIFY:
        case WM_ACTIVATE:
        //case WM_NCACTIVATE:
            drawTitleBar();
        }
        return bResult;
    }
private:
    HMODULE m_hDll;
    HRSRC m_hRes;
    UButton *m_pBnTest;
    UCheckButton *m_pCKTest;
    URadioButton *m_pRATest;
    URadioButton *m_pRASecond;
    UGroupBox *m_pGBTest;
    UIconButton *m_pIconTest;

    UDialogBox * m_pDlgTest;

    void drawTitleBar()
    {
        HDC hdc = GetWindowDC(m_hDlg);

        RECT rtMinButton, rtMaxButton, rtExitButton;

        HBRUSH hBrush, hOldBrush;
        hBrush = ::CreateSolidBrush(RGB(0,100,255));
        hOldBrush = (HBRUSH)::SelectObject(hdc, hBrush);

        RECT rtWnd, rtTitle, rtButtons;
        ::GetWindowRect(m_hDlg, &rtWnd);
        //
        rtTitle.left = GetSystemMetrics(SM_CXFRAME);
        rtTitle.top = GetSystemMetrics(SM_CYFRAME);
        rtTitle.right = rtWnd.right - rtWnd.left - GetSystemMetrics(SM_CXFRAME);
        rtTitle.bottom = rtTitle.top + GetSystemMetrics(SM_CYSIZE);

        //15X15
        rtButtons.left = rtTitle.right-60;
        rtButtons.top= rtTitle.top+2;
        rtButtons.right = rtTitle.right-42;
        rtButtons.bottom = rtTitle.top+20;
        rtMinButton = rtButtons;
        //15X15
        rtMaxButton.left = rtMinButton.left + 18;
        rtMaxButton.top = rtMinButton.top;
        rtMaxButton.right = rtMinButton.right + 16;
        rtMaxButton.bottom = rtMinButton.bottom;

        rtExitButton.left = rtMaxButton.left + 18;
        rtExitButton.top =  rtMaxButton.top;
        rtExitButton.right = rtMaxButton.right + 16;
        rtExitButton.bottom = rtMaxButton.bottom;


        //
        HBITMAP hBitmap, hOldBitmap;
        BITMAP BmpInfo;
        BITMAP pOldBitmap;
        HDC hDisplayMemDC;
        hDisplayMemDC = ::CreateCompatibleDC(hdc);

        //Caption
        POINT  DrawPonit;
        DrawPonit.x =    rtTitle.left-4;
        DrawPonit.y =    rtTitle.top-4;
        hBitmap = ::LoadBitmap(m_hInst, MAKEINTRESOURCE(IDB_TITLE_LEFT));
        hOldBitmap= (HBITMAP)::SelectObject(hDisplayMemDC, hBitmap);
        ::GetObject(hBitmap, sizeof(BITMAP), &BmpInfo);
        ::BitBlt(hdc, rtTitle.left-4, rtTitle.top-4, BmpInfo.bmWidth, BmpInfo.bmHeight, hDisplayMemDC, 0, 0, SRCCOPY);
        DrawPonit.x = DrawPonit.x + BmpInfo.bmWidth;
        ::SelectObject(hDisplayMemDC, hOldBitmap);
        ::DeleteObject(hBitmap);

        hBitmap = ::LoadBitmap(m_hInst, MAKEINTRESOURCE(IDB_TOP));
        hOldBitmap = (HBITMAP)::SelectObject(hDisplayMemDC, hBitmap);
        ::GetObject(hBitmap, sizeof(BITMAP), &BmpInfo);
        while(DrawPonit.x<= rtMinButton.left-66)
        {
            ::BitBlt(hdc, DrawPonit.x, DrawPonit.y, BmpInfo.bmWidth, BmpInfo.bmHeight, hDisplayMemDC, 0, 0, SRCCOPY);
            DrawPonit.x = DrawPonit.x + BmpInfo.bmWidth;
        }
        ::SelectObject(hDisplayMemDC, hOldBitmap);
        ::DeleteObject(hBitmap);

        hBitmap = ::LoadBitmap(m_hInst, MAKEINTRESOURCE(IDB_TITLE_RIGHT));
        hOldBitmap = (HBITMAP)::SelectObject(hDisplayMemDC, hBitmap);
        ::GetObject(hBitmap, sizeof(BITMAP), &BmpInfo);
        ::BitBlt(hdc, DrawPonit.x, DrawPonit.y, BmpInfo.bmWidth, BmpInfo.bmHeight, hDisplayMemDC, 0, 0, SRCCOPY);

        ::SelectObject(hDisplayMemDC, hOldBitmap);
        ::DeleteObject(hBitmap);



        //button
        rtButtons = rtMinButton;
        hBitmap = ::LoadBitmap(m_hInst, MAKEINTRESOURCE(IDB_NORMAL));
        ::GetObject(hBitmap, sizeof(BITMAP), &BmpInfo);
        hOldBitmap = (HBITMAP)::SelectObject(hDisplayMemDC, hBitmap);
        ::BitBlt(hdc, rtButtons.left-6, rtButtons.top-2, BmpInfo.bmWidth, BmpInfo.bmHeight, hDisplayMemDC, 0, 0, SRCCOPY);
        ::SelectObject(hDisplayMemDC, hOldBitmap);
        ::DeleteObject(hBitmap);

        int nOldMode = ::SetBkMode(hdc, TRANSPARENT);
        COLORREF clOldText = ::GetTextColor(hdc);
        HFONT hTitleFont;
        hTitleFont = ::CreateFont( 12, // nHeight
            8, // nWidth
            0, // nEscapement
            0, // nOrientation
            FW_BOLD, // nWeight
            FALSE, // bItalic
            FALSE, // bUnderline
            0, // cStrikeOut
            ANSI_CHARSET, // nCharSet
            OUT_DEFAULT_PRECIS, // nOutPrecision
            CLIP_DEFAULT_PRECIS, // nClipPrecision
            DEFAULT_QUALITY, // nQuality
            DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily
            _T("SimSun")); // lpszFac        pDC->SelectStockObject(SYSTEM_FIXED_FONT);
        HFONT hOldFont;
        hOldFont = (HFONT)::SelectObject(hdc, hTitleFont);
        char szTitle[256];
        ::GetWindowText(m_hDlg, szTitle, 255);
        ::SetTextColor(hdc, RGB(80,255,25));
        wsprintf(szTitle, "hello skin!");
        ::TextOut(hdc, 165, 10, szTitle, strlen(szTitle));

        ::SetBkMode(hdc, nOldMode);
        ::SetTextColor(hdc, clOldText);
        ::SelectObject(hdc, hOldFont);
        // pDC->TextOut(60,60,m_StrTitle);
        //
        hBitmap = ::LoadBitmap(m_hInst, MAKEINTRESOURCE(IDB_LEFT_DOWN));
        ::GetObject(hBitmap, sizeof(BITMAP), &BmpInfo);
        hOldBitmap = (HBITMAP)::SelectObject(hDisplayMemDC, hBitmap);
        int i ;
        for (i= 20;i<=rtWnd.bottom;i=i+BmpInfo.bmHeight-3)
        {
            ::BitBlt(hdc, 0, rtButtons.top+i, BmpInfo.bmWidth, BmpInfo.bmHeight, hDisplayMemDC, 0, 0, SRCCOPY);
        }
        ::SelectObject(hDisplayMemDC, hOldBitmap);
        ::DeleteObject(hBitmap);

        //
        hBitmap = ::LoadBitmap(m_hInst, MAKEINTRESOURCE(IDB_RIGHT_DOWN));
        ::GetObject(hBitmap, sizeof(BITMAP), &BmpInfo);
        hOldBitmap = (HBITMAP)::SelectObject(hDisplayMemDC, hBitmap);
        GetClientRect(m_hDlg, &rtWnd);
        for (i= 25;i<=rtWnd.bottom+27;i=i+BmpInfo.bmHeight-3)
        {
            ::BitBlt(hdc, rtWnd.right, i, BmpInfo.bmWidth, BmpInfo.bmHeight, hDisplayMemDC, 0, 0, SRCCOPY);
        }
        ::SelectObject(hDisplayMemDC, hOldBitmap);
        ::DeleteObject(hBitmap);

        //
        hBitmap = ::LoadBitmap(m_hInst, MAKEINTRESOURCE(IDB_DOWN));
        ::GetObject(hBitmap, sizeof(BITMAP), &BmpInfo);
        hOldBitmap = (HBITMAP)::SelectObject(hDisplayMemDC, hBitmap);
        GetClientRect(m_hDlg, &rtWnd);
        for (i= 9; i<=rtWnd.right; i=i+2)
        {
            //GetClientRect(hdlg, &rtWnd);
            ::BitBlt(hdc, i, rtWnd.bottom, BmpInfo.bmWidth, BmpInfo.bmHeight, hDisplayMemDC, 0, 0, SRCCOPY);
        }
        ::SelectObject(hDisplayMemDC, hOldBitmap);
        ::DeleteObject(hBitmap);

        //
        //    m_rtIcon.left = 3;//rtWnd.Width()-5;
        //    m_rtIcon.top =  GetSystemMetrics(SM_CYFRAME);
        //    m_rtIcon.right = m_rtIcon.left + 32;
        //    m_rtIcon.bottom = m_rtIcon.top + 32;
        //    ::DrawIconEx(pDC->m_hDC, m_rtIcon.left, m_rtIcon.top, m_hIcon,
        //    m_rtIcon.Width(), m_rtIcon.Height(), 0, NULL, DI_NORMAL);
        //    m_rtIcon.OffsetRect(rtWnd.TopLeft());

        ReleaseDC(m_hDlg, hDisplayMemDC);
        ReleaseDC(m_hDlg, hdc);
    }
};

UDLGAPP_T(USkinnedDialog, IDD_DIALOG1);


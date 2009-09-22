#define  WINVER 0x0501

#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "udialog.h"
#include "ubutton.h"
#include "umsg.h"
#include "udlgapp.h"

#include "skin.h"

extern HINSTANCE g_hInst;

HMODULE hdll;
HRSRC hRes;
UDialogBox *ueDlg = NULL;

RECT rtMinButton, rtMaxButton, rtExitButton;

void drawTitleBar(HWND hdlg)
{
    HDC hdc = GetWindowDC(hdlg);

    HBRUSH hBrush, hOldBrush;
    hBrush = ::CreateSolidBrush(RGB(0,100,255));
    hOldBrush = (HBRUSH)::SelectObject(hdc, hBrush);

    RECT rtWnd, rtTitle, rtButtons;
    GetWindowRect(hdlg, &rtWnd);
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
    hBitmap = ::LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_TITLE_LEFT));
    hOldBitmap= (HBITMAP)::SelectObject(hDisplayMemDC, hBitmap);
    ::GetObject(hBitmap, sizeof(BITMAP), &BmpInfo);
    ::BitBlt(hdc, rtTitle.left-4, rtTitle.top-4, BmpInfo.bmWidth, BmpInfo.bmHeight, hDisplayMemDC, 0, 0, SRCCOPY);
    DrawPonit.x = DrawPonit.x + BmpInfo.bmWidth;
    ::SelectObject(hDisplayMemDC, hOldBitmap);
    ::DeleteObject(hBitmap);

    hBitmap = ::LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_TOP));
    hOldBitmap = (HBITMAP)::SelectObject(hDisplayMemDC, hBitmap);
    ::GetObject(hBitmap, sizeof(BITMAP), &BmpInfo);
    while(DrawPonit.x<= rtMinButton.left-66)
    {
        ::BitBlt(hdc, DrawPonit.x, DrawPonit.y, BmpInfo.bmWidth, BmpInfo.bmHeight, hDisplayMemDC, 0, 0, SRCCOPY);
        DrawPonit.x = DrawPonit.x + BmpInfo.bmWidth;
    }
    ::SelectObject(hDisplayMemDC, hOldBitmap);
    ::DeleteObject(hBitmap);

    hBitmap = ::LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_TITLE_RIGHT));
    hOldBitmap = (HBITMAP)::SelectObject(hDisplayMemDC, hBitmap);
    ::GetObject(hBitmap, sizeof(BITMAP), &BmpInfo);
    ::BitBlt(hdc, DrawPonit.x, DrawPonit.y, BmpInfo.bmWidth, BmpInfo.bmHeight, hDisplayMemDC, 0, 0, SRCCOPY);

    ::SelectObject(hDisplayMemDC, hOldBitmap);
    ::DeleteObject(hBitmap);



    //button
    rtButtons = rtMinButton;
    hBitmap = ::LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_NORMAL));
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
    ::GetWindowText(hdlg, szTitle, 255);
    ::SetTextColor(hdc, RGB(80,255,25));
    wsprintf(szTitle, "hello skin!");
    ::TextOut(hdc, 165, 10, szTitle, strlen(szTitle));

    ::SetBkMode(hdc, nOldMode);
    ::SetTextColor(hdc, clOldText);
    ::SelectObject(hdc, hOldFont);
    // pDC->TextOut(60,60,m_StrTitle);
    //
    hBitmap = ::LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_LEFT_DOWN));
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
    hBitmap = ::LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_RIGHT_DOWN));
    ::GetObject(hBitmap, sizeof(BITMAP), &BmpInfo);
    hOldBitmap = (HBITMAP)::SelectObject(hDisplayMemDC, hBitmap);
    GetClientRect(hdlg, &rtWnd);
    for (i= 25;i<=rtWnd.bottom+27;i=i+BmpInfo.bmHeight-3)
    {
        ::BitBlt(hdc, rtWnd.right, i, BmpInfo.bmWidth, BmpInfo.bmHeight, hDisplayMemDC, 0, 0, SRCCOPY);
    }
    ::SelectObject(hDisplayMemDC, hOldBitmap);
    ::DeleteObject(hBitmap);

    //
    hBitmap = ::LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_DOWN));
    ::GetObject(hBitmap, sizeof(BITMAP), &BmpInfo);
    hOldBitmap = (HBITMAP)::SelectObject(hDisplayMemDC, hBitmap);
    GetClientRect(hdlg, &rtWnd);
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

    ReleaseDC(hdlg, hDisplayMemDC);
    ReleaseDC(hdlg, hdc);
}

BOOL CALLBACK MyDlgProc(HWND hDlg, UINT message,
WPARAM wParam, LPARAM lParam)
{
    //g_hMainWnd = hDlg;

    //PAINTSTRUCT ps;
    //HDC hdc;
    LRESULT lrst;
    RECT rcWnd;

    static const int IDC_BUTTON_UE = 3333;
    static UButton ueTest(hDlg, IDC_BUTTON_UE, g_hInst);

    static const int IDC_BUTTON_CHECK = 3334;
    static UCheckButton ueCheck(hDlg, IDC_BUTTON_CHECK, g_hInst);

    static const int IDC_BUTTON_RA = 3335;
    static URadioButton ueRa(hDlg, IDC_BUTTON_RA, g_hInst);
    static const int IDC_BUTTON_RA2 = 3336;
    static URadioButton ueRa2(hDlg, IDC_BUTTON_RA2, g_hInst);

    static const int IDC_GROUPBOX = 3337;
    static UGroupBox ueGB(hDlg, IDC_GROUPBOX, g_hInst);

    static const int IDC_ICONBUTTON = 3338;
    static UIconButton ueIco(hDlg, IDC_ICONBUTTON, g_hInst);

    RECT rcRa = {150, 190, 250, 290};
    RECT rcRa2 = {320, 190, 420, 290};
    RECT rcCH = {150, 50, 250, 150};
    RECT rcIco = {20, 200, 115, 300};
    RECT rcGB = {120, 40, 550, 400};
    RECT rcTest = {20, 60, 110, 160};
    switch (message)
    {
    case WM_INITDIALOG :
        ueTest.setStyles(WS_TABSTOP);
        ueTest.create();
        ueTest.setWindowText(_T("help"));
        ueTest.setPosition(&rcTest);

        ueCheck.create();
        ueCheck.setWindowText("Check me");
        ueCheck.setPosition(&rcCH);

        ueRa.create();
        ueRa.setWindowText("Radio me");
        ueRa.setPosition(&rcRa);

        ueRa2.create();
        ueRa2.setWindowText("Radio you");
        ueRa2.setPosition(&rcRa2);

        ueGB.create();
        ueGB.setPosition(&rcGB);

        ueIco.create();
        HICON hIcon;
        //hIcon = LoadIcon(hInst, MAKEINTRESOURCE(nResID));
        hIcon = (HICON)LoadImage( g_hInst, // small class icon
        MAKEINTRESOURCE(IDI_APP),
        IMAGE_ICON,
        GetSystemMetrics(SM_CXSMICON)*2,
        GetSystemMetrics(SM_CYSMICON)*2,
        LR_DEFAULTCOLOR );
        ueIco.setIcon(hIcon);
        ueIco.setWindowText("hhh");
        ueIco.setPosition(&rcIco);
        break;
    case WM_LBUTTONDOWN:
        PostMessage(hDlg, WM_NCLBUTTONDOWN, (WPARAM)HTCAPTION, (LPARAM)lParam);
        break;
    case WM_COMMAND :
        switch (LOWORD (wParam))
        {
        case IDC_BUTTON_UE:
            showMsg("You Press me!", "info", hDlg);
            return TRUE;
        case IDC_BUTTON_CHECK:
            showMsg("Check me!", "info");
            if (IsDlgButtonChecked(hDlg, IDC_BUTTON_CHECK))
            {
                ueTest.disable();
            }
            else
            {
                ueTest.enable();
            }
            return TRUE;
        case IDC_BUTTON_RA:
            showCustomInfoMsg("Radio me!", hDlg);
            return TRUE;
        case IDC_BUTTON_RA2:
            if (ueDlg == NULL)
            {
            hdll = ::LoadLibrary("libskin.dll");
            if (NULL == hdll)
            {
                showCustomInfoMsg("Cannot load dll", hDlg);
                return FALSE;
            }
            hRes = ::FindResource(hdll, MAKEINTRESOURCE(IDD_DIALOG1), RT_DIALOG);
            if (hRes == NULL)
            {
                showCustomInfoMsg("Could not locate dialog box.", hDlg);
                return FALSE;
            }
            ueDlg = new UDialogBox(hdll, IDD_DIALOG1, UDialogBox::DefaultDlgProc, hDlg);
            ueDlg->create();
            }
            //::FreeLibrary(hdll);
            return TRUE;
        case IDC_ICONBUTTON:
            showCustomInfoMsg("Icon here!", hDlg);
            return TRUE;
        case IDOK :
            ueTest.setWindowText("New Text");
            return TRUE;
        
		case IDCANCEL :

			if (ueDlg)
			{
				UDialogBox::onCancel(ueDlg->getHWND());
				delete ueDlg;
				ueDlg = NULL;
			}


			IStop();

			if (hdll)
				::FreeLibrary(hdll);

			hdll = NULL;

			return UDialogBox::onCancel(hDlg) ;  
		}
    case WM_MOVE:
    case WM_PAINT:
    case WM_NCPAINT:
    case WM_NOTIFY:
    case WM_ACTIVATE:
    //case WM_NCACTIVATE:
        //hdc = BeginPaint(hDlg, &ps);
        //lrst = UDialogBox::DefaultDlgProc(hDlg, message, wParam, lParam);
        //::DefWindowProc(hDlg, message, wParam, lParam) ;
        lrst = UDialogBox::DefaultDlgProc(hDlg, message, wParam, lParam);
        drawTitleBar(hDlg);
        //showCustomInfoMsg("xx", hDlg);
        //EndPaint(hDlg, &ps);
        return lrst;
    }

    return UDialogBox::DefaultDlgProc(hDlg, message, wParam, lParam);
}

BEGIN_DLGAPP
    IStart( g_hInst, ::GetCurrentThreadId() );
    UDialogBox dialog(hInstance, IDD_DIALOG1, MyDlgProc);
    dialog.create();
END_DLGAPP


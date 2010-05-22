#include "resource.h"

#include <windows.h>
#include <windowsx.h>
#include <tchar.h>

#include "udialogx.h"
#include "ubutton.h"
#include "udlgapp.h"
#include "udc.h"

#include "adt/uautoptr.h"

int CaptureAnImage(HWND hWnd, LPCTSTR sFilename = _T("C:\\default_cap.bmp"));

using huys::UDialogBox;

class UDialogExt : public UDialogBox
{
    enum {
        ID_UBUTTON = 11
    };
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
    : UDialogBox(hInst, nID)
    {}

    BOOL onInit()
    {
        m_pBtn = new UButton(m_hDlg, ID_UBUTTON, m_hInst);
        m_pBtn->setPos(300, 200, 100, 100);
        m_pBtn->create();
        m_pBtn->setWindowText(_T("Capture"));
        return TRUE;
    }

    BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch (LOWORD(wParam))
        {
        case ID_UBUTTON:
            {
            //return CaptureAnImage(m_hDlg, _T("window.bmp"));
                return FALSE;
            }
        default:
            return UDialogBox::onCommand(wParam, lParam);
        }
    }

    virtual void onDraw(HDC hdc)
    {
        HBRUSH hBrush = (HBRUSH)::GetStockObject(BLACK_BRUSH);
        USmartDC udc(hdc);

        huys::URectL rect(10, 10, 60, 110);
        udc.fillRect(rect, hBrush);

        udc.setBKColor(::GetSysColor(COLOR_BTNFACE));
        udc.setBKMode(TRANSPARENT);
        udc.textOut(rect.left() + 15, rect.bottom()+10, "1", 1);

        HBRUSH hBrushWhite = (HBRUSH)::GetStockObject(WHITE_BRUSH);

        rect.offset(100, 0);
        udc.frameRect(rect, hBrush);
        udc.textOut(rect.left() + 15, rect.bottom()+10, "2", 1);

        huys::URectL rcClient;
        ::GetClientRect(m_hDlg, rcClient);

        udc.setViewportOrg(rcClient.left(), rcClient.bottom());
        udc.setMapMode(MM_HIENGLISH);

        //SetViewportOrgEx(hdc, 100, 100, NULL);
        udc.setPenColor(huys::red);
        udc.setBrushColor(huys::aliceblue);

        huys::URectL rect2(2500, 2100, 3000, 3100);
        udc.rectangle(rect2);
        udc.textOut(rect2.left() + 150, rect2.top()-100, "3", 1);

        udc.setPenColor(huys::aqua);
        udc.setBrushColor(huys::xpblue);

        rect2.offset(1000, 0);
        udc.rectangle(rect2);
        //udc.invertRect(&rc2);
        udc.textOut(rect2.left() + 150, rect2.top() - 100, "4", 1);
    }

    BOOL onCtrlColorDlg(HDC hdc)
    {
        return  (BOOL)(HBRUSH)GetStockObject(LTGRAY_BRUSH);
    }
private:
    huys::ADT::UAutoPtr<UButton> m_pBtn;
};

UDLGAPP_T(UDialogExt, IDD_TEST);

int CaptureAnImage(HWND hWnd, LPCTSTR sFilename)
{
    //HDC hdcScreen;
    //HDC hdcWindow;
    //HDC hdcMemDC = NULL;

    UDC udcScreen;
    UDC udcWindow;
    //UMemDC udcMem;

    HBITMAP hbmScreen = NULL;
    BITMAP bmpScreen;

    // Retrieve the handle to a display device context for the client
    // area of the window.
    //hdcScreen = GetDC(NULL);
    //hdcWindow = GetDC(hWnd);
    udcScreen.getDC(NULL);
    udcWindow.getDC(hWnd);

    // Create a compatible DC which is used in a BitBlt from the window DC
    //hdcMemDC = CreateCompatibleDC(hdcWindow);
    UMemDC udcMem(udcWindow);

    //if(!hdcMemDC)
    if (udcMem.isNull())
    {
        return -1;
        //MessageBox(hWnd, _T("StretchBlt has failed"), _T("Failed"), MB_OK);
        //ReleaseDC(hWnd, hdcMemDC);
        //ReleaseDC(NULL,hdcScreen);
        //ReleaseDC(hWnd,hdcWindow);

    }



    // Get the client area for size calculation
    RECT rcClient;
    GetClientRect(hWnd, &rcClient);

    //This is the best stretch mode
    //SetStretchBltMode(hdcWindow,HALFTONE);
    udcWindow.setStretchBltMode(HALFTONE);

    //The source DC is the entire screen and the destination DC is the current window (HWND)
    //if(!StretchBlt(hdcWindow,
    //    0,0,
    //    rcClient.right, rcClient.bottom,
    //    hdcScreen,
    //    0,0,
    //    GetSystemMetrics (SM_CXSCREEN),
    //    GetSystemMetrics (SM_CYSCREEN),
    //    SRCCOPY))
    //{
    //    MessageBox(hWnd, _T("StretchBlt has failed"), _T("Failed"), MB_OK);
    //    ReleaseDC(hWnd, hdcMemDC);
    //    ReleaseDC(NULL,hdcScreen);
    //    ReleaseDC(hWnd,hdcWindow);
    //}
    if (!udcWindow.stretchBlt( 0, 0, rcClient.right, rcClient.bottom,
                          udcScreen,
                          0,0,
                          GetSystemMetrics (SM_CXSCREEN),
                          GetSystemMetrics (SM_CYSCREEN),
                          SRCCOPY))
    {
        return -1;
    }

    // Create a compatible bitmap from the Window DC
    hbmScreen = CreateCompatibleBitmap(udcWindow, rcClient.right-rcClient.left, rcClient.bottom-rcClient.top);

    if(!hbmScreen)
    {
        //MessageBox(hWnd, _T("CreateCompatibleBitmap Failed"), _T("Failed"), MB_OK);
        DeleteObject(hbmScreen);
        //ReleaseDC(hWnd, hdcMemDC);
        //ReleaseDC(NULL,hdcScreen);
        //ReleaseDC(hWnd,hdcWindow);
        return -1;
    }

    // Select the compatible bitmap into the compatible memory DC.
    SelectObject(udcMem,hbmScreen);

    // Bit block transfer into our compatible memory DC.
    if(!udcMem.bitBlt(
        0,0,
        rcClient.right-rcClient.left, rcClient.bottom-rcClient.top,
        udcWindow,
        0,0,
        SRCCOPY))
    {
        //MessageBox(hWnd, _T("BitBlt has failed"), _T("Failed"), MB_OK);
        DeleteObject(hbmScreen);
        //ReleaseDC(hWnd, hdcMemDC);
        //ReleaseDC(NULL,hdcScreen);
        //ReleaseDC(hWnd,hdcWindow);
        return -1;
    }


    // Get the BITMAP from the HBITMAP
    GetObject(hbmScreen,sizeof(BITMAP),&bmpScreen);

    BITMAPFILEHEADER   bmfHeader;
    BITMAPINFOHEADER   bi;



    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = bmpScreen.bmWidth;
    bi.biHeight = bmpScreen.bmHeight;
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    DWORD dwBmpSize = ((bmpScreen.bmWidth * bi.biBitCount + 31) / 32) * 4 * bmpScreen.bmHeight;

    // Starting with 32-bit Windows, GlobalAlloc and LocalAlloc are implemented as wrapper functions that
    // call HeapAlloc using a handle to the process's default heap. Therefore, GlobalAlloc and LocalAlloc
    // have greater overhead than HeapAlloc.
    HANDLE hDIB = GlobalAlloc(GHND,dwBmpSize+sizeof(BITMAPINFOHEADER));
    LPBITMAPINFOHEADER lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDIB);
    *lpbi = bi;


    // Gets the "bits" from the bitmap and copies them into a buffer
    // which is pointed to by lpbi
    GetDIBits(
        udcWindow,
        hbmScreen,
        0,
        (UINT)bmpScreen.bmHeight,
        lpbi,
        (BITMAPINFO *)lpbi,
        DIB_RGB_COLORS);

    // A file is created, this is where we will save the screen capture.
    HANDLE hFile = CreateFile(sFilename,
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    // Add the size of the headers to the size of the bitmap to get the total file size
    DWORD dwSizeofDIB   =    dwBmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    //Offset to where the actual bitmap bits start.
    bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);

    //Size of the file
    bmfHeader.bfSize = dwSizeofDIB;

    //bfType must always be BM for Bitmaps
    bmfHeader.bfType = 0x4D42; //BM


    DWORD dwBytesWritten;
    WriteFile(hFile,(LPSTR)&bmfHeader, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
    WriteFile(hFile,(LPSTR)lpbi,dwSizeofDIB,&dwBytesWritten,NULL);

    //Unlock and Free the DIB from the heap
    GlobalUnlock(hDIB);
    GlobalFree(hDIB);

    //Close the handle for the file that was created
    CloseHandle(hFile);

    //Clean up
    DeleteObject(hbmScreen);
    //ReleaseDC(hWnd, hdcMemDC);
    //ReleaseDC(NULL,hdcScreen);
    //ReleaseDC(hWnd,hdcWindow);

    return 0;

}

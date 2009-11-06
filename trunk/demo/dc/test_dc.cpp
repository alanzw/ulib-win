#include "resource.h"

#include <windows.h>
#include <windowsx.h>
#include <tchar.h>

#include "udialogx.h"
#include "ubutton.h"
#include "udlgapp.h"
#include "udc.h"

int CaptureAnImage(HWND hWnd, LPCTSTR sFilename = _T("C:\\default_cap.bmp"));

using huys::UDialogBox;

class UDialogExt : public UDialogBox
{
    enum {
        ID_UBUTTON = 11
    };
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
    : UDialogBox(hInst, nID),
      m_pBtn(0)
    {}

    ~UDialogExt()
    {
        CHECK_PTR(m_pBtn);
    }

    BOOL onInit()
    {
        m_pBtn = new UButton(m_hDlg, ID_UBUTTON, m_hInst);
        RECT rc = {20, 20, 160, 160};
        m_pBtn->create();
        m_pBtn->setPosition(&rc);
        m_pBtn->setWindowText(_T("Capture"));
        return TRUE;
    }

    BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch (LOWORD(wParam))
        {
        case ID_UBUTTON:
            {
            return CaptureAnImage(m_hDlg, _T("window.bmp"));
            }
        default:
            return UDialogBox::onCommand(wParam, lParam);
        }
    }
private:
    UButton *m_pBtn;
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
    GetDIBits(udcWindow,hbmScreen,0,
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
        FILE_ATTRIBUTE_NORMAL ,NULL);

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

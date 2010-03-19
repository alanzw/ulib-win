#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "uwinapp.h"
#include "ubasewindow.h"
#include "ugdi.h"
#include "colors.h"

#include "ulistbox.h"

#include "adt/uautoptr.h"

#include "uprinter.h"

#include "ucommondialog.h"

class UMyWindow : public UBaseWindow
{
    enum {
        ID_LB_PRINTER = 11002
    };
public:
   UMyWindow()
   : UBaseWindow(NULL, ::GetModuleHandle(NULL))
   {
        this->setTitle(_T("UExplorer 0.0.1"));
        this->setMenu(MAKEINTRESOURCE(IDR_MENU_MAIN));
   }

   BOOL onCreate()
   {
       this->setIconBig(IDI_APP);

       m_lbPrinter = new UListBox(this, ID_LB_PRINTER);

       RECT rc = {0};
       this->getClientRect(&rc);

       rc.top += 50;
       rc.bottom -= 50;
       rc.left += 50;
       rc.right -= 50;
       m_lbPrinter->setRect(&rc);
       m_lbPrinter->create();



       return UBaseWindow::onCreate();
   }

    //
    virtual void onDraw(HDC hdc)
    {

    }
    //
    virtual BOOL onEraseBkgnd(HDC hdc)
    {
        RECT rc = {0};
        ::GetClientRect(*this, &rc);
        huys::URectangle urc(rc);
        urc.setFilledColor(huys::black);
        urc.setFilledStyle(BS_SOLID);
        //urc.Draw(hdc);
        return FALSE;
    }

    //
    virtual BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch (LOWORD(wParam))
        {
        case IDM_ABOUT:
            return onMenuAbout();
        case IDM_PRINT:
            return onMenuPrint();
        case IDM_EXIT:
            return UBaseWindow::onClose();
        default:
            return UBaseWindow::onCommand(wParam, lParam);
        }
    }
private:
    BOOL onMenuAbout()
    {
        this->showMsg(_T("UPrinter v0.0.1"), _T("About"));
        return FALSE;
    }

    BOOL onMenuPrint()
    {
        UCommonDialog::UPrinterDialog dlg(*this);
        dlg.choose();
        return FALSE;
    }
private:
    huys::ADT::UAutoPtr<UListBox> m_lbPrinter;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    UWinApp app;
    app.setMainWindow(new UMyWindow);
    app.init(hInstance);
    return app.run();
}

//
// RawDataToPrinter - sends binary data directly to a printer
//
// szPrinterName: NULL-terminated string specifying printer name
// lpData:        Pointer to raw data bytes
// dwCount        Length of lpData in bytes
//
// Returns: TRUE for success, FALSE for failure.
//
BOOL RawDataToPrinter(LPTSTR szPrinterName, LPBYTE lpData, DWORD dwCount)
{
    BOOL     bStatus = FALSE;
    HANDLE     hPrinter = NULL;
    DOC_INFO_1 DocInfo;
    DWORD      dwJob = 0L;
    DWORD      dwBytesWritten = 0L;

    // Open a handle to the printer.
    bStatus = OpenPrinter( szPrinterName, &hPrinter, NULL );
    if (bStatus) {
        // Fill in the structure with info about this "document."
        DocInfo.pDocName = (LPTSTR)_T("My Document");
        DocInfo.pOutputFile = NULL;
        DocInfo.pDatatype = (LPTSTR)_T("RAW");

        // Inform the spooler the document is beginning.
        dwJob = StartDocPrinter( hPrinter, 1, (LPBYTE)&DocInfo );
        if (dwJob > 0) {
            // Start a page.
            bStatus = StartPagePrinter( hPrinter );
            if (bStatus) {
                // Send the data to the printer.
                bStatus = WritePrinter( hPrinter, lpData, dwCount, &dwBytesWritten);
                EndPagePrinter (hPrinter);
            }
            // Inform the spooler that the document is ending.
            EndDocPrinter( hPrinter );
        }
        // Close the printer handle.
        ClosePrinter( hPrinter );
    }
    // Check to see if correct number of bytes were written.
    if (!bStatus || (dwBytesWritten != dwCount)) {
        bStatus = FALSE;
    } else {
        bStatus = TRUE;
    }
    return bStatus;
}

//
//  RawDataToXpsPrinter - sends binary data directly to a printer
//          with an XPSDrv Printer Driver
//
// szPrinterName: NULL-terminated string specifying printer name
// lpData:        Pointer to raw data bytes
// dwCount        Length of lpData in bytes
//
// Returns: TRUE for success, FALSE for failure.
//
BOOL RawDataToXpsPrinter (LPTSTR szPrinterName, LPBYTE lpData, DWORD dwCount)
{
    BOOL     bStatus = FALSE;
    HANDLE     hPrinter = NULL;
    DOC_INFO_1       DocInfo;
    DWORD    dwPrtJob = 0L;
    DWORD    dwBytesWritten = 0L;

    // Open a handle to the printer.
    bStatus = OpenPrinter (szPrinterName, &hPrinter, NULL);

    if (bStatus) {
        // Fill in the structure with info about this "document."
        DocInfo.pDocName = (LPTSTR)_T("My Document");
        DocInfo.pOutputFile = NULL;
        DocInfo.pDatatype = (LPTSTR)_T("XPS_PASS");

        dwPrtJob = StartDocPrinter (
            hPrinter,
            1,
            (LPBYTE)&DocInfo);

        if (dwPrtJob > 0) {
            // Send the data to the printer.
            bStatus = WritePrinter (
                hPrinter,
                lpData,
                dwCount,
                &dwBytesWritten);
        }

        EndDocPrinter (hPrinter);

        // Close the printer handle.
        bStatus = ClosePrinter(hPrinter);
    }

    if (!bStatus || (dwCount != dwBytesWritten)) {
        bStatus = FALSE;
    } else {
        bStatus = TRUE;
    }

    return bStatus;
}

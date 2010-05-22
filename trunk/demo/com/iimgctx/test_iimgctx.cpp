#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <ocidl.h>
#include <olectl.h>

#define   INITGUID
#include <initguid.h>
#define CINTERFACE
#include <iimgctx.h>

#include "ubasewindow.h"
#include "uwinapp.h"
#include "ubutton.h"
#include "adt/uautoptr.h"


class UIImgCtxWindow : public UBaseWindow
{
    enum {
        ID_BN_TEST = 10001
    };
public:
    UIImgCtxWindow()
    : UBaseWindow(NULL, ::GetModuleHandle(NULL))
    {
        this->setTitle(_T("IImgCtx Test 0.0.1"));
        this->setPos(100, 100, 800, 600);

        ::CoInitialize(NULL);
    }

    ~UIImgCtxWindow()
    {
        if (m_pImgCtx )
        {
            m_pImgCtx->Release();
        }

        ::CoUninitialize();
    }

    BOOL onCreate()
    {
        this->setIconBig(IDI_APP);

        if (!loadFile("back.jpg"))
        {
            //return FALSE;
        }

        m_bnTest = new UButton(this, ID_BN_TEST);
        m_bnTest->setPos(100, 100, 100, 100);
        m_bnTest->create();
        m_bnTest->setWindowText(_T("test"));

        return UBaseWindow::onCreate();
    }

    //
    /* virtual */ void onDraw(HDC hdc)
    {
        RECT rc = {0};
        this->getClientRect(&rc);
    }

    /* virtual */ BOOL onChar(WPARAM wParam, LPARAM lParam)
    {
        switch (wParam)
        {
        case VK_ESCAPE:
            return UBaseWindow::onClose();
        default:
            return UBaseWindow::onChar(wParam, lParam);
        }
    }

    /* virtual */ BOOL onEraseBkgnd(HDC hdc)
    {
        RECT rc;
        this->getClientRect(&rc);
        RECT imgrect;
        ::SelectPalette(hdc, m_hPal, TRUE);
        ::RealizePalette(hdc);

        imgrect.left = imgrect.top = 0;
        imgrect.bottom = sizeImage.cy;
        imgrect.right = sizeImage.cx;
        m_pImgCtx->Draw(hdc, &rc);
        return TRUE;
    }

    BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch (LOWORD(wParam))
        {
        case ID_BN_TEST:
            return onBnTest();
        default:
            return UBaseWindow::onCommand(wParam, lParam);
        }
    }
private:
    IImgCtx *m_pImgCtx;
    HPALETTE  m_hPal;
    SIZE  sizeImage;

    huys::ADT::UAutoPtr<UButton> m_bnTest;
private:
    BOOL loadFile(const char *sFilename)
    {
        HRESULT hr;
        ULONG ulState;

        m_pImgCtx = NULL;
        hr = ::CoCreateInstance( CLSID_IImgCtx, NULL, CLSCTX_INPROC_SERVER, IID_IImgCtx, (LPVOID*) &m_pImgCtx);
        if ( FAILED(hr) )
        {
            return FALSE;
        }

        //hr = m_pImgCtx->Load(L"http://www.baidu.com/img/baidu_logo.gif", 0);
        hr = m_pImgCtx->Load(L"res://test_iimgctx.exe//BACK.JPG", 0);
        if ( FAILED(hr) )
        {
            return FALSE;
        }

        do {
            m_pImgCtx->GetStateInfo(&ulState, &sizeImage, FALSE);

            // This image should load/decode instantly, but everyone deserves some sleep
            Sleep(5);
        } while( (ulState & IMGLOAD_LOADING) );

        m_pImgCtx->GetPalette(&m_hPal);

        return FALSE;
    }

    BOOL onBnTest()
    {
        showMsg(_T("hello!"));
    }
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    UWinApp app;
    app.setMainWindow(new UIImgCtxWindow);
    app.init(hInstance);
    return app.run();
}

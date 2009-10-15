#include "resource.h"

#include <windows.h>
#include <tchar.h>
//#include <ocidl.h>
//#include <olectl.h>

#include "ubasewindow.h"
#include "uwinapp.h"
#include "uedit.h"

//#include "ufile.h"
#include "udllman.h"

class UIStreamWindow : public UBaseWindow
{
    enum {
        ID_EDT_SOURCE = 2000,
        ID_EDT_DEST   = 2001
    };
public:
    UIStreamWindow()
    : UBaseWindow(NULL, ::GetModuleHandle(NULL)),
    m_pIStream(0)
    {
        this->setTitle(_T("IStream Test 0.0.1"));
        this->setPos(100, 100, 800, 600);
    }
    ~UIStreamWindow()
    {
        if ( m_pIStream )
        {
            m_pIStream->Release();
            m_pIStream = 0;
        }

        CHECK_PTR(m_pEdtSource);
        CHECK_PTR(m_pEdtDest);
    }

    BOOL onCreate()
    {
        this->setIconBig(IDI_APP);

        lstrcpy(origin, _T("Here is a string!"));

        m_pEdtSource = new UEdit(this, ID_EDT_SOURCE);
        m_pEdtSource->setPos(100, 100, 500, 200);
        m_pEdtSource->create();
        m_pEdtSource->setText(origin);

        m_pEdtDest = new UEdit(this, ID_EDT_SOURCE);
        m_pEdtDest->setPos(100, 300, 500, 400);
        m_pEdtDest->create();

        static UDllMan udm(_T("shlwapi.dll"));
        //call IStream *SHCreateMemStream (
        //      const BYTE *pInit,
        //      UINT cbInit
        //  );
        m_pIStream = udm.callFunc<IStream *, const BYTE *, UINT>((LPCSTR)12, (LPBYTE)origin, sizeof(origin));
        if (NULL != m_pIStream)
        {
            ULONG ulRead;
            HRESULT hr = m_pIStream->Read(dest, sizeof(dest), &ulRead);
            if (!FAILED(hr))
            {
                m_pEdtDest->setText(dest);
            }
        }
        return UBaseWindow::onCreate();
    }

    //
    virtual void onDraw(HDC hdc)
    {
        RECT rc = {0};
        this->getClientRect(&rc);
    }

    BOOL onChar(WPARAM wParam, LPARAM lParam)
    {
        switch (wParam)
        {
        case VK_ESCAPE:
            return UBaseWindow::onClose();
        default:
            return UBaseWindow::onChar(wParam, lParam);
        }
    }
private:
    IStream *m_pIStream;
    UEdit *m_pEdtSource;
    UEdit *m_pEdtDest;
    TCHAR origin[100];
    TCHAR dest[100];
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    UWinApp app;
    app.setMainWindow(new UIStreamWindow);
    app.init(hInstance);
    return app.run();
}

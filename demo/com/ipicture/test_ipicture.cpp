#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <ocidl.h>
#include <olectl.h>

#include <iostream>
#include <fstream>

using std::ifstream;
using std::ios_base;

#include "ubasewindow.h"
#include "uwinapp.h"

class UIPicWindow : public UBaseWindow
{
public:
    UIPicWindow()
        : UBaseWindow(NULL, ::GetModuleHandle(NULL)),
        m_pIPic(0),
        w(0), h(0)
    {
        this->setTitle(_T("IPicture Test 0.0.1"));
    }

    ~UIPicWindow()
    {
        if ( m_pIPic )
        {
            m_pIPic->Release();
            m_pIPic = 0;
        }
    }

    BOOL onCreate()
    {
        this->setIconBig(IDI_APP);

        if (!loadFile("back.jpg"))
        {
            return FALSE;
        }

        m_pIPic->get_Width(&w);
        m_pIPic->get_Height(&h);

        return UBaseWindow::onCreate();
    }

    //
    virtual void onDraw(HDC hdc)
    {
        RECT rc = {0};
        this->getClientRect(&rc);

        if (0 != m_pIPic)
        {
            m_pIPic->Render(hdc, 0, 0, rc.right-rc.left, rc.bottom-rc.top,
                0, h, w, -h, NULL);
        }
    }
private:
    IPicture *m_pIPic;

    BOOL loadFile(const char *sFilename)
    {
        bool bResult = false;

        if ( m_pIPic != NULL)
        {
            m_pIPic->Release();
            m_pIPic = NULL;
        }

        ifstream file( sFilename, ios_base::binary | ios_base::in );
        if ( file )
        {
            file.seekg( 0, ios_base::end );
            long length = file.tellg();
            file.seekg( 0, ios_base::beg );

            HGLOBAL hGlobal = GlobalAlloc( GMEM_MOVEABLE, length );
            void* p = GlobalLock(hGlobal);
            file.read( (char *) p, length );
            GlobalUnlock(hGlobal);
            IStream* pStream = NULL;
            if ( CreateStreamOnHGlobal(hGlobal, TRUE, &pStream) == S_OK)
            {
                HRESULT hr = ::OleLoadPicture( pStream,
                    length,
                    FALSE,
                    IID_IPicture,
                    (LPVOID *) &m_pIPic);
                pStream->Release();
                return TRUE;
            }
        }
        return FALSE;
    }

    long w, h;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    UWinApp app;
    app.setMainWindow(new UIPicWindow);
    app.init(hInstance);
    return app.run();
}

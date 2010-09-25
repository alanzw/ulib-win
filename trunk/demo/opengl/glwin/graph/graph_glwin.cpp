#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include <ctime>
#include <cmath>

#include <GL/gl.h>
#include <GL/glu.h>

#include "uwinapp.h"
#include "ubasewindow.h"
#include "uglut.h"
#include "uicon.h"
#include "umsg.h"

#include "uglgraph.h"

UGLGraph* myGraph;

class UGLWindow : public UBaseWindow
{
public:
    UGLWindow(HINSTANCE hInst = GetModuleHandle(NULL))
    : UBaseWindow(NULL, hInst), m_bFullscreen(FALSE),
    m_hdc(0), m_hrc(0)
    {
        setTitle(_T("OpenGL Window"));
    }

    BOOL onPreRegisterWindowClass(huys::UWindowClass &uwc)
    {
        uwc.setStyles(CS_OWNDC);
        return FALSE;
    }

    BOOL onPreCreateWindow()
    {
        if (IDYES == showYesNoMsgbox(_T("Fullscreen?"), _T("Start Fullscreen?")))
        {
            m_bFullscreen = TRUE;
        }

        int width= 640;
        int height = 480;
        int bits=16;

        setPos(0, 0, width, height);

        if (m_bFullscreen)
        {
            DEVMODE dmScreenSettings;                                // Device Mode
            memset(&dmScreenSettings,0,sizeof(dmScreenSettings));    // Makes Sure Memory's Cleared
            dmScreenSettings.dmSize=sizeof(dmScreenSettings);        // Size Of The Devmode Structure
            dmScreenSettings.dmPelsWidth    = width;                // Selected Screen Width
            dmScreenSettings.dmPelsHeight    = height;                // Selected Screen Height
            dmScreenSettings.dmBitsPerPel    = bits;                    // Selected Bits Per Pixel
            dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

            // Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
            if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
            {
                // If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
                if (showYesNoMsgbox(_T("The Requested Fullscreen Mode Is Not Supported By\nYour Video Card.")
                    _T("Use Windowed Mode Instead?"),_T("OpenGL"))==IDYES)
                {
                    m_bFullscreen=FALSE;        // Windowed Mode Selected.  Fullscreen = FALSE
                }
                else
                {
                    // Pop Up A Message Box Letting User Know The Program Is Closing.
                    showMsg(_T("Program Will Now Close."),_T("ERROR"));
                    return FALSE;                                    // Return FALSE
                }
            }

        }

        if (m_bFullscreen)
        {
            setStyles(WS_POPUP);
            setExStyles(WS_EX_APPWINDOW);
            ::ShowCursor(FALSE);
        }
        else
        {
            //setStyles(WS_OVERLAPPEDWINDOW);
            setExStyles(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);
        }

        adjustWindowRectEx();

        return TRUE;
    }

    BOOL onCreate()
    {
        initGL();

        this->setIconBig(IDI_ULIB);

        return UBaseWindow::onCreate();
    }

    BOOL onDestroy()
    {
        UGlut::DisableOpenGL(getHandle(), m_hdc, m_hrc);

        return UBaseWindow::onDestroy();
    }

    void onDraw(HDC hdc)
    {
        render();
        //SwapBuffers(m_hdc);
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
    HDC m_hdc;
    HGLRC m_hrc;
    UIcon m_uico;

    BOOL m_bFullscreen;
private:
    BOOL initGL()
    {
        UGlut::EnableOpenGL(*this, m_hdc, m_hrc);

        RECT rc = {0};
        this->getClientRect(&rc);
        int w = rc.right - rc.left;
        int h = rc.bottom - rc.top;

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(60.0, (float) w / (float) h, 0.1, 100.0);

        glMatrixMode(GL_MODELVIEW);
        glViewport(0, 0, w, h);

        glShadeModel(GL_SMOOTH);
        glClearColor(0.0f, 0.0f, 0.0f, 0.f);                // Black Background
        //glClearDepth(1.0f);                                // Depth Buffer Setup
        myGraph = UGLGraph::instance();
        myGraph->setup( 500, 60, 10, 10, 2, 2, 1, 200 );

        return TRUE;
    }

public:
    void render()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(60.0, (float) 800 / (float) 600, 0.1, 100.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // This is a dummy function. Replace with custom input/data
        float time = clock() / 1000.0;
        float value;
        value = 5*sin( 5*time ) + 10.f;

        myGraph->update( value );
        myGraph->draw();
        //
        SwapBuffers(m_hdc);
    }
};

class UGLWinApp : public UWinApp
{
    BOOL m_bQuit;
public:
    UGLWinApp()
    : m_bQuit(FALSE)
    {}

    BOOL run()
    {
        m_pMainWindow->show();
        m_pMainWindow->update();

        //BOOL bQuit = FALSE;
        MSG msg;

        while (!m_bQuit)
        {
            // check for messages
            if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
            {
                // handle or dispatch messages
                if ( msg.message == WM_QUIT)
                {
                    m_bQuit = TRUE;
                }
                else
                {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
            }
            else
            {
                // OpenGL animation code goes here
                static_cast<UGLWindow *>(m_pMainWindow)->render();
                //m_pMainWindow->invalidate();
                //SwapBuffers(m_hdc);
            }
        }
        // shutdown OpenGL
        //UGlut::DisableOpenGL( m_hDlg, m_hdc, m_hrc );
        // destroy the window explicitly
        m_pMainWindow->onDestroy();
        return (msg.wParam);
    }
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    UGLWinApp app;

    app.setMainWindow(new UGLWindow);

    app.init(hInstance);

    return app.run();
}


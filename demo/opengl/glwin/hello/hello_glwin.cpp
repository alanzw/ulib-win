#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include "uwinapp.h"
#include "ubasewindow.h"
#include "uglut.h"
#include "uicon.h"
#include "umsg.h"

class UGLWindow : public UBaseWindow
{
public:
    UGLWindow(HINSTANCE hInst = ::GetModuleHandle(NULL))
    : UBaseWindow(NULL, hInst),
      m_hdc(0), m_hrc(0), m_bFullscreen(FALSE), cnt1(0.0)
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

        if (!m_bFullscreen)
        {
            m_uico.loadIconEx(::GetModuleHandle(NULL), IDI_APP);
            this->setIconBig(m_uico);
        }
        else
        {
            //ReSizeGLScene(640, 480); // Set Up Our Perspective GL Screen
        }
        setTopMost();
        
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
        SwapBuffers(m_hdc);
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

    BOOL onSize(WPARAM wParam, LPARAM lParam)
    {
        ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));
        return FALSE;
    }
private:
    HDC m_hdc;
    HGLRC m_hrc;
    UIcon m_uico;
    
    BOOL m_bFullscreen;
    
    GLfloat    cnt1;
private:
    BOOL initGL()
    {
        UGlut::EnableOpenGL(*this, m_hdc, m_hrc);

        return TRUE;
    }
    
    
    void ReSizeGLScene(GLsizei width, GLsizei height)        // Resize And Initialize The GL Window
    {
        if (height==0)                                        // Prevent A Divide By Zero By
        {
            height=1;                                        // Making Height Equal One
        }

        glViewport(0,0,width,height);                        // Reset The Current Viewport

        glMatrixMode(GL_PROJECTION);                        // Select The Projection Matrix
        glLoadIdentity();                                    // Reset The Projection Matrix

        // Calculate The Aspect Ratio Of The Window
        gluPerspective(.0f,(GLfloat)width/(GLfloat)height,0.5f,10.0f);

        glMatrixMode(GL_MODELVIEW);                            // Select The Modelview Matrix
        glLoadIdentity();                                    // Reset The Modelview Matrix
    }

    void render()
    {
        //glViewport(0, 0, 640.0, 480.0);

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //glScalef(0.5,0.5,0.5);
        glRotatef(cnt1,0,0,1);
        //
        glBegin(GL_LINES);

        //
        glColor3f(1.f, 0.f, 0.f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(1.0f, 0.0f, 0.0f);
        glVertex3f(1.0f, 0.0f, 0.0f);
        glVertex3f(0.9f, 0.1f, 0.0f);
        glVertex3f(1.0f, 0.0f, 0.0f);
        glVertex3f(0.9f, -0.1f, 0.0f);

        //
        glColor3f(0.f, 1.f, 0.f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.1f, 0.9f, 0.0f);
        glVertex3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-0.1f, 0.9f, 0.0f);

        //
        glColor3f(0.f, 0.f, 1.f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, 0.1f, 0.9f);
        glVertex3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, -0.1f, 0.9f);

        //
        glEnd();

        //
        glBegin(GL_POLYGON);
        glVertex2f(0.0f, 0.2f);
        glVertex2f(1.0f, 0.5f);
        glVertex2f(1.0f, 1.0f);
        glEnd();
        
        cnt1+=50.f; 
    }
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    UWinApp app;

    app.setMainWindow(new UGLWindow);

    app.init(hInstance);

    return app.run();
}


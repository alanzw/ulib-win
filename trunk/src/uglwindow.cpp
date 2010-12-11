#include <windows.h>
#include <tchar.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include <cmath>
#include <cstdio>

#include "uglut.h"
#include "uglwindow.h"
#include "umsg.h"

UGLWindow::UGLWindow(HINSTANCE hInst)
: UBaseWindow(NULL, hInst), m_hdc(0), m_hrc(0), m_bFullscreen(FALSE)
{
    setTitle(_T("OpenGL Window"));
    
    _init_func = 0;
    _disp_func = 0;
    _reshape_func = 0;
    
    m_uInterval = 100;
}

BOOL UGLWindow::onPreRegisterWindowClass(huys::UWindowClass &uwc)
{
    uwc.setStyles(CS_OWNDC);
    return FALSE;
}

BOOL UGLWindow::onPreCreateWindow()
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
        if (0 != _reshape_func)
        {
            (*_reshape_func)(width, height);
        }
        else
        {
            UGlut::ResizeScene(width, height);
        }
        return TRUE;
}

BOOL UGLWindow::onCreate()
{
    UGlut::EnableOpenGL(*this, m_hdc, m_hrc);
    
    if (0 != _init_func)
    {
        (*_init_func)();
    }
    
    this->setTimer(ID_TIMER_INTERNAL, m_uInterval);
    
    return UBaseWindow::onCreate();
}

BOOL UGLWindow::onDestroy()
{
    UGlut::DisableOpenGL(getHandle(), m_hdc, m_hrc);
    return UBaseWindow::onDestroy();
}

void UGLWindow::onDraw(HDC hdc)
{
    render();
    SwapBuffers(m_hdc);
}

BOOL UGLWindow::onChar(WPARAM wParam, LPARAM lParam)
{
    switch (wParam)
    {
    case VK_ESCAPE:
        return UBaseWindow::onClose();
    default:
        return UBaseWindow::onChar(wParam, lParam);
    }
}

BOOL UGLWindow::onSize(WPARAM wParam, LPARAM lParam)
{
    UGlut::ResizeScene(LOWORD(lParam),HIWORD(lParam));
    return FALSE;
}


void UGLWindow::render()
{
    if (0 != _disp_func)
    {
        (*_disp_func)();
    }
}


BOOL UGLWindow::onTimer(WPARAM wParam, LPARAM lParam)
{
    if (wParam == ID_TIMER_INTERNAL)
    {
		wglMakeCurrent(m_hdc, m_hrc );
        render();
        // Swap buffers
        SwapBuffers(m_hdc);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

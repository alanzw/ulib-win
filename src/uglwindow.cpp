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
    
    _post_create = 0;
    
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
        
        if (m_bFullscreen && !UGlut::check_fullscreen(m_bFullscreen))
            return FALSE;

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
    
    if (0 != _post_create)
    {
        _post_create(this);
    }
    
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

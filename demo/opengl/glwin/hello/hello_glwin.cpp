#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include <GL/GL.h>
#include <GL/GLU.h>

#include "uwinapp.h"
#include "ubasewindow.h"
#include "uglut.h"
#include "uicon.h"

class UGLWindow : public UBaseWindow
{
public:
    UGLWindow(HINSTANCE hInst = ::GetModuleHandle(NULL))
    : UBaseWindow(NULL, hInst),
      m_hdc(0), m_hrc(0)
    {
        setTitle(_T("OpenGL Window"));
    }

    BOOL onPreRegisterWindowClass(huys::UWindowClass &uwc)
    {
        uwc.setStyles(CS_OWNDC);
        return FALSE;
    }

    BOOL onCreate()
    {
        initGL();

        m_uico.loadIconEx(::GetModuleHandle(NULL), IDI_APP);
        this->setIconBig(m_uico);

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

private:
    HDC m_hdc;
    HGLRC m_hrc;
    UIcon m_uico;
private:
    BOOL initGL()
    {
        UGlut::EnableOpenGL(*this, m_hdc, m_hrc);

        return TRUE;
    }

    void render()
    {
        glViewport(0, 0, 300.0, 300.0);

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

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
    }
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    UWinApp app;

    app.setMainWindow(new UGLWindow);

    app.init(hInstance);

    return app.run();
}


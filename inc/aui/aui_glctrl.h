#ifndef U_AUI_GL_CTRL_H
#define U_AUI_GL_CTRL_H

#include <GL/gl.h>
#include <GL/glu.h>

#include "ubasewindow.h"
#include "uglut.h"

#include "adt/uautoptr.h"
#include "adt/uvector.h"

#define  UAUIGLCTRL_CLASSNAME "UAUI_UGLCTRL_{7BD1020E-02CB-4a7c-BADD-C1FFBD33299A}"

namespace AUI
{


class UGLCtrl :  public UBaseWindow
{
    enum {
        ID_TIMER_INTERNAL = 22
    };
public:
    typedef void (*GL_RenderFunction)();
    typedef huys::ADT::UVector<GL_RenderFunction> render_callbacks;

private:
    render_callbacks m_render_cbs;
    
    static int timer_id_offset;
    
    int timer_id;
public:
    UGLCtrl(UBaseWindow *pWndParent, UINT id = -1)
    : UBaseWindow(pWndParent)
    {
        setMenu(0);
        setWndClassName(_T(UAUIGLCTRL_CLASSNAME));
        setTitle(_T("DOCK"));

        addStyles(WS_CHILD);
        setExStyles(WS_EX_TOOLWINDOW | WS_EX_APPWINDOW);

        m_uInterval = 100;
        
        timer_id = ID_TIMER_INTERNAL + timer_id_offset;
        
        ++timer_id_offset;
       
        setID(id);
    }

    ~UGLCtrl()
    {
        this->killTimer(timer_id);
    }

    BOOL onPreRegisterWindowClass(huys::UWindowClass &uwc)
    {
        uwc.setStyles(CS_OWNDC);
        return FALSE;
    }

    BOOL onCreate()
    {
        UGlut::EnableOpenGL(*this, m_hdc, m_hrc);

        this->setTimer(timer_id, m_uInterval);

        return UBaseWindow::onCreate();
    }

    BOOL onClose()
    {
        this->hide();
        return FALSE;
    }

    void onDraw(HDC hdc)
    {
		wglMakeCurrent(m_hdc, m_hrc );
        render();
        SwapBuffers(m_hdc);
    }

    BOOL onDestroy()
    {
        UGlut::DisableOpenGL(getHandle(), m_hdc, m_hrc);

        return UBaseWindow::onDestroy();
    }

    //
    void setInterval(UINT uInterval)
    {
        m_uInterval = uInterval;
    }

    BOOL onSize(WPARAM wParam, LPARAM lParam)
    {
        ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));
        return FALSE;
    }


    BOOL onTimer(WPARAM wParam, LPARAM lParam)
    {
    
        if (wParam == timer_id)
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
public:
    void addRender(GL_RenderFunction func)
    {
        m_render_cbs.push_back(func);
    }

    void resetRenders()
    {
        m_render_cbs.clear();
    }
private:
    HDC m_hdc;
    HGLRC m_hrc;
    UINT m_uInterval;

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

        glClearColor(.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //glScalef(0.5,0.5,0.5);
        glRotatef(2.0,0,0,1);

        if (0 < m_render_cbs.size())
        {
            render_callbacks::const_iterator it = m_render_cbs.begin();
            for (; it!=m_render_cbs.end(); ++it)
            {
                (*it)(); // call it
            }
        }
        else
        {

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

        //cnt1+=(cnt1>10?.0f:0.05f);
        }
    }
};

int UGLCtrl::timer_id_offset  = 0;

typedef huys::ADT::UAutoPtr<UGLCtrl> UGLCtrlP;

}; // namespace AUI

#endif // U_AUI_GL_CTRL_H

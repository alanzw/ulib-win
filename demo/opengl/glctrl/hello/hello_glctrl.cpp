#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include <GL/GL.h>
#include <GL/GLU.h>

#include "udialogx.h"
#include "udlgapp.h"
#include "ustatic.h"
#include "uglut.h"

class UOpenGLCtrl : public UStatic
{
    enum {
        ID_TIMER_INTERNAL = 22
    };
public:
    UOpenGLCtrl(HWND hParent, UINT nResource, HINSTANCE hInst)
        : UStatic(hParent, nResource, hInst)
    {
        m_dwStyles &= ~SS_SIMPLE;
    }

    ~UOpenGLCtrl()
    {
        this->killTimer(ID_TIMER_INTERNAL);
        UGlut::DisableOpenGL(m_hSelf, m_hdc, m_hrc);
    }

    virtual BOOL create()
    {
        BOOL bRet = UStatic::create();
        this->subclassProc();

        initGL();

        this->setTimer(ID_TIMER_INTERNAL, 100);

        return  bRet;
    }

    BOOL onMessage( UINT message, WPARAM wParam, LPARAM lParam )
    {
        BOOL bRet = UStatic::onMessage(message, wParam, lParam);

        return bRet;
    }

    BOOL onPaint()
    {
        HDC hdc;
        PAINTSTRUCT ps;
        RECT rc;

        this->getClientRect(&rc);

        hdc = BeginPaint(m_hSelf, &ps);

        draw(hdc, rc);

        EndPaint(m_hSelf, &ps);

        return FALSE;
    }

    virtual BOOL onTimer(WPARAM wParam, LPARAM lParam)
    {
        switch (wParam)
        {
        case ID_TIMER_INTERNAL:
            {
                render();
                // Swap buffers
                SwapBuffers(m_hdc);

                return TRUE;
            }
        default:
            return FALSE;
        }  
    }

private:

    BOOL initGL()
    {
        UGlut::EnableOpenGL(m_hSelf, m_hdc, m_hrc);

        return TRUE;
    }

    void draw(HDC &hdc, RECT &rc)
    {}



    void render()
    {
        glViewport(0, 0, 200.0,200.0);

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
private:
    HDC m_hdc;
    HGLRC m_hrc;
};


using huys::UDialogBox;

class UDialogExt : public UDialogBox
{
    enum {
        IDC_GLCTRL = 12222
    };
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
    : UDialogBox(hInst, nID),
      m_pGlCtrl(0)
    {}

    ~UDialogExt()
    {
        CHECK_PTR(m_pGlCtrl);
    }

    virtual BOOL onInit()
    {
        m_pGlCtrl = new UOpenGLCtrl(m_hDlg, IDC_GLCTRL, m_hInst);
        m_pGlCtrl->create();

        RECT rc;
        ::GetClientRect(m_hDlg, &rc);

        rc.left += 10;
        rc.top += 10;
        rc.right -= 10;
        rc.bottom -= 10;

        m_pGlCtrl->setPosition(&rc);

        return TRUE;
    }

    virtual BOOL onChar(WPARAM wParam, LPARAM lParam)
    {
        switch (wParam)
        {
        case VK_ESCAPE:
            return UDialogBox::onCancel();
        default:
            return UDialogBox::onChar(wParam, lParam);
        }
    }
private:
    UOpenGLCtrl *m_pGlCtrl;
};

UDLGAPP_T(UDialogExt, IDD_TEST);


#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include "ugldialog.h"
#include "udlgapp.h"
#include "uglut.h"

using huys::UGLDialog;

class MyGLDialog : public UGLDialog
{
public:
    MyGLDialog(HINSTANCE hInst, UINT nID)
    : UGLDialog(hInst, nID)
    {}

    virtual BOOL initGL()
    {
        RECT rc;
        ::GetClientRect(m_hDlg, &rc);
        //UGlut::ResizeScene(rc.right-rc.left, rc.bottom-rc.top);
        glViewport (0, 0, (GLsizei) rc.right-rc.left, (GLsizei)rc.bottom-rc.top);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D ((GLdouble)-2.0, (GLdouble)2, (GLdouble)-2.0, (GLdouble)2);

        return TRUE;
    }

    virtual BOOL animate()
    {
        /* rotate a triangle around */
        glClear(GL_COLOR_BUFFER_BIT);
        //if (animate)
        glRotatef(1.0f, 0.0f, 0.0f, 1.0f);
        glBegin(GL_TRIANGLES);
          glIndexi(1);
          glColor3f(1.0f, 0.0f, 0.0f);
          glVertex2i(0,  1);
          glIndexi(2);
          glColor3f(0.0f, 1.0f, 0.0f);
          glVertex2i(-1, -1);
          glIndexi(3);
          glColor3f(0.0f, 0.0f, 1.0f);
          glVertex2i(1, -1);
        glEnd();
        glFlush();

        return TRUE;
    }

    virtual BOOL onChar(WPARAM wParam, LPARAM lParam)
    {
        switch (wParam)
        {
        case VK_ESCAPE:
            return UGLDialog::onCancel();
        default:
            return UGLDialog::onChar(wParam, lParam);
        }
    }
};

UDLGAPP_T(MyGLDialog, IDD_TEST);


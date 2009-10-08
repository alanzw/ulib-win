#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include <string.h>

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

    BOOL initGL()
    {
        RECT rc;
        ::GetClientRect(m_hDlg, &rc);
        //glViewport(0, 0, rc.right-rc.left, rc.bottom-rc.top);
        //UGlut::ResizeScene(rc.right-rc.left, rc.bottom-rc.top);
        int w = rc.right-rc.left;
        int h = rc.bottom-rc.top;
        glViewport (0, 0, (GLsizei) w, (GLsizei) h);
        glMatrixMode (GL_PROJECTION);
        glLoadIdentity ();
        if (w <= h)
            gluOrtho2D (0.0, 30.0, 0.0, 30.0 * (GLfloat) h/(GLfloat) w);
        else
            gluOrtho2D (0.0, 30.0 * (GLfloat) w/(GLfloat) h, 0.0, 30.0);
        glMatrixMode(GL_MODELVIEW);

        glClearColor (0.0, 0.0, 0.0, 0.0);
        glShadeModel (GL_SMOOTH);

        return TRUE;
    }

    virtual BOOL animate()
    {
        glClear (GL_COLOR_BUFFER_BIT);
        glBegin (GL_TRIANGLES);
        glColor3f (1.0, 0.0, 0.0);
        glVertex2f (5.0, 5.0);
        glColor3f (0.0, 1.0, 0.0);
        glVertex2f (25.0, 5.0);
        glColor3f (0.0, 0.0, 1.0);
        glVertex2f (5.0, 25.0);
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


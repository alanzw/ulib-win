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
    GLuint listName;
public:
    MyGLDialog(HINSTANCE hInst, UINT nID)
        : UGLDialog(hInst, nID)
    {
    }

    BOOL initGL()
    {
        RECT rc;
        ::GetClientRect(m_hDlg, &rc);
        //UGlut::ResizeScene(rc.right-rc.left, rc.bottom-rc.top);

        int w = rc.right-rc.left;
        int h = rc.bottom-rc.top;

        listName = glGenLists (1);
        glNewList (listName, GL_COMPILE);
        glColor3f (1.0, 0.0, 0.0);  /*  current color red  */
        glBegin (GL_TRIANGLES);
        glVertex2f (0.0, 0.0);
        glVertex2f (1.0, 0.0);
        glVertex2f (0.0, 1.0);
        glEnd ();
        //glTranslatef (1.5, 0.0, 0.0); /*  move position  */
        glEndList ();
        glShadeModel (GL_FLAT);

        glViewport(0, 0, w, h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        if (w <= h)
            gluOrtho2D (0.0, 2.0, -0.5 * (GLfloat) h/(GLfloat) w,
                1.5 * (GLfloat) h/(GLfloat) w);
        else
            gluOrtho2D (0.0, 2.0 * (GLfloat) w/(GLfloat) h, -0.5, 1.5);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        return TRUE;
    }

    void drawLine (void)
    {
        glBegin (GL_LINES);
        glVertex2f (0.0, 0.5);
        glVertex2f (15.0, 0.5);
        glEnd ();
    }

    virtual BOOL animate()
    {
        GLuint i;

        glClear (GL_COLOR_BUFFER_BIT);
        glColor3f (0.0, 1.0, 0.0);  /*  current color green  */
        for (i = 0; i < 8; i++)
        {
            glCallList (listName);
            glTranslatef (1.5, 0.0, 0.0);
        }

        drawLine();

        for (i = 0; i < 8; i++)
        {
            glTranslatef (-1.5, 0.0, 0.0);
            glCallList (listName);
        }
        /*  where is the line drawn?  */
        glFlush ();

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


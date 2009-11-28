#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include <cmath>

#include "ugldialog.h"
#include "udlgapp.h"
#include "uglut.h"

static int leftFirst = GL_TRUE;

using huys::UGLDialog;

class MyGLDialog : public UGLDialog
{
void drawLeftTriangle(void)
{
   /* draw yellow triangle on LHS of screen */

   glBegin (GL_TRIANGLES);
      glColor4f(1.0, 1.0, 0.0, 0.75);
      glVertex3f(0.1, 0.9, 0.0); 
      glVertex3f(0.1, 0.1, 0.0); 
      glVertex3f(0.7, 0.5, 0.0); 
   glEnd();
}

void drawRightTriangle(void)
{
   /* draw cyan triangle on RHS of screen */

   glBegin (GL_TRIANGLES);
      glColor4f(0.0, 1.0, 1.0, 0.75);
      glVertex3f(0.9, 0.9, 0.0); 
      glVertex3f(0.3, 0.5, 0.0); 
      glVertex3f(0.9, 0.1, 0.0); 
   glEnd();
}
public:
    MyGLDialog(HINSTANCE hInst, UINT nID)
    : UGLDialog(hInst, nID)
    {
    }

    BOOL initGL()
    {
        RECT rc;
        ::GetClientRect(m_hDlg, &rc);
        GLsizei w = rc.right-rc.left;
        GLsizei h = rc.bottom-rc.top;

        glViewport(0, 0, (GLsizei) w, (GLsizei) h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        if (w <= h) 
            gluOrtho2D (0.0, 1.0, 0.0, 1.0*(GLfloat)h/(GLfloat)w);
        else 
            gluOrtho2D (0.0, 1.0*(GLfloat)w/(GLfloat)h, 0.0, 1.0);

        glEnable (GL_BLEND);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glShadeModel (GL_FLAT);
        glClearColor (0.0, 0.0, 0.0, 0.0);    
        return TRUE;
    }

    virtual BOOL animate()
    {
        glClear(GL_COLOR_BUFFER_BIT);

        if (leftFirst)
        {
            drawLeftTriangle();
            drawRightTriangle();
        }
        else
        {
            drawRightTriangle();
            drawLeftTriangle();
        }

        glFlush();

        return TRUE;
    }

    virtual BOOL onChar(WPARAM wParam, LPARAM lParam)
    {
        switch (wParam)
        {
        case 't':
        case 'T':
            leftFirst = !leftFirst;
            return FALSE;	
        case VK_ESCAPE:
            return UGLDialog::onCancel();
        default:
            return UGLDialog::onChar(wParam, lParam);
        }
    }

private:
};

UDLGAPP_T(MyGLDialog, IDD_TEST);


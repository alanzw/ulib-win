#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include "ugldialog.h"
#include "udlgapp.h"
#include "uglut.h"

using huys::UGLDialog;

//float rotqube = 0.0f;

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
        //UGlut::ResizeScene(rc.right-rc.left, rc.bottom-rc.top);

        int w = rc.right-rc.left;
        int h = rc.bottom-rc.top;

        glClearColor (0.0, 0.0, 0.0, 0.0);
        glShadeModel (GL_FLAT);

        glViewport (0, 0, (GLsizei) w, (GLsizei) h);
        glMatrixMode (GL_PROJECTION);
        glLoadIdentity ();
        gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
        glMatrixMode (GL_MODELVIEW);

        return TRUE;
    }

    virtual BOOL animate()
    {
        GLdouble eqn[4] = {0.0, 1.0, 0.0, 0.0};
        GLdouble eqn2[4] = {1.0, 0.0, 0.0, 0.0};

        glClear(GL_COLOR_BUFFER_BIT);

        glColor3f (1.0, 1.0, 1.0);
        glPushMatrix();
        glTranslatef (0.0, 0.0, -5.0);

        /*    clip lower half -- y < 0          */
        glClipPlane (GL_CLIP_PLANE0, eqn);
        glEnable (GL_CLIP_PLANE0);
        /*    clip left half -- x < 0           */
        glClipPlane (GL_CLIP_PLANE1, eqn2);
        glEnable (GL_CLIP_PLANE1);

        glRotatef (90.0, 1.0, 0.0, 0.0);
        //glutWireSphere(1.0, 20, 16);
        //Quadrics are part of glu library that make parameterized shapes
        GLUquadricObj *qobj;
        qobj = gluNewQuadric();
        gluQuadricDrawStyle(qobj, GLU_LINE); /* wireframe */
        // rotate the wire sphere so it's vertically
        // oriented
        //::glRotatef( 90.0f, 1.0f, 0.0f, 0.0f );
        glColor3f( 1.0f, 0.0f, 0.0f );
        gluSphere( qobj, 0.5, 20, 10);
        glPopMatrix();

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


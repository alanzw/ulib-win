#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include <cstdio>
#include <cstring>
#include <cstdlib>

#include <GL/gl.h>
#include <GL/glu.h>

#include "ugldialog.h"
#include "udlgapp.h"
#include "uglut.h"

GLint fogMode;

void renderSphere (GLfloat x, GLfloat y, GLfloat z)
{
    glPushMatrix();
    glTranslatef (x, y, z);
    //glutSolidSphere(0.4, 16, 16);
    //Quadrics are part of glu library that make parameterized shapes
    GLUquadricObj *qobj;
    qobj = gluNewQuadric();
    gluQuadricDrawStyle(qobj, GLU_FILL);
    // rotate the wire sphere so it's vertically
    // oriented
    //::glRotatef( 90.0f, 1.0f, 0.0f, 0.0f );
    glColor3f( 1.0f, 0.0f, 0.0f );
    gluSphere( qobj, 0.5, 20, 10);
    glPopMatrix();
}

//
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
        int w = rc.right-rc.left;
        int h = rc.bottom-rc.top;
        glViewport(0, 0, (GLsizei) w, (GLsizei) h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        if (w <= h)
        glOrtho (-2.5, 2.5, -2.5*(GLfloat)h/(GLfloat)w,
        2.5*(GLfloat)h/(GLfloat)w, -10.0, 10.0);
        else
        glOrtho (-2.5*(GLfloat)w/(GLfloat)h,
        2.5*(GLfloat)w/(GLfloat)h, -2.5, 2.5, -10.0, 10.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity ();

        GLfloat position[] = { 0.5, 0.5, 3.0, 0.0 };

        glEnable(GL_DEPTH_TEST);

        glLightfv(GL_LIGHT0, GL_POSITION, position);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        {
            GLfloat mat[3] = {0.1745, 0.01175, 0.01175};	
            glMaterialfv (GL_FRONT, GL_AMBIENT, mat);
            mat[0] = 0.61424; mat[1] = 0.04136; mat[2] = 0.04136;	
            glMaterialfv (GL_FRONT, GL_DIFFUSE, mat);
            mat[0] = 0.727811; mat[1] = 0.626959; mat[2] = 0.626959;
            glMaterialfv (GL_FRONT, GL_SPECULAR, mat);
            glMaterialf (GL_FRONT, GL_SHININESS, 0.6*128.0);
        }

        glEnable(GL_FOG);
        {
            GLfloat fogColor[4] = {0.5, 0.5, 0.5, 1.0};

            fogMode = GL_EXP;
            glFogi (GL_FOG_MODE, fogMode);
            glFogfv (GL_FOG_COLOR, fogColor);
            glFogf (GL_FOG_DENSITY, 0.35);
            glHint (GL_FOG_HINT, GL_DONT_CARE);
            glFogf (GL_FOG_START, 1.0);
            glFogf (GL_FOG_END, 5.0);
        }
        glClearColor(0.5, 0.5, 0.5, 1.0);  /* fog color */

        return TRUE;
    }

    virtual BOOL animate()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderSphere (-2., -0.5, -1.0);
        renderSphere (-1., -0.5, -2.0);
        renderSphere (0., -0.5, -3.0);
        renderSphere (1., -0.5, -4.0);
        renderSphere (2., -0.5, -5.0);
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


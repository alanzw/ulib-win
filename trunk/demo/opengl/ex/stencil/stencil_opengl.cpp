#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include <cmath>

#include "ugldialog.h"
#include "udlgapp.h"
#include "uglut.h"

#define YELLOWMAT   1
#define BLUEMAT 2

using huys::UGLDialog;

class MyGLDialog : public UGLDialog
{
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
        
        glViewport(0, 0, (GLsizei) w, (GLsizei) h);

        /* create a diamond shaped stencil area */
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        if (w <= h)
        gluOrtho2D(-3.0, 3.0, -3.0*(GLfloat)h/(GLfloat)w,
        3.0*(GLfloat)h/(GLfloat)w);
        else
        gluOrtho2D(-3.0*(GLfloat)w/(GLfloat)h,
        3.0*(GLfloat)w/(GLfloat)h, -3.0, 3.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glClear(GL_STENCIL_BUFFER_BIT);
        glStencilFunc (GL_ALWAYS, 0x1, 0x1);
        glStencilOp (GL_REPLACE, GL_REPLACE, GL_REPLACE);
        glBegin(GL_QUADS);
        glVertex2f (-1.0, 0.0);
        glVertex2f (0.0, 1.0);
        glVertex2f (1.0, 0.0);
        glVertex2f (0.0, -1.0);
        glEnd();

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0, (GLfloat) w/(GLfloat) h, 3.0, 7.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0.0, 0.0, -5.0);
        

        GLfloat yellow_diffuse[] = { 0.7, 0.7, 0.0, 1.0 };
        GLfloat yellow_specular[] = { 1.0, 1.0, 1.0, 1.0 };

        GLfloat blue_diffuse[] = { 0.1, 0.1, 0.7, 1.0 };
        GLfloat blue_specular[] = { 0.1, 1.0, 1.0, 1.0 };

        GLfloat position_one[] = { 1.0, 1.0, 1.0, 0.0 };

        glNewList(YELLOWMAT, GL_COMPILE);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, yellow_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, yellow_specular);
        glMaterialf(GL_FRONT, GL_SHININESS, 64.0);
        glEndList();

        glNewList(BLUEMAT, GL_COMPILE);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, blue_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, blue_specular);
        glMaterialf(GL_FRONT, GL_SHININESS, 45.0);
        glEndList();

        glLightfv(GL_LIGHT0, GL_POSITION, position_one);

        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHTING);
        glEnable(GL_DEPTH_TEST);

        glClearStencil(0x0);
        glEnable(GL_STENCIL_TEST); 
        return TRUE;
    }

    virtual BOOL animate()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* draw blue sphere where the stencil is 1 */
        glStencilFunc (GL_EQUAL, 0x1, 0x1);
        glStencilOp (GL_KEEP, GL_KEEP, GL_KEEP);
        glCallList (BLUEMAT);
        //glutSolidSphere (0.5, 15, 15);
        GLUquadricObj *qobj;
        qobj = gluNewQuadric();
        gluQuadricDrawStyle(qobj, GLU_FILL); /* wireframe */
        // rotate the wire sphere so it's vertically
        // oriented
        //::glRotatef( 90.0f, 1.0f, 0.0f, 0.0f );
        glColor3f( 1.0f, 0.0f, 0.0f );
        gluSphere( qobj, 1.0, 16, 16);
        /* draw the tori where the stencil is not 1 */
        glStencilFunc (GL_NOTEQUAL, 0x1, 0x1);
        glPushMatrix();
        glRotatef (45.0, 0.0, 0.0, 1.0);
        glRotatef (45.0, 0.0, 1.0, 0.0);
        glCallList (YELLOWMAT);
        //glutSolidTorus (0.275, 0.85, 15, 15);
        glPushMatrix();
        glRotatef (90.0, 1.0, 0.0, 0.0);
        //glutSolidTorus (0.275, 0.85, 15, 15);
        glPopMatrix();
        glPopMatrix();

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

private:
};

UDLGAPP_T(MyGLDialog, IDD_TEST);


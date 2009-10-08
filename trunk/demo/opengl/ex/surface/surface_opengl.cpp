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

#ifndef CALLBACK
#define CALLBACK
#endif

typedef void (*CallBackFunc)();

//
GLfloat ctlpoints[4][4][3];
int showPoints = 0;

GLUnurbsObj *theNurb;

/*
*  Initializes the control points of the surface to a small hill.
*  The control points range from -3 to +3 in x, y, and z
*/
void init_surface(void)
{
    int u, v;
    for (u = 0; u < 4; u++) {
        for (v = 0; v < 4; v++) {
            ctlpoints[u][v][0] = 2.0*((GLfloat)u - 1.5);
            ctlpoints[u][v][1] = 2.0*((GLfloat)v - 1.5);

            if ( (u == 1 || u == 2) && (v == 1 || v == 2))
            ctlpoints[u][v][2] = 3.0;
            else
            ctlpoints[u][v][2] = -3.0;
        }
    }				
}

void CALLBACK nurbsError(GLenum errorCode)
{
    const GLubyte *estring;

    estring = gluErrorString(errorCode);
    fprintf (stderr, "Nurbs Error: %s\n", estring);
    exit(0);
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
		glViewport (0, 0, (GLsizei) w, (GLsizei) h);
		glMatrixMode(GL_PROJECTION); 
		glLoadIdentity();
		gluPerspective (45.0, (GLdouble)w/(GLdouble)h, 3.0, 8.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef (0.0, 0.0, -5.0);

        GLfloat mat_diffuse[] = { 0.7, 0.7, 0.7, 1.0 };
        GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
        GLfloat mat_shininess[] = { 100.0 };

        glClearColor (0.0, 0.0, 0.0, 0.0);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_AUTO_NORMAL);
        glEnable(GL_NORMALIZE);

        init_surface();

        theNurb = gluNewNurbsRenderer();
        gluNurbsProperty(theNurb, GLU_SAMPLING_TOLERANCE, 25.0);
        gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_FILL);
        //gluNurbsCallback(theNurb, GLU_ERROR, (CallBackFunc)&nurbsError);

        return TRUE;
    }

    virtual BOOL animate()
    {
        GLfloat knots[8] = {0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0};
        int i, j;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glPushMatrix();
        glRotatef(330.0, 1.,0.,0.);
        glScalef (0.5, 0.5, 0.5);

        gluBeginSurface(theNurb);
        gluNurbsSurface(theNurb, 
        8, knots, 8, knots,
        4 * 3, 3, &ctlpoints[0][0][0], 
        4, 4, GL_MAP2_VERTEX_3);
        gluEndSurface(theNurb);

        if (showPoints) {
            glPointSize(5.0);
            glDisable(GL_LIGHTING);
            glColor3f(1.0, 1.0, 0.0);
            glBegin(GL_POINTS);
            for (i = 0; i < 4; i++) {
                for (j = 0; j < 4; j++) {
                    glVertex3f(ctlpoints[i][j][0], 
                    ctlpoints[i][j][1], ctlpoints[i][j][2]);
                }
            }
            glEnd();
            glEnable(GL_LIGHTING);
        }
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


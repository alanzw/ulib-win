#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include <stdio.h>

#include "ugldialog.h"
#include "udlgapp.h"
#include "uglut.h"

using huys::UGLDialog;

GLfloat ctrlpoints[4][3] = {
    { -4.0, -4.0, 0.0 },
	{ -2.0,  4.0, 0.0 },
    {  2.0, -4.0, 0.0 }, 
    {  4.0,  4.0, 0.0 }
};

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

        glViewport(0, 0, (GLsizei) w, (GLsizei) h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        if (w <= h)
        {
            glOrtho(-5.0, 5.0, -5.0*(GLfloat)h/(GLfloat)w,
				5.0*(GLfloat)h/(GLfloat)w, -5.0, 5.0);
        }
		else
        {
        	glOrtho(-5.0*(GLfloat)w/(GLfloat)h,
                5.0*(GLfloat)w/(GLfloat)h, -5.0, 5.0, -5.0, 5.0);
		}
		glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glClearColor(0.0, 0.0, 0.0, 0.0);
        glShadeModel(GL_FLAT);
        glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints[0][0]);
        glEnable(GL_MAP1_VERTEX_3);

        return TRUE;
    }

    virtual BOOL animate()
    {
        int i = 0;

        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(1.0, 1.0, 1.0);

		glBegin(GL_LINE_STRIP);
        for (i = 0; i <= 30; i++)
        {
        	glEvalCoord1f((GLfloat) i/30.0);
        }
		glEnd();
        /* The following code displays the control points as dots. */
        glPointSize(5.0);
        glColor3f(1.0, 1.0, 0.0);
        glBegin(GL_POINTS);
        for (i = 0; i < 4; i++)
        {
        	glVertex3fv(&ctrlpoints[i][0]);
        }
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


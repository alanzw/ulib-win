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
    : UGLDialog(hInst, nID){}

    BOOL initGL()
    {
        RECT rc;
        ::GetClientRect(m_hDlg, &rc);
        //UGlut::ResizeScene(rc.right-rc.left, rc.bottom-rc.top);

        int w = rc.right-rc.left;
        int h = rc.bottom-rc.top;

        glViewport (0, 0, (GLsizei) w, (GLsizei) h);
        glMatrixMode (GL_PROJECTION);
        glLoadIdentity ();
        if (w <= h)
            glOrtho (-50.0, 50.0, -50.0*(GLfloat)h/(GLfloat)w,
                     50.0*(GLfloat)h/(GLfloat)w, -1.0, 1.0);
        else
            glOrtho (-50.0*(GLfloat)w/(GLfloat)h,
                     50.0*(GLfloat)w/(GLfloat)h, -50.0, 50.0, -1.0, 1.0);
        glMatrixMode(GL_MODELVIEW);

        glShadeModel(GL_SMOOTH);                            // Enable Smooth Shading
        glClearColor(0.0f, 0.0f, 0.0f, 0.5f);                // Black Background
        glClearDepth(1.0f);                                    // Depth Buffer Setup
        glEnable(GL_DEPTH_TEST);                            // Enables Depth Testing
        glDepthFunc(GL_LEQUAL);                                // The Type Of Depth Testing To Do
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);    // Really Nice Perspective Calculations

        return TRUE;
    }

    void draw_triangle(void)
    {
        glBegin (GL_LINE_LOOP);
        glVertex2f(0.0, 25.0);
        glVertex2f(25.0, -25.0);
        glVertex2f(-25.0, -25.0);
        glEnd();
    }

    virtual BOOL animate()
    {
        // Clear Screen And Depth Buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Reset The Current Modelview Matrix
        glLoadIdentity();

        gluLookAt (0.0, 0.0, 7.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
        //glScalef(0.5f, 0.5f, 0.5f);

        UGlut::switchToWireframe();


        //glTranslatef(0.0f, 0.0f,-7.0f);    // Translate Into The Screen 7.0 Units
        glLoadIdentity();
        glColor3f(1.0, 1.0, 1.0);
        draw_triangle();                   /* solid lines */
        glEnable(GL_LINE_STIPPLE);         /* dashed lines */
        glLineStipple(1, 0xF0F0);
        glLoadIdentity();
        glTranslatef(-20.0, 0.0, 0.0);
        draw_triangle();
        glLineStipple(1, 0xF00F);

        /*long dashed lines */
        glLoadIdentity();
        glScalef(1.5, 0.5, 1.0);
        draw_triangle();
        glLineStipple(1, 0x8888);          /* dotted lines */
        glLoadIdentity();

        glRotatef (90.0, 0.0, 0.0, 1.0);
        draw_triangle ();
        glDisable (GL_LINE_STIPPLE);

        UGlut::switchToSolid();

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


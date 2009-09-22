#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include <cmath>

#include "ugldialog.h"
#include "udlgapp.h"
#include "uglut.h"

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
        UGlut::ResizeScene(rc.right-rc.left, rc.bottom-rc.top);

        glShadeModel(GL_SMOOTH);                            // Enable Smooth Shading
        glClearColor(0.0f, 0.0f, 0.0f, 0.5f);                // Black Background
        glClearDepth(1.0f);                                    // Depth Buffer Setup
        glEnable(GL_DEPTH_TEST);                            // Enables Depth Testing
        glDepthFunc(GL_LEQUAL);                                // The Type Of Depth Testing To Do
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);    // Really Nice Perspective Calculations

        // Clear Screen And Depth Buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Reset The Current Modelview Matrix
        glLoadIdentity();
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);
        glTranslatef(0.0f, 0.0f,-2.5f);    // Translate Into The Screen 7.0 Units
        
        return TRUE;
    }

    virtual BOOL animate()
    {
        glClear(GL_COLOR_BUFFER_BIT);

        // Points
        glBegin(GL_POINTS);
        glColor3f(1,0,0);    glVertex2f(-0.1f,-0.1f); // red point
        glColor3f(0,1,0);    glVertex2f( 0.1f,-0.1f); // green point
        glColor3f(0,0,1);    glVertex2f( 0.1f, 0.1f); // blue point
        glColor3f(1,1,1);    glVertex2f(-0.1f, 0.1f); // white point
        glEnd();

        // Lines
        glColor3f(0.1f,0.5f,0.1f);
        glBegin(GL_LINES);
        // Horziontal :
        glVertex2f(-1,0);
        glVertex2f(1,0);
        // Vertical :
        glVertex2f(0,-1);
        glVertex2f(0,1);
        glEnd();

        // Red Triangle
        glColor3f(1,0,0);
        glBegin(GL_TRIANGLES);
        glVertex2f(-0.5f,-0.2f);
        glVertex2f(-0.8f,-0.8f);
        glVertex2f(-0.2f,-0.8f);
        glEnd();

        // Blue Quad
        glColor3f(0,0,1);
        glBegin(GL_TRIANGLE_STRIP);
        glVertex2f(0.2f,0.2f);
        glVertex2f(0.8f,0.2f);
        glVertex2f(0.2f,0.8f);
        glVertex2f(0.8f,0.8f);
        glEnd();

        // Colorful Quad
        glBegin(GL_QUADS);
        glColor3f(1,0,0);    glVertex2f(-0.8f,0.2f);
        glColor3f(0,1,0);    glVertex2f(-0.2f,0.2f);
        glColor3f(0,0,1);    glVertex2f(-0.2f,0.8f);
        glColor3f(1,0,1);    glVertex2f(-0.8f,0.8f);
        glEnd();

        // Five Sided Polygon
        glColor3f(0,1,0);
        glBegin(GL_POLYGON);
        glVertex2f(0.2f,-0.4f);
        glVertex2f(0.3f,-0.8f);
        glVertex2f(0.7f,-0.8f);
        glVertex2f(0.8f,-0.4f);
        glVertex2f(0.5f,-0.2f);
        glEnd();


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


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
    {}

    BOOL initGL()
    {
        RECT rc;
        ::GetClientRect(m_hDlg, &rc);
        int w = rc.right - rc.left;
        int h = rc.bottom - rc.left;
        glViewport(0, 0, (GLsizei) w, (GLsizei) h);
        glViewport(0, 0, (GLsizei) w, (GLsizei) h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        if (w <= h)
        {
            glOrtho(-0.0, 555.0, -0.0*(GLfloat)h/(GLfloat)w,
                555.0*(GLfloat)h/(GLfloat)w, -5.0, 5.0);
        }
        else
        {
            glOrtho(-0.0*(GLfloat)w/(GLfloat)h,
                555.0*(GLfloat)w/(GLfloat)h, -0.0, 555.0, -5.0, 5.0);
        }
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glClearColor (0, 0, 0, 0); // Clear to black
        glMatrixMode (GL_PROJECTION); // Projection view
        //glOrtho(0, 512, 0, 512, 0, 0); // Define display coordinates (left, bottom, right, top)

        return TRUE;
    }

    virtual BOOL animate()
    {
        static int mph = 0;

        mph = (mph + 1)%160;

        const double pi = 3.141592;
        const GLint X = 256;
        const GLint Y = 256;
        const GLint Radius = 120;

        glClear (GL_COLOR_BUFFER_BIT);
        glPushMatrix();
        // Draw the outline
        glColor3f (1, 0.5, 0);

        // Draw the filled circle
        glLineWidth (1);
        glBegin(GL_POLYGON);
        GLfloat angle = 0.f;
        GLfloat colorgrad;
        for (angle = 0.; angle < pi * 2;  angle += 0.01)
        // Let's toy with the color a little to make it a gradient
        {
            colorgrad = 0.2 - angle * 0.1;
            glColor3f (colorgrad, colorgrad, colorgrad);

            glVertex2f (X + sin(angle) * Radius, Y + cos(angle) * Radius);
        }
        glEnd ();


        // Speed marks

        glLineWidth (1);
        glColor3f (0.8, 0.8, 0.8);  // Mostly whiteish
        glEnable (GL_BLEND);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable (GL_LINE_SMOOTH);
        glBegin (GL_LINES);

        // This looks a little ugly, I'll explain ...
        GLfloat startangle = 2 * pi * 0.125;  // We are starting the marks at 12.5% left of bottom center
        GLfloat endangle = 2 * pi - startangle; // We are ending the marks at 12.5% right of bottom center
        GLfloat usableRange = 2 * pi * 0.75;  // Our usable range is 75% of the diameter

        for (angle =  startangle;  angle < endangle; angle += usableRange / 10)  // We want 11 marks so we divide by 10
        {
            glVertex2f (X + sin(angle) * Radius * 0.8, Y + cos(angle) * Radius * 0.8);  // We start the line inside a little
            glVertex2f (X + sin(angle) * Radius, Y + cos(angle) * Radius);  // To the end point at the edge
        }
        glEnd ();
        glDisable (GL_BLEND);

        // Outline

        glColor3f (0.4, 0.4, 0.4); // Grayish
        glLineWidth (5);
        glEnable (GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable (GL_LINE_SMOOTH);
        glBegin (GL_LINE_LOOP);
        for (angle = 0; angle < pi * 2; angle += 0.01)
        {
            glVertex2f (X + sin(angle) * Radius, Y + cos(angle) * Radius);
        }
        glDisable (GL_BLEND);
        glEnd ();

        // Draw the needle
        glColor3f (0.5, 0, 0); // Red
        glLineWidth (5); // Let's make it wider
        glEnable (GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable (GL_LINE_SMOOTH);
        glBegin (GL_LINES);

        // Here is our formula, the 20 represents the starting point = 0 and the 200 represents 100% of the range
        // So each MPH is 0.5% of the diameter
        // Confused?  Don't worry about it.

        angle = (1 - ((mph + 20) / 200)) * 2 * pi;  // each ticker value of 1 represents 0.5% degrees with a free 10% starting point = 0
        // ticker range is constrained to 75% of radius
        glVertex2f (X + sin(angle) * Radius * -0.03, Y + cos(angle) * Radius * -0.03);
        glVertex2f (X + sin(angle) * Radius * 0.9, Y + cos(angle) * Radius * 0.9);

        glDisable (GL_BLEND);
        glEnd ();

        // Here is the little thing holding the needle in place
        // Let's make it small and subtle
        // But it helps give us a sense of centeredness

        glPointSize (2);
        glColor3f (0.2, 0, 0);  // Dark Red

        glEnable (GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable (GL_POINT_SMOOTH);

        glBegin(GL_POINTS);
        glVertex2f (X, Y);
        glEnd ();
        glDisable (GL_BLEND);

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
private:
};

UDLGAPP_T(MyGLDialog, IDD_TEST);


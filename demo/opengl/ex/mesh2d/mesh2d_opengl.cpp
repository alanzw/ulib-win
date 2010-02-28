#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include <cstdio>

#include "ugldialog.h"
#include "udlgapp.h"
#include "uglut.h"
#include "umsg.h"

using huys::UGLDialog;

float rotqube = 0.0f;

GLfloat xmin = -600.0f;
GLfloat xmax = 2600.0f;
GLfloat ymin = -100.0f;
GLfloat ymax = 2000.0f;

struct Coordinate2D
{
    double x;
    double y;
};

Coordinate2D xy[91][21];

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
            glOrtho (xmin, xmax, ymin*(GLfloat)h/(GLfloat)w,
            ymax*(GLfloat)h/(GLfloat)w, -1.0, 1.0);
        else
            glOrtho (xmin*(GLfloat)w/(GLfloat)h,
            xmax*(GLfloat)w/(GLfloat)h, ymin, ymax, -1.0, 1.0);
        glMatrixMode(GL_MODELVIEW);

        glShadeModel(GL_SMOOTH);                            // Enable Smooth Shading
        glClearColor(0.0f, 0.0f, 0.0f, 0.5f);                // Black Background
        glClearDepth(1.0f);                                    // Depth Buffer Setup
        glEnable(GL_DEPTH_TEST);                            // Enables Depth Testing
        glDepthFunc(GL_LEQUAL);                                // The Type Of Depth Testing To Do
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);    // Really Nice Perspective Calculations

        readMesh();

        return TRUE;
    }

    void readMesh()
    {
        char buf[256] = {0};
        FILE *pFile = fopen("step.plt", "r");

        if (NULL == pFile)
        {
            ::showMsg("Error fopen!", "error", m_hDlg);
            this->onCancel();
            return;
        }

        //fgets(buf, 256, pFile); //

        for (int j=0; j<21; ++j)
        {
            for (int i=0; i<91; ++i)
            {
                fscanf(pFile, "%lf", &xy[i][j].x);
                fscanf(pFile, "%lf", &xy[i][j].y);
            }
        }

        fclose(pFile);
    }

    void drawLines(void)
    {
        int i, j;
        glLineWidth(1.5);
        glColor3f(1.0f, 0.0f, 0.0f);
        glBegin(GL_LINE_STRIP);
        for (i=0; i<91; ++i)
        {
            glVertex2f((GLfloat)xy[i][0].x, (GLfloat)xy[i][0].y);
        }
        glEnd();


        glLineWidth(1.0);
        glColor3f(1.0f, 1.0f, 1.0f);
        for (j=1; j<20; ++j)
        {
            glBegin (GL_LINE_STRIP);
            for (i=0; i<91; ++i)
            {
                glVertex2f((GLfloat)xy[i][j].x, (GLfloat)xy[i][j].y);
            }
            glEnd();
        }

        glLineWidth(1.5);
        glColor3f(1.0f, 0.0f, 0.0f);
        glBegin(GL_LINE_STRIP);
        for (i=0; i<91; ++i)
        {
            glVertex2f((GLfloat)xy[i][20].x, (GLfloat)xy[i][20].y);
        }
        glEnd();


        glLineWidth(1.5);
        glColor3f(1.0f, 0.0f, 0.0f);
        glBegin(GL_LINE_STRIP);
        for (j=0; j<21; ++j)
        {
            glVertex2f((GLfloat)xy[0][j].x, (GLfloat)xy[0][j].y);
        }
        glEnd();


        glLineWidth(1.0);
        glLineWidth(1.0);
        glColor3f(1.0f, 1.0f, 1.0f);
        for (i=1; i<90; ++i)
        {
            glBegin (GL_LINE_STRIP);
            for (j=0; j<21; ++j)
            {
                glVertex2f((GLfloat)xy[i][j].x, (GLfloat)xy[i][j].y);
            }
            glEnd();
        }

        glLineWidth(1.5);
        glColor3f(1.0f, 0.0f, 0.0f);
        glBegin(GL_LINE_STRIP);
        for (j=0; j<21; ++j)
        {
            glVertex2f((GLfloat)xy[90][j].x, (GLfloat)xy[90][j].y);
        }
        glEnd();
    }

    virtual BOOL animate()
    {
        // Clear Screen And Depth Buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Reset The Current Modelview Matrix
        glLoadIdentity();

        gluLookAt (0.0, 0.0, 100.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
        //glScalef(1.0f, 0.1f, 1.0f);

        //UGlut::switchToWireframe();


        //glTranslatef(0.0f, 0.0f,-7.0f);    // Translate Into The Screen 7.0 Units
        glLoadIdentity();
        glColor3f(1.0, 1.0, 1.0);


        drawLines();

        //UGlut::switchToSolid();

        rotqube +=0.9f;            // Increase Angle

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


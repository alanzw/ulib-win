#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "uwinapp.h"
#include "uglut.h"
#include "uglwindow.h"

GLdouble rot_angle;

void my_init()
{
    rot_angle = 0.0;

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glShadeModel(GL_FLAT);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_BLEND);
}

void my_disp()
{   
    rot_angle += 0.25;

    if( rot_angle > 360.0 )
        rot_angle -= 360.0;

    glClear(GL_COLOR_BUFFER_BIT);

    glPushMatrix();
        glRotated(rot_angle, 1.0, 0.0, 0.0);
        glRotated(rot_angle, 0.0, 1.0, 0.0);
        glBegin(GL_TRIANGLE_STRIP);
            glColor4f(1.0, 0.0, 0.0, 0.3);
            glVertex3f(-1.0, -1.0, 1.0);
            glVertex3f(1.0, -1.0, 1.0);
            glVertex3f(-1.0, 1.0, 1.0);
            glVertex3f(1.0, 1.0, 1.0);
            glColor4f(0.0, 1.0, 0.0, 0.3);
            glVertex3f(-1.0, 1.0, -1.0);
            glVertex3f(1.0, 1.0, -1.0);
            glColor4f(0.0, 0.0, 1.0, 0.3);
            glVertex3f(-1.0, -1.0, -1.0);
            glVertex3f(1.0, -1.0, -1.0);
        glEnd();
        glBegin(GL_TRIANGLE_STRIP);
            glColor4f(1.0, 1.0, 0.0, 0.3);
            glVertex3f(-1.0, 1.0, -1.0);
            glVertex3f(-1.0, 1.0, 1.0);
            glVertex3f(-1.0, -1.0, -1.0);
            glVertex3f(-1.0, -1.0, 1.0);
            glColor4f(0.0, 1.0, 1.0, 0.3);
            glVertex3f(1.0, -1.0, -1.0);
            glVertex3f(1.0, -1.0, 1.0);
            glColor4f(1.0, 0.0, 1.0, 0.3);
            glVertex3f(1.0, 1.0, -1.0);
            glVertex3f(1.0, 1.0, 1.0);
        glEnd();
    glPopMatrix();

}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    UWinApp app;

    UGLWindow * p = new UGLWindow;

    p->setInterval(10);
    p->setInit(&my_init);
    p->setDisplay(&my_disp);

    app.setMainWindow(p);

    app.init(hInstance);

    return app.run();
}


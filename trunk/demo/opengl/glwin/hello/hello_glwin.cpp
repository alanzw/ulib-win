#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include "uwinapp.h"
#include "ubasewindow.h"
#include "uglwindow.h"

GLfloat cnt1 = 0.0f;

void my_disp()
{
    //glViewport(0, 0, 640.0, 480.0);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    //glScalef(0.5,0.5,0.5);
    glTranslatef(0, 0,-5.0f);
    glRotatef(cnt1,0,0,1);
    //
    glBegin(GL_LINES);

    //
    glColor3f(1.f, 0.f, 0.f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(1.0f, 0.0f, 0.0f);
    glVertex3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.9f, 0.1f, 0.0f);
    glVertex3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.9f, -0.1f, 0.0f);

    //
    glColor3f(0.f, 1.f, 0.f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.1f, 0.9f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-0.1f, 0.9f, 0.0f);

    //
    glColor3f(0.f, 0.f, 1.f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.1f, 0.9f);
    glVertex3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, -0.1f, 0.9f);

    //
    glEnd();

    //
    glBegin(GL_POLYGON);
    glVertex2f(0.0f, 0.2f);
    glVertex2f(1.0f, 0.5f);
    glVertex2f(1.0f, 1.0f);
    glEnd();

    cnt1+=(cnt1>10?.0f:0.05f);
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    UWinApp app;

    UGLWindow * p = new UGLWindow;

    p->setInterval(1000);
    p->setDisplay(&my_disp);

    app.setMainWindow(p);

    app.init(hInstance);

    return app.run();
}

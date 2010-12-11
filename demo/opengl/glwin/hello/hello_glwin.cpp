#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include "uwinapp.h"
#include "ubasewindow.h"
#include "uglwindow.h"

GLfloat cnt1 = 0.0f;

void reshape (int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h); 
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 4.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void my_disp()
{
    //glViewport(0, 0, 640.0, 480.0);
    //reshape(640, 480);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    //glScalef(0.5,0.5,0.5);
    //glTranslatef(0, 0,-5.0f);
    //glRotatef(cnt1,0,0,1);
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

    //cnt1+= 0.05f;
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

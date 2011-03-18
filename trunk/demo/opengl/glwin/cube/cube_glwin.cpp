#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include "uwinapp.h"
#include "ubasewindow.h"
#include "uicon.h"
#include "umsg.h"

#include "uglut.h"

#include "uglwindow.h"

UGlut::UGLCube cube;

float cnt1;

static void reshape(int width, int height)
{
    GLfloat h = (GLfloat) height / (GLfloat) width;

    glViewport(0, 0, (GLint) width, (GLint) height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1.0, 1.0, -h, h, 5.0, 60.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -40.0);
}

void my_init()
{
    glMatrixMode(GL_PROJECTION);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_LIGHTING);
    glEnable(GL_CULL_FACE);

    GLfloat lightAmbient[] = {0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat lightDiffuse[] = {0.7f, 0.1f, 0.7f, 1.0f};
    GLfloat lightPosition[] = {0.0f, 0.0f, 1.0f, 1.0f};

    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);

    glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    cube.size(0.5f);
    cube.setColor(1.f, 0.f, 1.f);
}

void my_disp()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glRotatef(cnt1, 0.f, 0.5f, 0.5f);

    cube.render();

    cnt1+=(cnt1>10?.0f:0.05f);
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    UWinApp app;

    UGLWindow * p = new UGLWindow;

    p->setInterval(10);
    p->setReshape(&reshape);
    p->setInit(&my_init);
    p->setDisplay(&my_disp);

    app.setMainWindow(p);

    app.init(hInstance);

    return app.run();
}


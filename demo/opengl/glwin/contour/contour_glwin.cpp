#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "uwinapp.h"
#include "uglut.h"
#include "uglwindow.h"

bool bTranslate = false;

GLfloat cnt1 = 0.0f;

UINT TextureArray[1];

void my_init()
{
}


void my_disp()
{
    int i, istart, iend;
    float s, t;
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 1.0f, 0.0f);

    if (!bTranslate)
    {
        glTranslatef(0, 0,-5.0f);
        bTranslate = true;
    }

    glRotatef(cnt1,0,0,1);

    glPointSize(3.0);
    
    glBegin(GL_POINTS);
      for (i=0; i<4; ++i)
      {
        glVertex3f(i, i+1, i);
      }
    glEnd();

    cnt1 += ( cnt1 < 100 ? 2.f : 0.f);

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


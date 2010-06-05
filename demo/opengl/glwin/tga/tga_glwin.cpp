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
    glEnable(GL_TEXTURE_2D);

    UGlut::TGA_Texture(TextureArray, "texture.tga", 0);
}


void my_disp()
{

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (!bTranslate)
        {
            glTranslatef(0, 0,-5.0f);
            bTranslate = true;
        }

        glRotatef(cnt1,0,0,1);

        glBindTexture(GL_TEXTURE_2D, TextureArray[0]);

        glBegin(GL_QUADS);
            // top left vertex
            glTexCoord2f(0.0f, 1.0f);
            glVertex2f(0, 1);
            // bottom left vertex
            glTexCoord2f(0.0f, 0.0f);
            glVertex2f(0, 0);
            // bottom right vertex
            glTexCoord2f(1.0f, 0.0f);
            glVertex2f(1, 0);
            // top right vertex
            glTexCoord2f(1.0f, 1.0f);
            glVertex2f(1, 1);
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


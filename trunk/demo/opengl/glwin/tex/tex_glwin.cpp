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

#define ImageWidth 64
#define ImageHeight 64
GLubyte Image[ImageWidth][ImageHeight][3];

void makeImage(void)
{
    int i, j, r,g,b;
    
    for (i = 0; i < ImageWidth; i++) 
	{
		for (j = 0; j < ImageHeight; j++) 
		{
			r=(i*j)%255;
			g=(4*i)%255;
			b=(4*j)%255;
			Image[i][j][0] = (GLubyte) r;
			Image[i][j][1] = (GLubyte) g;
			Image[i][j][2] = (GLubyte) b;
		}
    }
}

void my_init()
{
    rot_angle = 0.0;

    glClearColor (0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
	//  生成纹理数据
    makeImage();
	//  设置像素存储模式
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	//  定义二维纹理映射
    glTexImage2D(GL_TEXTURE_2D, 0, 3, ImageWidth, 
		ImageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, &Image[0][0][0]);
	//  定义纹理映射参数
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//  数值纹理环境参数
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	//  使用二维纹理
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_FLAT);
}

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
    reshape(640, 480);
    
    rot_angle += 0.25;

    if( rot_angle > 360.0 )
        rot_angle -= 360.0;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f(-2.0, -1.0, 0.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(-2.0, 1.0, 0.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(0.0, 1.0, 0.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(0.0, -1.0, 0.0);

    glTexCoord2f(0.0, 0.0); glVertex3f(1.0, -1.0, 0.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(1.0, 1.0, 0.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(2.41421, 1.0, -1.41421);
    glTexCoord2f(1.0, 0.0); glVertex3f(2.41421, -1.0, -1.41421);
    glEnd();
    glFlush();

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


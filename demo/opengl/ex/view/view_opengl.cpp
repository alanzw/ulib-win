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

void DrawCube()
{
    // Colorful Quad
    glBegin(GL_QUAD_STRIP);
    glColor3f(1,0,0);
    glVertex3f(-0.5f,-0.5f,-0.5f);
    glVertex3f( 0.5f,-0.5f,-0.5f);
    glColor3f(1,1,0);
    glVertex3f(-0.5f, 0.5f,-0.5f);
    glVertex3f( 0.5f, 0.5f,-0.5f);
    glColor3f(0,1,0);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f( 0.5f, 0.5f, 0.5f);
    glColor3f(0,1,1);
    glVertex3f(-0.5f,-0.5f, 0.5f);
    glVertex3f( 0.5f,-0.5f, 0.5f);
    glColor3f(1,0,1);
    glVertex3f(-0.5f,-0.5f,-0.5f);
    glVertex3f( 0.5f,-0.5f,-0.5f);
    glEnd();

    glColor3f(0,0,1);
    glBegin(GL_QUADS);
    glVertex3f(-0.5f,-0.5f,-0.5f);
    glVertex3f(-0.5f, 0.5f,-0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f,-0.5f, 0.5f);

    glVertex3f( 0.5f,-0.5f,-0.5f);
    glVertex3f( 0.5f, 0.5f,-0.5f);
    glVertex3f( 0.5f, 0.5f, 0.5f);
    glVertex3f( 0.5f,-0.5f, 0.5f);
    glEnd();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0f, 1.3f, 0.1f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);
}
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

        glEnable(GL_DEPTH_TEST);
        glShadeModel(GL_FLAT);

        return TRUE;
    }

    virtual BOOL animate()
    {
    float alpha = GetTickCount()/10.0f;
    float scale = sinf(GetTickCount()/1000.0f)+1.1f;

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    glTranslatef(0,0,-4);

    glPushMatrix();
    glTranslatef(-2,-2,0);
    glRotatef(alpha,1.0f,0.5f,0.0f);
    DrawCube();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(2,-2,0);
    glRotatef(alpha,1.0f,-0.5f,0.5f);
    DrawCube();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(2, 2,0);
    glRotatef(alpha,0.0f,0.5f,-1.0f);
    DrawCube();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(-2, 2,0);
    glRotatef(alpha,1.0f,-0.5f,1.0f);
    DrawCube();
    glPopMatrix();

    glPushMatrix();
    glScalef(scale,scale,scale);
    glRotatef(alpha,-0.5f,1.0f,0.0f);
    DrawCube();
    glPopMatrix();

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


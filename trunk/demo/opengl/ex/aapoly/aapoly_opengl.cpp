#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include "ugldialog.h"
#include "udlgapp.h"
#include "uglut.h"


GLboolean polySmooth = GL_TRUE;

#define NFACE 6
#define NVERT 8

void drawCube(GLdouble x0, GLdouble x1, GLdouble y0, GLdouble y1, GLdouble z0, GLdouble z1)
{
    static GLfloat v[8][3];
    static GLfloat c[8][4] = {
        {0.0, 0.0, 0.0, 1.0}, {1.0, 0.0, 0.0, 1.0},
        {0.0, 1.0, 0.0, 1.0}, {1.0, 1.0, 0.0, 1.0},
        {0.0, 0.0, 1.0, 1.0}, {1.0, 0.0, 1.0, 1.0},
        {0.0, 1.0, 1.0, 1.0}, {1.0, 1.0, 1.0, 1.0}
    };

    /*  indices of front, top, left, bottom, right, back faces  */
    static GLubyte indices[NFACE][4] = {
        {4, 5, 6, 7}, {2, 3, 7, 6}, {0, 4, 7, 3},
        {0, 1, 5, 4}, {1, 5, 6, 2}, {0, 3, 2, 1}
    };

    v[0][0] = v[3][0] = v[4][0] = v[7][0] = x0;
    v[1][0] = v[2][0] = v[5][0] = v[6][0] = x1;
    v[0][1] = v[1][1] = v[4][1] = v[5][1] = y0;
    v[2][1] = v[3][1] = v[6][1] = v[7][1] = y1;
    v[0][2] = v[1][2] = v[2][2] = v[3][2] = z0;
    v[4][2] = v[5][2] = v[6][2] = v[7][2] = z1;

    glEnableClientState (GL_VERTEX_ARRAY);
    glEnableClientState (GL_COLOR_ARRAY);
    glVertexPointer (3, GL_FLOAT, 0, v);
    glColorPointer (4, GL_FLOAT, 0, c);
    glDrawElements (GL_QUADS, NFACE*4, GL_UNSIGNED_BYTE, indices);
    glDisableClientState (GL_VERTEX_ARRAY);
    glDisableClientState (GL_COLOR_ARRAY);
}

using huys::UGLDialog;

class MyGLDialog : public UGLDialog
{
public:
    MyGLDialog(HINSTANCE hInst, UINT nID)
    : UGLDialog(hInst, nID)
    {}

    virtual BOOL initGL()
    {
        RECT rc;
        ::GetClientRect(m_hDlg, &rc);
        int w = rc.right - rc.left;
        int h = rc.bottom - rc.left;
        glViewport(0, 0, (GLsizei) w, (GLsizei) h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(30.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        return TRUE;
    }

    virtual BOOL animate()
    {
        glCullFace (GL_BACK);
        glEnable (GL_CULL_FACE);
        glBlendFunc (GL_SRC_ALPHA_SATURATE, GL_ONE);
        glClearColor (0.0, 0.0, 0.0, 0.0);
    
        if (polySmooth)
        {
            glClear (GL_COLOR_BUFFER_BIT);
            glEnable (GL_BLEND);
            glEnable (GL_POLYGON_SMOOTH);
            glDisable (GL_DEPTH_TEST);
        }
        else
        { 
            glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glDisable (GL_BLEND);
            glDisable (GL_POLYGON_SMOOTH);
            glEnable (GL_DEPTH_TEST);
        }

        glPushMatrix ();
        glTranslatef (0.0, 0.0, -8.0);    
        glRotatef (30.0, 1.0, 0.0, 0.0);
        glRotatef (60.0, 0.0, 1.0, 0.0); 
        drawCube(-0.5, 0.5, -0.5, 0.5, -0.5, 0.5);
        glPopMatrix ();

        glFlush ();

        return TRUE;
    }

    virtual BOOL onChar(WPARAM wParam, LPARAM lParam)
    {
        switch (wParam)
        {
        case VK_ESCAPE:
            return UGLDialog::onCancel();
        case 't':
        case 'T':
            polySmooth = !polySmooth;
            this->redraw();
            return FALSE;
        default:
            return UGLDialog::onChar(wParam, lParam);
        }
    }
};

UDLGAPP_T(MyGLDialog, IDD_TEST);


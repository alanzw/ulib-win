#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include "ugldialog.h"
#include "udlgapp.h"
#include "uglut.h"

using huys::UGLDialog;



class MyGLDialog : public UGLDialog
{
    enum {
        MA_PAN = 1,    /* pan state bit     */
        MA_ROTATE,     /* rotate state bits */
        MA_ZOOM        /* zoom state bit    */
    };

    GLfloat trans[3];    /* current translation */
    GLfloat rot[2];      /* current rotation */

    int omx, omy, mx, my;
    GLuint state;

    GLboolean left;    /* left button currently down? */
    GLboolean right;   /* right button currently down? */

    //
    float rotqube;
public:
    MyGLDialog(HINSTANCE hInst, UINT nID)
    : UGLDialog(hInst, nID)
    {
        trans[0] = 0.0f;
        trans[1] = 0.0f;
        trans[2] = -7.0f;
        rot[0] = rot[1] = 0.0f;
        state   = 0;
        left  = GL_FALSE;
        right = GL_FALSE;

        //
        rotqube = 0.0f;
    }

    virtual BOOL initGL()
    {
        RECT rc;
        ::GetClientRect(m_hDlg, &rc);
        UGlut::ResizeScene(rc.right-rc.left, rc.bottom-rc.top);

        glShadeModel(GL_SMOOTH);                             // Enable Smooth Shading
        glClearColor(0.0f, 0.0f, 0.0f, 0.5f);                // Black Background
        glClearDepth(1.0f);                                  // Depth Buffer Setup
        glEnable(GL_DEPTH_TEST);                             // Enables Depth Testing
        glDepthFunc(GL_LEQUAL);                              // The Type Of Depth Testing To Do
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);   // Really Nice Perspective Calculations

        return TRUE;
    }

    virtual BOOL animate()
    {
        // Clear Screen And Depth Buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Reset The Current Modelview Matrix
        glLoadIdentity();

        glPushMatrix();
        glTranslatef(trans[0], trans[1], trans[2]);
        glRotatef(rot[0], 1.0f, 0.0f, 0.0f);
        glRotatef(rot[1], 0.0f, 1.0f, 0.0f);

        glBegin(GL_TRIANGLES);

#define TOP glIndexi(1); glColor3f(1.0f, 0.0f, 0.0f); glVertex3i(0, 1, 0)
#define FR  glIndexi(2); glColor3f(0.0f, 1.0f, 0.0f); glVertex3i(1, -1, 1)
#define FL  glIndexi(3); glColor3f(0.0f, 0.0f, 1.0f); glVertex3i(-1, -1, 1)
#define BR  glIndexi(3); glColor3f(0.0f, 0.0f, 1.0f); glVertex3i(1, -1, -1)
#define BL  glIndexi(2); glColor3f(0.0f, 1.0f, 0.0f); glVertex3i(-1, -1, -1)

        TOP; FL; FR;
        TOP; FR; BR;
        TOP; BR; BL;
        TOP; BL; FL;
        FR; FL; BL;
        BL; BR; FR;

        glEnd();
        glPopMatrix();
        glFlush();

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

    virtual BOOL onMouseMove(WPARAM wParam, LPARAM lParam)
    {
        if (state) {
            omx = mx;
            omy = my;
            mx = LOWORD(lParam);
            my = HIWORD(lParam);
            /* Win32 is pretty braindead about the x, y position that
                it returns when the mouse is off the left or top edge
                of the window (due to them being unsigned). therefore,
                roll the Win32's 0..2^16 pointer co-ord range to the
                more amenable (and useful) 0..+/-2^15. */
            if(mx & 1 << 15) mx -= (1 << 16);
            if(my & 1 << 15) my -= (1 << 16);
            update(state, omx, mx, omy, my);
            //PostMessage(hWnd, WM_PAINT, 0, 0);
        }
        return UGLDialog::onMouseMove(wParam, lParam);
    }

    virtual BOOL onLButtonDown(WPARAM wParam, LPARAM lParam)
    {
        ::SetCapture(m_hDlg);
        mx = LOWORD(lParam);
        my = HIWORD(lParam);
        state |= MA_PAN;
        return UGLDialog::onLButtonDown(wParam, lParam);
    }

    virtual BOOL onRButtonDown(WPARAM wParam, LPARAM lParam)
    {
        ::SetCapture(m_hDlg);
        mx = LOWORD(lParam);
        my = HIWORD(lParam);
        state |= MA_ROTATE;
        return UGLDialog::onRButtonDown(wParam, lParam);
    }

    virtual BOOL onLButtonUp(WPARAM wParam, LPARAM lParam)
    {
        ::ReleaseCapture();
        state = 0;
        return UGLDialog::onLButtonUp(wParam, lParam);
    }

    virtual BOOL onRButtonUp(WPARAM wParam, LPARAM lParam)
    {
        ::ReleaseCapture();
        state = 0;
        return UGLDialog::onRButtonUp(wParam, lParam);
    }
    
    virtual BOOL onMButtonDown(WPARAM wParam, LPARAM lParam)
    {
        ::SetCapture(m_hDlg);
        state |= MA_ZOOM;
        return UGLDialog::onMButtonDown(wParam, lParam);
    }
    
    virtual BOOL onMButtonUp(WPARAM wParam, LPARAM lParam)
    {
        ::ReleaseCapture();
        state = 0;
        return UGLDialog::onMButtonUp(wParam, lParam);
    }
    
private:
    void update(int state, int ox, int nx, int oy, int ny)
    {
        int dx = ox - nx;
        int dy = ny - oy;

        switch(state) {
        case MA_PAN:
            trans[0] -= dx / 100.0f;
            trans[1] -= dy / 100.0f;
            break;
        case MA_ROTATE:
            rot[0] += (dy * 180.0f) / 500.0f;
            rot[1] -= (dx * 180.0f) / 500.0f;

#define clamp(x) x = x > 360.0f ? x-360.0f : x < -360.0f ? x+=360.0f : x
            clamp(rot[0]);
            clamp(rot[1]);
            break;
        case MA_ZOOM:
            trans[2] -= (dx+dy) / 100.0f;
            break;
        }
    }
};

UDLGAPP_T(MyGLDialog, IDD_TEST);


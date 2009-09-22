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

        glShadeModel(GL_SMOOTH);                            // Enable Smooth Shading
        glClearColor(0.0f, 0.0f, 0.0f, 0.5f);                // Black Background
        glClearDepth(1.0f);                                    // Depth Buffer Setup
        glEnable(GL_DEPTH_TEST);                            // Enables Depth Testing
        glDepthFunc(GL_LEQUAL);                                // The Type Of Depth Testing To Do
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);    // Really Nice Perspective Calculations

        glEnable(GL_TEXTURE_2D);

        return TRUE;
    }

    virtual BOOL animate()
    {
        // Clear Screen And Depth Buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Reset The Current Modelview Matrix
        glLoadIdentity();

        glTranslatef(0, 0,-5.0f);


    glBegin(GL_QUADS);
        // top left vertex
        glVertex3f(-1, 1, 0);
        // bottom left vertex
        glVertex3f(-1, -1, 0);
        // bottom right vertex
        glVertex3f(1, -1, 0);
        // top right vertex
        glVertex3f(1, 1, 0);
    glEnd();

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
        return UGLDialog::onMouseMove(wParam, lParam);
    }

    virtual BOOL onLButtonDown(WPARAM wParam, LPARAM lParam)
    {
        return UGLDialog::onLButtonDown(wParam, lParam);
    }

    virtual BOOL onRButtonDown(WPARAM wParam, LPARAM lParam)
    {
        return UGLDialog::onRButtonDown(wParam, lParam);
    }

    virtual BOOL onLButtonUp(WPARAM wParam, LPARAM lParam)
    {
        return UGLDialog::onLButtonUp(wParam, lParam);
    }

    virtual BOOL onRButtonUp(WPARAM wParam, LPARAM lParam)
    {
        return UGLDialog::onRButtonUp(wParam, lParam);
    }
private:
};

UDLGAPP_T(MyGLDialog, IDD_TEST);


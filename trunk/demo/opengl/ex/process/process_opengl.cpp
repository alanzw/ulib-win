#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include <cmath>

#include "ugldialog.h"
#include "udlgapp.h"
#include "uglut.h"

GLfloat g_process = -1.5;

using huys::UGLDialog;

class MyGLDialog : public UGLDialog
{
	enum {
		ID_TIMER_INTERNAL = 22
	};
public:
    MyGLDialog(HINSTANCE hInst, UINT nID)
    : UGLDialog(hInst, nID)
    {}

    ~MyGLDialog()
    {
        this->killTimer(ID_TIMER_INTERNAL);
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

        // Clear Screen And Depth Buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Reset The Current Modelview Matrix
        glLoadIdentity();
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);
        glTranslatef(0.0f, 0.0f,-2.5f);    // Translate Into The Screen 7.0 Units

        glEnable (GL_LINE_SMOOTH);
        glEnable (GL_BLEND);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glHint (GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
        glLineWidth (1.5);
          
        this->setTimer(ID_TIMER_INTERNAL, 500);

        return TRUE;
    }

    virtual BOOL animate()
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glPushMatrix();
        glBegin (GL_QUADS);
        glColor3f (1.0, 0.0, 0.0);
        glVertex2f (-1.5, 0.5);
        glVertex2f (-1.5, -0.5);
        glColor3f (0.0, 0.0, 0.9);
        glVertex2f (1.5, -0.5);
        glVertex2f (1.5, 0.5);
        glEnd ();
        
        glBegin (GL_QUADS);
        glColor3f (0.0, 0.8, 0.6);
        glVertex2f (-1.5, 0.5);
        glVertex2f (-1.5, -0.5);
        glVertex2f (g_process, -0.5);
        glVertex2f (g_process, 0.5);
        glEnd ();
        
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
    
	virtual BOOL onTimer(WPARAM wParam, LPARAM lParam)
	{
		switch (wParam)
		{
		case ID_TIMER_INTERNAL:
			{
                if (g_process < 1.5)
                {
                    g_process += 0.05;
                }
                else
                {
                    g_process = -1.5;
                }
				return FALSE;
			}
		default:
			;
		}
		return FALSE;
	}
private:
};

UDLGAPP_T(MyGLDialog, IDD_TEST);


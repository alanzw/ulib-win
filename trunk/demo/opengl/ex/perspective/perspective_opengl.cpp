#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include "ugldialog.h"
#include "udlgapp.h"
#include "uglut.h"
#include "umsg.h"

using huys::UGLDialog;

float rotqube = 0.0f;

class MyGLDialog : public UGLDialog
{
public:
    MyGLDialog(HINSTANCE hInst, UINT nID)
    : UGLDialog(hInst, nID)
    {
        x = 45.0f;
    
    }

    BOOL initGL()
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

        gluLookAt (0.0, 0.0, 7.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
        //glScalef(0.5f, 0.5f, 0.5f);

        UGlut::switchToWireframe();

        //glTranslatef(0.0f, 0.0f,-7.0f);    // Translate Into The Screen 7.0 Units
        glRotatef(rotqube,0.0f,1.0f,0.0f);    // Rotate The cube around the Y axis
        glRotatef(rotqube,1.0f,1.0f,1.0f);
        glBegin(GL_QUADS);        // Draw The Cube Using quads
        glColor3f(0.0f,1.0f,0.0f);    // Color Blue
        glVertex3f( 1.0f, 1.0f,-1.0f);    // Top Right Of The Quad (Top)
        glVertex3f(-1.0f, 1.0f,-1.0f);    // Top Left Of The Quad (Top)
        glVertex3f(-1.0f, 1.0f, 1.0f);    // Bottom Left Of The Quad (Top)
        glVertex3f( 1.0f, 1.0f, 1.0f);    // Bottom Right Of The Quad (Top)
        glColor3f(1.0f,0.5f,0.0f);    // Color Orange
        glVertex3f( 1.0f,-1.0f, 1.0f);    // Top Right Of The Quad (Bottom)
        glVertex3f(-1.0f,-1.0f, 1.0f);    // Top Left Of The Quad (Bottom)
        glVertex3f(-1.0f,-1.0f,-1.0f);    // Bottom Left Of The Quad (Bottom)
        glVertex3f( 1.0f,-1.0f,-1.0f);    // Bottom Right Of The Quad (Bottom)
        glColor3f(1.0f,0.0f,0.0f);    // Color Red
        glVertex3f( 1.0f, 1.0f, 1.0f);    // Top Right Of The Quad (Front)
        glVertex3f(-1.0f, 1.0f, 1.0f);    // Top Left Of The Quad (Front)
        glVertex3f(-1.0f,-1.0f, 1.0f);    // Bottom Left Of The Quad (Front)
        glVertex3f( 1.0f,-1.0f, 1.0f);    // Bottom Right Of The Quad (Front)
        glColor3f(1.0f,1.0f,0.0f);    // Color Yellow
        glVertex3f( 1.0f,-1.0f,-1.0f);    // Top Right Of The Quad (Back)
        glVertex3f(-1.0f,-1.0f,-1.0f);    // Top Left Of The Quad (Back)
        glVertex3f(-1.0f, 1.0f,-1.0f);    // Bottom Left Of The Quad (Back)
        glVertex3f( 1.0f, 1.0f,-1.0f);    // Bottom Right Of The Quad (Back)
        glColor3f(0.0f,0.0f,1.0f);    // Color Blue
        glVertex3f(-1.0f, 1.0f, 1.0f);    // Top Right Of The Quad (Left)
        glVertex3f(-1.0f, 1.0f,-1.0f);    // Top Left Of The Quad (Left)
        glVertex3f(-1.0f,-1.0f,-1.0f);    // Bottom Left Of The Quad (Left)
        glVertex3f(-1.0f,-1.0f, 1.0f);    // Bottom Right Of The Quad (Left)
        glColor3f(1.0f,0.0f,1.0f);    // Color Violet
        glVertex3f( 1.0f, 1.0f,-1.0f);    // Top Right Of The Quad (Right)
        glVertex3f( 1.0f, 1.0f, 1.0f);    // Top Left Of The Quad (Right)
        glVertex3f( 1.0f,-1.0f, 1.0f);    // Bottom Left Of The Quad (Right)
        glVertex3f( 1.0f,-1.0f,-1.0f);    // Bottom Right Of The Quad (Right)
        glEnd();            // End Drawing The Cube

        UGlut::switchToSolid();

        rotqube +=0.9f;            // Increase Angle

        return TRUE;
    }

    virtual BOOL onChar(WPARAM wParam, LPARAM lParam)
    {
        switch (wParam)
        {
        case VK_ESCAPE:
            return UGLDialog::onCancel();
        case 'w':
        case 'W':
            return this->onKeyW();
        case 's':
        case 'S':
            return this->onKeyS();
        default:
            return UGLDialog::onChar(wParam, lParam);
        }
    }
    
    float x;
private:
    BOOL onKeyW()
    {
        huys::URectI rect;
        ::GetClientRect(m_hDlg, rect);

        x += 1.0f;


        glMatrixMode(GL_PROJECTION);           // Select The Modelview Matrix
        glLoadIdentity();
        gluPerspective(x,(GLfloat)rect.width()/(GLfloat)rect.height(), 0.1f, 100.0f);
        glMatrixMode(GL_MODELVIEW);           // Select The Modelview Matrix 
        //glLoadIdentity();  
        //showMsgFormat("x", "%f", x);
        
        return FALSE;
    }

    BOOL onKeyS()
    {
        huys::URectI rect;
        ::GetClientRect(m_hDlg, rect);


        x -= 1.0f;
        
        glMatrixMode(GL_PROJECTION);           // Select The Modelview Matrix
        glLoadIdentity();
        gluPerspective(x,(GLfloat)rect.width()/(GLfloat)rect.height(), 0.1f, 100.0f);
        glMatrixMode(GL_MODELVIEW);           // Select The Modelview Matrix 
        //glLoadIdentity();  
        //showMsgFormat("x", "%f", x);

        return FALSE;
    }
};

UDLGAPP_T(MyGLDialog, IDD_TEST);


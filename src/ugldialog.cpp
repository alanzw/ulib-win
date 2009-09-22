#include <windows.h>
#include <tchar.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include "uglut.h"
#include "ugldialog.h"

namespace huys
{

UGLDialog::UGLDialog( HINSTANCE hInst,
                      UINT nID,
                      DLGPROC lpDialogFunc /*=UDialogBox::DefaultDlgProc*/,
                      HWND hParent /*= NULL*/)
: UDialogBox(hInst, nID, lpDialogFunc, hParent),
m_bQuit(FALSE)
{}

UGLDialog::~UGLDialog()
{}

BOOL UGLDialog::create()
{
    m_hDlg = CreateDialog(m_hInst, MAKEINTRESOURCE(m_nDialogID), m_hParent, m_lpDialogFunc);
    this->setLong();

#if (WINVER >= 0x0500)
    AnimateWindow(m_hDlg, 1000, AW_SLIDE|AW_HOR_POSITIVE);
    RECT rc;
    GetClientRect(m_hDlg, &rc);
    ::InvalidateRect(m_hDlg, &rc, FALSE);
#else
    ShowWindow(m_hDlg, SW_SHOW);
#endif
    return this->go();
}

BOOL UGLDialog::go()
{

    UGlut::EnableOpenGL(m_hDlg, m_hdc, m_hrc);

    if (!initGL())
    {
        return FALSE;
    }
    
    //BOOL bQuit = FALSE;
    MSG msg;

    while (!m_bQuit)
    {
        // check for messages
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            // handle or dispatch messages
            if ( msg.message == WM_QUIT)
            {
                m_bQuit = TRUE;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }

        }
        else
        {
            // OpenGL animation code goes here
            animate();

            SwapBuffers( m_hdc );
        }
    }
    // shutdown OpenGL
    UGlut::DisableOpenGL( m_hDlg, m_hdc, m_hrc );
    // destroy the window explicitly
    this->onCancel();
    return 0;
    //return UDialogBox::go();
}


BOOL UGLDialog::onCancel()
{
    m_bQuit = TRUE;
    return UDialogBox::onCancel();
}

BOOL UGLDialog::animate()
{

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // setup viewing
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 50.0, 0.0, 50.0);
    glMatrixMode(GL_MODELVIEW);

    //gluLookAt(2.0, 2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 3.0);

    //glPushMatrix();
/*
    glBegin(GL_POLYGON);
    uglColorRed(); glVertex2f(0.0, 0.0);
    uglColorGreen(); glVertex2f(0.0, 0.3);
    uglColorBlue(); glVertex2f(0.3, 0.3);
    uglColorYellow(); glVertex2f(0.4, 0.15);
    uglColorCyan(); glVertex2f(0.3, 0.0);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(0.5,0.5);
    glVertex2f(0.0,0.8);
    glVertex2f(0.8,0.8);
    glVertex2f(0.8,0.0);
    glEnd();

    glRectf(-0.5, -0.5, 0.0, 0.0);

    glBegin(GL_POLYGON);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 1.0, 0.0);
    glVertex3f(1.0, 0.0, 1.0);
    glEnd();
*/


    GLfloat vertices[3][2] = {{0.0, 0.0}, {25.0, 50.0}, {50.0, 0.0}};
    GLfloat p[2] = {7.5, 5.0};
    //
    int j, k;
    //int rand();
    glBegin(GL_POINTS);
    for (k=0; k<5000; k++)
    {
        j = rand()%3;

        //
        p[0] = (p[0] + vertices[j][0])/2;
        p[1] = (p[1] + vertices[j][1])/2;
        //
        glVertex2fv(p);
    }
    glEnd();

    glFlush();

    //glPopMatrix();

    return TRUE;
}

BOOL UGLDialog::onDestroy()
{
    m_bQuit = TRUE;
    //return UDialogBox::onDestroy();
    return TRUE;
}
} // namespace huys


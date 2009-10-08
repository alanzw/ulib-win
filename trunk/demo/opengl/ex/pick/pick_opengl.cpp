#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include <stdio.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include "ugldialog.h"
#include "udlgapp.h"
#include "uglut.h"

using huys::UGLDialog;

int board[3][3];   /*  amount of color for each square    */

class MyGLDialog : public UGLDialog
{
public:
    MyGLDialog(HINSTANCE hInst, UINT nID)
    : UGLDialog(hInst, nID)
    {
        //int i, j;
        //for (i = 0; i < 3; i++)
        //    for (j = 0; j < 3; j ++)
        //        board[i][j] = 0;
        ::ZeroMemory((char *)board, sizeof(board));
    }

    /*  The nine squares are drawn.  In selection mode, each
     *  square is given two names:  one for the row and the
     *  other for the column on the grid.  The color of each
     *  square is determined by its position on the grid, and
     *  the value in the board[][] array.
     */
    void drawSquares(GLenum mode)
    {
        GLuint i = 0;
		GLuint j = 0;
        for (i = 0; i < 3; i++)
		{
            if (mode == GL_SELECT) glLoadName (i);
            for (j = 0; j < 3; j ++)
			{
                if (mode == GL_SELECT) glPushName (j);
                glColor3f((GLfloat) i/3.0, (GLfloat) j/3.0, (GLfloat) board[i][j]/3.0);
                glRecti (i, j, i+1, j+1);
                if (mode == GL_SELECT) glPopName();
            }
        }
    }

    /*  processHits prints out the contents of the
     *  selection array.
     */
    void processHits (GLint hits, GLuint buffer[])
    {
        unsigned int i, j;
        GLuint ii, jj, names, *ptr;

        printf ("hits = %d\n", hits);
        ptr = (GLuint *) buffer;
        for (i = 0; i < hits; i++) {    /*  for each hit  */
            names = *ptr;
            printf(" number of names for this hit = %d\n", names); ptr++;
            printf("  z1 is %g;", (float) *ptr/0x7fffffff); ptr++;
            printf(" z2 is %g\n", (float) *ptr/0x7fffffff); ptr++;
            printf("   names are ");
            for (j = 0; j < names; j++)
			{ /*  for each name */
                printf ("%d ", *ptr);
                if (j == 0)  /*  set row and column  */
                    ii = *ptr;
                else if (j == 1)
                    jj = *ptr;
                ptr++;
            }
            printf("\n");
            board[ii][jj] = (board[ii][jj] + 1) % 3;
        }
    }

    /*  pickSquares() sets up selection mode, name stack,
     *  and projection matrix for picking.  Then the
     *  objects are drawn.
     */
#define BUFSIZE 512

    void pickSquares(int button, int state, int x, int y)
    {
        GLuint selectBuf[BUFSIZE];
        GLint hits;
        GLint viewport[4];

        //if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN)
        //    return;

        glGetIntegerv (GL_VIEWPORT, viewport);

        glSelectBuffer (BUFSIZE, selectBuf);
        (void) glRenderMode (GL_SELECT);

        glInitNames();
        glPushName(0);

        glMatrixMode (GL_PROJECTION);
        glPushMatrix ();
        glLoadIdentity ();
        /*  create 5x5 pixel picking region near cursor location    */
        gluPickMatrix ((GLdouble) x, (GLdouble) (viewport[3] - y),
            5.0, 5.0, viewport);
        gluOrtho2D (0.0, 3.0, 0.0, 3.0);
        drawSquares (GL_SELECT);

        glMatrixMode (GL_PROJECTION);
        glPopMatrix ();
        glFlush ();

        hits = glRenderMode (GL_RENDER);
        processHits (hits, selectBuf);
        // glutPostRedisplay();
    }

    BOOL initGL()
    {
        RECT rc;
        ::GetClientRect(m_hDlg, &rc);
        //UGlut::ResizeScene(rc.right-rc.left, rc.bottom-rc.top);

        int w = rc.right-rc.left;
        int h = rc.bottom-rc.top;

        glViewport (0, 0, (GLsizei) w, (GLsizei) h);
        glMatrixMode (GL_PROJECTION);
        glLoadIdentity ();
        if (w <= h)
            glOrtho (-5.0, 5.0, -5.0*(GLfloat)h/(GLfloat)w,
                5.0*(GLfloat)h/(GLfloat)w, -1.0, 1.0);
        else
            glOrtho (-5.0*(GLfloat)w/(GLfloat)h,
                5.0*(GLfloat)w/(GLfloat)h, -5.0, 5.0, -1.0, 1.0);
        glMatrixMode(GL_MODELVIEW);

        glShadeModel(GL_SMOOTH);                            // Enable Smooth Shading
        glClearColor(0.0f, 0.0f, 0.0f, 0.5f);                // Black Background
        glClearDepth(1.0f);                                    // Depth Buffer Setup
        glEnable(GL_DEPTH_TEST);                            // Enables Depth Testing
        glDepthFunc(GL_LEQUAL);                                // The Type Of Depth Testing To Do
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);    // Really Nice Perspective Calculations

        return TRUE;
    }

    virtual BOOL animate()
    {
        glClear(GL_COLOR_BUFFER_BIT);
        drawSquares (GL_RENDER);
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
};

UDLGAPP_T(MyGLDialog, IDD_TEST);


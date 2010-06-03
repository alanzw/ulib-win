#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include "ugldialog.h"
#include "udlgapp.h"
#include "uglut.h"

GLuint teapotList;

/*
 * Rim, body, lid, and bottom data must be reflected in x and y;
 * handle and spout data across the y axis only.
 */
static int patchdata[][16] =
{
    { 102, 103, 104, 105,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15 }, /* rim    */
    {  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27 }, /* body   */
    {  24,  25,  26,  27,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40 },
    {  96,  96,  96,  96,  97,  98,  99, 100, 101, 101, 101, 101,   0,   1,   2,   3 }, /* lid    */
    {   0,   1,   2,   3, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117 },
    { 118, 118, 118, 118, 124, 122, 119, 121, 123, 126, 125, 120,  40,  39,  38,  37 }, /* bottom */
    {  41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56 }, /* handle */
    {  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  28,  65,  66,  67 },
    {  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,  80,  81,  82,  83 }, /* spout  */
    {  80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,  95 }
};

static double cpdata[][3] =
{
    {0.2, 0, 2.7}, {0.2, -0.112, 2.7}, {0.112, -0.2, 2.7}, {0,
    -0.2, 2.7}, {1.3375, 0, 2.53125}, {1.3375, -0.749, 2.53125},
    {0.749, -1.3375, 2.53125}, {0, -1.3375, 2.53125}, {1.4375,
    0, 2.53125}, {1.4375, -0.805, 2.53125}, {0.805, -1.4375,
    2.53125}, {0, -1.4375, 2.53125}, {1.5, 0, 2.4}, {1.5, -0.84,
    2.4}, {0.84, -1.5, 2.4}, {0, -1.5, 2.4}, {1.75, 0, 1.875},
    {1.75, -0.98, 1.875}, {0.98, -1.75, 1.875}, {0, -1.75,
    1.875}, {2, 0, 1.35}, {2, -1.12, 1.35}, {1.12, -2, 1.35},
    {0, -2, 1.35}, {2, 0, 0.9}, {2, -1.12, 0.9}, {1.12, -2,
    0.9}, {0, -2, 0.9}, {-2, 0, 0.9}, {2, 0, 0.45}, {2, -1.12,
    0.45}, {1.12, -2, 0.45}, {0, -2, 0.45}, {1.5, 0, 0.225},
    {1.5, -0.84, 0.225}, {0.84, -1.5, 0.225}, {0, -1.5, 0.225},
    {1.5, 0, 0.15}, {1.5, -0.84, 0.15}, {0.84, -1.5, 0.15}, {0,
    -1.5, 0.15}, {-1.6, 0, 2.025}, {-1.6, -0.3, 2.025}, {-1.5,
    -0.3, 2.25}, {-1.5, 0, 2.25}, {-2.3, 0, 2.025}, {-2.3, -0.3,
    2.025}, {-2.5, -0.3, 2.25}, {-2.5, 0, 2.25}, {-2.7, 0,
    2.025}, {-2.7, -0.3, 2.025}, {-3, -0.3, 2.25}, {-3, 0,
    2.25}, {-2.7, 0, 1.8}, {-2.7, -0.3, 1.8}, {-3, -0.3, 1.8},
    {-3, 0, 1.8}, {-2.7, 0, 1.575}, {-2.7, -0.3, 1.575}, {-3,
    -0.3, 1.35}, {-3, 0, 1.35}, {-2.5, 0, 1.125}, {-2.5, -0.3,
    1.125}, {-2.65, -0.3, 0.9375}, {-2.65, 0, 0.9375}, {-2,
    -0.3, 0.9}, {-1.9, -0.3, 0.6}, {-1.9, 0, 0.6}, {1.7, 0,
    1.425}, {1.7, -0.66, 1.425}, {1.7, -0.66, 0.6}, {1.7, 0,
    0.6}, {2.6, 0, 1.425}, {2.6, -0.66, 1.425}, {3.1, -0.66,
    0.825}, {3.1, 0, 0.825}, {2.3, 0, 2.1}, {2.3, -0.25, 2.1},
    {2.4, -0.25, 2.025}, {2.4, 0, 2.025}, {2.7, 0, 2.4}, {2.7,
    -0.25, 2.4}, {3.3, -0.25, 2.4}, {3.3, 0, 2.4}, {2.8, 0,
    2.475}, {2.8, -0.25, 2.475}, {3.525, -0.25, 2.49375},
    {3.525, 0, 2.49375}, {2.9, 0, 2.475}, {2.9, -0.15, 2.475},
    {3.45, -0.15, 2.5125}, {3.45, 0, 2.5125}, {2.8, 0, 2.4},
    {2.8, -0.15, 2.4}, {3.2, -0.15, 2.4}, {3.2, 0, 2.4}, {0, 0,
    3.15}, {0.8, 0, 3.15}, {0.8, -0.45, 3.15}, {0.45, -0.8,
    3.15}, {0, -0.8, 3.15}, {0, 0, 2.85}, {1.4, 0, 2.4}, {1.4,
    -0.784, 2.4}, {0.784, -1.4, 2.4}, {0, -1.4, 2.4}, {0.4, 0,
    2.55}, {0.4, -0.224, 2.55}, {0.224, -0.4, 2.55}, {0, -0.4,
    2.55}, {1.3, 0, 2.55}, {1.3, -0.728, 2.55}, {0.728, -1.3,
    2.55}, {0, -1.3, 2.55}, {1.3, 0, 2.4}, {1.3, -0.728, 2.4},
    {0.728, -1.3, 2.4}, {0, -1.3, 2.4}, {0, 0, 0}, {1.425,
    -0.798, 0}, {1.5, 0, 0.075}, {1.425, 0, 0}, {0.798, -1.425,
    0}, {0, -1.5, 0.075}, {0, -1.425, 0}, {1.5, -0.84, 0.075},
    {0.84, -1.5, 0.075}
};

static double tex[2][2][2] =
{
    { {0.0, 0.0}, {1.0, 0.0} },
    { {0.0, 1.0}, {1.0, 1.0} }
};

static void fghTeapot( GLint grid, GLdouble scale, GLenum type )
{
    double p[4][4][3], q[4][4][3], r[4][4][3], s[4][4][3];
    long i, j, k, l;

    glPushAttrib( GL_ENABLE_BIT | GL_EVAL_BIT );
    glEnable( GL_AUTO_NORMAL );
    glEnable( GL_NORMALIZE );
    glEnable( GL_MAP2_VERTEX_3 );
    glEnable( GL_MAP2_TEXTURE_COORD_2 );

    glPushMatrix();
    glRotated( 270.0, 1.0, 0.0, 0.0 );
    glScaled( 0.5 * scale, 0.5 * scale, 0.5 * scale );
    glTranslated( 0.0, 0.0, -1.5 );

    for (i = 0; i < 10; i++) {
        for (j = 0; j < 4; j++) {
            for (k = 0; k < 4; k++) {
                for (l = 0; l < 3; l++) {
                    p[j][k][l] = cpdata[patchdata[i][j * 4 + k]][l];
                    q[j][k][l] = cpdata[patchdata[i][j * 4 + (3 - k)]][l];
                    if (l == 1)
                    q[j][k][l] *= -1.0;
                    if (i < 6) {
                        r[j][k][l] =
                        cpdata[patchdata[i][j * 4 + (3 - k)]][l];
                        if (l == 0)
                        r[j][k][l] *= -1.0;
                        s[j][k][l] = cpdata[patchdata[i][j * 4 + k]][l];
                        if (l == 0)
                        s[j][k][l] *= -1.0;
                        if (l == 1)
                        s[j][k][l] *= -1.0;
                    }
                }
            }
        }

        glMap2d(GL_MAP2_TEXTURE_COORD_2, 0.0, 1.0, 2, 2, 0.0, 1.0, 4, 2,
        &tex[0][0][0]);
        glMap2d(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 4, 0.0, 1.0, 12, 4,
        &p[0][0][0]);
        glMapGrid2d(grid, 0.0, 1.0, grid, 0.0, 1.0);
        glEvalMesh2(type, 0, grid, 0, grid);
        glMap2d(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 4, 0.0, 1.0, 12, 4,
        &q[0][0][0]);
        glEvalMesh2(type, 0, grid, 0, grid);
        if (i < 6) {
            glMap2d(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 4, 0.0, 1.0, 12, 4,
            &r[0][0][0]);
            glEvalMesh2(type, 0, grid, 0, grid);
            glMap2d(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 4, 0.0, 1.0, 12, 4,
            &s[0][0][0]);
            glEvalMesh2(type, 0, grid, 0, grid);
        }
    }

    glPopMatrix();
    glPopAttrib();
}


/* -- INTERFACE FUNCTIONS -------------------------------------------------- */

/*
* Renders a beautiful wired teapot...
*/
void glutWireTeapot( GLdouble size )
{
    //FREEGLUT_EXIT_IF_NOT_INITIALISED ( "glutWireTeapot" );
    /* We will use the general teapot rendering code */
    fghTeapot( 10, size, GL_LINE );
}

/*
* Renders a beautiful filled teapot...
*/
void glutSolidTeapot( GLdouble size )
{
    //FREEGLUT_EXIT_IF_NOT_INITIALISED ( "glutSolidTeapot" );
    /* We will use the general teapot rendering code */
    fghTeapot( 7, size, GL_FILL );
}


/*
* Move object into position.  Use 3rd through 12th
* parameters to specify the material property.  Draw a teapot.
*/
void renderTeapot(GLfloat x, GLfloat y,
GLfloat ambr, GLfloat ambg, GLfloat ambb,
GLfloat difr, GLfloat difg, GLfloat difb,
GLfloat specr, GLfloat specg, GLfloat specb, GLfloat shine)
{
    GLfloat mat[4];

    glPushMatrix();
    glTranslatef(x, y, 0.0);
    mat[0] = ambr; mat[1] = ambg; mat[2] = ambb; mat[3] = 1.0;
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat);
    mat[0] = difr; mat[1] = difg; mat[2] = difb;
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat);
    mat[0] = specr; mat[1] = specg; mat[2] = specb;
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat);
    glMaterialf(GL_FRONT, GL_SHININESS, shine * 128.0);
    glCallList(teapotList);
    glPopMatrix();
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
        if (w <= h)
        glOrtho(0.0, 16.0, 0.0, 16.0*(GLfloat)h/(GLfloat)w,
        -10.0, 10.0);
        else
        glOrtho(0.0, 16.0*(GLfloat)w/(GLfloat)h, 0.0, 16.0,
        -10.0, 10.0);
        glMatrixMode(GL_MODELVIEW);

        GLfloat ambient[] = {0.0, 0.0, 0.0, 1.0};
        GLfloat diffuse[] = {1.0, 1.0, 1.0, 1.0};
        GLfloat specular[] = {1.0, 1.0, 1.0, 1.0};
        GLfloat position[] = {0.0, 3.0, 3.0, 0.0};

        GLfloat lmodel_ambient[] = {0.2, 0.2, 0.2, 1.0};
        GLfloat local_view[] = {0.0};

        glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
        glLightfv(GL_LIGHT0, GL_POSITION, position);
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
        glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);

        glFrontFace(GL_CW);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_AUTO_NORMAL);
        glEnable(GL_NORMALIZE);
        glEnable(GL_DEPTH_TEST);
        /*  be efficient--make teapot display list  */
        teapotList = glGenLists(1);
        glNewList (teapotList, GL_COMPILE);
        glutSolidTeapot(1.0);
        glEndList ();

        return TRUE;
    }

    virtual BOOL animate()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderTeapot(2.0, 17.0, 0.0215, 0.1745, 0.0215,
        0.07568, 0.61424, 0.07568, 0.633, 0.727811, 0.633, 0.6);
        renderTeapot(2.0, 14.0, 0.135, 0.2225, 0.1575,
        0.54, 0.89, 0.63, 0.316228, 0.316228, 0.316228, 0.1);
        renderTeapot(2.0, 11.0, 0.05375, 0.05, 0.06625,
        0.18275, 0.17, 0.22525, 0.332741, 0.328634, 0.346435, 0.3);
        renderTeapot(2.0, 8.0, 0.25, 0.20725, 0.20725,
        1, 0.829, 0.829, 0.296648, 0.296648, 0.296648, 0.088);
        renderTeapot(2.0, 5.0, 0.1745, 0.01175, 0.01175,
        0.61424, 0.04136, 0.04136, 0.727811, 0.626959, 0.626959, 0.6);
        renderTeapot(2.0, 2.0, 0.1, 0.18725, 0.1745,
        0.396, 0.74151, 0.69102, 0.297254, 0.30829, 0.306678, 0.1);
        renderTeapot(6.0, 17.0, 0.329412, 0.223529, 0.027451,
        0.780392, 0.568627, 0.113725, 0.992157, 0.941176, 0.807843,
        0.21794872);
        renderTeapot(6.0, 14.0, 0.2125, 0.1275, 0.054,
        0.714, 0.4284, 0.18144, 0.393548, 0.271906, 0.166721, 0.2);
        renderTeapot(6.0, 11.0, 0.25, 0.25, 0.25,
        0.4, 0.4, 0.4, 0.774597, 0.774597, 0.774597, 0.6);
        renderTeapot(6.0, 8.0, 0.19125, 0.0735, 0.0225,
        0.7038, 0.27048, 0.0828, 0.256777, 0.137622, 0.086014, 0.1);
        renderTeapot(6.0, 5.0, 0.24725, 0.1995, 0.0745,
        0.75164, 0.60648, 0.22648, 0.628281, 0.555802, 0.366065, 0.4);
        renderTeapot(6.0, 2.0, 0.19225, 0.19225, 0.19225,
        0.50754, 0.50754, 0.50754, 0.508273, 0.508273, 0.508273, 0.4);
        renderTeapot(10.0, 17.0, 0.0, 0.0, 0.0, 0.01, 0.01, 0.01,
        0.50, 0.50, 0.50, .25);
        renderTeapot(10.0, 14.0, 0.0, 0.1, 0.06, 0.0, 0.50980392, 0.50980392,
        0.50196078, 0.50196078, 0.50196078, .25);
        renderTeapot(10.0, 11.0, 0.0, 0.0, 0.0,
        0.1, 0.35, 0.1, 0.45, 0.55, 0.45, .25);
        renderTeapot(10.0, 8.0, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0,
        0.7, 0.6, 0.6, .25);
        renderTeapot(10.0, 5.0, 0.0, 0.0, 0.0, 0.55, 0.55, 0.55,
        0.70, 0.70, 0.70, .25);
        renderTeapot(10.0, 2.0, 0.0, 0.0, 0.0, 0.5, 0.5, 0.0,
        0.60, 0.60, 0.50, .25);
        renderTeapot(14.0, 17.0, 0.02, 0.02, 0.02, 0.01, 0.01, 0.01,
        0.4, 0.4, 0.4, .078125);
        renderTeapot(14.0, 14.0, 0.0, 0.05, 0.05, 0.4, 0.5, 0.5,
        0.04, 0.7, 0.7, .078125);
        renderTeapot(14.0, 11.0, 0.0, 0.05, 0.0, 0.4, 0.5, 0.4,
        0.04, 0.7, 0.04, .078125);
        renderTeapot(14.0, 8.0, 0.05, 0.0, 0.0, 0.5, 0.4, 0.4,
        0.7, 0.04, 0.04, .078125);
        renderTeapot(14.0, 5.0, 0.05, 0.05, 0.05, 0.5, 0.5, 0.5,
        0.7, 0.7, 0.7, .078125);
        renderTeapot(14.0, 2.0, 0.05, 0.05, 0.0, 0.5, 0.5, 0.4,
        0.7, 0.7, 0.04, .078125);
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


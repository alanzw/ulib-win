#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include <time.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include "ugldialog.h"
#include "udlgapp.h"
#include "uglut.h"

//#define M_PI 3.14159265f

#define sinf(x) (float)sin((x))
#define cosf(x) (float)cos((x))
#define XSIZE   100
#define YSIZE   75

#define RINGS 5
#define BLUERING 0
#define BLACKRING 1
#define REDRING 2
#define YELLOWRING 3
#define GREENRING 4

#define BACKGROUND 8

enum {
    BLACK = 0,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE
};

typedef short Point[2];

GLenum directRender;

unsigned char rgb_colors[RINGS][3];
int mapped_colors[RINGS];
float dests[RINGS][3];
float offsets[RINGS][3];
float angs[RINGS];
float rotAxis[RINGS][3];
int iters[RINGS];
GLuint theTorus;

void FillTorus(float rc, int numc, float rt, int numt)
{
    int i, j, k;
    float s, t;
    float x, y, z;
    float pi, twopi;

    pi = M_PI;
    twopi = 2 * pi;

    for (i = 0; i < numc; i++) {
        glBegin(GL_QUAD_STRIP);
        for (j = 0; j <= numt; j++) {
            for (k = 1; k >= 0; k--) {
                s = (float)((i + k) % numc + 0.5f);
                t = (float)(j % numt);

                x = cosf(t * twopi / numt) * cosf(s * twopi / numc);
                y = sinf(t * twopi / numt) * cosf(s * twopi / numc);
                z = sinf(s * twopi / numc);
                glNormal3f(x, y, z);

                x = (rt + rc * cosf(s * twopi / numc)) * cosf(t * twopi / numt);
                y = (rt + rc * cosf(s * twopi / numc)) * sinf(t * twopi / numt);
                z = rc * sinf(s * twopi / numc);
                glVertex3f(x, y, z);
            }
        }
        glEnd();
    }
}

float Clamp(int iters_left, float t)
{
    if (iters_left < 3) {
        return 0.0;
    }
    return (iters_left - 2) * t / iters_left;
}

float MyRand(void)
{
    return 10.0f * ( ((float) rand())/((float) RAND_MAX) - 0.5f);
}

void ReInit(void)
{
    int i;
    float deviation;

    deviation = MyRand() / 2.0f;
    deviation = deviation * deviation;
    for (i = 0; i < RINGS; i++) {
        offsets[i][0] = MyRand();
        offsets[i][1] = MyRand();
        offsets[i][2] = MyRand();
        angs[i] = 260.0f * MyRand();
        rotAxis[i][0] = MyRand();
        rotAxis[i][1] = MyRand();
        rotAxis[i][2] = MyRand();
        iters[i] = (int)(deviation * MyRand() + 60);
    }
}

using huys::UGLDialog;

class MyGLDialog : public UGLDialog
{
    GLuint listName;
public:
    MyGLDialog(HINSTANCE hInst, UINT nID)
    : UGLDialog(hInst, nID)
    {
    }

    BOOL initGL()
    {
        RECT rc;
        ::GetClientRect(m_hDlg, &rc);
        //UGlut::ResizeScene(rc.right-rc.left, rc.bottom-rc.top);

        int w = rc.right-rc.left;
        int h = rc.bottom-rc.top;

        glViewport(0, 0, w, h);

        int i;
        float top_y = 1.0f;
        float bottom_y = 0.0f;
        float top_z = 0.15f;
        float bottom_z = 0.69f;
        float spacing = 2.5f;
        static float lmodel_ambient[] = {0.0f, 0.0f, 0.0f, 0.0f};
        static float lmodel_twoside[] = {GL_FALSE};
        static float lmodel_local[] = {GL_FALSE};
        static float light0_ambient[] = {0.1f, 0.1f, 0.1f, 1.0f};
        static float light0_diffuse[] = {1.0f, 1.0f, 1.0f, 0.0f};
        static float light0_position[] = {0.8660254f, 0.5f, 1.0f, 0.0f};
        static float light0_specular[] = {1.0f, 1.0f, 1.0f, 0.0f};
        static float bevel_mat_ambient[] = {0.0f, 0.0f, 0.0f, 1.0f};
        static float bevel_mat_shininess[] = {40.0f};
        static float bevel_mat_specular[] = {1.0f, 1.0f, 1.0f, 0.0f};
        static float bevel_mat_diffuse[] = {1.0f, 0.0f, 0.0f, 0.0f};

        srand(time(NULL));
        ReInit();
        for (i = 0; i < RINGS; i++) {
            rgb_colors[i][0] = rgb_colors[i][1] = rgb_colors[i][2] = 0;
        }
        rgb_colors[BLUERING][2] = 255;
        rgb_colors[REDRING][0] = 255;
        rgb_colors[GREENRING][1] = 255;
        rgb_colors[YELLOWRING][0] = 255;
        rgb_colors[YELLOWRING][1] = 255;
        mapped_colors[BLUERING] = BLUE;
        mapped_colors[REDRING] = RED;
        mapped_colors[GREENRING] = GREEN;
        mapped_colors[YELLOWRING] = YELLOW;
        mapped_colors[BLACKRING] = BLACK;

        dests[BLUERING][0] = -spacing;
        dests[BLUERING][1] = top_y;
        dests[BLUERING][2] = top_z;

        dests[BLACKRING][0] = 0.0;
        dests[BLACKRING][1] = top_y;
        dests[BLACKRING][2] = top_z;

        dests[REDRING][0] = spacing;
        dests[REDRING][1] = top_y;
        dests[REDRING][2] = top_z;

        dests[YELLOWRING][0] = -spacing / 2.0f;
        dests[YELLOWRING][1] = bottom_y;
        dests[YELLOWRING][2] = bottom_z;

        dests[GREENRING][0] = spacing / 2.0f;
        dests[GREENRING][1] = bottom_y;
        dests[GREENRING][2] = bottom_z;

        theTorus = glGenLists(1);
        glNewList(theTorus, GL_COMPILE);
        FillTorus(0.1f, 8, 1.0f, 25);
        glEndList();

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glEnable(GL_DEPTH_TEST);
        glClearDepth(1.0);

        glClearColor(0.5, 0.5, 0.5, 0.0);
        glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
        glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
        glEnable(GL_LIGHT0);

        glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, lmodel_local);
        glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, lmodel_twoside);
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
        glEnable(GL_LIGHTING);

        glMaterialfv(GL_FRONT, GL_AMBIENT, bevel_mat_ambient);
        glMaterialfv(GL_FRONT, GL_SHININESS, bevel_mat_shininess);
        glMaterialfv(GL_FRONT, GL_SPECULAR, bevel_mat_specular);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, bevel_mat_diffuse);

        glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
        glEnable(GL_COLOR_MATERIAL);
        glShadeModel(GL_SMOOTH);

        glMatrixMode(GL_PROJECTION);
        gluPerspective(45, 1.33, 0.1, 100.0);
        glMatrixMode(GL_MODELVIEW);

        return TRUE;
    }

    void CALLBACK DrawScene(void)
    {
        int i;

        glPushMatrix();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);

        for (i = 0; i < RINGS; i++) {
            glColor3ubv(rgb_colors[i]);
            glPushMatrix();
            glTranslatef(dests[i][0] + offsets[i][0], 
            dests[i][1] + offsets[i][1],
            dests[i][2] + offsets[i][2]);
            glRotatef(angs[i], rotAxis[i][0], rotAxis[i][1], rotAxis[i][2]);
            glCallList(theTorus);
            glPopMatrix();
        }
        glPopMatrix();
    }
    
    
    virtual BOOL animate()
    {
        int i, j;
        int more = GL_FALSE;
        for (i = 0; i < RINGS; i++) {
            if (iters[i]) {
                for (j = 0; j < 3; j++) {
                    offsets[i][j] = Clamp(iters[i], offsets[i][j]);
                }
                angs[i] = Clamp(iters[i], angs[i]);
                iters[i]--;
                more = GL_TRUE;
            }
        }
        if(more) {
            DrawScene();
        } else {
            ;
        }

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


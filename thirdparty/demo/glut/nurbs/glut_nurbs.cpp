/* Working with Nurbs:  

An example by D. W. Hyatt for Supercomputer Applications.   
The code presented here is an adaptation of an undocumented example on
nurbs called "molehill.c".  It was found as an example on the OpenGL site,
but the actual author was not identified.

This program uses a 3-D structure in OpenGL called a Nurb.  A nurb
is a continuous surface that is controlled by a few distinct points.
In this example, a three dimensional grtid of 16 points is assigned
some simple 3-D positions in space, but then several points are then
skewed.  The first stretch moved the middle four points up to a height
of 6, and then several of the corner points are moved as well.  The
result is a curved surface that contains all of those points.

*/

#include <GL/glut.h>

GLfloat mat_color_diffuse[] = { 0.8, 0.6, 1.0, 1.0 };
GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_shininess[] = { 100.0 };
GLfloat knots[8] = { 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0 };

GLfloat nurbarray[4][4][3];

GLUnurbsObj *nurb;

int row, col;
static double spin = 330.0;


static void display(void)
{
    glClearColor(0.2, 0.0, 0.4, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glCallList(1);
    glFlush();
}

void initialize()
{
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_NORMALIZE);
    nurb = gluNewNurbsRenderer();
    gluNurbsProperty(nurb, GLU_SAMPLING_TOLERANCE, 25.0);
    gluNurbsProperty(nurb, GLU_DISPLAY_MODE, GLU_FILL);

}

void createSurface()
{
    // Build control points for NURBS surface 
    for(row=0; row<4; row++) {
        for(col=0; col<4; col++) {
            // Initialize Nurb Array with a "bump" in the middle 
            nurbarray[row][col][0] = 2.0*((GLfloat)row);
            nurbarray[row][col][1] = 2.0*((GLfloat)col);
            nurbarray[row][col][2] = 0.0;

            // Stretch up middle. 
            if((row==1 || row == 2) && (col == 1 || col == 2))
            nurbarray[row][col][2] = 6.0;

        }
    }

    // Stretch far right corner in z direction. 
    nurbarray[3][3][2] = 5.0;

    // Stretch near corner in x direction. 
    nurbarray[0][0][0] = -3.0;

    // Stretch far right corner in y direction. 
    nurbarray[3][0][1] = -2.0;

}


void createView()
{
    glMatrixMode(GL_PROJECTION);
    gluPerspective(55.0, 1.0, 2.0, 24.0);
    glMatrixMode(GL_MODELVIEW);
    // Move away from image an rotate in x-direction to give nice view.
    glTranslatef(-2.0, -1.0, -15.0);
    glRotatef(spin, 1.0, 0.0, 0.0);
}

void renderSurface()
{
    glNewList(1, GL_COMPILE);
    /* Render Nurb Surface */
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_color_diffuse);
    gluBeginSurface(nurb);
    gluNurbsSurface(nurb, 8, knots, 8, knots,
    4 * 3, 3, &nurbarray[0][0][0],
    4, 4, GL_MAP2_VERTEX_3);
    gluEndSurface(nurb);

    glEndList();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutCreateWindow("Nurb Test");
    initialize();
    createSurface();
    createView();
    renderSurface();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;             /* ANSI C requires main to return int. */
}


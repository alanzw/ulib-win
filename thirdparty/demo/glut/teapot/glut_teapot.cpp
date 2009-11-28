/*  light2.c

    This program creates an animation of a green teapot, and uses the
    function glutIdleFunc() to choose between two different modes in
    the animation, one that causes the display to spin and one that
    does nothing (NULL)
    */

#include<GL/glut.h>
#include <stdlib.h>

static GLfloat spin = 0.0;
GLint toggle = 0;

static GLfloat light0_position[] = {2.0, 8.0, 2.0, 0.0};
GLfloat mat_specular[]         = {1.0, 1.0, 1.0, 1.0};
GLfloat mat_shininess[]        = {50.0};
GLfloat mat_amb_diff_color[]     = {0.5, 0.7, 0.5, 0.5};
GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
GLfloat light_ambient[] = {0.15, 0.15, 0.15};
GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};

void init () {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

void display (void)
{ glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_amb_diff_color);
    glLoadIdentity();
    gluLookAt(2.0, 4.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glPushMatrix();
    glScalef(2.0, 2.0, 2.0);
    glRotatef(spin, 0.0, 1.0, 0.0);
    glutSolidTeapot(1.0);
    glPopMatrix();
    glutSwapBuffers();
}



void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();    /*  define the projection  */
    gluPerspective(45.0, (GLfloat) w / (GLfloat) h, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void spinDisplay()
{ spin += 5.0;
    if (spin >360.0)
        spin = spin - 360.0;
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
    switch (button) {
    case GLUT_LEFT_BUTTON:
        if (state == GLUT_DOWN)
            glutIdleFunc(spinDisplay);
        break;
    case GLUT_MIDDLE_BUTTON:
    case GLUT_RIGHT_BUTTON:
        if (state == GLUT_DOWN)
            glutIdleFunc(NULL);
        break;
    default:
        break;
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition (0, 0);
    glutInitWindowSize(400, 400);
    glutCreateWindow ("Scaling Transformation");
    init ();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}

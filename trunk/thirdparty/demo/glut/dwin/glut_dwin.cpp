#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <GL/glut.h>

GLfloat light_diffuse[] = {2.0, 2.0, 1.0, 1.0};
GLfloat light_position[] = {5.0, 5.0, 5.0, 0.0};
GLUquadricObj *qobj;

// Window variables
int win1, win2;

// Display List
int list = 1;

void display(void) // Display routine for both Window 1 and 2
{
    glClearColor(0.4, 0.4, 0.6, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (glutGetWindow() == win1)
    { // Displaying Window 1
        glColor3f(1.0, 0.0, 0.0);
        glPushMatrix();
        glRotatef(60.0, 0.0, 1.0, 0.0);
        glutWireCube(3.0);
        glPopMatrix();
        glCallList(list);   /* render sphere display list for object 1 */
    }
    else
    { // Displaying Window 2
        glCallList(1);      /* render sphere display list for object 1 */
        glPushMatrix();
        glTranslatef(0.6, 0.6, 0.8);
        glCallList(2);    /* render sphere display list for object 2 */
        glPopMatrix();
    }
    glutSwapBuffers();
}

void display_win1(void)  // Initial Window 1 display calls the other routine
{
    glPushMatrix();
    glTranslatef(0.0, 0.0, -3.0);
    display();
    glPopMatrix();
}

// This function can tell which window is being left or entered
void which_window(int state)
{
    printf("%s  Window %d \n",
        state == GLUT_LEFT ? "Leaving" : "Entering",
        glutGetWindow());
}


// Simple Mouse Function
void mouse(int button, int state, int x, int y)
{
    printf("button: %d %s %d,%d\n", button, state == GLUT_UP ? "UP" : "down", x, y);
}

void init(void)
{
    // Define first Quadric Object, a solid sphere as a call list.
    gluQuadricDrawStyle(qobj, GLU_FILL);
    glNewList(1, GL_COMPILE);  /* create sphere display list for Object 1*/
    gluSphere(qobj,  1.0,  20,  20);
    glEndList();

    // Create a second Quadric Object, a wireframe sphere
    gluQuadricDrawStyle(qobj, GLU_LINE);
    glNewList(2, GL_COMPILE);  /* create sphere display list for Object 2*/
    gluSphere(qobj,  0.5,  10,  10);
    glEndList();

    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    gluPerspective( 40.0, 1.0, 1.0,  20.0);
    glMatrixMode(GL_MODELVIEW);
    gluLookAt(0.0, 0.0, 5.0,  0.0, 0.0, 0.0,  0.0, 1.0, 0.);
    glTranslatef(0.0, 0.0, -1.0);
}

int main(int argc, char **argv)
{
    qobj = gluNewQuadric();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    // Create First Window
    win1 = glutCreateWindow("First Window");
    glutEntryFunc(which_window);
    glutMouseFunc(mouse);
    init();
    glutDisplayFunc(display_win1);

    // Create Second Window
    win2 = glutCreateWindow("Second Window");
    glutEntryFunc(which_window);
    glutMouseFunc(mouse);
    init();
    glutDisplayFunc(display);

    glutMainLoop();
    return 0;             /* ANSI C requires main to return int. */
}


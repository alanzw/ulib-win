/* Animated Parabola and Squares */

// Header files
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

// Global variables for rotation factors
static float spin = 0.0;
static float spin2 = 0.0;

// This Section creates a Window, but uses only GL commands rather than X.
static void init( void )
{
    /* one-time init (clearColor, set palette, etc) */
    glClearColor(0.0, 0.0, 0.0, 1.0 );
    glShadeModel( GL_FLAT );
}

static void reshape( int width, int height )  // Essential Magic again!
{
    glViewport(0, 0, (GLint)width, (GLint)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glLoadIdentity();
    glOrtho( -3.0, 3.0, -3.0, 3.0, -3.0, 3.0 );
    glMatrixMode(GL_MODELVIEW);
}


static void key_up()  //  A Function accessing the UP ARROW Key
{
    printf("KEY_UP\n");;
    spin -= 2.0;
    if (spin <= 0.0) spin = 360.0;
    spin2 -= 15.0;
    if (spin2 <= 0.0) spin2 += 360.0;
}


static void key_down()  // A Function accessing the DOWN ARROW Key
{
    printf("KEY_DOWN\n");
    spin += 2.0;
    if (spin >= 360.0) spin = 0;
    spin2 += 15.0;
    if (spin2 >= 360.0) spin2 -= 360.00;
}


static void key_esc()  // A Function Accessing the ESCAPE key
{
    exit(0);          // If pressed, the window QUITS
}

void key_pressed(unsigned char key, int x, int y)
{
    switch(key)
    {
    case GLUT_KEY_UP : 
        key_up();
        break;
    case GLUT_KEY_DOWN : 
        key_down();
        break;
    case 27 : 
        key_esc();
        break;
    }

}

void babyRectangle()   // Draw rotating red rectangle, offset to edge
{
    glColor3f(0.8, 0.0, 0.0);
    glPushMatrix();
    glTranslatef(2.0, 2.0, 0.0);
    glRotatef(spin2, 0.0, 0.0, 1.0);
    glRectf(-0.2, -0.2, 0.2, 0.2);
    //glRectf(1.8, 1.8, 2.2, 2.2);
    glPopMatrix();
}

void myRectangle()  // Draw a large Yellow Rectangle and rotate image
{
    glColor3f(1.0, 1.0, 0.0);
    glPushMatrix();
    glRotatef(spin, 0.0, 0.0, 1.0);
    glRectf(-2.0, -2.0, 2.0, 2.0);
    glPopMatrix();
}

void myCube()    // Draw wire frame green cube: Roatate, Translate, Scale
{
    static float myscale= 1.0;
    static float step = -0.1;
    glColor3f(0.0, 0.7, 0.0);
    glPushMatrix();
    glRotatef( 10.0, 1.0, 0.0, 1.0);
    glRotatef(spin, 0.0, 1.0, 0.0);
    glTranslatef(0.0, 0.0, -1.0);
    glScalef(myscale, myscale, myscale);
    myscale += step;
    if (myscale <= 0.5) step = 0.05;  // If too small, start growing
    if (myscale >= 1.5) step = -0.05; // If too big, start shrinking
    glBegin(GL_LINE_STRIP);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(1.0, 0.0, 0.0);
    glVertex3f(1.0, 1.0, 0.0);
    glVertex3f(0.0, 1.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex3f(0.0, 0.0, 1.0);
    glVertex3f(1.0, 0.0, 1.0);
    glVertex3f(1.0, 1.0, 1.0);
    glVertex3f(0.0, 1.0, 1.0);
    glVertex3f(0.0, 0.0, 1.0);
    glEnd();
    glBegin(GL_LINES);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 1.0);
    glVertex3f(1.0, 1.0, 0.0);
    glVertex3f(1.0, 1.0, 1.0);
    glVertex3f(0.0, 1.0, 0.0);
    glVertex3f(0.0, 1.0, 1.0);
    glVertex3f(1.0, 0.0, 0.0);
    glVertex3f(1.0, 0.0, 1.0);
    glEnd();

    glPopMatrix();
}

void DrawMyStuff()  // The Drawing Routine
{
    float i, j;
    double x,y, xstep;
    myRectangle();
    babyRectangle();
    myRectangle();
    babyRectangle();
    glColor3f(0.5, 0.5, 1.0);
    glPushMatrix();
    glRotatef(spin, 1.0, 1.0,0.0);
    glBegin( GL_POINTS );
    for (x = -3; x < 3; x += .001) // Step from -1 to +1 inefficiently
    {
        y = 1 - 2 * x * x;         // Calculate y value of parabola
        glVertex3f(x, y, 0.0);
    }


    glEnd();
    glPopMatrix();

    myCube();
}


static void display( )  // Main display routine called from event loop
{
    int i;

    init();  // Call Init routine

    /* clear viewport */
    glClear( GL_COLOR_BUFFER_BIT );


    /* draw stuff */
    DrawMyStuff();


    /* flush / swap buffers */
    glFlush();
    glutSwapBuffers();
}



int main( int argc, char **argv )
{
    glutInit(&argc,argv);
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB );  // Initialize Display in RGB mode
    glutInitWindowPosition(50, 50);
    glutInitWindowSize( 400, 400);
    glutCreateWindow("My Window");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(key_pressed);
    glutMainLoop();
    return 0;
}

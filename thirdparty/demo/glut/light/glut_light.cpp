/*  Light1.c
    This programs draws a simple Solar system that animates constantly
*/
#include <math.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

static void init()
{                                 // Initialization routine
    GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};     // Define highlight properties
    GLfloat mat_shininess[]= {50.0};                   // Define shininess of surface
    GLfloat light_position[] = {10.0, 40.0, 1.0, 0.0}; // Define light source position
    glClearColor(0.3, 0.3, 0.3, 0.0);                  // Set background to gray
    glShadeModel(GL_SMOOTH);                           // Smooth transitions between edges
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);   // Set material properties
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess); // Set material properties
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);   // Set light properties
    glColorMaterial(GL_FRONT,GL_DIFFUSE);                // Set Color Capability

    glEnable(GL_COLOR_MATERIAL);         // Enable color
    glEnable(GL_LIGHTING);               // Enable lighting for surfaces
    glEnable(GL_LIGHT0);                 // Enable light source
    glEnable(GL_DEPTH_TEST);             // Enable depth buffering
}

static void reshape(int width, int height)
{
    //  Determines Projection type
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)width / (GLfloat)height, 1.00, 30.0);

    // Perspective Parms( Angle, Aspect Ratio, Front Clipping, Back Clipping)

    gluLookAt(0.0, 2.5, 3.0, 0.0, -0.5, -1.0, 0.0, 1.0, 0.0);
    // LookAt Parms (Eye_X, Eye_Y, Eye_Z, View_X, View_Y, View_Z, Up_X, Up_Y, UpZ)

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glLoadIdentity();
}

static void display()
{
    static double x = 0.0; // This value will be remembered each time display is called
    x+=5; // Increment 5 degrees each call
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );  // Clear Color and Depth
    glPushMatrix();                      // Begin drawing image
    glRotatef( x,0.0,1.0,0.0);      // Rotate
    glColor3f(1.0,0.0, 0.0);        // Set Drawing Color to Red
    glutSolidSphere(0.7,30,20);        // Draw Sphere(Radius, Slices, Stacks)
    glPushMatrix();              // Draw orbiting planet
    glTranslatef(0.5,0.0,-1.0);      // Move away from origin
    glColor3f(1.0,1.0,0.0);             // Set color to yellow
    glutSolidSphere(0.3,20,16);      // Draw Sphere
    glPushMatrix();                 // Draw Moon
    glRotatef( x, 0.0, 1.0, 0.0);         // Rotate
    glTranslatef(0.3,0.0,-0.5);         // Move reference point
    glColor3f(0.0,0.0,1.0);             // Color Blue
    glutSolidSphere(0.2,20,16);         // Draw Sphere
    glPopMatrix();              // Return to former reference
    glPopMatrix();            // Return to first refernce
    glPushMatrix();              // Draw orbiting planet
    glTranslatef(-0.5,0.0,1.0);      // Move reference poit
    glColor3f(0.0,1.0,0.0);          // Set color to green
    glutSolidSphere(0.3,20,16);      // Draw sphere
    glPopMatrix();            // Return to first reference
    glPopMatrix();              // Return to base reference
    glFlush();                  // Flush drawing routines
    glutSwapBuffers();              // Swap buffers
    glutPostRedisplay();              // Display results
}

int main(int argc, char** argv)
{    // Run program with possible parameters
    // "glut" Initialization Functions rather than "aux" versions
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH ); // Initialize modes
    glutInitWindowSize( 550, 550);
    glutInitWindowPosition( 50, 50);
    glutCreateWindow( argv[0]); // Note that arg[0] is name of program (Window name)
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}

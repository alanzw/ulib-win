// This Program introduces the ability to draw multiple sub windows.
// It uses the command glOrtho() for orthographic projection and scaling.
// as well as keyboard and mouse routines.

#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <GL/glut.h>

// Integers to identify the three windows
int main_window, sub_window1, sub_window2;

// These are evil global variables that determine the main window size
int width=830;  // Two 400 pixel windows and three 10-pixel borders
int height=420; // Top and bottom borders of 10 pixels each.

// This string will be used to display which window is being viewed.
char s[10] ="Window n";

// These values will be used to zoom in and out of the second window.
// Initial values for the zoom are the same as the fixed window.
float orthoX1= 1.0, orthoY1=1.0;

// This is a function that will do something when a key is pressed in Window 1
void key_pressed1(unsigned char key, int x, int y)
{
    if (isprint(key))
    {
        printf("First Subwindow   key: `%c' %d,%d\n", key, x, y);
    }
    else
    {
        printf("First Subwindow   key: 0x%x %d,%d\n", key, x, y);
    }
}

// This is a function that will do something when a key is pressed in Window 2
void key_pressed2(unsigned char key, int x, int y)
{
    if (isprint(key))
    {
        printf("Second Subwindow   key: `%c' %d,%d\n", key, x, y);

    }
    else
    {
        printf("Second Subwindow   key: 0x%x %d,%d\n", key, x, y);
    }
}


// This routine tells what to do when the mouse is used in Window 1
// It will zoom into Window 2 making the image larger.

void mouse1(int button, int state, int x, int y)
{
    printf("WINDOW 1:   button: %d %s %d,%d\n", button, state == GLUT_UP ? "UP" : "down", x, y);
    if (state == GLUT_DOWN)
    {
        orthoX1 -= 0.2;
        orthoY1 -= 0.2;

        glutSetWindow(sub_window2);               // Reset Window #2
        glutPositionWindow( (width-30)/2+20,10);     // Figure out top corner
        glutReshapeWindow( (width-30)/2, height-20); // Figure out size

    }
    glutPostRedisplay();   //  Cause the image to be redisplayed

}


// This routine tells what to do when the mouse is used in Window 2
// It will zoom out of Window 2 making the image smaller.

void mouse2(int button, int state, int x, int y)
{
    printf("WINDOW 2:   button: %d %s %d,%d\n", button, state == GLUT_UP ? "UP" : "down", x, y);
    if (state == GLUT_DOWN)
    {
        orthoX1 += 0.2;
        orthoY1 += 0.2;

        glutSetWindow(sub_window2);               // Reset Window #2
        glutPositionWindow( (width-30)/2+20,10);     // Figure out top corner
        glutReshapeWindow( (width-30)/2, height-20); // Figure out size

    }
    glutPostRedisplay();    // Force redisplay
}



// The following code permits drawing text on the screen
// Bitmap character fonts must be pre-defined

void bitmap_output(int x, int y, int z, char *string, void *font)
{
    int len, i;

    glRasterPos3f(x, y, 0);
    len = (int) strlen(string);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, string[i]);
    }
}


// Draws a graph of a simple line.

void DrawGraph()
{
    float x;
    glBegin(GL_POINTS);              // Start drawing points
    glColor3f(0.0,0.0,0.0);        // Set drawing color to black
    for (x=-0.9; x<0.9; x+=0.01)     // Step through x values
    {
        glVertex3f(x,x,0.0);        // Put a point at (x,x)
    }
    glEnd();
}


// Draws a set of labeled axes

void DrawAxes()
{
    glBegin(GL_LINES);
    glColor3f(1.0, 1.0, 0.0);
    glVertex3f(-0.9,0.0, 0.0);   // Draw x-axis
    glVertex3f(0.9, 0.0, 0.0);
    glVertex3f(0.0, -0.9,0.0);   // Draw y-axis
    glVertex3f(0.0, 0.9, 0.0);
    glEnd();

    glPushMatrix();         // Label y-axis
    glTranslatef(0.05, 0.85, 0);
    bitmap_output(0,0,0, "y", GLUT_BITMAP_TIMES_ROMAN_24);
    glPopMatrix();

    glPushMatrix();         // Label x-axis
    glTranslatef(0.8, -0.1, 0);
    bitmap_output(0,0,0, "x", GLUT_BITMAP_TIMES_ROMAN_24);
    glPopMatrix();

}


//Draws some titles on the screen in available Times Roman fonts

void DrawTitles( )
{
    glColor3f(0.0, 0.0, 0.0);
    glPushMatrix();
    glTranslatef(-0.9, 0.8, 0);
    bitmap_output(0,0,0, s, GLUT_BITMAP_TIMES_ROMAN_24);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.5, -0.95, 0);
    bitmap_output(0,0,0, "The above graph is for the linear equation   y = x",
        GLUT_BITMAP_TIMES_ROMAN_10);
    glPopMatrix();
}


//  Left Display Window is RED.

void display1(void)
{
    glClearColor(1.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glOrtho( -1.0, 1.0, -1.0, 1.0, -1.0, 1.0 ); // Default scaling (-1..+1)
    sprintf(s,"Window 1");
    DrawAxes();
    DrawGraph();
    DrawTitles();
    glFlush();          //  Force writing of GL commands

}

//  Right Display Window is BLUE.

void display2(void)
{
    glClearColor(0.0, 0.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glOrtho( -orthoX1, orthoX1, -orthoY1, orthoY1, -1.0, 1.0 );
    sprintf(s,"Window 2");
    DrawAxes();
    DrawGraph();
    DrawTitles();
    glFlush();          //  Force writing of GL commands
}

// Main display is WHITE.

void display(void)
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
}


// This routine tells what to do if the window is resized.

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);  // This gets coordinates of the new window
    width = w;
    height = h;

    glutSetWindow(sub_window1);          // Stuff for window #1
    glutPositionWindow(10,10);
    glutReshapeWindow( (width-30)/2, height-20);

    glutSetWindow(sub_window2);          // Stuff for window #2
    glutPositionWindow( (width-30)/2 + 20 ,10);
    glutReshapeWindow( (width-30)/2, height-20);

}

int main(int argc, char **argv)
{
    // Initialization stuff
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowSize(width, height);

    // Make Main outer window
    main_window = glutCreateWindow("Two Active Windows ");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    // Create First subwindow

    sub_window1 = glutCreateSubWindow(main_window, 10, 10, 400 , 400);
    glutDisplayFunc(display1);        // Pointer to display function
    glutKeyboardFunc(key_pressed1);    // Pointer to keyboard function
    glutMouseFunc(mouse1);        // Pointer to mouse routine

    // Create second subwindow

    sub_window2 = glutCreateSubWindow(main_window, 420, 10, 400, 400);
    glutDisplayFunc(display2);        // Pointer do other display function
    glutKeyboardFunc(key_pressed2);    // Pointer to other keyboard function
    glutMouseFunc(mouse2);        // Pointer to other mouse routine

    // Enter Glut Main Loop and wait for events
    glutMainLoop();
    return 0;             /* ANSI C requires main to return int. */
}

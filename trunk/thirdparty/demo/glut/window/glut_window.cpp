//  This program shows how to open a single window and plot bitmapped text
//  on the screen in Times Roman font.
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <GL/glut.h>

#define initial_width 400             // Initial Window Width = 400 pixels
#define initial_height 400            // Initial Window Height = 400 pixels

int main_window, sub_window;          // Outer Window and Inner Window ID numbers

// Draw text on the screen using pre-defined Bitmap character fonts
// Passed arguments include location in 3-space, pointer to string, pointer to font
void bitmap_output(int x, int y, int z, char *string, void *font)
{  int len, i;
    glRasterPos3f(x, y, 0);            // Locate Raster Position in 3-space
    len = (int) strlen(string);        // Find length of string
    for (i = 0; i < len; i++)          // Loop through plotting all characters in font style
    {        
        glutBitmapCharacter(font, string[i]);
    }
}

// Draw a graph of a simple line.
void DrawGraph()
{ float x;
    glBegin(GL_POINTS);                         // Start drawing points
    glColor3f(0.0,0.0,1.0);                   // Set drawing color to black
    for (x=-0.9; x<0.9; x+=0.01)              // Step through x values
    {
        glVertex3f(x,x,0.0);                    // Put a point at (x,x)
    }
    glEnd();
}

// Draw a set of labeled axes
void DrawAxes()
{
    glBegin(GL_LINES);                         // Start Drawing Lines
    glColor3f(1.0, 1.0, 0.0);                 // Set Drawing Color
    glVertex3f(-0.9,0.0, 0.0);                // Starting Point for x-axis
    glVertex3f(0.9, 0.0, 0.0);                // Ending Point
    glVertex3f(0.0, -0.9,0.0);                // Staring Point for y-axiz
    glVertex3f(0.0, 0.9, 0.0);                // Ending Point
    glEnd();

    // Now label the axes

    glPushMatrix();                             // New frame of reference
    glTranslatef(0.05, 0.85, 0);              // Move to new location
    bitmap_output(0,0,0, "y", GLUT_BITMAP_TIMES_ROMAN_24);
    glPopMatrix();                              // Return to old reference

    glPushMatrix();                             // New frame of reference
    glTranslatef(0.8, -0.1, 0);               // Move to new location
    bitmap_output(0,0,0, "x", GLUT_BITMAP_TIMES_ROMAN_24);
    glPopMatrix();                              // Return to old reference

}

//Draw some titles on the screen in available Times Roman fonts

void DrawTitles()
{
    glColor3f(0.0, 1.0, 0.0);                   // Set color to Black
    glPushMatrix();                             // New frame of reference
    glTranslatef(-0.9, 0.8, 0);                 // Move to new location
    bitmap_output(0,0,0, "Example #1", GLUT_BITMAP_TIMES_ROMAN_24);
    glPopMatrix();                              // Return to old reference

    glPushMatrix();                             // New frame of reference
    glTranslatef(-0.5, -0.95, 0);               // Move to new location
    bitmap_output(0,0,0, "The above graph is for the linear equation   y = x",
    GLUT_BITMAP_TIMES_ROMAN_10);
    glPopMatrix();                              // Reurn to old reference
}

// Display window

void display1(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    DrawAxes();
    DrawGraph();
    DrawTitles();
    glFlush();                                  // Force all drawing
}

void init(void)
{
    /* select clearing (background) color */
    glClearColor(0.0, 0.0, 0.0, 0.0);

    /* initialize viewing values */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
}


int main(int argc, char **argv)
{
    glutInit(&argc, argv);                      // Initialize with any passed arguments to main
    glutInitDisplayMode(GLUT_RGB);              // Initialize RGB mode in OpenGL
    glutInitWindowSize(initial_width, initial_height);  // Initialize window
    main_window = glutCreateWindow("My First Window ");  // Create main window with title
    init();                      // My initialization
    glutDisplayFunc(display1);                  // Pass pointer to inner window display
    glClearColor(0.0, 0.0, 0.0, 1.0);           // Set drawing color to red
    glutMainLoop();                             // Enter main loop - wait for interrupts
    return 0;             /* ANSI C requires main to return int. */
}

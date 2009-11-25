// Program to demonstrate how to open a window size and position it properly

#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
int wide=300, high=300;   // Global Variables for Length and Width
int winX=40, winY=40;	  // X and Y positions for window


void DrawMyStuff()  // The Drawing Routine
{ 
    double x,y;
    glColor3f(1.0, 1.0, 0.0);		// Set drawing color to yellow
    glBegin( GL_POINTS );		// Start drawing POINTS
        for (x = -1; x <= 1; x += .001) // Step from -1 to +1 by 0.001 
          { y =  4* x -  4* x * x ;     // Calculate y value of parabola
            glVertex3f(x, y, 0.0);      // Plot point at (x,y) z=0 in 2D
          }
   glEnd();				// End drawing points
} 

void display(void)
{ /* clear all pixels */

   glClear(GL_COLOR_BUFFER_BIT);
   DrawMyStuff();			// Call my Drawing Routine
   glFlush();  // Force writing all pending OpenGL actions to the screen.
}

void myinit(void)
{ /* select clearing (background) color   */
   glClearColor(0.5, 0.5, 0.5, 0.0);     // These RGB values make gray
   
/* initialize viewing values */
   glMatrixMode(GL_PROJECTION);			// Select Matrix Mode
   glLoadIdentity();				// Provide Base Matrix 
   glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);   	// Set window dimensions
   // Parameters:  glOrtho(Left, Right, Bottom, Top, Front, Back)
}

/* Declare initial window size, position, and display mode */

int main(int argc, char* argv[])
{  
   printf("Creating window %d pixels wide and %d pixels high", wide, high);
   printf(" at (%d,%d).\n", winX, winY);
   glutInit(&argc, argv);   // Call glut's initialization routine
   glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);	// Set Single Buffer, RGB Color
   glutInitWindowSize(wide, high);  	// Initialize window size
   glutInitWindowPosition(winX,winY); 	// Position window
   glutCreateWindow("My Window");	// Actually create window with title
   myinit();				// Call my initialization routine

   glutDisplayFunc(display);		// Call my display routine 
   glutMainLoop();			// Fall into glut's main event loop 
   return 0;
}

/* 
 *  Adapted for use in Supercomputer Applications 
 *  at TJHSST from the OpenGL Tutor by Mahesh Kumar
 *  Parts of this program might have been adopted from or inspired by 
 *  sample programs in 
 *  "OpenGL Programming Guide", 
 *  Copyright (c) 1993-1997, Silicon Graphics, Inc.
 *  "Interactive Computer Graphics"  
 *  Copyright (c) 1997, Addison-Wesley,
 *  and/or those written by Dr. Sumanta Guha guha@cs.uwm.edu
 *  and/or those written by Dr. Ichiro Suzuki suzuki@cs.uwm.edu
 */










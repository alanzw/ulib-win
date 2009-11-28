/* Logistic Difference Equation and Bifurcation 
*
* This program draws an inverted parabola for the function
*           y = k*x*(1-x)
* where the initial k and x are determined by mouse input
* and the range for k is between 0 and 4 and x ranges from 0
* to 1.  The program then plots 500 iterations of the
* function as each new x value is determioned from the
* previous y.  This is demonstrated by reflecting thhrough
* the equation of the line, y = x.
*/
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#include <GL/glut.h>

#define initial_width 600
#define initial_height 600
#define max_colors 10

int main_window, sub_window1;

// The following code permits drawing text on the screen 
// Bitmap character fonts must be pre-defined

float colarry[max_colors][3] = {{1.0, 0.0, 0.0}, {0.0, 0.8, 0.0}, 
    {0.0, 0.0, 0.8}, {0.8, 0.0, 0.8},
    {0.9, 0.9, 0.0}, {0.0, 0.8, 0.8},
    {1.0, 0.5, 0.5}, {0.3, 1.0, 0.3},
    {0.5, 0.5, 1.0}, {0.0, 0.0, 0.0} };

// Routine to plot text 
void bitmap_output(int x, int y, int z, char *string, void *font)
{
    int len, i;

    glRasterPos3f(x, y, 0);
    len = (int) strlen(string);
    for (i = 0; i < len; i++) {
        glutBitmapCharacter(font, string[i]);
    }
}



// Routine to plot parabola for equation F(x)=k*x*(1-x)

void plot_parabola(float k)
{
    float x, y, step;
    int i;
    glBegin(GL_LINE_STRIP);
    for (i=0; i<600; i++)
    {x=  i*(1.0/600);
        y=  k*x - k*x*x;
        //if (i%20 == 0) {printf ("%d  %f  %f \n", i, x, y); }
        glVertex3f(x, y, 0.0);
    }
    glEnd();
    glFlush();
}

// Plot diagonal line, y=x, used in iteration process

void plot_line(void)
{
    glBegin(GL_LINE_STRIP);
    glColor3f(0.8, 0.8, 0.8);
    glVertex3f(0.0,0.0,0.0);
    glVertex3f(1.0, 1.0, 0.0);
    glEnd();
    glFlush();
}

// The Help routine tells people how to use the module
void myhelp(void)
{
    char s[80];
    glColor3f(0.0,0.0,0.6);
    glPushMatrix();
    glTranslatef(0.05, 0.95, 0);
    sprintf(s,"Logistic Difference Equation: F(x)=kx(1-x)");
    bitmap_output(0,0,0, s, GLUT_BITMAP_TIMES_ROMAN_24);
    glPopMatrix();
    sprintf(s,"1. Right mouse button and cursor position selects values");
    glPushMatrix();
    glTranslatef(0.05, 0.85, 0);
    bitmap_output(0,0,0, s, GLUT_BITMAP_TIMES_ROMAN_24);
    glPopMatrix();
    sprintf(s,"     Vertical Component:  k between 0 and 4");
    glPushMatrix();
    glTranslatef(0.05, 0.80, 0);
    bitmap_output(0,0,0, s, GLUT_BITMAP_TIMES_ROMAN_24);
    glPopMatrix();
    sprintf(s,"     Horozontal Component:  x between 0 and 1");
    glPushMatrix();
    glTranslatef(0.05, 0.75, 0);
    bitmap_output(0,0,0, s, GLUT_BITMAP_TIMES_ROMAN_24);
    glPopMatrix();
    sprintf(s,"2. Left mouse button draws 500 iterations");
    glPushMatrix();
    glTranslatef(0.05, 0.65, 0);
    bitmap_output(0,0,0, s, GLUT_BITMAP_TIMES_ROMAN_24);
    glPopMatrix();
    sprintf(s,"3. Middle mouse button repeats this message");
    glPushMatrix();
    glTranslatef(0.05, 0.55, 0);
    bitmap_output(0,0,0, s, GLUT_BITMAP_TIMES_ROMAN_24);
    glPopMatrix();
}

//  This routine controls interactive features.

void mouse1(int button, int state, int x, int y)
{
    float y0,xn,yn,xtemp;
    static float k , x0;
    static char s1[25], s2[25], s3[25], s4[25];
    int i;
    static int num = 0;

    // Selecting Button Zero will plot 500 iterations of the logistic
    // difference equation for the parameters chosen. 

    if ( button== 0 && state == GLUT_DOWN)
    {
        glClearColor(1.0, 1.0, 0.7, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        sprintf (s4," X0 =  %f \0",x0);
        sprintf (s2," K =  %f \0",k);
        glPushMatrix();
        glColor3f(1.0,1.0,0.7);
        glTranslatef(0.05, 0.95, 0);
        bitmap_output(0,0,0, s1, GLUT_BITMAP_TIMES_ROMAN_24);
        glColor3f(0.0,0.0,0.6);
        bitmap_output(0,0,0, s2, GLUT_BITMAP_TIMES_ROMAN_24);
        for(i=0; i<25; i++) {s1[i] = s2[i]; }
        glPopMatrix();
        glPushMatrix();
        glColor3f(1.0,1.0,0.7);
        glTranslatef(0.65, 0.95, 0);
        bitmap_output(0,0,0, s3, GLUT_BITMAP_TIMES_ROMAN_24);
        glColor3f(0.0,0.0,0.6);
        bitmap_output(0,0,0, s4, GLUT_BITMAP_TIMES_ROMAN_24);
        for(i=0; i<25; i++) {s3[i] = s4[i]; }
        glPopMatrix();
        plot_line();
        glColor3f(1.0, 0.0, 0.0);
        plot_parabola(k);
        xn= x0;
        glBegin(GL_LINE_STRIP);
        glVertex3f(x0, 0.0, 0.0);
        for (i = 0; i < 500; i++) {
            glColor3f( colarry[num][0], colarry[num][1], colarry[num][2]);
            num++;
            if (num>= max_colors) num=0;
            yn=k*xn - k*xn*xn;
            glVertex3f(xn,yn,0.0);
            glVertex3f(yn, yn, 0.0);
            //printf("%f %f \n", xn, yn);
            xn = yn;
        }
        glEnd();
        glFlush();
    }

    //  Button 2 shows the values for K and X as well as the shape of
    //  the parabola.
    if ( (button== 2 )&& state == GLUT_DOWN)
    {
        glColor3f(1.0, 1.0, 0.7);
        glClearColor(1.0, 1.0, 0.7, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        plot_parabola(k);
        k =  4- y/(600.0/4);
        x0 = x/600.0;
        sprintf (s4," X0 =  %f \0",x0);
        sprintf (s2," K =  %f \0",k);
        glPushMatrix();
        glColor3f(1.0,1.0,0.7);
        glTranslatef(0.05, 0.95, 0);
        bitmap_output(0,0,0, s1, GLUT_BITMAP_TIMES_ROMAN_24);
        glColor3f(0.0,0.0,0.6);
        bitmap_output(0,0,0, s2, GLUT_BITMAP_TIMES_ROMAN_24);
        for(i=0; i<25; i++) {s1[i] = s2[i]; }
        glPopMatrix();
        glPushMatrix();
        glColor3f(1.0,1.0,0.7);
        glTranslatef(0.65, 0.95, 0);
        bitmap_output(0,0,0, s3, GLUT_BITMAP_TIMES_ROMAN_24);
        glColor3f(0.0,0.0,0.6);
        bitmap_output(0,0,0, s4, GLUT_BITMAP_TIMES_ROMAN_24);
        for(i=0; i<25; i++) {s3[i] = s4[i]; }
        glPopMatrix();
        glColor3f(1.0, 0.0, 0.0);
        glColor3f(1.0, 0.0, 0.0);
        plot_parabola(k);
        glFlush();
    }


    // Button 1 will clear the screen
    if ( button== 1 && state == GLUT_DOWN)
    {
        glClearColor(1.0, 1.0, 0.7, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        myhelp();
    }

}



// Main display window 

void display(void)
{
    glClearColor(1.0, 1.0, 0.7, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    myhelp();
    glFlush();
}



// 
void reshape(int w, int h)
{
    int width = initial_width;
    int height = initial_height;
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho( 0.0, 1.0, 0.0, 1.0, -1.0, 1.0 ); // The default is 1.0
    glMatrixMode(GL_MODELVIEW);

}

int main(int argc, char **argv)
{
    float r,g,b = 0.0;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowSize(initial_width, initial_height);
    main_window = glutCreateWindow("My First Window ");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse1);

    glutMainLoop();
    return 0;             /* ANSI C requires main to return int. */
}

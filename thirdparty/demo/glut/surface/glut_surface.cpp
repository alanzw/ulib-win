/*
	Solid Surface Program
 	Inspired by Sean Dobbs, TJ class of 1999

	This program generates a surface in 3-D using strips
	of triangles.  In order to implement lighting, the
	normal of each triangle must be calculated.
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#define MAX 40

// Globals
struct colorval
{
	double rd, gr, bl;
};

static double 	altitude[MAX][MAX], spin=0.0, z=0.0 , spin2=0.0,
		red=1.0, green=1.0, blue=1.0;

static struct colorval colors[MAX][MAX];

// Lets make a window with GL functions

// Here we enable most of the stuff for lighting
static void Init(void)
{   // These are some material properties
    GLfloat mat_specular[] = {0.4, 0.4, 0.4, 1.0};
    GLfloat mat_shininess[] = {40.0};

    // where the light is positioned 
    GLfloat light_position[] = {10.0, 10.0, 20.0, 0.0};

    // Here is where to set various properties 
    glClearColor(0.4, 0.4, 0.4, 1.0);
    glShadeModel( GL_SMOOTH );
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glColorMaterial(GL_FRONT, GL_DIFFUSE);

    // This enables the properties 
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
}

static void Reshape( int width, int height )
{   // This program uses perspective rather than orthographic projection
    glViewport(0, 0, (GLint)width, (GLint)height);
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)width / (GLfloat)height, 1.0, MAX * 4.0);
    glMatrixMode(GL_MODELVIEW);
}


void SetAltitude()
{	// This uses an ugly math equation to generate some height values
    int a, b;
    for(a=0; a<MAX; a++)
	for(b=0; b<MAX; b++)
             altitude[a][b] = 8*sin(a/2.0)*cos(b/2.0);


}

void SetColors()
{   // This creates some arbitrary color values in a secondary matrix
    int x,y;
    double temp;
    for( x=0; x<MAX; x++)
	for(y=0; y<MAX; y++)
	{
  	   temp = altitude[x][y];
	   colors[x][y].gr = temp / 5.0;
	   colors[x][y].rd = 0.8 - temp / 15.0;
	   colors[x][y].bl = 1.0;
	}
}

// The next three functions are used to compute the normal vector that is
// perpendicular to the face of the polygon that we draw.
void normalize(float v[3])
{
  GLfloat d=sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
    if(d== 0.0) return;

  v[0]/=d;
  v[1]/=d;
  v[2]/=d;
}

void normcrossprod(float v1[3], float v2[3], float out[3])
{
  GLint i,j;
  GLfloat length;

  out[0] = v1[1]*v2[2] - v1[2]*v2[1];
  out[1] = v1[2]*v2[0] - v1[0]*v2[2];
  out[2] = v1[0]*v2[1] - v1[1]*v2[0];
  normalize(out);
}

void trinormal(float x1,float y1,float z1,float x2,float y2,float z2,
               float x3,float y3,float z3)
{
  GLfloat d1[3],d2[3],norm[3];

  int j;

  d1[0]=x2-x1;
  d1[1]=y2-y1;
  d1[2]=z2-z1;
  d2[0]=x3-x2;
  d2[1]=y3-y2;
  d2[2]=z3-z2;

  normcrossprod(d1,d2,norm);
  glNormal3fv(norm);
}

// Defined keys

static void spinleft() { spin -= 5; }

static void spinright() { spin += 5; }

static void zoomout() { z -= 5; }

static void zoomin() { z += 5; }

static void spinup() { spin2 += 5; }

static void spindown() { spin2 -= 5; }

// DrawMyStuff: This function draws a series of strips made of simple
// triangles.  The first two vertices are selected, and then with each
// additional vertex, a triangle is created.  The color of the triangle
// is determined from the altitude[][] matrix, and then the normals
// are calculated.  In order to handle the various manipulations to
// the surface, various translations and rotations are applied.
void DrawMyStuff()
{ 	
    int a,b;
    glLoadIdentity();

    // gluLookAt(EyeX, EyeY, EyeZ, PosX, PosY, PosZ, UpX, UpY, UpZ)
    gluLookAt(MAX, MAX/2.0, 2*MAX, MAX/2.0, -1.0, MAX/2.0,  0.0, 1.0, 0.0);

    glTranslatef(MAX/2.0, 0.0, MAX/2.0 );
    glRotatef(spin, 0.0, 1.0, 0.0);
    glRotatef(spin2, 1.0, 0.0, 0.0);
    glTranslatef(-MAX/2.0, 0.0, -MAX/2.0 );
    glTranslatef(0.0, 0.0, -z );

    // Make strips of triangles with vertices n, and n+1.
    for( a=1;a<MAX-1;a++)
        { 	
          glBegin(GL_TRIANGLE_STRIP);
          glColor3f(colors[a][0].rd,colors[a][0].gr,colors[a][0].bl);
          glVertex3f(a, altitude[a][0], 0 );
          glVertex3f(a+1, altitude[a+1][0], 0);

          for( b=1;b<MAX-1;b++)
          {	
              glColor3f(colors[a][b].rd,colors[a][b].gr,colors[a][b].bl);
              glVertex3f(a, altitude[a][b], b);
              trinormal(a,altitude[a][b-1],b-1,
                        a,altitude[a][b],b,
                        a+1,altitude[a+1][b-1],b-1);
              glVertex3f(a+1, altitude[a+1][b], b);
              trinormal(a,altitude[a][b],b,
                        a+1,altitude[a+1][b],b,
                        a,altitude[a][b-1],b-1);
           }
           glEnd();
        }

}

// used for GLUT keypresses
static void keypress(unsigned char key, int x, int y)
{
   switch(key)
   {
	case 'h': spinleft(); break;
	case 'l': spinright(); break;
	case 'j': spinup(); break;
	case 'k': spindown(); break;
	case 'o': zoomout(); break;
	case 'i': zoomin(); break;
   }
}

// The main loop
static void display()
{
   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
   DrawMyStuff();

   glFlush();
   glutSwapBuffers();
   glutPostRedisplay();
}

// Main Loop of Program.
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    SetAltitude();
    SetColors();
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(550,550);
    glutInitWindowPosition(50,50);
    glutCreateWindow(argv[0]);
    Init();
    glutKeyboardFunc(keypress);
    glutDisplayFunc(display);
    glutReshapeFunc(Reshape);
    glutMainLoop();
    return 0;	
}

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

#define SCRSIZE 7            // screen size factor
#define NUMP 20                // number of balls points
#define NUMS (NUMP+1)        // number of springs
#define MASS 1.0f                // default point mass
#define BALLRADIUS 0.516f        // default radius of the ball
#define KS 755.0f                // spring constant
#define KD 35.0f                // spring damping constant
#define GY -10.0f
#define DT 0.005f
#define FINAL_PRESSURE 45.0f

/* Mouse */

#define windW 380
#define windH 380

int mousedown = 0;
float xMouse, yMouse;
int closest_i=1;                // point closest to mouse index

/* Structure of the point */
typedef struct
{
    float        x,y;                    // position
    float        vx,vy;            // velocity
    float        fx,fy;            // force accumulator
} CPoint2d;

/* Structure of the springs */
typedef struct
{
      int i,j;                    // points indexes
       float length;                // rest length
    float    nx,ny;                        // normal vector   
} CSpring;

  
/* There we will keep an object */
float Pressure = 0;
CPoint2d myPoints[NUMP+1];
CSpring mySprings[NUMS+1];

 
/* Add new spring */
void AddSpring(int pi, int i, int j)
{
    mySprings[pi].i = i;    mySprings[pi].j = j;
    mySprings[pi].length = sqrt(     (myPoints[ i ].x - myPoints[ j ].x)*(myPoints[ i ].x - myPoints[ j ].x)
                        +     (myPoints[ i ].y - myPoints[ j ].y)*(myPoints[ i ].y - myPoints[ j ].y) );
}

/* Create Ball Object (points & springs) */
void CreateBall(void)
{
    int i;
     for(i=1 ; i <= NUMP ; ++i)        // create points
     {
        myPoints[i].x = BALLRADIUS * sin( i * (2.0 * 3.14) /  NUMP  );
        myPoints[i].y = BALLRADIUS * cos(i * (2.0 * 3.14) /  NUMP  ) + SCRSIZE/2;
     }
     for(i=1 ; i <= NUMP ; ++i)        // create springs
           AddSpring(i,i,i+1);   
       AddSpring(i-1,i-1,1);
}

/* Reshape Window */
void Reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-SCRSIZE, SCRSIZE, -SCRSIZE, SCRSIZE);
    glMatrixMode(GL_MODELVIEW);
}

/* Force Calculation Subroutine */
void AccumulateForces(void)
{
    int i;
       float x1,x2,y1,y2;        // positions of spring points p1, p2
    float r12d;            // length of p1 - p2 vector
    float vx12;            // vx1 - vx2
    float vy12;            // vy1 - vy2
    float f;                // hooke force value
    float Fx,Fy;            // force vector
    float volume=0;            // volume of the body
    float pressurev;        // pressure force value

    /* gravity */
    for(i=1 ; i <= NUMP ; ++i)
    {
        myPoints[i].fx = 0;
        myPoints[i].fy = MASS * GY * (Pressure - FINAL_PRESSURE >= 0);

        /* user clicked (mouse spring) */
        if(i==closest_i)        // closest point only
        if(mousedown)            // if user clicked
        {
            x1 = myPoints[ i ].x;        // get points coords.
            y1 = myPoints[ i ].y;
            x2 = xMouse;
            y2 = yMouse;

            r12d = sqrt ( (x1 - x2) *(x1 - x2)  +  (y1 - y2) * (y1 - y2) );
                                                            // square
                                                            // root  of the distance
            f = (r12d -  2.2) * 22 + (myPoints[ i ].vx * (x1 - x2) + myPoints[ i ].y * (y1 - y2)) * 54 / r12d;

            // calculate spring force
            Fx = ((x1 - x2) / r12d ) * f;
            Fy = ((y1 - y2) / r12d ) * f;

            // accumulate force
            myPoints[i].fx -= Fx;
            myPoints[i].fy -= Fy;

        }
    }

    /* spring force */
     for(i=1 ; i <= NUMS ; ++i)
     {
            x1 = myPoints[ mySprings[i].i ].x;    y1 = myPoints[ mySprings[i].i ].y;
            x2 = myPoints[ mySprings[i].j ].x;    y2 = myPoints[ mySprings[i].j ].y;
            r12d = sqrt ( (x1 - x2) *(x1 - x2)  +  (y1 - y2) * (y1 - y2) );    // square
                                                            // root  of the distance
            if(r12d != 0)
            {
                vx12 = myPoints[ mySprings[i].i ].vx - myPoints[ mySprings[i].j ].vx;
                vy12 = myPoints[ mySprings[i].i ].vy - myPoints[ mySprings[i].j ].vy;

                f = (r12d -  mySprings[i].length) * KS + (vx12 * (x1 - x2) + vy12 * (y1 - y2)) * KD / r12d;

                Fx = ((x1 - x2) / r12d ) * f;
                Fy = ((y1 - y2) / r12d ) * f;

                myPoints[ mySprings[i].i  ].fx -= Fx;
                myPoints[ mySprings[i].i  ].fy -= Fy;

                myPoints[  mySprings[i].j  ].fx += Fx;
                myPoints[  mySprings[i].j  ].fy += Fy;

            }

            /* Calculate normal vectors to springs */
            mySprings[i].nx =  (y1 - y2) / r12d;
            mySprings[i].ny =  -(x1 - x2) / r12d;
    }
    
    /* pressure force */
    /* Calculate Volume of the Ball (Gauss Theorem) */
    for(i=1 ; i<=NUMS-1 ; i++)
    {
        x1 = myPoints[ mySprings[i].i ].x;    y1 = myPoints[ mySprings[i].i ].y;
        x2 = myPoints[ mySprings[i].j ].x;    y2 = myPoints[ mySprings[i].j ].y;
        r12d = sqrt ( (x1 - x2) *(x1 - x2)  +  (y1 - y2) * (y1 - y2) );    // square
                                                        // root  of the distance
        volume += 0.5 * fabs(x1 - x2) * fabs(mySprings[i].nx) * (r12d);
    }
    for(i=1 ; i<=NUMS-1 ; i++)
    {
        x1 = myPoints[ mySprings[i].i ].x;    y1 = myPoints[ mySprings[i].i ].y;
        x2 = myPoints[ mySprings[i].j ].x;    y2 = myPoints[ mySprings[i].j ].y;
        r12d = sqrt ( (x1 - x2) *(x1 - x2)  +  (y1 - y2) * (y1 - y2) );    // square
                                                        // root  of the distance
        pressurev = r12d * Pressure * (1.0f/volume);
        myPoints[ mySprings[i].i ].fx += mySprings[ i ].nx * pressurev;
        myPoints[ mySprings[i].i ].fy += mySprings[ i ].ny * pressurev;
        myPoints[ mySprings[i].j ].fx += mySprings[ i ].nx * pressurev;
        myPoints[ mySprings[i].j ].fy += mySprings[ i ].ny * pressurev;
    }
  
}




/**
 * Euler Integrator
 */

void IntegrateEuler()
{
    int i;
     float dry,drx;            // dr for Euler integration
    
    /* Euler Integrator (second Newton's law) */

     for(i=1 ; i <= NUMP ; ++i)
     {
           /* x */
        myPoints[i].vx = myPoints[i].vx + ( myPoints[i].fx / MASS )* DT;
        drx = myPoints[i].vx * DT;
        
        /* Boundaries  X */
        if(myPoints[i].x + drx < -SCRSIZE)
        {
            drx = -SCRSIZE - myPoints[ i ].x;
               myPoints[i].vx = - 0.1 *myPoints[i].vx;
               myPoints[i].vy = 0.95 *myPoints[i].vy;
        }else
        /* Boundaries  X */
        if(myPoints[i].x + drx > SCRSIZE)
        {
            drx = SCRSIZE - myPoints[ i ].x;
               myPoints[i].vx = - 0.1 *myPoints[i].vx;
               myPoints[i].vy = 0.95 *myPoints[i].vy;
        }

        myPoints[i].x = myPoints[i].x + drx;

           /* y */
        myPoints[i].vy = myPoints[i].vy + myPoints[i].fy * DT;
        dry = myPoints[i].vy * DT;

        /* Boundaries  Y */
        if(myPoints[i].y + dry < -SCRSIZE)
        {
            dry = -SCRSIZE - myPoints[ i ].y;
               myPoints[i].vy = - 0.1 *myPoints[i].vy;
               myPoints[i].vx = 0.95 *myPoints[i].vx;
        }
        /* Boundaries  Y */
        if(myPoints[i].y + dry > SCRSIZE)
        {
            dry = SCRSIZE - myPoints[ i ].y;
               myPoints[i].vy = - 0.1 *myPoints[i].vy;
        }

        myPoints[i].y = myPoints[i].y + dry;


        /* fast chek if outside */
        if(myPoints[i].x > SCRSIZE)
            myPoints[i].x = SCRSIZE;
        if(myPoints[i].y > SCRSIZE)
            myPoints[i].y = SCRSIZE;
        if(myPoints[i].x < -SCRSIZE)
            myPoints[i].x = -SCRSIZE;
        if(myPoints[i].y < -SCRSIZE)
            myPoints[i].y = -SCRSIZE;

     }

  
}
     
/* Idle function */
void Idle(void)
{
    AccumulateForces();
    IntegrateEuler();
  /**
      * Update Pressure (pump an air into the ball)
      */
      if(Pressure < FINAL_PRESSURE)
      {
        Pressure += FINAL_PRESSURE/300.0f;
           printf("Pressure = %4.4f\n",Pressure);
      }    
    glutPostRedisplay();
}


/* Visualization */
void Draw(void)
{
    int i;
    glClearColor(1,1,1,0);
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_QUADS);
        for(i = 1 ; i <= NUMS-1 ; i++)
        {
            glColor3f(0.8,0.4,0.4);
            glVertex2f(myPoints[ mySprings[i].i ].x,myPoints[ mySprings[i].i ].y);
            glVertex2f(myPoints[ mySprings[i].j ].x,myPoints[ mySprings[i].j ].y);

            glVertex2f(myPoints[ NUMP - mySprings[i].i +1].x,myPoints[ NUMP - mySprings[i].i + 1].y);
            glVertex2f(myPoints[ NUMP - mySprings[i].j +1].x,myPoints[ NUMP - mySprings[i].j + 1].y);
        }
    glEnd();

    if(mousedown)
    {
      glColor3f(0,0,0);
      glBegin(GL_LINES);
        glVertex2f(xMouse,yMouse);
              glVertex2f(myPoints[closest_i].x,myPoints[closest_i].y);
      glEnd();
    }

    glutSwapBuffers(); 
}

/* Find point in the model which is closest to mouse click point */

void FindClosestPoint(void)
{
    float dmin;
    float mousepointd;
    int i;

    // find closest point
    dmin = sqrt(pow(myPoints[closest_i].x - xMouse,2) + pow(myPoints[closest_i].y - yMouse,2));

    for(i=1 ; i <= NUMP ; ++i)
    {
        mousepointd = sqrt(    pow(myPoints[i].x - xMouse,2) +
                            pow(myPoints[i].y - yMouse,2));
        if(mousepointd < dmin)
        {
            dmin = mousepointd;
            closest_i = i;
        }
    }
}

/* Clicked mouse */
void Mouse (int button, int state, int x, int y)
{
    
    if (button == GLUT_LEFT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            mousedown = 1;

            xMouse = SCRSIZE * 2.0 * ((float)x/(float)windW - 0.5);
            yMouse = -SCRSIZE * 2.0 * ((float)y/(float)windH - 0.5);

        }
        else if (state == GLUT_UP)
        {
            FindClosestPoint();
               mousedown = 0;
          }
     }
}

/* Mouse Motion */
void Motion (int x, int y)
{
    if (mousedown)
    {
        xMouse = SCRSIZE * 2.0 * ((float)x/(float)windW - 0.5);
        yMouse = -SCRSIZE * 2.0 * ((float)y/(float)windH - 0.5);
        glutPostRedisplay();
     }
}

/* Main Function */
int main(int argc, char **argv)
{
    glutInit(&argc,argv); 
    glutInitWindowPosition( 150, 150 ); glutInitWindowSize( windW, windH );
    glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE );
    glutCreateWindow("Soft Body 2D v1.0 by Maciej Matyka");

    CreateBall();

    glutReshapeFunc(Reshape);
    glutDisplayFunc(Draw);
    glutIdleFunc(Idle);
    glutMouseFunc(Mouse);
    glutMotionFunc(Motion);

    glutMainLoop();

    return 0;
}


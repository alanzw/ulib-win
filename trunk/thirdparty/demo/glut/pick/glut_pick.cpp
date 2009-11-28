#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include <gl\gl.h>
#include <gl\glu.h>
#include <gl\glut.h>

typedef struct cameras {
    float pos[3];
    float lookAt[3];
    float lookUp[3];
} camera;

// callback to initialize the scenes for each subwindow
void initScene(int argc, char **argv);

//
void init(camera *cam);

// callback to draw the window
void draw();

// callback when picking occurs
void picked(GLuint name,int sw);

// keyboard callback
void processKeyboard(unsigned char key, int x, int y);

// callback for quiting
void quit();

static camera cam;
// Selection Buffer
#define SelBufferSize 512

static int mainWindow;
static int border = 6, h=200, w=350;

// Picking Stuff //
#define RENDER                    1
#define SELECT                    2
#define BUFSIZE 1024
GLuint selectBuf[BUFSIZE];
GLint hits;
int mode = RENDER;
int cursorX, cursorY;


static GLint snowman_display_list;

void quit() {}

void processKeyboard(unsigned char key, int x, int y) {
    printf("key: %d\n",key);
}

void picked(GLuint name, int sw) {
    printf("my name = %d in %d\n", name, sw);
}


void init(camera *cam) {
    cam->pos[0] = 1.5;
    cam->pos[1] = 3.75;
    cam->pos[2] = 3;

    cam->lookAt[0] = 1.5;
    cam->lookAt[1] = 1.75;
    cam->lookAt[2] = 0;

    cam->lookUp[0] = 0;
    cam->lookUp[1] = 1;
    cam->lookUp[2] = 0;
}

void drawSnowMan() {
    glColor3f(1.0f, 1.0f, 1.0f);

    // Draw Body
    glTranslatef(0.0f ,0.75f, 0.0f);
    glutSolidSphere(0.75f,20,20);

    // Draw Head
    glTranslatef(0.0f, 1.0f, 0.0f);
    glutSolidSphere(0.25f,20,20);

    // Draw Eyes
    glPushMatrix();
    glColor3f(0.0f,0.0f,0.0f);
    glTranslatef(0.05f, 0.10f, 0.18f);
    glutSolidSphere(0.05f,10,10);
    glTranslatef(-0.1f, 0.0f, 0.0f);
    glutSolidSphere(0.05f,10,10);
    glPopMatrix();

    // Draw Nose
    glColor3f(1.0f, 0.5f , 0.5f);
    glRotatef(0.0f,1.0f, 0.0f, 0.0f);
    glutSolidCone(0.08f,0.5f,10,2);
}

GLuint createDL() {
    GLuint snowManDL;

    // Create the id for the list
    snowManDL = glGenLists(1);

    //
    glNewList(snowManDL,GL_COMPILE);
    drawSnowMan();
    glEndList();
    //
    return(snowManDL);
}

void initScene(int argc, char **argv) {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    snowman_display_list = createDL();
}

void draw() {
    // Draw ground
    glColor3f(0.9f, 0.9f, 0.9f);
    glBegin(GL_QUADS);
        glVertex3f(-100.0f, 0.0f, -100.0f);
        glVertex3f(-100.0f, 0.0f,  100.0f);
        glVertex3f( 100.0f, 0.0f,  100.0f);
        glVertex3f( 100.0f, 0.0f, -100.0f);
    glEnd();

    // Draw 4 SnowMen
    for(int i = 0; i < 2; i++)
        for(int j = 0; j < 2; j++) {
            glPushMatrix();
            glPushName(i*2+j);
            glTranslatef(i*3.0,0,-j * 3.0);
            glCallList(snowman_display_list);
            glPopName();
            glPopMatrix();
   }
}

//----------------------
// Resizing
//----------------------
void changeSize(int w1, int h1) {
    float ratio;

    h = h1;
    w = w1;
    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).

    if(h == 0)
        h = 1;

    ratio = 1.0f * w / h;
    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    // Set the clipping volume
    gluPerspective(45,ratio,0.1,1000);

    // setting the camera now
    glMatrixMode(GL_MODELVIEW);
}

//---------------
// Picking Stuff
//---------------
void startPicking() {
    GLint viewport[4];
    float ratio;

    glSelectBuffer(BUFSIZE,selectBuf);

    glGetIntegerv(GL_VIEWPORT,viewport);

    glRenderMode(GL_SELECT);

    glInitNames();

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    gluPickMatrix(cursorX,viewport[3]-cursorY,5,5,viewport);
    ratio = (viewport[2]+0.0) / viewport[3];
    gluPerspective(45,ratio,0.1,1000);
    glMatrixMode(GL_MODELVIEW);
}

void processHits2 (GLint hits, GLuint buffer[], int sw)
{
   GLint i, j, numberOfNames;
   GLuint names, *ptr, minZ,*ptrNames;

   ptr = (GLuint *) buffer;
   minZ = 0xffffffff;
   for (i = 0; i < hits; i++) {
      names = *ptr;
      ptr++;
      if (*ptr < minZ) {
          numberOfNames = names;
          minZ = *ptr;
          ptrNames = ptr+2;
      }

      ptr += names+2;
    }
   if (numberOfNames > 0) {
      printf ("You picked snowman  ");
      ptr = ptrNames;
      for (j = 0; j < numberOfNames; j++,ptr++) {
         printf ("%d ", *ptr);
      }
    }
   else
       printf("You didn't click a snowman!");
    printf ("\n");

}

void stopPicking() {

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glFlush();
    hits = glRenderMode(GL_RENDER);
    if (hits != 0){
        processHits2(hits,selectBuf,0);
    }
    mode = RENDER;
}

//-----------------
// Rendering
//-----------------
void renderScene() {
    glutSetWindow(mainWindow);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (mode == SELECT) {
        startPicking();
    }
    glLoadIdentity();

    gluLookAt(cam.pos[0],
              cam.pos[1],
              cam.pos[2],

              cam.lookAt[0],
              cam.lookAt[1],
              cam.lookAt[2],

              cam.lookUp[0],
              cam.lookUp[1],
              cam.lookUp[2]);

    draw();

    if (mode == SELECT)
        stopPicking();
    else
        glutSwapBuffers();
}

//-------------------
// Keyboard and Mouse
//-------------------
void processNormalKeys(unsigned char key, int x, int y) {
    if (key == 27) {
        quit();
        exit(0);
    }
    else
        processKeyboard(key, x, y);
}

void mouseStuff(int button, int state, int x, int y) {
    if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN)
        return;

    cursorX = x;
    cursorY = y;
    mode = SELECT;
}

//---------
// Main
//---------
int main(int argc, char **argv)
{

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH  | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(w,h);
    mainWindow = glutCreateWindow("SnowMen from Lighthouse 3D");

    glutKeyboardFunc(processNormalKeys);
    glutReshapeFunc(changeSize);
    glutDisplayFunc(renderScene);
    glutMouseFunc(mouseStuff);
    glutIdleFunc(renderScene);

    initScene(argc,argv);
    init(&cam);

    glutMainLoop();

    return 0;
}


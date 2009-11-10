#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <GL/glut.h>

typedef float Color[3];
int iPointNum=0;
int x1=0,x2=0,y3=0,y2=0;
int winWidth=400,winHeight=400;
double length;

bool rgbColorEqual(Color c1, Color c2)
{
    if (abs(c1[1]-c2[1]) <0.001 && abs(c1[2]-c2[2]) <0.001 && abs(c1[0]-c2[0]) <0.001) return true;
    else return false;
}

void setPixel(GLint x,  GLint y)
{
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

void getPixel(GLint x, GLint y, Color &c)
{
    glReadPixels(x,y,1,1,GL_RGB,GL_FLOAT,c);
}

//为像素填充颜色
void boundaryFill4 (double x, double y, Color fillColor, Color borderColor, double length)
{
    Color interiorColor;

    /* Set current color to fillColor, then perform following oprations. */
    getPixel (x, y, interiorColor);

    if(length == 0)
	{
        return;
    }


    if (!(rgbColorEqual(interiorColor,borderColor)) && !(rgbColorEqual(interiorColor,fillColor)))

    {
        for(int i = -length; i < (int)length; i++)
        {
            for(int j = -length; j < (int)length; j++)
            {
                if((i*i)+(j*j) <= length*length)
                {
                    setPixel (i, j);
                }
            }
        }

    }

}

void drawPixel(GLint xCoord, GLint yCoord)
{
    glBegin(GL_POINTS);
    glVertex2i(xCoord, yCoord);
    glEnd();
}


void CirclePoints(GLint x,GLint y)
{
    drawPixel(x, y); drawPixel(y, x);
    drawPixel(-x, y); drawPixel(y, -x);
    drawPixel(x, -y); drawPixel(-y, x);
    drawPixel(-x, -y); drawPixel(-y, -x);
}

void BresenhamCircle(GLint r)
{
    int delta, d1, d2, dir;
    GLint x,y;
    x=0;
    y=r;
    delta = 2*(1-r);
    while(y>=0)
    {
        CirclePoints(x,y);
        if(delta < 0){
            d1 = 2* (delta + y) -1;
            if(d1 <=0) dir  = 1;
            else      dir = 2;
        }
        else {
            if( delta > 0){
                d2 = 2*(delta-x)-1;
                if(d2 <= 0) dir = 2;
                else            dir = 3;
            }
            else      dir = 2;
        }
        switch(dir)
        {
        case 1:
            x++;
            delta+=2*x + 1;
            break;
        case 2:
            x++; y--;
            delta+=2*(x-y+1) + 1;
            break;
        case 3:
            y--;
            delta+=-2*y + 1;
            break;
        }/*end of switch*/
    }/*end of while*/
}/*end of BresenhamCircle*/


//该函数用于设置渲染状态
void SetupRC(void)
{
    glClearColor(1.0f, 1.0f, 1.0f,1.0f);
}

//当窗口大小改变时由GLUT函数调用
void ChangeSize(GLsizei width, GLsizei Height)
{
    GLfloat aspectRatio;

    if (Height == 0) {
        Height = 1;
    }

    glViewport(0, 0, width, Height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    aspectRatio = (GLfloat)width / (GLfloat) Height;

    if (width <= Height) {
        glOrtho(-100.0, 100.0, -100.0 / aspectRatio, 100.0 / aspectRatio, 1.0, -1.0);
    }
    else{
        glOrtho(-100.0 * aspectRatio, 100.0 * aspectRatio, -100.0, 100.0, 1.0, -1.0);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glFlush();
}

void init(void)
{//初始化工作
    glClearColor(1.0,1.0,1.0,0.0);
    glMatrixMode (GL_PROJECTION);      //指定投影矩阵

    gluOrtho2D (0.0, 400.0, 0.0, 400.0);  //指定显示的区域

}
void myDisplay(void)
{
    Color a={1.0,0.0,0.0},b={0.0,0.0,1.0};
    glColor3fv(b);
    glClear(GL_COLOR_BUFFER_BIT);
    /*glBegin(GL_LINE_LOOP);
glVertex2i(100, 100);
glVertex2i(200, 100);
glVertex2i(150, 150);
glEnd();
*/
    length = pow((y2-y3)*(y2-y3)+(x2-x1)*(x2-x1),1.0/2);
    printf("%f\n",length);
    BresenhamCircle(length);

    glColor3fv(a);
    boundaryFill4(0,0,a,b,length);
    glFlush();
}

void MousePlot(GLint button,GLint action,GLint xMouse,GLint yMouse)
{
    if(button==GLUT_LEFT_BUTTON&&action==GLUT_DOWN)
    {
        if(iPointNum==0||iPointNum==2)
        {
            iPointNum=1;
            x1=xMouse;
            y3=winHeight-yMouse;
        }
        else{
            iPointNum=2;
            x2=xMouse;
            y2=winHeight-yMouse;
            glutPostRedisplay();
        }
    }
    if(button==GLUT_RIGHT_BUTTON&&action==GLUT_DOWN)
    {
        iPointNum=0;
        glutPostRedisplay();
    }

    glutDisplayFunc(myDisplay);
}

void PassiveMouseMove(GLint xMouse,GLint yMouse)
{
    if(iPointNum==1)
    {
        x2=xMouse;
        y2=winHeight-yMouse;
        glutPostRedisplay();
    }
}

int main(int argc, char *argv[])
{

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(400, 400);
    glutCreateWindow("窗口名称");
    glutMouseFunc(MousePlot);

    glutPassiveMotionFunc(PassiveMouseMove);

    init();
    glutDisplayFunc(myDisplay);
    glutReshapeFunc(ChangeSize);

    SetupRC();
    glutMainLoop();

    return 0;
}

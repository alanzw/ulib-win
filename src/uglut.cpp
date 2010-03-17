/*
 * =====================================================================================
 *
 *       Filename:  uglut.cpp
 *
 *    Description:  UGlut Functions
 *
 *        Version:  1.0
 *        Created:  2009-8-6 5:58:45
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#include <windows.h>
#include <tchar.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include <cmath>

#include "uglut.h"

namespace UGlut
{

int EnableOpenGL(HWND hWnd, HDC &hDC, HGLRC &hRC)
{
    PIXELFORMATDESCRIPTOR pfd;
    int iFormat;

    // get the device context (DC)
    hDC = ::GetDC( hWnd );

    // set the pixel format for the DC
    ::ZeroMemory( &pfd, sizeof( pfd ) );
    pfd.nSize = sizeof( pfd );
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
        PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    if ((iFormat = ChoosePixelFormat(hDC, &pfd))==0)
    {
        return 1;
    }

    if (SetPixelFormat( hDC, iFormat, &pfd ) == FALSE)
    {
        return 1;
    }

    // create and enable the render context (RC)
    hRC = wglCreateContext( hDC );
    wglMakeCurrent( hDC, hRC );

    return 0;
}

int DisableOpenGL(HWND hWnd, HDC &hDC, HGLRC &hRC)
{
    wglMakeCurrent( NULL, NULL );
    wglDeleteContext( hRC );
    ::ReleaseDC( hWnd, hDC );

    return 0;
}

void ResizeScene(int nWidth, int nHeight)
{
    if (nHeight==0)    // Prevent A Divide By Zero By
    {
        nHeight=1;    // Making Height Equal One
    }

    glViewport(0, 0, nWidth, nHeight); // Reset The Current Viewport

    glMatrixMode(GL_PROJECTION);       // Select The Projection Matrix
    glLoadIdentity();                   // Reset The Projection Matrix

    // Calculate The Aspect Ratio Of The Window
    gluPerspective(45.0f,(GLfloat)nWidth/(GLfloat)nHeight, 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);           // Select The Modelview Matrix
    glLoadIdentity();
}

void ULIB_API switchToWireframe()
{
    // Turn on wireframe mode
    glPolygonMode(GL_FRONT, GL_LINE);
    glPolygonMode(GL_BACK, GL_LINE);
}

void ULIB_API switchToSolid()
{
    // Turn off wireframe mode
    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_FILL);
}



/////////////////////////////////////////////////////////////////////////////////////////////////
//										THE CCAMERA POSITION CAMERA
/////////////////////////////////////////////////////////////////////////////////////////////////
void UGLCamera::Position_Camera(float pos_x,  float pos_y,  float pos_z,
							  float view_x, float view_y, float view_z,
							  float up_x,   float up_y,   float up_z)
{
	UGLVector3 vPos	= UGLVector3(pos_x,  pos_y,  pos_z);
	UGLVector3 vView = UGLVector3(view_x, view_y, view_z);
	UGLVector3 vUp	= UGLVector3(up_x,   up_y,   up_z);
	
	mPos  = vPos;							// set the position
	mView = vView;							// set the view
	mUp   = vUp;							// set the up vector
}


/////////////////////////////////////////////////////////////////////////////////////////////////
//										THE CCAMERA MOVE CAMERA
/////////////////////////////////////////////////////////////////////////////////////////////////
void UGLCamera::Move_Camera(float cameraspeed)
{
	UGLVector3 vVector = UGLVector3(0,0,0);		// init a new view vector
	vVector = mView - mPos;					// Get the view vector
	
	// forward positive cameraspeed and backward negative -cameraspeed.
	mPos.x  = mPos.x  + vVector.x * cameraspeed;
	mPos.z  = mPos.z  + vVector.z * cameraspeed;
	mView.x = mView.x + vVector.x * cameraspeed;
	mView.z = mView.z + vVector.z * cameraspeed;
}


/////////////////////////////////////////////////////////////////////////////////////////////////
//										THE CCAMERA ROTATE VIEW
/////////////////////////////////////////////////////////////////////////////////////////////////
void UGLCamera::Rotate_View(float x, float y, float z)
{
	UGLVector3 vVector = mView - mPos;

	if(x) 
	{
		mView.z = (float)(mPos.z + sin(x)*vVector.y + cos(x)*vVector.z);
		mView.y = (float)(mPos.y + cos(x)*vVector.y - sin(x)*vVector.z);
	}
	if(y) 
	{
		mView.z = (float)(mPos.z + sin(y)*vVector.x + cos(y)*vVector.z);
		mView.x = (float)(mPos.x + cos(y)*vVector.x - sin(y)*vVector.z);
	}
	if(z) 
	{
		mView.x = (float)(mPos.x + sin(z)*vVector.y + cos(z)*vVector.x);		
		mView.y = (float)(mPos.y + cos(z)*vVector.y - sin(z)*vVector.x);
	}
}


//NEW//////////////////NEW//////////////////NEW//////////////////NEW////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////
//										THE CCAMERA MOUSE MOVE
/////////////////////////////////////////////////////////////////////////////////////////////////
void UGLCamera::Mouse_Move()
{
	POINT mousePos;	
	int mid_x = mWindowWidth  >> 1;	
	int mid_y = mWindowHeight >> 1;	
	float angle_y  = 0.0f;				
	float angle_z  = 0.0f;							
	
	GetCursorPos(&mousePos);	// Get the 2D mouse cursor (x,y) position					
	
	if( (mousePos.x == mid_x) && (mousePos.y == mid_y) ) return;

	SetCursorPos(mid_x, mid_y);	// Set the mouse cursor in the middle of the window						

	// Get the direction from the mouse, and bring the number down to a reasonable amount
	angle_y = (float)( (mid_x - mousePos.x) ) / 1000;		
	angle_z = (float)( (mid_y - mousePos.y) ) / 1000;

	// The higher the number(acceleration) is the faster the camera looks around.
	mView.y += angle_z * 2;

	// limit the rotation around the x-axis
	if((mView.y - mPos.y) > 8)  mView.y = mPos.y + 8;
	if((mView.y - mPos.y) <-8)  mView.y = mPos.y - 8;
	
	Rotate_View(0, -angle_y, 0); // Rotate
}


}; // namespace UGlut


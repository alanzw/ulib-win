/*
 * =====================================================================================
 *
 *       Filename:  uglut.h
 *
 *    Description:  Customized OpenGL Utility Tools
 *
 *        Version:  1.0
 *        Created:  2009-8-6 5:55:17
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#ifndef U_GLUT_H
#define U_GLUT_H

#include "ulib.h"

namespace UGlut
{

///
int ULIB_API EnableOpenGL(HWND hWnd, HDC &hDC, HGLRC &hRC);

///
int ULIB_API DisableOpenGL(HWND hWnd, HDC &hDC, HGLRC &hRC);

///
void ULIB_API ResizeScene(int nWidth, int nHeight);

///
void ULIB_API switchToWireframe();

///
void ULIB_API switchToSolid();


//////////////////////////////////////
//The UGLVector3 Struct
//////////////////////////////////////
struct UGLVector3					// expanded 3D vector struct
{			
	UGLVector3()
    {}
	
    UGLVector3 (float new_x, float new_y, float new_z)	 
	{
        x = new_x;
        y = new_y;
        z = new_z;
    }
	//
	UGLVector3 operator+(UGLVector3 vVector)
    {
        return UGLVector3(vVector.x+x, vVector.y+y, vVector.z+z);
    }
	//
	UGLVector3 operator-(UGLVector3 vVector)
    {
        return UGLVector3(x-vVector.x, y-vVector.y, z-vVector.z);
    }
	//
	UGLVector3 operator*(float number)
    {
        return UGLVector3(x*number, y*number, z*number);
    }
	//
	UGLVector3 operator/(float number)
    {
        return UGLVector3(x/number, y/number, z/number);
    }
	
	float x, y, z;
};

//////////////////////////////////////
//The UGLCamera Class
//////////////////////////////////////
class ULIB_API UGLCamera 
{
public:
    UGLVector3 mPos;	
	UGLVector3 mView;		
	UGLVector3 mUp;			


	int mWindowWidth;
	int mWindowHeight;


	// This function let you control the camera with the mouse
	void Mouse_Move();

	void Rotate_View(float x, float y, float z);
	void Move_Camera(float cameraspeed);
	void Position_Camera(float pos_x, float pos_y,float pos_z,
			 		     float view_x, float view_y, float view_z,
						 float up_x,   float up_y,   float up_z);
};


}; // namespace UGlut

#endif // U_GLUT_H


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

    UGLVector3& operator=(const UGLVector3 &other)
    {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }

    void set(float new_x, float new_y, float new_z)
    {
        x = new_x;
        y = new_y;
        z = new_z;
    }

    bool operator!=(const UGLVector3 &other)
    {
        return (x!=other.x || y!=other.y || z!=other.z);
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

#define UGL_MIN(x,y) ((x)<(y)?(x):(y))

struct ULIB_API UGLColor
{
    float r;
    float g;
    float b;

    UGLColor(float _r = .0f,float _g=.0f,float _b=.0f)
    :r(_r),g(_g),b(_b)
    {};

    UGLColor& operator=(const UGLColor &clr)
    {
        r = clr.r;
        g = clr.g;
        b = clr.b;

        return *this;
    }
    
    void set(float _r,float _g,float _b)
    {
        r = _r;
        g = _g;
        b = _b;
    }

    inline UGLColor add(const UGLColor& c)const
    { return UGLColor(r + c.r, g + c.g, b + c.b); }
    
    inline UGLColor multiply(float s) const 
    { return UGLColor(r * s, g * s, b * s); }
    
    inline UGLColor modulate(const UGLColor& c) const
    { return UGLColor(r * c.r, g * c.g, b * c.b); }
    
    inline void saturate()
    {
        r = UGL_MIN(r, (float)1);
        g = UGL_MIN(g, (float)1);
        b = UGL_MIN(b, (float)1); 
    }
    
    static inline UGLColor black(){ return UGLColor(0,0,0); }
    static inline UGLColor white(){ return UGLColor(1,1,1); }
    static inline UGLColor red()  { return UGLColor(1,0,0); }
    static inline UGLColor green(){ return UGLColor(0,1,0); }
	static inline UGLColor blue() { return UGLColor(0,0,1); }
};

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


class ULIB_API UGLCube
{
public:
    UGLCube()
    {}
    
    UGLCube(const UGLCube &other)
    {
        *this = other;
    }
    
    
    ~UGLCube()
    {}

    UGLCube& operator=(const UGLCube &other)
    {
        for (unsigned int i=0; i<8; ++i)
        {
            this->vertex[i] = other.vertex[i];
        }
        this->color = other.color;

        return *this;
    }


    bool operator == (const UGLCube &other)
    {
        for (unsigned int i=0; i<8; ++i)
        {
            if (this->vertex[i] != other.vertex[i])
            {
                return false;
            }
        }
        return true;
    }

    void size(float factor)
    {
        vertex[0].set(-1.0f*factor, -1.0f*factor,  1.0f*factor);
        vertex[1].set( 1.0f*factor, -1.0f*factor,  1.0f*factor);
        vertex[2].set( 1.0f*factor, -1.0f*factor, -1.0f*factor);
        vertex[3].set(-1.0f*factor, -1.0f*factor, -1.0f*factor);
        vertex[4].set(-1.0f*factor,  1.0f*factor,  1.0f*factor);
        vertex[5].set( 1.0f*factor,  1.0f*factor,  1.0f*factor);
        vertex[6].set( 1.0f*factor,  1.0f*factor, -1.0f*factor);
        vertex[7].set(-1.0f*factor,  1.0f*factor, -1.0f*factor);
    }

    void setColor(float _r,float _g,float _b)
    {
        color.set(_r, _g, _b);
    }
    
    void render()
    {
        glPushMatrix();
        glColor3f(color.r, color.g, color.b);
        glBegin(GL_QUADS);
          //
          glNormal3f(0.0f, 0.0f, 1.0f);
          glVertex3f(vertex[0].x, vertex[0].y, vertex[0].z);
          glVertex3f(vertex[1].x, vertex[1].y, vertex[1].z);
          glVertex3f(vertex[5].x, vertex[5].y, vertex[5].z);
          glVertex3f(vertex[4].x, vertex[4].y, vertex[4].z);
          //
          glNormal3f(1.0f, 0.0f, 0.0f);
          glVertex3f(vertex[1].x, vertex[1].y, vertex[1].z);
          glVertex3f(vertex[2].x, vertex[2].y, vertex[2].z);
          glVertex3f(vertex[6].x, vertex[6].y, vertex[6].z);
          glVertex3f(vertex[5].x, vertex[5].y, vertex[5].z);
          //
          glNormal3f(0.0f, 0.0f, -1.0f);
          glVertex3f(vertex[2].x, vertex[2].y, vertex[2].z);
          glVertex3f(vertex[3].x, vertex[3].y, vertex[3].z);
          glVertex3f(vertex[7].x, vertex[7].y, vertex[7].z);
          glVertex3f(vertex[6].x, vertex[6].y, vertex[6].z);
          //
          glNormal3f(-1.0f, 0.0f, 0.0f);
          glVertex3f(vertex[3].x, vertex[3].y, vertex[3].z);
          glVertex3f(vertex[0].x, vertex[0].y, vertex[0].z);
          glVertex3f(vertex[4].x, vertex[4].y, vertex[4].z);
          glVertex3f(vertex[7].x, vertex[7].y, vertex[7].z);
          //
          glNormal3f(0.0f, 1.0f, 0.0f);
          glVertex3f(vertex[4].x, vertex[4].y, vertex[4].z);
          glVertex3f(vertex[5].x, vertex[5].y, vertex[5].z);
          glVertex3f(vertex[6].x, vertex[6].y, vertex[6].z);
          glVertex3f(vertex[7].x, vertex[7].y, vertex[7].z);
          //
          glNormal3f(0.0f, -1.0f, 0.0f);
          glVertex3f(vertex[0].x, vertex[0].y, vertex[0].z);
          glVertex3f(vertex[3].x, vertex[3].y, vertex[3].z);
          glVertex3f(vertex[2].x, vertex[2].y, vertex[2].z);
          glVertex3f(vertex[1].x, vertex[1].y, vertex[1].z);
        glEnd(); 
        glPopMatrix();
    }
private:
    UGLVector3 vertex[8];
    UGLColor color;
};


class UGLTexture
{
public:
	UGLTexture()
		{}
	~UGLTexture()
		{}
private:
	unsigned char *m_pData;    // datas
	unsigned int   m_nWidth;    // width (pixels)
	unsigned int   m_nHeight;   // height (pixels)
	unsigned int   m_nDepth;    // bits per pixel 

	BITMAPINFOHEADER m_biHeader;      // image header (display on device context)
	unsigned int     m_nWidthByte32; // width (in bytes, and 32 bits aligned)

};

}; // namespace UGlut

#endif // U_GLUT_H


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



}; // namespace UGlut


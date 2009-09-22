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

}; // namespace UGlut

#endif // U_GLUT_H


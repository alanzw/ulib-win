/*
 * =====================================================================================
 *
 *       Filename:  uwndclassx.h
 *
 *    Description:  UWindowClass implement
 *
 *        Version:  1.0
 *        Created:  2009-8-4 4:21:14
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#ifndef U_WNDCLASSX_H
#define U_WNDCLASSX_H

#include "ulib.h"

namespace huys
{

/**
  typedef struct {
	UINT cbSize;
	UINT style;
	WNDPROC lpfnWndProc;
	int cbClsExtra;
	int cbWndExtra;
	HINSTANCE hInstance;
	HICON hIcon;
	HCURSOR hCursor;
	HBRUSH hbrBackground;
	LPCTSTR lpszMenuName;
	LPCTSTR lpszClassName;
	HICON hIconSm;
  } WNDCLASSEX, *PWNDCLASSEX;
*/

class ULIB_API UWindowClass
{
public:
    UWindowClass(LPCTSTR lpszClassName, HINSTANCE hInstance);

    ~UWindowClass() {};

    BOOL registerIt();

    LPCTSTR name() const;

    void setStyles(DWORD dwStyle);

    void setProcdure(WNDPROC lpfnWndProc);

    void setMenu(LPCTSTR lpMenuName)
    {
        m_wcx.lpszMenuName = lpMenuName;
    }
    
    static bool isWndClass(LPCTSTR lpWndClassName, HINSTANCE hInstance);
protected:
    WNDCLASSEX m_wcx;

private:
};

}; // namespace huys

#endif // U_WNDCLASSX_H


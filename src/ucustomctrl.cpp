/*
 * =====================================================================================
 *
 *       Filename:  ucustomctrl.cpp
 *
 *    Description:  Customized Control Class Implement
 *
 *        Version:  1.0
 *        Created:  2009-9-18 14:50:59
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

#include "ucustomctrl.h"

#include "uwndclassx.h"

#include "ugdi.h"

UCustomCtrl::UCustomCtrl(HWND hParent, UINT nResource, HINSTANCE hInst)
: UControl(hParent, nResource, hInst)
{
	if (!huys::UWindowClass::isWndClass(UCTCTRL, hInst))
	{
		huys::UWindowClass wndclass(UCTCTRL, hInst);
		wndclass.setProcdure(DefWindowProc);
		//wndclass.setStyles(CS_DBLCLKS);
		if (!wndclass.registerIt())
		{
			LOG_STRING(_T("Erorr register wndclassex %s!"), UCTCTRL);
		}
	}
}

UCustomCtrl::~UCustomCtrl()
{

}

BOOL UCustomCtrl::create()
{
	BOOL bRet = UControl::create(UCTCTRL);
	bRet &= this->subclassProc();
	return bRet;
}

static bool bStart = false;
static huys::ULine line; 

BOOL UCustomCtrl::onLButtonDown( WPARAM wParam, LPARAM lParam )
{
	if (!bStart)
	{
		bStart = true;
		int xPos = GET_X_LPARAM(lParam); 
		int yPos = GET_Y_LPARAM(lParam);
		line.SetStartPnt(xPos, yPos);
	}
	return FALSE;
}

BOOL UCustomCtrl::onLButtonUp( WPARAM wParam, LPARAM lParam )
{
	bStart = false;
	return FALSE;
}

BOOL UCustomCtrl::onMouseMove( WPARAM wParam, LPARAM lParam )
{
	if(bStart)
	{
		int xPos = GET_X_LPARAM(lParam); 
		int yPos = GET_Y_LPARAM(lParam);
		line.SetEndPnt(xPos, yPos);

		HDC hdc = ::GetDC(m_hSelf);
		line.Draw(hdc);
		::ReleaseDC(m_hSelf, hdc);

		line.SetStartPnt(xPos, yPos);
	}
	return FALSE;
}


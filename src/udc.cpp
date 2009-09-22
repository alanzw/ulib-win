/*
 * =====================================================================================
 *
 *       Filename:  udc.cpp
 *
 *    Description:  implement UDevContext
 *
 *        Version:  1.0
 *        Created:  2009-8-23 4:48:26
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#define _WIN32_WINNT 0x0500
#define WINVER 0x0500

#include <windows.h>
#include <tchar.h>

#include <cassert>

#include "udc.h"

UDevContext::UDevContext()
: UGDIObject()
{}

UDevContext::~UDevContext()
{}

bool UDevContext::attach(HDC hdc)
{
    assert(NULL != m_hObj);
    m_hObj = hdc;
    return true;
}

bool UDevContext::dettach()
{
    assert(NULL != m_hObj);
    m_hObj = NULL;
    return true;
}

huys::Color UDevContext::setPenColor(huys::Color clr)
{
    return ::SetDCPenColor((HDC)m_hObj, clr);
}

huys::Color UDevContext::setBrushColor(huys::Color clr)
{
    return ::SetDCBrushColor((HDC)m_hObj, clr);
}

/*
huys::Color UDevContext::getPenColor()
{
    return ::GetDCPenColor((HDC)m_hObj);
}

huys::Color UDevContext::getBrushColor()
{
    return ::GetDCBrushColor((HDC)m_hObj);
}
*/

HGDIOBJ UDevContext::getCurObj(UINT uObjectType)
{
    return ::GetCurrentObject((HDC)m_hObj, uObjectType);
}

UPaintDC::UPaintDC(HWND hWnd)
{
    m_hWnd = hWnd;
    m_hObj = ::BeginPaint(m_hWnd, &m_ps);
}

UPaintDC::~UPaintDC()
{
    ::EndPaint(m_hWnd, &m_ps);
}

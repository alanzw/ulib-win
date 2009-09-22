/*
 * =====================================================================================
 *
 *       Filename:  uobject.cpp
 *
 *    Description:  implement windows user object
 *
 *        Version:  1.0
 *        Created:  2009-7-3 0:04:56
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

#include "uobject.h"

UUserObject::UUserObject()
{
    m_hObj = NULL;
}

UUserObject::~UUserObject()
{
}

bool UUserObject::load()
{
    return true;
}

bool UUserObject::create()
{
    return true;
}

bool UUserObject::destroy()
{
    return true;
}

UKernelObject::UKernelObject()
{
    m_hObj = NULL;
}

UKernelObject::~UKernelObject()
{}

bool UKernelObject::create()
{
    return true;
}

bool UKernelObject::open()
{
    return true;
}

bool UKernelObject::close()
{
    ::CloseHandle(m_hObj);
    return true;
}

UGDIObject::UGDIObject()
: m_hOldObj(NULL),
  m_hdc(NULL)
{
    m_hObj = NULL;
}

UGDIObject::~UGDIObject()
{
    if (NULL != m_hObj)
    {
        ::DeleteObject(m_hObj);
        m_hObj = NULL;
    }

}

bool UGDIObject::select(HDC hdc)
{
    m_hdc = hdc;
    m_hOldObj = ::SelectObject(hdc, m_hObj);
    return true;
}

bool UGDIObject::restore()
{
    ::SelectObject(m_hdc, m_hOldObj);
    ::DeleteObject(m_hObj);
    m_hObj = NULL;
    return true;
}


/*
 * =====================================================================================
 *
 *       Filename:  umutex.cpp
 *
 *    Description:  Mutex
 *
 *        Version:  1.0
 *        Created:  2009-7-4 3:03:45
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
 
#include "umutex.h"

UMutex::UMutex()
: UKernelObject(),
  m_dwError(0)
{  
}

UMutex::~UMutex()
{  
}

HANDLE UMutex::create(BOOL bInitialOwner, LPSECURITY_ATTRIBUTES lpMutexAttributes, LPCTSTR lpName)
{
    m_hObj = ::CreateMutex(lpMutexAttributes, bInitialOwner, lpName);
    m_dwError = ::GetLastError();
    return m_hObj;
}

HANDLE UMutex::open( LPCTSTR lpName,
                     BOOL bInheritHandle /*= FALSE*/,
                     DWORD dwDesiredAccess /*= SYNCHRONIZE*/)
{
    m_hObj = ::OpenMutex( dwDesiredAccess, bInheritHandle, lpName);
    m_dwError = ::GetLastError();
    return m_hObj;
}

BOOL UMutex::release()
{
    return ::ReleaseMutex(m_hObj);
}

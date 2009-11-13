/*
 * =====================================================================================
 *
 *       Filename:  usemaphore.cpp
 *
 *    Description:  USemaphore implement
 *
 *        Version:  1.0
 *        Created:  2009-8-28 21:44:18
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

#include "usemaphore.h"

USemaphore::USemaphore()
{}

USemaphore::~USemaphore()
{}

HANDLE USemaphore::create( LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
                           LONG lInitialCount,
                           LONG lMaximumCount,
                           LPCTSTR lpName /*= NULL*/
                         )
{
    m_hObj = ::CreateSemaphore(lpSemaphoreAttributes, lInitialCount, lMaximumCount, lpName);
    return m_hObj;
}

#if WINVER >=  0x0600
HANDLE USemaphore::createEx( LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
                     LONG lInitialCount,
                     LONG lMaximumCount,
                     DWORD dwDesiredAccess,
                     LPCTSTR lpName /*= NULL*/)
{
    m_hObj = ::CreateSemaphoreEx(lpSemaphoreAttributes, lInitialCount, lMaximumCount, lpName, 0, dwDesiredAccess);
    return m_hObj;
}
#endif // WINVER >=  0x0600

BOOL USemaphore::release(LONG lReleaseCount, LPLONG lpPreviousCount)
{
    return ::ReleaseSemaphore(m_hObj, lReleaseCount, lpPreviousCount);
}


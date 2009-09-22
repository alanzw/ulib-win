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

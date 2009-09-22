/*
 * =====================================================================================
 *
 *       Filename:  usemaphore.h
 *
 *    Description:  Class USemaphore
 *
 *        Version:  1.0
 *        Created:  2009-8-28 21:40:41
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#ifndef U_SEMAPHORE_H
#define U_SEMAPHORE_H

#include "uobject.h"

class ULIB_API USemaphore : public UKernelObject
{
public:
    USemaphore();
    ~USemaphore();
    //
    HANDLE create( LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
                   LONG lInitialCount,
                   LONG lMaximumCount,
                   LPCTSTR lpName = NULL
                 );
};

#endif // U_SEMAPHORE_H


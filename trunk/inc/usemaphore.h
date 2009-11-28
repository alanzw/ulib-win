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

#if WINVER >=  0x0600
    HANDLE createEx( LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
                     LONG lInitialCount,
                     LONG lMaximumCount,
                     DWORD dwDesiredAccess,
                     LPCTSTR lpName = NULL);
#endif // WINVER >=  0x0600

    BOOL release(LONG lReleaseCount, LPLONG lpPreviousCount);

    class Lock
    {
    public:
        Lock(USemaphore &sem)
        : _sem(sem)
        {}

        ~Lock()
        {}

        void lock()
        {
            ::WaitForSingleObject(_sem, INFINITE);
        }

        void unlock()
        {
            _sem.release(1, NULL);
        }
    private:
        USemaphore &_sem;
    };
};

#endif // U_SEMAPHORE_H


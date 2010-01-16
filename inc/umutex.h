/*
 * =====================================================================================
 *
 *       Filename:  umutex.h
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
#ifndef U_MUTEX_H
#define U_MUTEX_H

#include "uobject.h"

class ULIB_API UMutex : public UKernelObject
{
public:
    UMutex();
    ~UMutex();

    //
    HANDLE create( BOOL bInitialOwner = TRUE,
                   LPSECURITY_ATTRIBUTES lpMutexAttributes = NULL,
                   LPCTSTR lpName = NULL
                 );

    //
    HANDLE open( LPCTSTR lpName,
                 BOOL bInheritHandle = FALSE,
                 DWORD dwDesiredAccess = SYNCHRONIZE
                );

    //
    BOOL release();

    //
    DWORD getErrorCode() const
    { return m_dwError; }

    class scoped_lock
    {
    private:
        bool _isLocked;
        UMutex & _mutex;
    public:
        explicit scoped_lock(UMutex &mutex)
        : _mutex(mutex), _isLocked(false)
        {
            lock();
        }

        ~scoped_lock()
        {
            if(owns_lock())
            {
                _mutex.release();
            }
        }
    private:
        bool owns_lock() const
        {
            return _isLocked;
        }

        void lock()
        {
            if(owns_lock())
            {
                throw 1111;
            }
            //m->lock();
            _mutex.create();
            _isLocked=true;
        }

        void unlock()
        {
            if(!owns_lock())
            {
                throw 1111;
            }
            _mutex.release();
            _isLocked = false;
        }
    };
private:
    DWORD m_dwError;
};

#endif // U_MUTEX_H

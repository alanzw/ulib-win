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
    UMutex(LPCTSTR sName);
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
};

#endif // U_MUTEX_H

/*
 * =====================================================================================
 *
 *       Filename:  usync.cpp
 *
 *    Description:  Synchronization utility
 *
 *        Version:  1.0
 *        Created:  2009-8-30 0:50:57
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */
#include <windows.h>

#include "usync.h"

namespace USync
{

DWORD waitSingle(UKernelObject *obj, DWORD dwMilliseconds)
{
    return ::WaitForSingleObject(*obj, dwMilliseconds);
}

DWORD waitMultiple(UKernelObject *obj, DWORD dwCount, DWORD dwMilliseconds)
{
    return ::WaitForMultipleObjects(dwCount, &((HANDLE)*obj), TRUE, dwMilliseconds);
}

}; // namespace USync


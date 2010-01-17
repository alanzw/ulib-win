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
    //return ::WaitForMultipleObjects(dwCount, &((HANDLE)*obj), TRUE, dwMilliseconds);
    HObjVector hobjs;
    for (int i = 0; i < dwCount; ++i)
    {
        hobjs.push_back((HANDLE)*obj);
    }
    return waitMultiple(hobjs, dwMilliseconds);
}

DWORD msgWaitMultiple(UObject *obj, DWORD dwCount, DWORD dwMilliseconds, DWORD dwWaitMask)
{
    //return ::MsgWaitForMultipleObjects(dwCount, &((HANDLE)*obj), TRUE, dwMilliseconds, dwWaitMask);
    HObjVector hobjs;
    for (int i = 0; i < dwCount; ++i)
    {
        hobjs.push_back(obj->m_hObj);
    }
    return ::MsgWaitForMultipleObjects(hobjs.size(), hobjs.begin(), TRUE, dwMilliseconds, dwWaitMask);
}

DWORD waitMultiple(HObjVector hobjs, DWORD dwMilliseconds)
{
    return ::WaitForMultipleObjects(hobjs.size(), hobjs.begin(), TRUE, dwMilliseconds);
}

}; // namespace USync


/*
 * =====================================================================================
 *
 *       Filename:  usync.h
 *
 *    Description:  Synchronization utility
 *
 *        Version:  1.0
 *        Created:  2009-8-30 0:44:51
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */
#include "uobject.h"

namespace USync
{

/** Return values:
 *    WAIT_ABANDONED
 *    WAIT_OBJECT_0
 *    WAIT_TIMEOUT
 *    WAIT_FAILED
 */
ULIB_API DWORD waitSingle(UObject *obj, DWORD dwMilliseconds = INFINITE);

/** Return values:
 *    WAIT_OBJECT_0 to (WAIT_OBJECT_0 + nCount¨C 1)
 *    WAIT_ABANDONED_0 to (WAIT_ABANDONED_0 + nCount¨C 1)
 *    WAIT_TIMEOUT
 *    WAIT_FAILED
 */
ULIB_API DWORD waitMultiple(UObject *obj, DWORD dwCount, DWORD dwMilliseconds);
ULIB_API DWORD msgWaitMultiple(UObject *obj, DWORD dwCount, DWORD dwMilliseconds, DWORD dwWaitMask);

}; // namespace USync


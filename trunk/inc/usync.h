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

ULIB_API DWORD waitSingle(UObject *obj, DWORD dwMilliseconds = INFINITY);
ULIB_API DWORD waitMultiple(UObject *obj, DWORD dwCount, DWORD dwMilliseconds);

}; // namespace USync


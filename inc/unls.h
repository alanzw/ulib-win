/*
 * =====================================================================================
 *
 *       Filename:  unls.h
 *
 *    Description:  Class UNls(National Language Support)
 *
 *        Version:  1.0
 *        Created:  2009-9-2 1:01:22
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */
#ifndef U_NLS_H
#define U_NLS_H

#include "ulib.h"

#ifndef LOCALE_NAME_MAX_LENGTH
#define LOCALE_NAME_MAX_LENGTH 85
#endif

namespace UNls
{

inline DWORD getSystemDefaultLCID(void)
{
    return ::GetSystemDefaultLCID();
}

inline DWORD getUserDefaultLCID(void)
{
    return ::GetUserDefaultLCID();
}

inline WORD getSystemDefaultLangID(void)
{
    return ::GetSystemDefaultLangID();
}

inline WORD getUserDefaultLangID(void)
{
    return ::GetUserDefaultLangID();
}

inline int getUserDefaultLocaleName(LPWSTR lpLocaleName)
{
#if WINVER > 0x0600
    return ::GetUserDefaultLocaleName(lpLocaleName, LOCALE_NAME_MAX_LENGTH);
#else
    return 0;
#endif
}

};

#endif // U_NLS_H


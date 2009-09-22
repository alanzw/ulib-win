#include <windows.h>
#include <tchar.h>

/**
 * The running time OS checking.
 *
 * @author huys
 */

#include "urt.h"

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

OSVersion getOSVer()
{
    return _osver;
}

OSVersion getWinMajor()
{
    return _winmajor;
}

OSVersion getWinMinor()
{
    return _winminor;
}

OSVersion getWinVer()
{
    return _winver;
}

UINT getArgc()
{
    return __argc;
}

TCHAR ** getArgv()
{
#ifdef _UNICODE
    return __wargv;
#else
    return __argv;
#endif
}

TCHAR ** getEnviron()
{
#ifdef _UNICODE
    return _wenviron;
#else
    return _environ;
#endif
}

const TCHAR * getPgmPtr()
{
#ifdef _UNICODE
    return _wpgmptr;
#else
    return _pgmptr;
#endif
}

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif


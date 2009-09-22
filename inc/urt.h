#ifndef U_RUNNING_TIME_H
#define U_RUNNING_TIME_H

#include "ulib.h"

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

#ifndef ULIB_API
  #error "ULIB_API is not defined yet. Please include ulib.h."
#endif

typedef unsigned int OSVersion;

ULIB_API OSVersion getOSVer();

ULIB_API OSVersion getWinMajor();

ULIB_API OSVersion getWinMinor();

ULIB_API OSVersion getWinVer();

ULIB_API UINT getArgc();

ULIB_API TCHAR ** getArgv();

ULIB_API TCHAR ** getEnviron();

ULIB_API const TCHAR * getPgmPtr();

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif

#endif // U_RUNNING_TIME_H

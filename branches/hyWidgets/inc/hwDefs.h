#ifndef HW_DEFS_H
#define HW_DEFS_H

#include <stdarg.h>

#ifdef USE_UNICODE
    typedef wchar_t hwChar;
    typedef wchar_t hwUChar;
    typedef wchar_t hwSChar;
#else
    typedef char hwChar;
    typedef unsigned char hwUChar;
    typedef signed char hwSChar;
#endif // USE_UNICODE

typedef hwChar* hwCharPtr;
typedef hwCharPtr* hwCharPtrPtr;

//#define HW_MSW

#ifdef HW_MSW

#ifdef BUILD_DLL
#define HW_API __declspec(dllexport)
#endif // BUILD_DLL

#ifdef USE_DLL
#define HW_API __declspec(dllimport)
#endif // USE_DLL

#ifndef HW_API
#define HW_API
#endif // HW_API

#endif // HW_MSW

#endif // HW_DEFS_H

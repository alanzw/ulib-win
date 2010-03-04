/**
 * @file
 * @author  huys <huys03@gmail.com>
 * @version 1.0.0
 */


/*! \mainpage ulib-win
 *
 * \section intro_sec Introduction
 *
 * What is ulib-win?
 * Some useful macros, functions and classes for windows API development.
 *
 * 1. Simple wrappers for win32 window, dialog and controls.
 *
 * 2. Simple wrappers for win32 process, thread and pipe.
 *
 * 3. Simple wrappers for system information query.
 *
 * 4. Simple wrappers for gdi, opengl and image.
 *
 * 5. Simple wrappers for DLL.
 *
 * 6. Simple wrappers for plugin architecture.
 *
 * 7. Simple wrappers for Hooks.
 *
 * \section install_sec Installation
 *
 * \subsection step1 Step 1: Edit the file make.inc
 *
 * \subsection step2 Step 2: run Make
 *
 */

#ifndef HUYS_ULIB_H
#define HUYS_ULIB_H

/// Macros for DLL building.
#ifdef BUILD_DLL
#define ULIB_API __declspec(dllexport)
#endif

#ifdef BUILD_EXE
#define ULIB_API __declspec(dllimport)
#endif

#if defined(BUILD_STATIC) || defined(WITHOUT_DLL)
  #ifdef ULIB_API
  #undef ULIB_API
  #endif // ULIB_API
  #define ULIB_API
#endif

#if  !defined(_WIN32)
#error ERROR: Only Win32 targets supported!
#endif

#ifndef UINTERFACE
#define UINTERFACE struct
#endif // UINTERFACE


#ifndef PO_IN
#define PO_IN
#endif // PO_IN

#ifndef PO_OUT
#define PO_OUT
#endif // PO_OUT

#ifndef PO_OPT
#define PO_OPT
#endif // PO_OPT

/// The top namespace
/** Just for testing.
 * Here is the detail description.
 */
namespace Ulib
{

template<bool> struct compile_time_check;

template<> struct compile_time_check<false>
{};

}; // namespace Ulib

/** Static Assert
 *  Usage: STATIC_ASSERT(expr, errormsg);
 */
#ifdef HU_DEBUG
#define STATIC_ASSERT(test, errormsg)                             \
    do {                                                          \
        struct ERROR_##errormsg {};                               \
        typedef Ulib::compile_time_check< (test) != 0 > tmplimpl; \
        tmplimpl aTemp = tmplimpl(ERROR_##errormsg());            \
        sizeof(aTemp);                                            \
    } while (0)
#else
#define STATIC_ASSERT(test, errormsg)                             \
    do {} while (0)
#endif


template <bool B>
inline void STATIC_ASSERT_IMPL()
{
    // B will be true or false, which will implictly convert to 1 or 0
    char STATIC_ASSERT_FAILURE[B] = {0};
}

#define STATIC_ASSERT0(B) STATIC_ASSERT_IMPL <B>()


/// For Message Reflection
#ifndef WM_REFLECT_BASE
/// Message Reflection Base
#define WM_REFLECT_BASE        0xBC00
/// Message Reflection CtrlColor
#define WM_REFLECT_CTLCOLOR    0xBC01
/// Message Reflection DrawItem
#define WM_REFLECT_DRAWITEM    0xBC02
/// Message Reflection MeasureItem
#define WM_REFLECT_MEASUREITEM 0xBC03
#endif

/// check and delete
#ifndef CHECK_PTR_MACRO
#define CHECK_PTR_MACRO
#define CHECK_PTR(p) \
  if (NULL != p)     \
  {                  \
      delete p;      \
      p = NULL;      \
  }
#endif // CHECK_PTR_MACRO


/// verify that types are complete for increased safety
template<class T>
inline void checked_delete(T * x)
{
    // intentionally complex - simplification causes regressions
    typedef char type_must_be_complete[ sizeof(T)? 1: -1 ];
    (void) sizeof(type_must_be_complete);
    delete x;
}

///
#include "uwarnings.h"
#include "uhelpers.h"

/// VC Link Stuff
#ifdef _MSC_VER

#if !defined(BUILD_DLL) && !defined(WITHOUT_DLL)
  #if defined(BUILD_EXE)
    #if (_MSC_VER == 1200)  // VC6
      #if defined(_DEBUG)
        #pragma comment(lib, "libuwin_debug.vc6.dll.lib")
      #else
        #pragma comment(lib, "libuwin_release.vc6.dll.lib")
      #endif // _DEBUG
    #endif //  _MSC_VER == 1200

    #if (_MSC_VER == 1400)  // VC8
      #if defined(_DEBUG)
        #pragma comment(lib, "libuwin_debug.vc8.dll.lib")
      #else
        #pragma comment(lib, "libuwin_release.vc8.dll.lib")
      #endif // _DEBUG
    #endif //  _MSC_VER == 1400

    #if (_MSC_VER == 1500)  // VC9
      #if defined(_DEBUG)
        #pragma comment(lib, "libuwin_debug.vc9.dll.lib")
      #else
        #pragma comment(lib, "libuwin_release.vc9.dll.lib")
      #endif // _DEBUG
    #endif //  _MSC_VER == 1500

    #if (_MSC_VER == 1600)  // VC10
      #if defined(_DEBUG)
        #pragma comment(lib, "libuwin_debug.vc10.dll.lib")
      #else
        #pragma comment(lib, "libuwin_release.vc10.dll.lib")
      #endif // _DEBUG
    #endif //  _MSC_VER == 1600
  #else
    //
    #pragma comment(lib, "comctl32.lib")
    #pragma comment(lib, "netapi32.lib")
    #pragma comment(lib, "opengl32.lib")
    #pragma comment(lib, "glu32.lib")
    #pragma comment(lib, "ws2_32.lib")
    //
    #if (_MSC_VER == 1200)  // VC6
      #pragma comment(lib, "fix/psapi.lib")
      #pragma comment(lib, "fix/iphlpapi.lib")
      #if defined(_DEBUG)
        #pragma comment(lib, "libuwin_debug.vc6.lib")
      #else
        #pragma comment(lib, "libuwin_release.vc6.lib")
      #endif // _DEBUG
    #endif //  _MSC_VER == 1200

    #if (_MSC_VER == 1400)  // VC8
      #if defined(_DEBUG)
        #pragma comment(lib, "libuwin_debug.vc8.lib")
      #else
        #pragma comment(lib, "libuwin_release.vc8.lib")
      #endif // _DEBUG
    #endif

    #if (_MSC_VER == 1500)  // VC9
      #if defined(_DEBUG)
        #pragma comment(lib, "libuwin_debug.vc9.lib")
      #else
        #pragma comment(lib, "libuwin_release.vc9.lib")
      #endif // _DEBUG
    #endif

    #if (_MSC_VER == 1600)  // VC10
      #if defined(_DEBUG)
        #pragma comment(lib, "libuwin_debug.vc10.lib")
      #else
        #pragma comment(lib, "libuwin_release.vc10.lib")
      #endif // _DEBUG
    #endif
  #endif // BUILD_EXE
#else
  #if (_MSC_VER == 1200)  // VC6
    #pragma comment(lib, "fix/psapi.lib")
    #pragma comment(lib, "fix/iphlpapi.lib")
  #endif

  #pragma comment(lib, "netapi32.lib")
  #pragma comment(lib, "shlwapi.lib")
  #pragma comment(lib, "msimg32")

#endif // BUILD_DLL
#endif // _MSC_VER

#ifdef HU_DEBUG
#include "udebug.h"
#define LOG_STRING UDebugger::odprintf
//#define LOG_STRING(format, args...) UDebugger::odprintf(format, ##args)
#else
#define LOG_STRING
#endif

#ifndef GET_X_LPARAM
#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))
#endif

#ifndef ULIB_APPEND
    #define ULIB_APPEND0(x,y) x##y
    #define ULIB_APPEND(x,y) ULIB_APPEND0(x,y)
#endif // ULIB_APPEND

#ifndef UCTCTRL
#define UCTCTRL _T("HUCTCTRL")
#endif

#endif // HUYS_ULIB_H


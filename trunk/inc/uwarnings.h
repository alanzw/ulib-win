#ifndef U_WARNINGS_H
#define U_WARNINGS_H

#ifdef _MSC_VER

/* Disable ridiculous warnings so that the code */
/* compiles cleanly using warning level 4.      */

// nonstandard extension used : nameless struct/union
#pragma warning(disable: 4201)

// unreferenced formal parameter
#pragma warning(disable: 4100)

// signed/unsigned mismatch
#pragma warning(disable: 4018)

// Suppress '... unreferenced inline function has been removed'
#pragma warning(disable: 4514)

// Suppress 'function ... not inlined' for Release builds
#pragma warning(disable: 4710)

// old vc6 warning about long class names
#pragma warning(disable: 4786)

// templates can't be exported
#pragma warning(disable: 4251)

// 'this' : used in base member initializer list
#pragma warning (disable:4355)

//
#pragma warning (disable:4996)

#endif

/// Force all EXEs/DLLs to use STRICT type checking.
#ifndef STRICT
#define STRICT
#endif

/// Show Error Messages for Debugging
#ifndef HU_CONSOLE
  #define  U_ERROR(text) \
    ::MessageBox(NULL, text, _T("Error"), MB_ERROR)
#else
  #define  U_ERROR(text) \
    printf(text)
#endif

#endif // U_WARNINGS_H


#ifndef U_SKIN_H
#define U_SKIN_H


#include "ulib.h"


enum tagSkinButtonStyle
{
    SKBS_NONE   = 0x0000,
    SKBS_SKINED = 0x0001
};

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

ULIB_API BOOL  __stdcall IStart( HINSTANCE hModule, DWORD dwThreadID );
ULIB_API BOOL  __stdcall IStop( void );

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif

#endif


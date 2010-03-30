#ifndef U_INITTERM_H
#define U_INITTERM_H

#ifdef __cplusplus
extern "C"
{
#endif

typedef void (*_PVFV)(void);
typedef int  (*_PIFV)(void);
typedef void (*_PVFI)(int);

void __cdecl _initterm ( _PVFV * pfbegin, _PVFV * pfend );

void __cdecl _atexit_init(void);

extern _PVFV __xc_a[];

extern _PVFV __xc_z[];

void __cdecl _DoExit( void );

#ifdef __cplusplus
}
#endif

#endif // U_INITTERM_H

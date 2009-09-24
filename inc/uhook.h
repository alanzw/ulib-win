/************************************************************************/
/* Hook Chains:
      a list of pointers to special, application-defined
      callback functions called hook procedures.

 * Hook Procedures:

 * Hook Types                                                           */
/************************************************************************/

#ifndef U_HOOK_H
#define  U_HOOK_H

#include "uobject.h"

class ULIB_API UHook : public UUserObject
{
public:
    UHook();
    ~UHook();
private:

};

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

ULIB_API BOOL setMyHook(HWND hWnd);
ULIB_API BOOL clearMyHook();

ULIB_API int getValue();
ULIB_API void setValue(int val);

ULIB_API BOOL setCBTHook(HWND hWnd, HWND hMsgWnd);
ULIB_API WNDPROC getOldProc();

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif

#define UWM_MOUSEMOVE_MSG _T("UWM_MOUSEMOVE_MSG_44E531B1_14D3_11d5_A025_006067718D04")

//
#ifndef IMPLEMENT_MSG
#define IMPLEMENT_MSG(x) static UINT x = ::RegisterWindowMessage(x##_MSG);
#endif

//
IMPLEMENT_MSG(UWM_MOUSEMOVE);

#endif // U_HOOK_H


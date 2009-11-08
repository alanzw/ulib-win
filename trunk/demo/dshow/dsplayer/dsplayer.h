#ifndef _DSPLAYER_H
#define _DSPLAYER_H

#define EXTERN_GUID(itf,l1,s1,s2,c1,c2,c3,c4,c5,c6,c7,c8)  \
  EXTERN_C const IID __declspec(selectany) itf = {l1,s1,s2,{c1,c2,c3,c4,c5,c6,c7,c8}}

#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <stdarg.h>


#endif // _DSPLAYER_H

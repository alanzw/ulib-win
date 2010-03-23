// wtlclock.h

#ifndef H_WTLCLOCK_H
#define H_WTLCLOCK_H

// Change these values to use different versions
#define WINVER      0x0400
//#define _WIN32_WINNT  0x0400
#define _WIN32_IE   0x0400
#define _RICHEDIT_VER   0x0100
#define _WTL_USE_CSTRING

#include <atlbase.h>
#include <atlapp.h>
extern CAppModule _Module;
#include <atlwin.h>
#include <atlframe.h>
#include <atlcrack.h>
#include <atlmisc.h>
#include <atlctrls.h>

// Redefine BEGIN_MSG_MAP to BEGIN_MSG_MAP_EX so that we can use BEGIN_MSG_MAP
// in header files - BEGIN_MSG_MAP_EX makes ClassView freak out. Note that this
// trick is only needed in VC6.
#undef BEGIN_MSG_MAP
#define BEGIN_MSG_MAP(x) BEGIN_MSG_MAP_EX(x)

#if _MSC_VER <= 0x0600
#define DS_SETFONT          0x40L   /* User specified font for Dlg controls */
#define DS_FIXEDSYS         0x0008L
#define DS_SHELLFONT        (DS_SETFONT | DS_FIXEDSYS)
#endif


#endif // H_WTLCLOCK_H

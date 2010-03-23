// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__A61358EA_0111_486D_AA62_11824A89AC2C__INCLUDED_)
#define AFX_STDAFX_H__A61358EA_0111_486D_AA62_11824A89AC2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if _MFC_VER <= 0x0600
#define WINVER 0x0500
#else
#define WINVER 0x0501
#endif

#define  _WIN32_IE 0x0500

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#define IDC_HAND            MAKEINTRESOURCE(32649)

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxtempl.h>

#if _MFC_VER <= 0x0600
#if (_WIN32_IE >= 0x0500)
#define TTM_SETTITLEA           (WM_USER + 32)  // wParam = TTI_*, lParam = char* szTitle
#define TTM_SETTITLEW           (WM_USER + 33)  // wParam = TTI_*, lParam = wchar* szTitle

#define TTS_BALLOON             0x40

#ifdef _UNICODE
#define TTM_SETTITLE            TTM_SETTITLEW
#else
#define TTM_SETTITLE            TTM_SETTITLEA
#endif

#endif

#define TTI_NONE                0
#define TTI_INFO                1
#define TTI_WARNING             2
#define TTI_ERROR               3

#define TTI_INFO_LARGE			4       
#endif


// VC6
#if defined(_MSC_VER) && _MSC_VER == 1200

#ifndef ULONG_PTR
#define ULONG_PTR unsigned long*
#endif

//#include <Specstrings.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;

// VS2005
#else

#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;

#endif


#define WM_USER_THREAD_FINISHED (WM_USER+0x101)
#define WM_USER_THREAD_UPDATE_PROGRESS (WM_USER+0x102)


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A61358EA_0111_486D_AA62_11824A89AC2C__INCLUDED_)

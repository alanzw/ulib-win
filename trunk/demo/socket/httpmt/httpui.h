//
// HTTPUI.H		Hyper Text Transfer Protocol Server
//
//				Multi-Threaded Model User Interface
//

#ifndef _HTTPUI_H
#define _HTTPUI_H

#include <winsock2.h>
#include <windowsx.h>
#include <commctrl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "httpMT.h"
#include "resource.h"


////////////////////////////////////////////////////////////

//
// HTTPMT.C -- User Interface Function Prototypes
//
BOOL OnInitDialog(HWND hwnd, 
				  HWND hwndFocus, 
				  LPARAM lParam); 

void OnCommand(HWND hwnd, 
			   int nId, 
			   HWND hwndCtl, 
			   UINT codeNotify); 

BOOL CALLBACK MainWndProc(HWND hwnd, 
						  UINT uMsg, 
						  WPARAM wParam, 
				          LPARAM lParam);

BOOL CALLBACK InfoWndProc(HWND hwnd, 
						  UINT uMsg, 
						  WPARAM wParam,
						  LPARAM lParam);

void FileRequested(HWND hwnd, 
				   int nError, 
				   LPCSTR lpFileName);

void Statistics(HWND hwnd, LPHTTPSTATS lpStats);

void ShowEvent(HWND hwnd, LPCSTR lpEvent);

////////////////////////////////////////////////////////////

//
// Message Handler macro for dialog boxes
//
#define HANDLE_DLG_MSG(hwnd, message, fn)				\
		case(message):									\
			return (BOOL) HANDLE_##message((hwnd),		\
										   (wParam),	\
										   (lParam),	\
										   (fn))	

#endif // _HTTPUI_H


/*
 * =====================================================================================
 *
 *       Filename:  uhotkey.cpp
 *
 *    Description:  HotKey Control
 *
 *        Version:  1.0
 *        Created:  2009-7-19 10:04:11
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "uhotkey.h"

UHotKey::UHotKey(HWND hParent, UINT nResource, HINSTANCE hInst)
  : UControl(hParent, nResource, hInst)
{}

UHotKey::~UHotKey()
{}

BOOL UHotKey::create()
{
    // Ensure that the common control DLL is loaded.
    InitCommonControls();

    return UControl::create(_T("msctls_hotkey32"));
}

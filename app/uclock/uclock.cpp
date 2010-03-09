#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "uclock.h"

UClock::UClock(HWND hParent, UINT nID, HINSTANCE hInst)
: UStatic(hParent, nID, hInst)
{
}

UClock::UClock()
{
}

UClock::~UClock()
{}


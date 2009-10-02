#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "uterminal.h"

UTerminal::UTerminal(HWND hParent, UINT nID, HINSTANCE hInst)
: UStatic(hParent, nID, hInst)
{
}

UTerminal::UTerminal()
{
}

UTerminal::~UTerminal()
{}


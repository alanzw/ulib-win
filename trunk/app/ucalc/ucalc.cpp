#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "ucalc.h"

UCalculator::UCalculator(HWND hParent, UINT nID, HINSTANCE hInst)
: UStatic(hParent, nID, hInst)
{
}

UCalculator::UCalculator()
{
}

UCalculator::~UCalculator()
{}


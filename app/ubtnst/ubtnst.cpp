#include <windows.h>
#include <tchar.h>

#include "ubtnst.h"

UButtonST::UButtonST(HWND hParent, UINT nID, HINSTANCE hInst)
: UButton(hParent, nID, hInst)
{

}

UButtonST::UButtonST()
{
}

UButtonST::UButtonST(UBaseWindow *pWndParent, UINT nID)
: UButton(pWndParent, nID)
{
}

UButtonST::~UButtonST()
{}


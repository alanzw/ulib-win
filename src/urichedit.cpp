#include <windows.h>
#include <tchar.h>
#include <commctrl.h>
#include <richedit.h>

#include "urichedit.h"

URichEdit::URichEdit(HWND hParent, UINT nResource, HINSTANCE hInst)
: UControl(hParent, nResource, hInst)
{}

URichEdit::~URichEdit()
{}

BOOL URichEdit::create()
{
    ::LoadLibrary(TEXT("Riched20.dll"));
    return UControl::create(RICHEDIT_CLASS, _T("Type here."));
}


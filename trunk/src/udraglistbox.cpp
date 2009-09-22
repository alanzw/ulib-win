#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "udraglistbox.h"

UDragListBox::UDragListBox(HWND hParent, UINT nResource, HINSTANCE hInst)
    : UListBox(hParent, nResource, hInst)
{}

UDragListBox::~UDragListBox()
{}

BOOL UDragListBox::create()
{
    UListBox::create();
    return ::MakeDragList(m_hSelf);
}



#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "ueditpad.h"

UEditPad::UEditPad()
{
}

UEditPad::UEditPad(UBaseWindow *pWndParent)
: UBaseWindow(pWndParent)
{
}

UEditPad::~UEditPad()
{}

BOOL UEditPad::onClose()
{
    this->hide();
    return FALSE;
}

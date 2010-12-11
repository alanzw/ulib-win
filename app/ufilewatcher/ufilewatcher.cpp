#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "ufilewatcher.h"

UFileWatcher::UFileWatcher()
{
}

UFileWatcher::UFileWatcher(UBaseWindow *pWndParent)
: UBaseWindow(pWndParent)
{
}

UFileWatcher::~UFileWatcher()
{}

BOOL UFileWatcher::onClose()
{
    this->hide();
    return FALSE;
}

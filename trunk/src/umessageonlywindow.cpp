#define  WINVER 0x0500

#include <windows.h>
#include <tchar.h>

#include "umessageonlywindow.h"
#include "uwndclassx.h"
#include "umsg.h"

UMessageOnlyWindow::UMessageOnlyWindow()
: UBaseWindow(HWND_MESSAGE)
{}

UMessageOnlyWindow::~UMessageOnlyWindow()
{}

BOOL UMessageOnlyWindow::setMessageOnly( HWND hwnd )
{
    ::SetParent(hwnd, HWND_MESSAGE);
    return FALSE;
}

HWND UMessageOnlyWindow::findWindowEx( LPCTSTR lpszClass, LPCTSTR lpszWindow )
{
    return ::FindWindowEx(HWND_MESSAGE, NULL, lpszClass, lpszWindow);
}

BOOL UMessageOnlyWindow::onMessage( UINT uMessage, WPARAM wParam, LPARAM lParam )
{
	if (UMessageOnlyWindow::WM_CUSTOM_MSG == uMessage)
	{
		return this->msgHandler();
	}

	return UBaseWindow::onMessage(uMessage, wParam, lParam);
}

BOOL UMessageOnlyWindow::msgHandler()
{
	::showMsg(_T("Got it"), _T("Info"), HWND_DESKTOP);
	return FALSE;
}


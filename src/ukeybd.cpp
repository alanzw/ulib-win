#include <windows.h>
#include <tchar.h>

#include "ukeybd.h"

void UKeyboard::setNumLock(BOOL bState /* = TRUE */)
{
     BYTE keyState[256];

     ::GetKeyboardState((LPBYTE)&keyState);
     if( (bState && !(keyState[VK_NUMLOCK] & 1)) ||
         (!bState && (keyState[VK_NUMLOCK] & 1)) )
     {
        // Simulate a key press
        keybd_event( VK_NUMLOCK,
                     0x45,
                     KEYEVENTF_EXTENDEDKEY | 0,
                     0 );

         // Simulate a key release
         keybd_event( VK_NUMLOCK,
                      0x45,
                      KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP,
                      0);
      }
}

int UKeyboard::getType()
{
    return ::GetKeyboardType(0);
}


bool USendKey::activeTargetWnd( HWND hWnd )
{
    if (NULL == hWnd)
    {
        return false;
    }

    ::SendMessage(hWnd, WM_SYSCOMMAND, SC_HOTKEY, (LPARAM) hWnd);
    ::SendMessage(hWnd, WM_SYSCOMMAND, SC_RESTORE, (LPARAM) hWnd);

    ::ShowWindow(hWnd, SW_SHOW);
    ::SetForegroundWindow(hWnd);
    ::SetFocus(hWnd);

    return TRUE;
}

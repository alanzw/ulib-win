#include "resource.h"
#include <windows.h>
#include <tchar.h>

#include "umailslot.h"
#include "uconsole.h"

int main()
{
    UMailSlot mslot;

    mslot.open(_T("\\\\.\\mailslot\\huys"));
    
    mslot.write( _T("Message 1 for mailslot"));
	mslot.write( _T("Message 2 for mailslot"));

	Sleep(3000); // Sleep 3 seconds

	mslot.write(_T("Message 3 for mailslot"));

    return 0;
}


#include "resource.h"
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>

#include "umailslot.h"
#include "uconsole.h"

int main(int argc, char *argv[])
{
    UMailSlot mslot;

    mslot.create(_T("\\\\.\\mailslot\\huys"));

    	// In a loop, check for new messages every 5 seconds.
	while (TRUE)
	{
		UConsole::PrintStdout(_T("Checking new messages...\n"));
		mslot.read(NULL);
		Sleep(5000); // Sleep 5 seconds
	}

    return 0;
}

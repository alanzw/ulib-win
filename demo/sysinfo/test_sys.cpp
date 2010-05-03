#include "resource.h"

#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <tchar.h>
using namespace std;

#include "usys.h"
#include "umsg.h"

int main(int argc, char *argv[])
{
    
    HGLOBAL h= ::GlobalAlloc(GMEM_FIXED|GMEM_ZEROINIT, 100);
    
    int *p = (int *)h;
    
	cout << p << " : " << GlobalSize(h) << endl;
    
    h = ::GlobalReAlloc(h, 120, 0);
    
    p = (int *)h;
    
    cout << p << " : " << GlobalSize(h) << endl;
    
    h = ::GlobalReAlloc(h, 140, 0);
    
	if (!h)
	{
		showError("Error");
	}
	
    p = (int *)h;
    
    cout << p << " : " << GlobalSize(h) << endl;    
    
    ::GlobalFree(h);
    
    
    USystem usys;

    cout << "Screen Width: " << usys.getScreenWidth()  << " pixels"  << endl;
    cout << "Screen Height: " << usys.getScreenHeight() << " pixels" << endl;

    //
    cout << "Minimized window arranged: " << usys.getArrange() << endl;
    cout << "Arrange mode: "  << endl;
    cout << "    " << usys.getArrangeText() << endl;
    //
    cout << "Boot Mode: " << usys.getBootMode() << " -- " << usys.getBootModeText() << endl;

    //
    cout << "Monitor Number: " << usys.getMonitorNum() << endl;
    //
    cout << "Mouse Number: " << usys.getMouseButtonNum() << endl;

    //
    cout << "Computer Name: " << usys.getComputerName() << endl;

   SYSTEM_INFO siSysInfo;

   // Copy the hardware information to the SYSTEM_INFO structure.

   GetSystemInfo(&siSysInfo);

   // Display the contents of the SYSTEM_INFO structure.

    printf("Hardware information: \n");
    printf("  OEM ID: %u\n", siSysInfo.dwOemId);
    printf("  Number of processors: %u\n",
    siSysInfo.dwNumberOfProcessors);
    printf("  Page size: %u\n", siSysInfo.dwPageSize);
    printf("  Processor type: %u\n", siSysInfo.dwProcessorType);
    printf("  Minimum application address: %lx\n",
    siSysInfo.lpMinimumApplicationAddress);
    printf("  Maximum application address: %lx\n",
    siSysInfo.lpMaximumApplicationAddress);
    printf("  Active processor mask: %u\n",
    siSysInfo.dwActiveProcessorMask);
    //
    cout << "Press any key to exit..." << endl;

    cin.ignore();

    return 0;
}



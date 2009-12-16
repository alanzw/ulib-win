#include "resource.h"

#include <windows.h>
#include <iostream>

using namespace std;

#include "usys.h"

int main(int argc, char *argv[])
{
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



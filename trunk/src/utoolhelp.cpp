#include <windows.h>
#include <tchar.h>
#include <tlhelp32.h>

#include <stdio.h>

#include "utoolhelp.h"

UToolHelp::UToolHelp()
: m_hSnapshot(INVALID_HANDLE_VALUE)
{

}

bool UToolHelp::snapall()
{
    m_hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);
    return (INVALID_HANDLE_VALUE != m_hSnapshot);
}

void printError( const TCHAR* msg )
{
    DWORD eNum;
    TCHAR sysMsg[256];
    TCHAR* p;

    eNum = GetLastError( );
    FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
    NULL, eNum,
    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
    sysMsg, 256, NULL );

    // Trim the end of the line and terminate it with a null
    p = sysMsg;
    while( ( *p > 31 ) || ( *p == 9 ) )
    ++p;
    do { *p-- = 0; } while( ( p >= sysMsg ) &&
    ( ( *p == '.' ) || ( *p < 33 ) ) );

    // Display the message
    _tprintf( TEXT("\n  WARNING: %s failed with error %d (%s)"), msg, static_cast<int>(eNum), sysMsg );
}



bool UToolHelp::listProcessModules( DWORD dwPID )
{
    HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
    MODULEENTRY32 me32;

    // Take a snapshot of all modules in the specified process.
    hModuleSnap = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, dwPID );
    if( hModuleSnap == INVALID_HANDLE_VALUE )
    {
        printError( TEXT("CreateToolhelp32Snapshot (of modules)") );
        return( FALSE );
    }

    // Set the size of the structure before using it.
    me32.dwSize = sizeof( MODULEENTRY32 );

    // Retrieve information about the first module,
    // and exit if unsuccessful
    if( !Module32First( hModuleSnap, &me32 ) )
    {
        printError( TEXT("Module32First") );  // show cause of failure
        CloseHandle( hModuleSnap );           // clean the snapshot object
        return( FALSE );
    }

    // Now walk the module list of the process,
    // and display information about each module
    do
    {
        _tprintf( TEXT("\n\n     MODULE NAME:     %s"),   me32.szModule );
        _tprintf( TEXT("\n     Executable     = %s"),     me32.szExePath );
        printf( "\n     Process ID     = 0x%08X",         (unsigned int)me32.th32ProcessID );
        printf( "\n     Ref count (g)  = 0x%04X",     (unsigned int)me32.GlblcntUsage );
        printf( "\n     Ref count (p)  = 0x%04X",     (unsigned int)me32.ProccntUsage );
        printf( "\n     Base address   = 0x%08X", (unsigned int)(DWORD) me32.modBaseAddr );
        printf( "\n     Base size      = %d",             (unsigned int)me32.modBaseSize );

    } while( Module32Next( hModuleSnap, &me32 ) );

    CloseHandle( hModuleSnap );

    return true;
}

bool UToolHelp::listProcessThreads( DWORD dwOwnerPID ) 
{ 
    HANDLE hThreadSnap = INVALID_HANDLE_VALUE; 
    THREADENTRY32 te32; 

    // Take a snapshot of all running threads  
    hThreadSnap = CreateToolhelp32Snapshot( TH32CS_SNAPTHREAD, 0 ); 
    if( hThreadSnap == INVALID_HANDLE_VALUE ) 
        return( FALSE ); 

    // Fill in the size of the structure before using it. 
    te32.dwSize = sizeof(THREADENTRY32); 

    // Retrieve information about the first thread,
    // and exit if unsuccessful
    if( !Thread32First( hThreadSnap, &te32 ) ) 
    {
        printError( TEXT("Thread32First") ); // show cause of failure
        CloseHandle( hThreadSnap );          // clean the snapshot object
        return( FALSE );
    }

    // Now walk the thread list of the system,
    // and display information about each thread
    // associated with the specified process
    do 
    { 
        if( te32.th32OwnerProcessID == dwOwnerPID )
        {
            printf( "\n\n     THREAD ID      = 0x%08X", (unsigned int)te32.th32ThreadID ); 
            printf( "\n     Base priority  = %d", (unsigned int)te32.tpBasePri ); 
            printf( "\n     Delta priority = %d", (unsigned int)te32.tpDeltaPri ); 
        }
    } while( Thread32Next(hThreadSnap, &te32 ) ); 

    CloseHandle( hThreadSnap );
    return true;
}

bool UToolHelp::listProcesses()
{
    HANDLE hProcessSnap;
    HANDLE hProcess;
    PROCESSENTRY32 pe32;
    DWORD dwPriorityClass;

    // Take a snapshot of all processes in the system.
    hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
    if( hProcessSnap == INVALID_HANDLE_VALUE )
    {
        printError( TEXT("CreateToolhelp32Snapshot (of processes)") );
        return false;
    }

    // Set the size of the structure before using it.
    pe32.dwSize = sizeof( PROCESSENTRY32 );

    // Retrieve information about the first process,
    // and exit if unsuccessful
    if( !Process32First( hProcessSnap, &pe32 ) )
    {
        printError( TEXT("Process32First") ); // show cause of failure
        CloseHandle( hProcessSnap );          // clean the snapshot object
        return false;
    }

    // Now walk the snapshot of processes, and
    // display information about each process in turn
    do
    {
        printf( "\n\n=====================================================" );
        _tprintf( TEXT("\nPROCESS NAME:  %s"), pe32.szExeFile );
        printf( "\n-----------------------------------------------------" );

        // Retrieve the priority class.
        dwPriorityClass = 0;
        hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID );
        if( hProcess == NULL )
            printError( TEXT("OpenProcess") );
        else
        {
            dwPriorityClass = GetPriorityClass( hProcess );
            if( !dwPriorityClass )
                printError( TEXT("GetPriorityClass") );
            CloseHandle( hProcess );
        }

        printf( "\n  Process ID        = 0x%08X", (unsigned int)pe32.th32ProcessID );
        printf( "\n  Thread count      = %d",   (unsigned int)pe32.cntThreads );
        printf( "\n  Parent process ID = 0x%08X", (unsigned int)pe32.th32ParentProcessID );
        printf( "\n  Priority base     = %d", (unsigned int)pe32.pcPriClassBase );
        if( dwPriorityClass )
            printf( "\n  Priority class    = %d", (unsigned int)dwPriorityClass );

    } while( Process32Next( hProcessSnap, &pe32 ) );

    CloseHandle( hProcessSnap );
    return true;
}

bool UToolHelp::listHeaps( DWORD dwPID )
{
    HEAPLIST32 hl;

    HANDLE hHeapSnap = CreateToolhelp32Snapshot(TH32CS_SNAPHEAPLIST, dwPID);

    hl.dwSize = sizeof(HEAPLIST32);

    if ( hHeapSnap == INVALID_HANDLE_VALUE )
    {
        printf ("CreateToolhelp32Snapshot failed (%d)\n", (unsigned int)GetLastError());
        return false;
    }

    if( Heap32ListFirst( hHeapSnap, &hl ) )
    {
        do
        {
            HEAPENTRY32 he;
            ZeroMemory(&he, sizeof(HEAPENTRY32));
            he.dwSize = sizeof(HEAPENTRY32);

            if( Heap32First( &he, GetCurrentProcessId(), hl.th32HeapID ) )
            {
                printf( "\nHeap ID: %d\n", (unsigned int)hl.th32HeapID );
                do
                {
                    printf( "Block size: %d\n", (unsigned int)he.dwBlockSize );

                    he.dwSize = sizeof(HEAPENTRY32);
                } while( Heap32Next(&he) );
            }
            hl.dwSize = sizeof(HEAPLIST32);
        } while (Heap32ListNext( hHeapSnap, &hl ));
    }
    else printf ("Cannot list first heap (%d)\n", (unsigned int)GetLastError());

    CloseHandle(hHeapSnap);

    return true;
}

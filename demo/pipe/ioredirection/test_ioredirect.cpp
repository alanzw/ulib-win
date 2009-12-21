#include <windows.h>
#include <tchar.h>
#include <stdio.h>
// #include <strsafe.h> // missing in MinGW

#define BUFSIZE 4096

HANDLE g_hChildStd_IN_RD = NULL;
HANDLE g_hChildStd_IN_WR = NULL;
HANDLE g_hChildStd_OUT_RD = NULL;
HANDLE g_hChildStd_OUT_WR = NULL;

HANDLE g_hInputFile = NULL;

void CreateChildProcess(void);
void WriteToPipe(void);
void ReadFromPipe(void);
void ErrorExit(PTSTR);

int _tmain(int argc, TCHAR *argv[])
{
    SECURITY_ATTRIBUTES saAttr;

    printf("\n->Start of parent execution.\n");

    // Set the bInheritHandle flag so pipe handles are inherited.
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    // Create a pipe for the child process's STDOUT.
    if ( ! CreatePipe(&g_hChildStd_OUT_RD, &g_hChildStd_OUT_WR, &saAttr, 0) )
    {  
        ErrorExit(TEXT("StdoutRd CreatePipe"));
    }
    // Ensure the read handle to the pipe for STDOUT is not inherited.
    if ( ! SetHandleInformation(g_hChildStd_OUT_RD, HANDLE_FLAG_INHERIT, 0) )
    {
        ErrorExit(TEXT("Stdout SetHandleInformation"));
    }
    // Create a pipe for the child process's STDIN.
    if (! CreatePipe(&g_hChildStd_IN_RD, &g_hChildStd_IN_WR, &saAttr, 0))
    {
        ErrorExit(TEXT("Stdin CreatePipe"));
    }
    // Ensure the write handle to the pipe for STDIN is not inherited.
    if ( ! SetHandleInformation(g_hChildStd_IN_WR, HANDLE_FLAG_INHERIT, 0) )
    {
        ErrorExit(TEXT("Stdin SetHandleInformation"));
    }
    
    // Create the child process.
    CreateChildProcess();

    // Get a handle to an input file for the parent.
    // This example assumes a plain text file and uses string output to verify data flow.
    if (argc == 1)
    {
        ErrorExit(TEXT("Please specify an input file.\n"));
    }
   
    g_hInputFile = CreateFile(
       argv[1],
       GENERIC_READ,
       0,
       NULL,
       OPEN_EXISTING,
       FILE_ATTRIBUTE_READONLY,
       NULL);

    if ( g_hInputFile == INVALID_HANDLE_VALUE )
    {
        ErrorExit(TEXT("CreateFile"));
    }
    
    // Write to the pipe that is the standard input for a child process.
    // Data is written to the pipe's buffers, so it is not necessary to wait
    // until the child process is running before writing data.
    WriteToPipe();
    printf( "\n->Contents of %s written to child STDIN pipe.\n", argv[1]);

    // Read from pipe that is the standard output for child process.
    printf( "\n->Contents of child process STDOUT:\n\n", argv[1]);
    ReadFromPipe();
    
    printf("\n->End of parent execution.\n");

    // The remaining open handles are cleaned up when this process terminates.
    // To avoid resource leaks in a larger application, close handles explicitly.

    return 0;
}

void CreateChildProcess()
// Create a child process that uses the previously created pipes for STDIN and STDOUT.
{
    TCHAR szCmdline[]=TEXT("child_ioredirect.exe");
    PROCESS_INFORMATION piProcInfo;
    STARTUPINFO siStartInfo;
    BOOL bSuccess = FALSE;

    // Set up members of the PROCESS_INFORMATION structure.
    ZeroMemory( &piProcInfo, sizeof(PROCESS_INFORMATION) );

    // Set up members of the STARTUPINFO structure.
    // This structure specifies the STDIN and STDOUT handles for redirection.
    ZeroMemory( &siStartInfo, sizeof(STARTUPINFO) );
    siStartInfo.cb = sizeof(STARTUPINFO);
    siStartInfo.hStdError = g_hChildStd_OUT_WR;
    siStartInfo.hStdOutput = g_hChildStd_OUT_WR;
    siStartInfo.hStdInput = g_hChildStd_IN_RD;
    siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

    // Create the child process.
    bSuccess = CreateProcess(NULL,
      szCmdline,     // command line
      NULL,          // process security attributes
      NULL,          // primary thread security attributes
      TRUE,          // handles are inherited
      0,             // creation flags
      NULL,          // use parent's environment
      NULL,          // use parent's current directory
      &siStartInfo,  // STARTUPINFO pointer
      &piProcInfo);  // receives PROCESS_INFORMATION

    // If an error occurs, exit the application.
    if ( ! bSuccess )
    {
        ErrorExit(TEXT("CreateProcess"));
    }
    else
    {
        // Close handles to the child process and its primary thread.
        // Some applications might keep these handles to monitor the status
        // of the child process, for example.
        CloseHandle(piProcInfo.hProcess);
        CloseHandle(piProcInfo.hThread);
    }
}

void WriteToPipe(void)
// Read from a file and write its contents to the pipe for the child's STDIN.
// Stop when there is no more data.
{
    DWORD dwRead, dwWritten;
    CHAR chBuf[BUFSIZE];
    BOOL bSuccess = FALSE;

    for (;;)
    {
        bSuccess = ReadFile(g_hInputFile, chBuf, BUFSIZE, &dwRead, NULL);
        if ( ! bSuccess || dwRead == 0 ) break;

        bSuccess = WriteFile(g_hChildStd_IN_WR, chBuf, dwRead, &dwWritten, NULL);
        if ( ! bSuccess ) break;
    }

// Close the pipe handle so the child process stops reading.

    if ( ! CloseHandle(g_hChildStd_IN_WR) )
    {
        ErrorExit(TEXT("StdInWr CloseHandle"));
    }
}

void ReadFromPipe(void)
// Read output from the child process's pipe for STDOUT
// and write to the parent process's pipe for STDOUT.
// Stop when there is no more data.
{
    DWORD dwRead, dwWritten;
    CHAR chBuf[BUFSIZE];
    BOOL bSuccess = FALSE;
    HANDLE hParentStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

    // Close the write end of the pipe before reading from the
    // read end of the pipe, to control child process execution.
    // The pipe is assumed to have enough buffer space to hold the
    // data the child process has already written to it.
    if (!CloseHandle(g_hChildStd_OUT_WR))
    {
        ErrorExit(TEXT("StdOutWr CloseHandle"));
    }
    
    for (;;)
    {
        bSuccess = ReadFile( g_hChildStd_OUT_RD, chBuf, BUFSIZE, &dwRead, NULL);
        if( ! bSuccess || dwRead == 0 ) break;

        bSuccess = WriteFile(hParentStdOut, chBuf, dwRead, &dwWritten, NULL);
        if (! bSuccess ) break;
   }
}

void ErrorExit(PTSTR lpszFunction)
// Format a readable error message, display a message box,
// and exit from the application.
{
    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError();

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
        (lstrlen((LPCTSTR)lpMsgBuf)+lstrlen((LPCTSTR)lpszFunction)+40)*sizeof(TCHAR));
    //
    //StringCchPrintf((LPTSTR)lpDisplayBuf,
    //    LocalSize(lpDisplayBuf) / sizeof(TCHAR),
    //    TEXT("%s failed with error %d: %s"),
    //    lpszFunction, dw, lpMsgBuf);
    wsprintf((LPTSTR)lpDisplayBuf,
            TEXT("%s failed with error %d: %s"),
            lpszFunction, dw, lpMsgBuf);
    MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
    ExitProcess(1);
}

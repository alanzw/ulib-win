#include <windows.h>
#include <stdio.h>

#include "upipe.h"

UPipe::UPipe()
{
    ;
}

UPipe::~UPipe()
{
    ;
}

BOOL UPipe::create()
{
    SECURITY_ATTRIBUTES saAttr;

    // Set the bInheritHandle flag so pipe handles are inherited.

    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    return ::CreatePipe(&m_hRead, &m_hWrite, &saAttr, 0);
}

BOOL UPipe::startChild(char *lpszChildName)
{
    // Set up members of the PROCESS_INFORMATION structure.
    ::ZeroMemory( &m_piChild, sizeof(PROCESS_INFORMATION) );

    // Set up members of the STARTUPINFO structure.
    ::ZeroMemory( &m_siChild, sizeof(STARTUPINFO) );

    m_siChild.cb = sizeof(STARTUPINFO);


    HANDLE hTemp = ::GetStdHandle(STD_OUTPUT_HANDLE);
    ::SetStdHandle(STD_OUTPUT_HANDLE, m_hWrite);
    ::GetStartupInfo(&m_siChild);

    m_siChild.dwFlags |= STARTF_USESTDHANDLES;
    m_siChild.hStdError = m_hWrite;
    m_siChild.hStdOutput = m_hWrite;

    BOOL bRet;
    bRet = CreateProcess( NULL,
                          lpszChildName,
                          NULL,
                          NULL,
                          TRUE,
                          0,
                          NULL,
                          NULL,
                          &m_siChild,
                          &m_piChild);

    ::SetStdHandle(STD_OUTPUT_HANDLE, hTemp);

    if (bRet == TRUE) //
        printf("Child Process created successfully!\n");
    else
        printf("CreateProcess Failed: %d\n", GetLastError());

    ::WaitForSingleObject(m_piChild.hProcess, INFINITE);

    return bRet;
}

void UPipe::read(char *sOutFilename /* = "out.dat" */)
{
    FILE *fp;
    fp = fopen(sOutFilename, "w");

    CloseHandle(m_hWrite);

    while (ReadFile(m_hRead, m_szReadBuffer, 100, &m_nReadNum, NULL))
    {
        m_szReadBuffer[m_nReadNum] = '\0';
        fprintf(fp, "%s", m_szReadBuffer);
    }
    fclose(fp);

    if (GetLastError() == ERROR_BROKEN_PIPE) //
        printf("Pipe closed by child\n");
    else
        printf("Pipe read error: %d\n", GetLastError());
}


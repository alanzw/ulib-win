#include "resource.h"

#include <stdio.h>
#include <windows.h>

int EnablePriv (char *szPriv)
{
    HANDLE hToken = 0;
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
    {
        printf("OpenProcessToken() failed -> %d", GetLastError());
        return -1;
    }

    TOKEN_PRIVILEGES newPrivs;
    if (!LookupPrivilegeValue (NULL, szPriv, &newPrivs.Privileges[0].Luid))
    {
        printf("LookupPrivilegeValue() failed -> %d", GetLastError());
        CloseHandle (hToken);
        return -1;
    }

    newPrivs.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    newPrivs.PrivilegeCount = 1;

    if (!AdjustTokenPrivileges(hToken, FALSE, &newPrivs, 0, NULL, NULL))
    {
        printf("AdjustTokenPrivileges() failed -> %d", GetLastError());
        CloseHandle (hToken);
        return -1;
    }

    CloseHandle (hToken);
    return 0;

}

void DoIt(char *szFileName, DWORD dwFlags)
{
    printf("\n\nAttempting to read %s, with 0x%x flags\n", szFileName, dwFlags);

    HANDLE hFile = CreateFile(szFileName, 
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        dwFlags,
        NULL);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        printf("CreateFile() failed -> %d", GetLastError());
        return;

    }

    char buff[128];
    DWORD cbRead=0, cbBuff = sizeof buff;

    ZeroMemory(buff, sizeof buff);
    if (ReadFile(hFile, buff, cbBuff, &cbRead, NULL))
    {
        printf("Success, read %d bytes\n\nText is: %s", cbRead, buff);
    } else {
        printf("ReadFile() failed -> %d", GetLastError());
    }

    CloseHandle(hFile);
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        printf("Usage: %s <filename>", argv[0]);
        return -1;
    }

    // Need to enable backup priv first.
    if (EnablePriv(SE_BACKUP_NAME) == -1)
    {
        return -2;
    }
    
    // Try with no backup flag - should get access denied.
    DoIt(argv[1], FILE_ATTRIBUTE_NORMAL);

    // Try with backup flag - should work!
    DoIt(argv[1], FILE_ATTRIBUTE_NORMAL | FILE_FLAG_BACKUP_SEMANTICS);

    return 0;
}


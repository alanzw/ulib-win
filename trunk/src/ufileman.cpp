#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <string.h>

#include "ufileman.h"

void UFileMan::printFile(const TCHAR *filename)
{
    FILE *phile;
    char foo[20];
    int i;

    phile = fopen(filename, "r");
    if (phile)
    {
        i = 0;
        while (fgets(foo, sizeof(foo), phile))
        {
           // when strlen(foo) < sizeof(foo) - 1,
           //  '\n' will be add to the tail.
           if ('\n' == foo[strlen(foo)-1])
           {
                foo[strlen(foo)-1] = '\0';
           }
//            puts(foo);
            printf("%d: %s\n", i, foo);
            i++;
        }
        fclose(phile);
    }
}

bool UFileMan::isFileReady(const TCHAR *pszFileName)
{
    WIN32_FIND_DATA findData;
    HANDLE hFind = INVALID_HANDLE_VALUE;

    hFind = FindFirstFile(pszFileName, &findData);

    if ( INVALID_HANDLE_VALUE == hFind)
    {
        return FALSE;
    }

    FindClose(hFind);
    return TRUE;
}

bool UFileMan::listDirFiles(const TCHAR *dir)
{
    if (NULL == dir)
    {
        return FALSE;
    }

    WIN32_FIND_DATA findData;
    HANDLE hFind = INVALID_HANDLE_VALUE;

    hFind = FindFirstFile(dir, &findData);
    if ( INVALID_HANDLE_VALUE == hFind)
    {
        return FALSE;
    }

    // Check if it is a directory
    if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
    {
        return FALSE;
    }

    printf("Files in %s\n", dir);
    while (0 != FindNextFile(hFind, &findData))
    {
        printf("\t%s\n", findData.cFileName);
    }

    FindClose(hFind);
    return TRUE;
}

DWORD UFileMan::getFileAttributes( LPCTSTR sFilename )
{
    return ::GetFileAttributes(sFilename);   
}

BOOL UFileMan::setFileAttributes( LPCTSTR sFilename, DWORD dwFlags )
{
    return ::SetFileAttributes(sFilename, dwFlags);   
}
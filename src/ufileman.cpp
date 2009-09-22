#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <string.h>

#include "ufileman.h"

void UFileMan::printFile(const TCHAR *filename)
{
    FILE *phile;
    char foo[10];
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
    HANDLE hFind;

    hFind = FindFirstFile(pszFileName, &findData);

    if ( INVALID_HANDLE_VALUE == hFind)
    {
        return FALSE;
    }

    FindClose(hFind);
    return TRUE;
}

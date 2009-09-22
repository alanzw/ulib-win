#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#include "unls.h"

int main(int argc, char *argv[])
{
    printf("Default locale id: %#.4x\n", UNls::getUserDefaultLCID());

    printf("Default language id: %#.4x\n", UNls::getUserDefaultLangID());

    printf("System default locale id: %#.4x\n", UNls::getSystemDefaultLCID());

    printf("System default language id: %#.4x\n", UNls::getSystemDefaultLangID());

    WCHAR wbuf[LOCALE_NAME_MAX_LENGTH] = {0};

    UNls::getUserDefaultLocaleName(wbuf);

    wprintf(L"Default locale name: %s\n", wbuf);

    getchar();

    return 0;
}

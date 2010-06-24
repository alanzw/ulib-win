#include "resource.h"
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>

#include "uconsole.h"
#include "utimer.h"

using namespace UConsole;

int main(int argc, char *argv[])
{
    int i;
    int x = 4;
    int y = 4;
    clrscr();
    for (i = 0; i < 8; i++)
    {
        gotoxy(x, y);
        setrgb((ColorOnColor)i);
        PrintStdout("HELP!\n");
        gotoxy(x, y+2);
        PrintStdoutFormat("%d\n", i);
        x = x + 8;
    }
    x = 4;
    y = 8;
    for (i = 8; i < 16; i++)
    {
        gotoxy(x, y);
        setrgb((ColorOnColor)i);
        PrintStdout("ME!\n");
        gotoxy(x, y+2);
        PrintStdoutFormat("%d\n", i);
        x = x + 8;
    }
    setrgb(BLUE_ON_BLACK);
    gotoxy(4, 16);
    PrintStdout("Press any key.");
    getch();
    setrgb(WHITE_ON_BlACK);
    PrintStdout("\n");
    gotoxy(4, 18);

//    UPAUSE;

    HANDLE hOut = ::GetStdHandle(STD_OUTPUT_HANDLE);

    if (INVALID_HANDLE_VALUE == hOut)
        fprintf(stderr, "Error...\n");

    PrintMsg(hOut, _T("Write to console...\n"));

    TCHAR response[256];

    ConsolePrompt("Hello Prompt:", response, 256, TRUE);

    PrintStrings(hOut, "You enter: ", response, "\n", NULL);

    getchar();

    HANDLE hFile = ::CreateFile(_T("build.bat"), GENERIC_READ,
        0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    CatFileEx(hFile, hOut);

    ::CloseHandle(hFile);
//    ::CloseHandle(hOut);

    getchar();

    UTimer ut;

    ut.beginQuery();
    Ascii2Unicode(_T("test.xx"), _T("test_uni.xx"), FALSE);
    ut.stopQuery();
    PrintStdoutFormat("Time lasts: %lf s\n", ut.getSecondsQuery());

    ut.beginQuery();
    Ascii2UnicodeOpt(_T("test.xx"), _T("test_uni.xx"), FALSE);
    ut.stopQuery();
    PrintStdoutFormat("Time lasts: %lf s\n", ut.getSecondsQuery());

    getchar();

    ReportError("XXXXX", 0, TRUE);

    return 0;
}



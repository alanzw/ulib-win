#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include "uconsole.h"

namespace UConsole
{

/* Write the messages to the output handle. */
BOOL PrintStrings (HANDLE hOut, ...)
{
    DWORD MsgLen, Count;
    LPCTSTR pMsg;
    va_list pMsgList; /* Current message string. */
    va_start (pMsgList, hOut); /* Start processing messages. */
    while ((pMsg = va_arg (pMsgList, LPCTSTR)) != NULL) {
        MsgLen = _tcslen (pMsg);
        /* WriteConsole succeeds only for console handles. */
        if (!WriteConsole (hOut, pMsg, MsgLen, &Count, NULL)
            /* Call WriteFile only if WriteConsole fails. */
            && !WriteFile (hOut, pMsg, MsgLen * sizeof (TCHAR),
            &Count, NULL))
            return FALSE;
    }
    va_end (pMsgList);
    return TRUE;
}

/* Single message version of PrintStrings. */
BOOL PrintMsg (HANDLE hOut, LPCTSTR pMsg)
{
    return PrintStrings(hOut, pMsg, NULL);
}

/*  */
BOOL PrintStdout(LPCTSTR pMsg)
{
    HANDLE hStdOut = ::CreateFile(_T("CONOUT$"), GENERIC_WRITE, 0,
            NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    BOOL bRet = PrintMsg(hStdOut, pMsg);

    ::CloseHandle(hStdOut);

    return bRet;
}

/*  */
BOOL PrintStderr(LPCTSTR pMsg)
{
    HANDLE hStdErr = ::CreateFile(_T("CONERR$"), GENERIC_WRITE, 0,
            NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    BOOL bRet = PrintMsg(hStdErr, pMsg);

    ::CloseHandle(hStdErr);

    return bRet;
}

BOOL ULIB_API PrintStdoutFormat(const TCHAR * szFormat, ...)
{
    TCHAR   szBuffer [1024] ;
    va_list pArgList ;

    // The va_start macro (defined in STDARG.H) is usually equivalent to:
    // pArgList = (char *) &szFormat + sizeof (szFormat) ;

    va_start (pArgList, szFormat) ;

    // The last argument to wvsprintf points to the arguments

    _vsntprintf (szBuffer, sizeof (szBuffer) / sizeof (TCHAR),
        szFormat, pArgList) ;

    // The va_end macro just zeroes out pArgList for no good reason

    va_end (pArgList) ;

    return PrintStdout(szBuffer);
}

/* Prompt the user at the console and get a response. */
BOOL ConsolePrompt ( LPCTSTR pPromptMsg,
                     LPTSTR pResponse,
                     DWORD MaxTchar,
                     BOOL Echo )
{
    HANDLE hStdIn, hStdOut;
    DWORD TcharIn, EchoFlag;
    BOOL bSuccess;
    hStdIn = CreateFile (_T ("CONIN$"),
        GENERIC_READ | GENERIC_WRITE, 0,
        NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    hStdOut = CreateFile (_T ("CONOUT$"), GENERIC_WRITE, 0,
        NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    EchoFlag = Echo ? ENABLE_ECHO_INPUT : 0;
    bSuccess =
        SetConsoleMode (hStdIn, ENABLE_LINE_INPUT | EchoFlag | ENABLE_PROCESSED_INPUT)
        && SetConsoleMode(hStdOut, ENABLE_WRAP_AT_EOL_OUTPUT | ENABLE_PROCESSED_OUTPUT)
        && PrintStrings (hStdOut, pPromptMsg, NULL)
        && ReadConsole (hStdIn, pResponse, MaxTchar, &TcharIn, NULL);
    if (bSuccess) pResponse [TcharIn - 2] = '\0';
    CloseHandle (hStdIn);
    CloseHandle (hStdOut);
    return bSuccess;
}

/* General-purpose function for reporting system errors. */
void ReportError(LPCTSTR UserMessage,
                 DWORD ExitCode,
                 BOOL PrintErrorMsg)
{
    DWORD eMsgLen;
    DWORD LastErr = GetLastError();
    LPTSTR lpvSysMsg;
    HANDLE hStdErr = GetStdHandle (STD_ERROR_HANDLE);
    PrintMsg (hStdErr, UserMessage);
    if (PrintErrorMsg)
    {
        eMsgLen = FormatMessage
            (FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM, NULL, LastErr,
            MAKELANGID (LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR) &lpvSysMsg, 0, NULL);

        PrintStrings (hStdErr, _T ("\n"), lpvSysMsg,
            _T ("\n"), NULL);
        /* Free the memory block containing the error message. */
        HeapFree (GetProcessHeap(), 0, lpvSysMsg); /* See Ch. 5. */
    }
    if (ExitCode > 0)
        ExitProcess (ExitCode);
    else
        return;
}

/* Function that does the work:
   read input data and copy it to standard output. */
#ifndef BUF_SIZE
#define BUF_SIZE 0x200
#endif

void CatFile (HANDLE hInFile, HANDLE hOutFile)
{
    DWORD nIn, nOut;
    BYTE Buffer [BUF_SIZE];
    while (ReadFile (hInFile, Buffer, BUF_SIZE, &nIn, NULL)
        && (nIn != 0)
        && WriteFile (hOutFile, Buffer, nIn, &nOut, NULL));
    return;
}

void CatFileEx(HANDLE hInFile, HANDLE hOutFile)
{
    DWORD nIn, nOut;
    BYTE Buffer [BUF_SIZE];
    if (::GetStdHandle(STD_OUTPUT_HANDLE) == hOutFile)
    {
        PrintMsg(hOutFile, _T("Using Standard Output...\n"));
        while (ReadFile(hInFile, Buffer, BUF_SIZE, &nIn, NULL)
            && (nIn != 0)
            && WriteConsole(hOutFile, Buffer, nIn, &nOut, NULL));
    }
    else
    {
        while (ReadFile (hInFile, Buffer, BUF_SIZE, &nIn, NULL)
            && (nIn != 0)
            && WriteFile (hOutFile, Buffer, nIn, &nOut, NULL));
    }


    return;
}

#ifndef BUF_SIZE
#define BUF_SIZE 256
#endif

/* ASCII to Unicode file copy function.
Behavior is modeled after CopyFile. */
BOOL Ascii2Unicode(LPCTSTR fIn, LPCTSTR fOut, BOOL bFailIfExists)
{
    HANDLE hIn, hOut;
    DWORD dwOut, nIn, nOut, iCopy;
    CHAR aBuffer [BUF_SIZE];
    WCHAR uBuffer [BUF_SIZE];
    BOOL WriteOK = TRUE;

    hIn = CreateFile (fIn, GENERIC_READ, 0, NULL,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    /* Determine CreateFile action if output file already exists. */
    dwOut = bFailIfExists ? CREATE_NEW : CREATE_ALWAYS;
    hOut = CreateFile (fOut, GENERIC_WRITE, 0, NULL,
        dwOut, FILE_ATTRIBUTE_NORMAL, NULL);
    while (ReadFile (hIn, aBuffer, BUF_SIZE, &nIn, NULL)
        && nIn > 0 && WriteOK)
    {
            for (iCopy = 0; iCopy < nIn; iCopy++)
                /* Convert each character. */
                uBuffer [iCopy] = (WCHAR) aBuffer [iCopy];
            WriteOK = WriteFile (hOut, uBuffer, 2 * nIn, &nOut, NULL);
    }
    CloseHandle (hIn);
    CloseHandle (hOut);
    return WriteOK;
}

BOOL Ascii2UnicodeOpt(LPCTSTR fIn, LPCTSTR fOut, BOOL bFailIfExists)
{
    HANDLE hIn, hOut;
    DWORD dwOut, nIn, nOut, iCopy;
    CHAR aBuffer [BUF_SIZE];
    WCHAR uBuffer [BUF_SIZE];
    BOOL WriteOK = TRUE;

    hIn = CreateFile (fIn, GENERIC_READ, 0, NULL,
        OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);

    /* Determine CreateFile action if output file already exists. */
    dwOut = bFailIfExists ? CREATE_NEW : CREATE_ALWAYS;
    hOut = CreateFile (fOut, GENERIC_WRITE, 0, NULL,
        dwOut, FILE_ATTRIBUTE_NORMAL, NULL);
    while (ReadFile (hIn, aBuffer, BUF_SIZE, &nIn, NULL)
        && nIn > 0 && WriteOK)
    {
        for (iCopy = 0; iCopy < nIn; iCopy++)
            /* Convert each character. */
            uBuffer [iCopy] = (WCHAR) aBuffer [iCopy];
        WriteOK = WriteFile (hOut, uBuffer, 2 * nIn, &nOut, NULL);
    }
    CloseHandle (hIn);
    CloseHandle (hOut);
    return WriteOK;
}

static POINT screensize;

//
// Clears the screen
//
void clrscr()
{
    COORD coordScreen = { 0, 0 };
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD dwConSize;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    GetConsoleScreenBufferInfo(hConsole, &csbi);
    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    screensize.x = csbi.dwSize.X;
    screensize.y = csbi.dwSize.Y;
    FillConsoleOutputCharacter(hConsole, TEXT(' '), dwConSize, coordScreen, &cCharsWritten);
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
    SetConsoleCursorPosition(hConsole, coordScreen);
}

//
// Moves the cursor to x, y in console window
// ie x=left\right y=top\bottom
//
void gotoxy(int x, int y)
{
    COORD point;
    if((x < 0 || x > screensize.x) || (y < 0 || y > screensize.y))
        return;
    point.X = x; point.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), point);
}

//
// Set text and background colors
//
void setrgb(ColorOnColor color)
{
    switch (color)
    {
    case WHITE_ON_BlACK:    // White on Black
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY |
            FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        break;
    case RED_ON_BLACK:    // Red on Black
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY |
            FOREGROUND_RED);
        break;
    case GREEN_ON_BLACK:    // Green on Black
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY |
            FOREGROUND_GREEN);
        break;
    case YELLOW_ON_BLACK:    // Yellow on Black
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY |
            FOREGROUND_RED | FOREGROUND_GREEN);
        break;
    case BLUE_ON_BLACK:    // Blue on Black
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY |
            FOREGROUND_BLUE);
        break;
    case MAGENTA_ON_BLACK:    // Magenta on Black
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY |
            FOREGROUND_RED | FOREGROUND_BLUE);
        break;
    case CYAN_ON_BLACK:    // Cyan on Black
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY |
            FOREGROUND_GREEN | FOREGROUND_BLUE);
        break;
    case BLACK_ON_GRAY:    // Black on Gray
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),BACKGROUND_INTENSITY |
            BACKGROUND_INTENSITY);
        break;
    case BLACK_ON_WHITE:    // Black on White
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),BACKGROUND_INTENSITY |
            FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
        break;
    case RED_ON_WHITE:    // Red on White
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),BACKGROUND_INTENSITY |
            FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE |
            FOREGROUND_RED);
        break;
    case GREEN_ON_WHITE: // Green on White
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),BACKGROUND_INTENSITY |
            FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE |
            FOREGROUND_GREEN);
        break;
    case YELLOW_ON_WHITE: // Yellow on White
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),BACKGROUND_INTENSITY |
            FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE |
            FOREGROUND_RED | FOREGROUND_GREEN);
        break;
    case BLUE_ON_WHITE: // Blue on White
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),BACKGROUND_INTENSITY |
            FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE |
            FOREGROUND_BLUE);
        break;
    case MAGENTA_ON_WHITE: // Magenta on White
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),BACKGROUND_INTENSITY |
            FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE |
            FOREGROUND_RED | FOREGROUND_BLUE);
        break;
    case CYAN_ON_WHITE: // Cyan on White
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),BACKGROUND_INTENSITY |
            FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE |
            FOREGROUND_GREEN | FOREGROUND_BLUE);
        break;
    case WHITE_ON_WHITE: // White on White
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),BACKGROUND_INTENSITY |
            FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE |
            FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        break;
    default : // White on Black
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY |
            FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        break;
    }
}

void setCaret(CaretShape cs, HANDLE hOut)
{
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 1;
	info.bVisible = TRUE;

	switch (cs)
    {
    case CARET_NONE:
		info.bVisible = FALSE;
		break;
	case CARET_SOLID:
		info.dwSize = 100;
		break;
	case CARET_NORMAL:
		break;
	default:
		return;
    }
	::SetConsoleCursorInfo (hOut, &info);
}

}; // namespace UConsole



#ifndef U_CONSOLE_H
#define U_CONSOLE_H

#include "ulib.h"

namespace UConsole
{

/* Write the messages to the output handle. */
BOOL ULIB_API PrintStrings (HANDLE hOut, ...);

/* Single message version of PrintStrings. */
BOOL ULIB_API PrintMsg (HANDLE hOut, LPCTSTR pMsg);

/* Output messages to STDOUT */
BOOL ULIB_API PrintStdout(LPCTSTR pMsg);
BOOL ULIB_API PrintStderr(LPCTSTR pMsg);

/* Output messages to STDOUT in the specific format. */
BOOL ULIB_API PrintStdoutFormat(const TCHAR * szFormat, ...);

/* Prompt the user at the console and get a response. */
BOOL ULIB_API ConsolePrompt( LPCTSTR pPromptMsg,
                             LPTSTR pResponse,
                             DWORD MaxTchar,
                             BOOL Echo );

/* General-purpose function for reporting system errors. */
void ULIB_API ReportError( LPCTSTR UserMessage,
                           DWORD ExitCode,
                           BOOL PrintErrorMsg);

/* Function that does the work:
   read input data and copy it to standard output. */
void ULIB_API CatFile (HANDLE hInFile, HANDLE hOutFile);

/* */
void ULIB_API CatFileEx(HANDLE hInFile, HANDLE hOutFile);

/* ASCII to Unicode file copy function.
   Behavior is modeled after CopyFile. */
BOOL ULIB_API Ascii2Unicode (LPCTSTR fIn, LPCTSTR fOut, BOOL bFailIfExists);

/*  */
BOOL ULIB_API Ascii2UnicodeOpt(LPCTSTR fIn, LPCTSTR fOut, BOOL bFailIfExists);

//
// Clears the screen
//
void ULIB_API clrscr();

//
// Moves the cursor to x, y in console window
// ie x=left\right y=top\bottom
//
void ULIB_API gotoxy(int x, int y);

//
// Set text and background colors
//
typedef enum tagColorOnColor {
    WHITE_ON_BlACK = 0,
    RED_ON_BLACK,
    GREEN_ON_BLACK,
    YELLOW_ON_BLACK,
    BLUE_ON_BLACK,
    MAGENTA_ON_BLACK,
    CYAN_ON_BLACK,
    BLACK_ON_GRAY,
    BLACK_ON_WHITE,
    RED_ON_WHITE,
    GREEN_ON_WHITE,
    YELLOW_ON_WHITE,
    BLUE_ON_WHITE,
    MAGENTA_ON_WHITE,
    CYAN_ON_WHITE,
    WHITE_ON_WHITE,
    WHITE_ON_BLACK
} ColorOnColor;


void ULIB_API setrgb(ColorOnColor color);

typedef enum tagCaretShape {
    CARET_NONE   = 0,
    CARET_SOLID  = 1,
    CARET_NORMAL = 2
} CaretShape;

void ULIB_API setCaret(CaretShape cs, HANDLE hOut);

class ULIB_API UConsole
{
public:
    UConsole();

    virtual ~UConsole();
private:

};

}; // namespace UConsole

#endif // U_CONSOLE_H

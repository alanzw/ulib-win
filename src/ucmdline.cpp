/*
 * =====================================================================================
 *
 *       Filename:  ucmdline.cpp
 *
 *    Description:  Command Line String Utility
 *
 *        Version:  1.0
 *        Created:  2009-6-21 22:45:35
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#include <windows.h>
#include <tchar.h>

#include "ucmdline.h"


UCmdLine::UCmdLine()
{}


UCmdLine::~UCmdLine()
{}


// CommandLineToArgv converts a command-line string to argc and
// argv similar to the ones in the standard main function.
// This is a specialized version coded specifically for rundll32
// and is not intended to be used in any other program.
LPTSTR * UCmdLine::CommandLineToArgv(LPCTSTR lpCmdLine, int *lpArgc)
{
    LPTSTR *argv, lpSrc, lpDest, lpArg;
    int argc, nBSlash, nNames;
    BOOL bInQuotes, bFirstChar;

    // If null was passed in for lpCmdLine, there are no arguments
    if (!lpCmdLine) {
        if (lpArgc)
            *lpArgc = 0;
        return 0;
    }

    lpSrc = (LPTSTR)lpCmdLine;
    // Skip spaces at beginning
    while (*lpSrc == _T(' ') || *lpSrc == _T('\t'))
        lpSrc++;

    // If command-line starts with null, there are no arguments
    if (*lpSrc == 0) {
        if (lpArgc)
            *lpArgc = 0;
        return 0;
    }

    lpArg = lpSrc;
    argc = 0;
    nBSlash = 0;
    bInQuotes = FALSE;
    bFirstChar = TRUE;
    nNames = 0;

    // Count the number of arguments
    while (nNames < 4) {
        if (*lpSrc == 0 || (*lpSrc == _T(',') && nNames == 2) 
                || ((*lpSrc == _T(' ') || *lpSrc == _T('\t')) && !bInQuotes)) {
            // Whitespace not enclosed in quotes signals the start of another argument

            argc++;

            // Skip whitespace between arguments
            while (*lpSrc == _T(' ') || *lpSrc == _T('\t') || (*lpSrc == _T(',') && nNames == 2))
                lpSrc++;
            if (*lpSrc == 0)
                break;
            if (nNames >= 3) {
                // Increment the count for the last argument
                argc++;
                break;
            }
            nBSlash = 0;
            bFirstChar = TRUE;
            continue;
        }
        else if (*lpSrc == _T('\\')) {
            // Count consecutive backslashes
            nBSlash++;
            bFirstChar = FALSE;
        }
        else if (*lpSrc == _T('\"') && !(nBSlash & 1)) {
            // Open or close quotes
            bInQuotes = !bInQuotes;
            nBSlash = 0;
        }
        else {
            // Some other character
            nBSlash = 0;
            if (bFirstChar && ((*lpSrc != _T('/') && nNames <= 1) || nNames > 1))
                nNames++;
            bFirstChar = FALSE;
        }
        lpSrc++;
    }

    // Allocate space for the pointers in argv and the strings in one block
    argv = (LPTSTR *)malloc(argc * sizeof(LPTSTR) + (_tcslen(lpArg) + 1) * sizeof
            (TCHAR));

    if (!argv) {
        // Memory allocation failed
        if (lpArgc)
            *lpArgc = 0;
        return 0;
    }

    lpSrc = lpArg;
    lpDest = lpArg = (LPTSTR)(argv + argc);
    argc = 0;
    nBSlash = 0;
    bInQuotes = FALSE;
    bFirstChar = TRUE;
    nNames = 0;

    // Fill the argument array
    while (nNames < 4) {
        if (*lpSrc == 0 || (*lpSrc == _T(',') && nNames == 2) || ((*lpSrc == _T
                        (' ') || *lpSrc == _T('\t')) && !bInQuotes)) {
            // Whitespace not enclosed in quotes signals the start of another argument

            // Null-terminate argument
            *lpDest++ = 0;
            argv[argc++] = lpArg;

            // Skip whitespace between arguments
            while (*lpSrc == _T(' ') || *lpSrc == _T('\t') || (*lpSrc == _T(',') && nNames == 2))
                lpSrc++;
            if (*lpSrc == 0)
                break;
            lpArg = lpDest;
            if (nNames >= 3) {
                // Copy the rest of the command-line to the last argument
                argv[argc++] = lpArg;
                _tcscpy(lpArg,lpSrc);
                break;
            }
            nBSlash = 0;
            bFirstChar = TRUE;
            continue;
        }
        else if (*lpSrc == _T('\\')) {
            *lpDest++ = _T('\\');
            lpSrc++;

            // Count consecutive backslashes
            nBSlash++;
            bFirstChar = FALSE;
        }
        else if (*lpSrc == _T('\"')) {
            if (!(nBSlash & 1)) {
                // If an even number of backslashes are before the quotes,
                // the quotes don't go in the output
                lpDest -= nBSlash / 2;
                bInQuotes = !bInQuotes;
            }
            else {
                // If an odd number of backslashes are before the quotes,
                // output a quote
                lpDest -= (nBSlash + 1) / 2;
                *lpDest++ = _T('\"');
                bFirstChar = FALSE;
            }
            lpSrc++;
            nBSlash = 0;
        }
        else {
            // Copy other characters
            if (bFirstChar && ((*lpSrc != _T('/') && nNames <= 1) || nNames > 1))
                nNames++;
            *lpDest++ = *lpSrc++;
            nBSlash = 0;
            bFirstChar = FALSE;
        }
    }

    if (lpArgc)
        *lpArgc = argc;
    return argv;
}


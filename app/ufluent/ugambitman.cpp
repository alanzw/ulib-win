/*
 * =====================================================================================
 *
 *       Filename:  ugambitman.cpp
 *
 *    Description:  UGambitMan implement
 *
 *        Version:  1.0
 *        Created:  2009-8-5 0:47:52
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

#include "uthread.h"
#include "uprocessman.h"
#include "ugambitman.h"
#include "udllman.h"

LPCTSTR s_GambitStartupWndcls = _T("#32770");
LPCTSTR s_GambitStartupWndTitle = _T("Gambit Startup");

LPCTSTR s_GambitMainWndClass = _T("EXCEEDW:MWCLIENT0");
//
#define DEFAULT_GAMBIT_TITLE_PRE "GAMBIT     Solver: FLUENT 5/6   ID: 11112"

enum {
    GS_ID_BN_RUN   = 0x0001,
    GS_ID_BN_CLOSE = 0x0002,
    GS_ID_ET_SID   = 0x03E9
};

LPCTSTR s_cmdExit = _T("exit");
LPCTSTR s_cmdSave = _T("save");

UGambitMan::UGambitMan()
: m_hGambitWnd(NULL)
{}

UGambitMan::~UGambitMan()
{}

static DWORD WINAPI check_gambit_session(LPVOID lpParam)
{
#if 0
    /// ***Wait for Gambit start
    Sleep(1000);

    UGambitMan *lpUgm = (UGambitMan *)lpParam;
    if (lpUgm->findWindow())
    {
    #if 1
        DWORD pid = 0;
        getWindowPid(lpUgm->getGambitWnd(), &pid);

        // Get a handle to the process.
        HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
                                       SYNCHRONIZE,
                                       FALSE,
                                       pid );

        if (NULL != hProcess)
        {
            LOG_STRING("Wait for gambit.exe!");
            ::WaitForSingleObject(hProcess, INFINITE);
        }
    #endif
    }
#endif

    LOG_STRING("Check Gambit PID.");

    DWORD pid = huys::DLLInject::GetTargetProcessIdFromProcname("gambit.exe");

    if (0 == pid)
    {
        return 1;
    }


    HANDLE hProcess = OpenProcess( SYNCHRONIZE, FALSE, pid );

    if (NULL != hProcess)
    {
        ::WaitForSingleObject(hProcess, INFINITE);
    }

    return 0;
}

bool UGambitMan::startExe()
{
    //return startProgram("D:\\GS\\Fluent.Inc\\ntbin\\ntx86\\gambit.exe", "D:\\", FALSE);
    #if 1
    m_utParam.sCmdline = _T("D:\\GS\\Fluent.Inc\\ntbin\\ntx86\\gambit.exe");
    m_utParam.nMessage = UGM_FINISHED;
    m_utParam.sWDir = _T("D:\\");
    m_utParam.lpExitFunc = &check_gambit_session;
    m_utParam.lpExitFuncParam = this;
    //m_utParam.lpExitFunc = NULL;
    //m_utParam.lpExitFuncParam = NULL;
    spawnNewThreadAndExec(&m_utParam);
    #endif
    Sleep(1000);
    sendCmdGambitStartup();
    return true;
}

bool UGambitMan::exitExe()
{
    return true;
}

bool UGambitMan::findWindow()
{
#if 0
    char title[256];
    HWND hLast = NULL;
    //
    while ( NULL != (m_hGambitWnd = ::FindWindowEx(NULL, hLast, s_GambitMainWndClass, NULL)) )
    {
        ::GetWindowText(m_hGambitWnd, title, 256);

        // Validation: "Fluent ***"
        title[6] = '\0';
        if (0 == strcmp(title, "GAMBIT") && 0x0000004D == ::GetWindowLong(m_hGambitWnd, GWL_USERDATA))
        {
            return true;
        }
        //
        hLast = m_hGambitWnd;
    }

    m_hGambitWnd = NULL;
    return false;
#endif

    m_hGambitWnd = ::FindWindow(s_GambitMainWndClass, DEFAULT_GAMBIT_TITLE_PRE);
    return (NULL != m_hGambitWnd);
}

bool UGambitMan::isExceedOk()
{
    return true;
}

void UGambitMan::sendCmdGambitStartup()
{
    if ((m_hGambitStartup = ::FindWindow(s_GambitStartupWndcls, s_GambitStartupWndTitle)) != NULL )
    {
        ::SendMessage(::GetDlgItem(m_hGambitStartup, GS_ID_ET_SID), WM_SETTEXT, 0, (LPARAM)"11112");
        ::SetFocus(::GetDlgItem(m_hGambitStartup, GS_ID_BN_RUN));
        ::SendMessage(::GetDlgItem(m_hGambitStartup, GS_ID_BN_RUN), WM_LBUTTONDOWN, 0, 0);
        ::SendMessage(::GetDlgItem(m_hGambitStartup, GS_ID_BN_RUN), WM_LBUTTONUP, 0, 0);
        Sleep(5);
        ::SendMessage(::GetDlgItem(m_hGambitStartup, GS_ID_BN_CLOSE), WM_LBUTTONDOWN, 0, 0);
        ::SendMessage(::GetDlgItem(m_hGambitStartup, GS_ID_BN_CLOSE), WM_LBUTTONUP, 0, 0);
    }
}

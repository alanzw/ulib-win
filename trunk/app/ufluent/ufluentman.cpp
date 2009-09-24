/*
 * =====================================================================================
 *
 *       Filename:  ufluentman.cpp
 *
 *    Description:  implement UFluentMan
 *
 *        Version:  1.0
 *        Created:  08/04/2009 09:49:00 PM
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
#include <cassert>
#include <string>

#include "umenu.h"

#include "uthread.h"
#include "ufluentman.h"
#include "uprocessman.h"
#include "umsg.h"
#include "udllman.h"

#include "uhook.h"

//
#define DEFAULT_STARTUP_FILE "G:\\huys\\ulib-win\\app\\ufluent\\fluent.scm"

//
#define DEFAULT_FLUENT_ROOTDIR "C:\\Fluent.Inc"
#define DEFAULT_FLUENT_ARCH    "ntx86"
#define DEFAULT_FLUENT_BIDIR   "ntbin"

#define FLUENT_EXECUTIVE_NAME "fl6326s.exe"

//
#define DEFAULT_FLUENT_TITLE_PRE "FLUENT"

//
LPCSTR FluentWindowClasses[] =
{
    "AfxFrameOrView70s", // fluent6.3.26 ntx86
    "AfxFrameOrView80s", // fluent6.3.26 amd64
    0
};

const UINT ID_FMW_ED_OUTPUT    = 0x00000480;

LPCSTR FluentVersionWndClass   = "#32770";
LPCSTR FlunetVersionWndCaption = "FLUENT Version";
const UINT ID_FVW_LB_VERSIONS  = 0x00000066;
const UINT ID_FVW_ED_SELECTION = 0x00000065;
const UINT ID_FVW_ST_SELECTION = 0x00000067;
const UINT ID_FVW_ST_VERSIONS  = 0x00000068;
const UINT ID_FVW_CB_MODE      = 0x00000069;
const UINT ID_FVW_ST_MODE      = 0x0000006A;
const UINT ID_FVW_BN_RUN       = 0x00000001;
const UINT ID_FVW_BN_EXIT      = 0x00000002;
//
struct FluentCmdSting
{
    LPCSTR sCmd;
    LPCSTR sString;
    bool bHasParam;
    LPCSTR sParam;
};

static FluentCmdSting s_FCmdStr[] =
{
    { "exit",    "(exit)",    false, 0 },
    { "load",    "(load %s)", true,  0 },
    { "display", "(display)", true,  0 },
    { 0,         0,           false, 0 }
};

LPCTSTR mapC2S(LPCTSTR cmd)
{
    int i = 0;
    while ( 0 != s_FCmdStr[i].sCmd && 0 != s_FCmdStr[i].sString)
    {
        if (strcmp(cmd, s_FCmdStr[i].sCmd) == 0)
        {
            return s_FCmdStr[i].sString;
        }
        else
        {
            ++i;
        }
    }
    return 0;
}

//
UFluentMan::UFluentMan()
: m_hFluentWnd(NULL),
  m_hFluentEditCtrl(NULL),
  m_nLineCount(0),
  m_nLineCountPrev(0)
{}

UFluentMan::~UFluentMan()
{}

//
static DWORD WINAPI check_fluent_session(LPVOID lpParam)
{

#if 0
    /// ***Wait for Fluent start
    Sleep(1000);

    UFluentMan *lpUfm = (UFluentMan *)lpParam;
    if (lpUfm->findWindow())
    {
        DWORD pid = 0;
        getWindowPid(lpUfm->getFluentWnd(), &pid);

        // Get a handle to the process.
        HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
                                       SYNCHRONIZE,
                                       FALSE,
                                       pid );

        if (NULL != hProcess)
        {
            ::WaitForSingleObject(hProcess, INFINITE);
        }

    }
#endif
    LOG_STRING("Check Fluent PID.");


    DWORD pid = huys::DLLInject::GetTargetProcessIdFromProcname("fl6326s.exe");

    LOG_STRING("Get PID %d.", pid);

    HANDLE hProcess = OpenProcess( SYNCHRONIZE, FALSE, pid );

    if (NULL != hProcess)
    {
        LOG_STRING("Wait for fl6326s.exe!");
        ::WaitForSingleObject(hProcess, INFINITE);
    }

    return 0;
}

bool UFluentMan::startExe()
{
    //return startProgram("D:\\GS\\Fluent.Inc\\ntbin\\ntx86\\fluent.exe 2d", NULL, FALSE);
    m_utParam.sCmdline = _T("D:\\GS\\Fluent.Inc\\ntbin\\ntx86\\fluent.exe 2d -i G:\\huys\\ulib-win\\app\\ufluent\\fluent.scm");
    m_utParam.nMessage = UFM_FINISHED;
    m_utParam.sWDir = _T("D:\\");
    m_utParam.lpExitFunc = &check_fluent_session;
    m_utParam.lpExitFuncParam = this;
    spawnNewThreadAndExc(&m_utParam);

    return true;
}

bool UFluentMan::exitExe()
{
    this->sendCmd("exit");
    return true;
}

//
bool UFluentMan::findWindow()
{
    int i = 0;
    HWND hLast = NULL;
    char title[256];
    //
    while(0 != FluentWindowClasses[i])
    {
        hLast = NULL;

        while ( NULL != (m_hFluentWnd = ::FindWindowEx(NULL, hLast, FluentWindowClasses[i], NULL)) )
        {
            ::GetWindowText(m_hFluentWnd, title, 256);

            // Validation: "Fluent ***"
            title[6] = '\0';
            if (0 == strcmp(title, "FLUENT") || 0 == strcmp(title, "fluent"))
            {
                return true;
            }
            //
            hLast = m_hFluentWnd;
        }
        i++;
    }
    return false;
}

int UFluentMan::getLineCount() const
{
    return ::SendMessage(m_hFluentEditCtrl, EM_GETLINECOUNT, 0, 0);
}


bool UFluentMan::sendCmdEnter()
{
    this->findWindow();
    m_hFluentEditCtrl = GetDlgItem(m_hFluentWnd, ID_FMW_ED_OUTPUT);
    return this->sendCmdChar('\n');
}

bool UFluentMan::sendCmd( LPCSTR sCmd )
{
    if (NULL == m_hFluentEditCtrl)
    {
        return false;
    }

    int i;
    for (i=0; i<(int)strlen(sCmd); i++)
    {
        this->sendCmdChar(sCmd[i]);
    }
    this->sendCmdEnter();
    return true;
}

bool UFluentMan::sendCmdChar( char c )
{
    if (NULL == m_hFluentEditCtrl)
    {
        return false;
    }

    return ::SendMessage(m_hFluentEditCtrl, WM_CHAR, (WPARAM)c, 1);
}

bool UFluentMan::breakCalc()
{
    this->findWindow();
    m_hFluentEditCtrl = GetDlgItem(m_hFluentWnd, ID_FMW_ED_OUTPUT);
    //ShowWindow(m_hFluentWnd, SW_NORMAL);
    BringWindowToTop(m_hFluentWnd);
    SetFocus(m_hFluentEditCtrl);
    // Simulate a key press
    keybd_event(VK_CONTROL,MapVirtualKey(VK_CONTROL,0),0,0);
    keybd_event('C', MapVirtualKey('C',0),0,0);
    // Simulate a key release
    keybd_event('C',MapVirtualKey('C',0),KEYEVENTF_KEYUP,0);
    keybd_event(VK_CONTROL,MapVirtualKey(VK_CONTROL,0),KEYEVENTF_KEYUP,0);
    //ShowWindow(m_hFluentWnd, SW_HIDE);

    //::PostMessage(m_hFluentEditCtrl, WM_KEYDOWN, (WPARAM)VK_CONTROL, (LPARAM)0x001D0001);
    //::PostMessage(m_hFluentEditCtrl, WM_KEYDOWN, (WPARAM)'C', (LPARAM)0x002E0001);
    //::PostMessage(m_hFluentEditCtrl, WM_KEYUP, (WPARAM)VK_CONTROL, (LPARAM)0xC01D0001);
    //::PostMessage(m_hFluentEditCtrl, WM_KEYUP, (WPARAM)'C', (LPARAM)0xC02E0001);
    return true;
}

bool UFluentMan::getResult()
{
    if (NULL == m_hFluentEditCtrl)
    {
        return false;
    }

    char title[2024];
    ::SendMessage(m_hFluentEditCtrl, WM_GETTEXT, 2024, (LPARAM)title);
    showMsg(title);
}

int UFluentMan::getLine( int nLineIndex )
{
    return ::SendMessage(m_hFluentEditCtrl, EM_GETLINE, (WPARAM)nLineIndex, (LPARAM)m_buf);
}

int UFluentMan::getNewLines()
{
    return (m_nLineCount - m_nLineCountPrev);
}

int UFluentMan::fetchNewResult()
{
    std::string sTmp;
    for (int i = m_nLineCountPrev; i<m_nLineCount; ++i)
    {
        this->getLine(i);
        sTmp = sTmp + m_buf;
    }
    return 0;
}

bool UFluentMan::loadScript( LPCSTR sFileName )
{
    char sCmd[512];
    sprintf(sCmd, "(load \"%s\")", sFileName);

    return this->sendCmd(sCmd);
}

void UFluentMan::addMenu()
{
    HMENU hMenu = ::GetMenu(m_hFluentWnd);
    if (!hMenu)
    {
        return;
    }

    //::AppendMenu(hMenu, MF_STRING, 11111, "Menu&1");
    HMENU hPopMenu = ::CreateMenu();
    ::AppendMenu(hPopMenu, MF_STRING, 11112, "CMenu&1");
    ::AppendMenu(hPopMenu, MF_STRING, 11113, "CMenu&2");
    ::AppendMenu(hMenu, MF_STRING|MF_POPUP, (UINT_PTR)hPopMenu, "Child");
}

LRESULT UFluentMan::newWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_COMMAND)
    {
        switch (wParam)
        {
        case 11112:
            {
                ::MessageBox(hwnd, "hww1", "info", MB_OK);
                return FALSE;
            }
        case 11113:
            {
                ::MessageBox(hwnd, "hww2", "info", MB_OK);
                return FALSE;
            }
        default:
            return CallWindowProc(getOldProc(), hwnd, uMsg, wParam, lParam);
        }

    }

}

WNDPROC UFluentMan::m_OriginProc = NULL;

void UFluentMan::subclassWnd(HWND hMsgWnd)
{
    //UFluentMan::m_OriginProc =  (WNDPROC) SetWindowLong(m_hFluentWnd, GWL_WNDPROC, (LONG) UFluentMan::newWndProc);
    setCBTHook(m_hFluentWnd, hMsgWnd);
    //setCBTHook(m_hFluentWnd, hMsgWnd);
    //BringWindowToTop(m_hFluentWnd);
    //ShowWindow(m_hFluentWnd, SW_MINIMIZE);
    //ShowWindow(m_hFluentWnd, SW_NORMAL);
    SetWindowPos(m_hFluentWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOREDRAW);
    //ShowWindow(m_hFluentWnd, SW_MINIMIZE);
    //ShowWindow(m_hFluentWnd, SW_NORMAL);
    SetWindowPos(m_hFluentWnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOREDRAW);
    SetWindowPos(m_hFluentWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOREDRAW);
    //SetFocus(m_hFluentWnd);
}

void UFluentMan::focusWnd()
{
    SetWindowPos(m_hFluentWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
}


#include "resource.h"
#include <windows.h>
#include <tchar.h>
#include <commctrl.h>
#include <cassert>

#include "udialogx.h"
#include "udlgapp.h"

#include "uedit.h"
#include "ubutton.h"
#include "umsg.h"

const UINT ID_EDIT_CMD = 1113;
const UINT ID_EDIT_TEST = 1114;
const UINT ID_EDIT_RESULT = 1115;
const UINT ID_BUTTON_TEST = 1116;
const UINT ID_BUTTON_FLUENT = 1117;
const UINT ID_BUTTON_GAMBIT = 1118;

const char *FLUENT_ROOTDIR = "D:\\GS\\Fluent.Inc";
const char *FLUENT_BINDIR = "ntbin";
const char *FLUENT_ARCH = "ntx86";

HANDLE g_hChildProcess = INVALID_HANDLE_VALUE;

BOOL runProgram(char *programName = "notepad.exe")
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    BOOL bResult;

    ZeroMemory( &si, sizeof(STARTUPINFO) );
    si.cb = sizeof(si);
    si.dwFlags=STARTF_USESHOWWINDOW;
    si.wShowWindow=SW_NORMAL;

    ZeroMemory( &pi, sizeof(PROCESS_INFORMATION) );

    bResult = CreateProcess(NULL,
        programName,
        NULL,
        NULL,
        TRUE,
        CREATE_NEW_CONSOLE| NORMAL_PRIORITY_CLASS,
        NULL,
        NULL,
        &si,
        &pi);

    if (!bResult) return FALSE;

    //WaitForSingleObject(pi.hProcess, INFINITE);

    g_hChildProcess = pi.hProcess;

    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);

    return TRUE;
}

char title[2024];

void sendCmd(const char *cmd)
{

    HWND hFluentWnd = NULL;
    //char title[2024];

    HWND hFluentEdit = NULL;

    HWND hLast = NULL;

    char moduleName[256];

    // Loop over to find the Fluent window with WindowClass
    //if ((hFluentWnd = ::FindWindow("AfxFrameOrView80s", NULL))!=NULL)
    // "AfxFrameOrView70s" for fluent6.3.26 ntx86
    // "AfxFrameOrView80s" for fluent6.3.26 amd64
    while ((hFluentWnd = ::FindWindowEx(NULL, hLast, "AfxFrameOrView70s", NULL))!=NULL ||
        (hFluentWnd = ::FindWindowEx(NULL, hLast, "AfxFrameOrView80s", NULL))!=NULL)
    {
        hLast = hFluentWnd;

        showMsg("Find it!");

        ::GetWindowText(hFluentWnd, title, 2024);
        showMsg(title);

        // Validation: "Fluent ***"
        title[6] = '\0'; //
        int rc = strcmp(title, "FLUENT");

        showMsgFormat(_T("info"), _T("strcmp \'%s\' result: %d"), title, rc);


        // Error: In WinXP/Win2003, the current module name is supplied.
        // int c = ::GetWindowModuleFileName(hFluentWnd, moduleName, 256);
        // showMsg(moduleName);

        if ((hFluentEdit = ::GetWindow(hFluentWnd, GW_CHILD)) != NULL)
        {
            showMsg("Find it!");

            int line_count = ::SendMessage(hFluentEdit, EM_GETLINECOUNT, 0, 0);

            showMsgFormat(_T("line"), _T("%d"), line_count);

            //int ret = ::GetWindowText(hFluentEdit, title, 1024);
            int ret = ::SendMessage(hFluentEdit, WM_GETTEXT, 2024, (LPARAM)title);

            //Edit_GetText(hFluentEdit, title, 1024);

            //int ret = ::GetDlgItemText(hFluentWnd, 480, title, 1024);

            if (ret == 0)
            {
                DWORD err = GetLastError();
            }
            showMsg(title);


            //char cmds[] = "hello";
            int i;
            for (i=0; i<(int)strlen(cmd); i++)
            {
                ::SendMessage(hFluentEdit, WM_CHAR, (WPARAM)cmd[i], 1);
            }


            ::SendMessage(hFluentEdit, WM_CHAR, (WPARAM)'\n', 1);

            //Sleep(2000);

            int linesNew = ::SendMessage(hFluentEdit, EM_GETLINECOUNT, 0, 0);
            showMsgFormat(_T("line"), _T("%d"), linesNew);
            char buf[512];
            DWORD *pw = (DWORD *)buf;
            //::ZeroMemory(title, sizeof(title));
            for (int i=line_count; i<linesNew-1; i++)
            {
                *pw = 510;
                ret = ::SendMessage(hFluentEdit, EM_GETLINE, (WPARAM)i, (LPARAM)buf);
                buf[ret] = '\0';
                showMsgFormat(_T("char"), _T("%d : %s"), ret, buf);
            }


        }
    }
}

void sendCmdGambitStartup()
{
    HWND hGambitStartup = NULL;
    HWND hBnRun = NULL;
    HWND hBnClose = NULL;

    HWND hComboSessionID = NULL;

    char tmp[] = "090628";

    if ((hGambitStartup = ::FindWindow("#32770", "Gambit Startup")) == NULL )
    {
        showMsg("Not found!");
    }
    else
    {
        hBnRun = ::GetDlgItem(hGambitStartup, 0x0001);
        hBnClose = ::GetDlgItem(hGambitStartup, 0x0002);
        hComboSessionID = ::GetDlgItem(hGambitStartup, 0x03E9);

        if (hBnRun == NULL || hBnClose == NULL || hComboSessionID == NULL)
        {
            showMsg("BnOK Not found!");
        }
        else
        {
            ::SendMessage(hComboSessionID, WM_SETTEXT, 0, (LPARAM)tmp);


            //::SendMessage(hBnRun, BM_CLICK, 0, 0); // Focus
            //::SendMessage(hBnRun, BM_CLICK, 0, 0); // Run
            //::SendMessage(hBnClose, BM_CLICK, 0, 0); // Close
        }
    }
}

using huys::UDialogBox;

class MyDialog : public UDialogBox
{
public:
    MyDialog(HINSTANCE hInst, UINT nID)
    : UDialogBox(hInst, nID) {}

    ~MyDialog()
    {
        CHECK_PTR(m_pEdtTest);
        CHECK_PTR(m_pEdtResult);
        CHECK_PTR(m_pEdtCmd);

        CHECK_PTR(m_pBtnTest);
        CHECK_PTR(m_pBtnFluent);
        CHECK_PTR(m_pBtnGambit);
    }

    virtual BOOL onInit() {
        RECT rc = {20, 20, 520, 100};

        m_pEdtTest = new UEdit(m_hDlg, ID_EDIT_TEST, m_hInst);
        m_pEdtTest->create();
        m_pEdtTest->setPosition(&rc);

        rc.top += 100;
        rc.bottom += 120;
        m_pEdtResult = new UEdit(m_hDlg, ID_EDIT_RESULT, m_hInst);
        m_pEdtResult->setStyles(WS_VSCROLL|ES_AUTOHSCROLL | ES_MULTILINE | ES_WANTRETURN);
        m_pEdtResult->create();
        m_pEdtResult->setPosition(&rc);


        rc.top += 160;
        rc.bottom += 160;
        rc.left += 120;
        m_pBtnTest = new UPushButton(m_hDlg, ID_BUTTON_TEST, m_hInst);
        m_pBtnTest->create();
        m_pBtnTest->setPosition(&rc);
        m_pBtnTest->setWindowText(_T("call"));

        rc.left = 20;
        rc.right = 50;
        m_pBtnFluent = new UPushButton(m_hDlg, ID_BUTTON_FLUENT, m_hInst);
        m_pBtnFluent->create();
        m_pBtnFluent->setPosition(&rc);
        m_pBtnFluent->setWindowText(_T("f"));


        rc.left = 60;
        rc.right = 90;
        m_pBtnGambit = new UPushButton(m_hDlg, ID_BUTTON_GAMBIT, m_hInst);
        m_pBtnGambit->create();
        m_pBtnGambit->setPosition(&rc);
        m_pBtnGambit->setWindowText(_T("g"));

        rc.left = 100;
        rc.right = 400;
        rc.top = 230;
        rc.bottom = 260;
        m_pEdtCmd = new UEdit(m_hDlg, ID_EDIT_CMD, m_hInst);
        m_pEdtCmd->create();
        m_pEdtCmd->setPosition(&rc);
        m_pEdtCmd->setWindowText(_T("D:\\GS\\Fluent.Inc"));

        return TRUE;
    }

    virtual BOOL onLButtonDown(WPARAM wParam, LPARAM lParam)
    {
        return FALSE;
    }

    virtual BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch(wParam)
        {
        case ID_BUTTON_TEST:
            {
                m_pEdtTest->getText(m_buf);
                //showMsgFormat(_T("Test"), _T("hello!\n%s"), m_buf);
                if ('\0' != m_buf[0])
                {
                    sendCmd(m_buf);
                    m_pEdtResult->setWindowText(title);
                }

                sendCmdGambitStartup();
            }
            break;
        case ID_BUTTON_FLUENT:
            //sendCmd("exit");
            {
                m_pEdtCmd->getText(m_buf);
                if ('\0' == m_buf[0])
                {
                   wsprintf(m_cmdline,
                      _T("%s\\%s\\%s\\fluent.exe 3ddp"),
                      FLUENT_ROOTDIR,
                      FLUENT_BINDIR,
                      FLUENT_ARCH);
                }
                else
                {
                   wsprintf(m_cmdline,
                      _T("%s\\%s\\%s\\fluent.exe 3ddp"),
                      m_buf,
                      FLUENT_BINDIR,
                      FLUENT_ARCH);
                }

                runProgram(m_cmdline);
            }
            break;
        case ID_BUTTON_GAMBIT:
            {
                m_pEdtCmd->getText(m_buf);
                if ('\0' == m_buf[0])
                {
                    wsprintf(m_cmdline,
                        _T("%s\\%s\\%s\\gambit.exe -id ./090628 -old -init test.jou"),
                        FLUENT_ROOTDIR,
                        FLUENT_BINDIR,
                        FLUENT_ARCH);
                }
                else
                {
                    wsprintf(m_cmdline,
                        _T("%s\\%s\\%s\\gambit.exe -id ./090628 -old -init test.jou"),
                        m_buf,
                        FLUENT_BINDIR,
                        FLUENT_ARCH);
                }

                runProgram(m_cmdline);
            }
        default:
            break;
        }
        return TRUE;
    }
private:
    UPushButton *m_pBtnTest;
    UPushButton *m_pBtnFluent;
    UPushButton *m_pBtnGambit;
    UEdit *m_pEdtTest;
    UEdit *m_pEdtResult;
    UEdit *m_pEdtCmd;
    TCHAR m_buf[512];
    TCHAR m_cmdline[512];
    int m_linePrev;
    int m_lineAfter;
};

UDLGAPP_T(MyDialog, IDD_DIALOG1);


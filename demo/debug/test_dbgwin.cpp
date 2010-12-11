#include "resource.h"

#define _WIN32_IE 0x0400
#define WINVER    0x0500

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "udialogx.h"
#include "udlgapp.h"

#include "ubasewindow.h"

#include "ulistbox.h"
#include "utoolbar.h"

#include "adt/uautoptr.h"

#include "aui/aui_tracewin.h"

struct dbwin_buffer {
        DWORD   dwProcessId;
        char    data[4096-sizeof(DWORD)];
};

DWORD WINAPI WinDebugMonitorThread(void *pData)
{
    return -1;
}

DWORD initialize_dbg()
{
    DWORD errorCode = 0;
    BOOL bSuccessful = FALSE;

    SetLastError(0);

    // Mutex: DBWin
    // ---------------------------------------------------------
    LPCTSTR DBWinMutex = _T("DBWinMutex");
    HANDLE m_hDBWinMutex = ::OpenMutex(
        MUTEX_ALL_ACCESS,
        FALSE,
        DBWinMutex
        );

    if (m_hDBWinMutex == NULL) {
        errorCode = GetLastError();
        return errorCode;
    }

    // Event: buffer ready
    // ---------------------------------------------------------
    LPCTSTR DBWIN_BUFFER_READY = _T("DBWIN_BUFFER_READY");
    HANDLE m_hEventBufferReady = ::OpenEvent(
        EVENT_ALL_ACCESS,
        FALSE,
        DBWIN_BUFFER_READY
        );

    if (m_hEventBufferReady == NULL) {
        m_hEventBufferReady = ::CreateEvent(
            NULL,
            FALSE,    // auto-reset
            TRUE,    // initial state: signaled
            DBWIN_BUFFER_READY
            );

        if (m_hEventBufferReady == NULL) {
            errorCode = GetLastError();
            return errorCode;
        }
    }

    // Event: data ready
    // ---------------------------------------------------------
    LPCTSTR DBWIN_DATA_READY = _T("DBWIN_DATA_READY");
    HANDLE m_hEventDataReady = ::OpenEvent(
        SYNCHRONIZE,
        FALSE,
        DBWIN_DATA_READY
        );

    if (m_hEventDataReady == NULL) {
        m_hEventDataReady = ::CreateEvent(
            NULL,
            FALSE,    // auto-reset
            FALSE,    // initial state: nonsignaled
            DBWIN_DATA_READY
            );

        if (m_hEventDataReady == NULL) {
            errorCode = GetLastError();
            return errorCode;
        }
    }

    // Shared memory
    // ---------------------------------------------------------
    LPCTSTR DBWIN_BUFFER = _T("DBWIN_BUFFER");
    HANDLE m_hDBMonBuffer = ::OpenFileMapping(
        FILE_MAP_READ,
        FALSE,
        DBWIN_BUFFER
        );

    if (m_hDBMonBuffer == NULL) {
        m_hDBMonBuffer = ::CreateFileMapping(
            INVALID_HANDLE_VALUE,
            NULL,
            PAGE_READWRITE,
            0,
            sizeof(struct dbwin_buffer),
            DBWIN_BUFFER
            );

        if (m_hDBMonBuffer == NULL) {
            errorCode = GetLastError();
            return errorCode;
        }
    }

    struct dbwin_buffer * m_pDBBuffer = (struct dbwin_buffer *)::MapViewOfFile(
        m_hDBMonBuffer,
        SECTION_MAP_READ,
        0,
        0,
        0
        );

    if (m_pDBBuffer == NULL) {
        errorCode = GetLastError();
        return errorCode;
    }

    // Monitoring thread
    // ---------------------------------------------------------
    BOOL m_bWinDebugMonStopped = FALSE;

    HANDLE m_hWinDebugMonitorThread = ::CreateThread(
        NULL,
        0,
        WinDebugMonitorThread,
        NULL,
        0,
        NULL
        );

    if (m_hWinDebugMonitorThread == NULL) {
        m_bWinDebugMonStopped = TRUE;
        errorCode = GetLastError();
        return errorCode;
    }

    // set monitor thread's priority to highest
    // ---------------------------------------------------------
    bSuccessful = ::SetPriorityClass(
        ::GetCurrentProcess(),
        REALTIME_PRIORITY_CLASS
        );

    bSuccessful = ::SetThreadPriority(
        m_hWinDebugMonitorThread,
        THREAD_PRIORITY_TIME_CRITICAL
        );

    return errorCode;
}

using huys::UDialogBox;

class UDialogExt : public UDialogBox
{
    enum {
        IDM_OPEN = 2000,
        IDM_SAVE = 2001,
        IDM_PIN  = 2002,
        IDM_HELP = 2003
    };

    enum {
        IDC_LISTBOX_DEBUG = 1001
    };
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
    : UDialogBox(hInst, nID)
    {}

    virtual BOOL onInit()
    {
        win = new AUI::UTraceWindow(m_hDlg);
        win->create();

        addToolbar();

        this->setDlgIconBig(::LoadIcon(m_hInst, MAKEINTRESOURCE(IDI_EYE)));

        listbox = new UListBox(m_hDlg, IDC_LISTBOX_DEBUG, m_hInst);

        huys::URectL rect;
        ::GetClientRect(m_hDlg, rect);
        //
        rect.offsetY(32, 0);

        listbox->setRect(rect);
        listbox->create();

        return TRUE;
    }

    BOOL onLButtonDown(WPARAM wParam, LPARAM lParam)
    {
        win->addLine("Here it is!");
        return FALSE;
    }

    BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch (LOWORD(wParam))
        {
        case IDM_OPEN:
            return onMenuOpen();
        case IDM_SAVE:
            return onMenuSave();
        case IDM_PIN:
            return onMenuPin();
        case IDM_HELP:
            return onMenuHelp();
        default:
            return UDialogBox::onCommand(wParam, lParam);
        }
    }
protected:
private:
    void addToolbar()
    {
        toolbar = new UToolBar(m_hDlg, 0, m_hInst);
        toolbar->setStyles(TBSTYLE_FLAT);
        toolbar->create();
        toolbar->addBitmap(1, IDB_OPEN);
        toolbar->addBitmap(1, IDB_SAVE);
        toolbar->addBitmap(1, IDB_PIN);
        toolbar->addBitmap(1, IDB_HELP);
        toolbar->addButton(0, IDM_OPEN, TBSTATE_ENABLED, TBSTYLE_AUTOSIZE);
        toolbar->addButton(1, IDM_SAVE, TBSTATE_ENABLED, TBSTYLE_AUTOSIZE);
        toolbar->addSeparator(4);
        toolbar->addButton(2, IDM_PIN, TBSTATE_ENABLED, TBSTYLE_AUTOSIZE);
        toolbar->addSeparator(4);
        toolbar->addButton(3, IDM_HELP, TBSTATE_ENABLED, TBSTYLE_AUTOSIZE);
        toolbar->autosize();
        toolbar->show();
    }

    BOOL onMenuOpen()
    {
        win->addLine(_T("On Menu Open!"));
        return FALSE;
    }

    BOOL onMenuSave()
    {
        win->addLine(_T("On Menu Save!"));
        return FALSE;
    }

    BOOL onMenuPin()
    {
        win->addLine(_T("On Menu Pin!"));
        BYTE state = toolbar->getState(IDM_PIN);
        if (state & TBSTATE_CHECKED)
        {
            this->setNoTopMost();
            toolbar->setState(IDM_PIN, state& ~TBSTATE_CHECKED);
            win->addLine(_T("Disable Topmost!"));
        }
        else
        {
            this->setTopMost();
            toolbar->setState(IDM_PIN, state|TBSTATE_CHECKED);
            win->addLine(_T("Enable Topmost!"));
        }

        return FALSE;
    }

    BOOL onMenuHelp()
    {
        win->addLine(_T("On Menu Help!"));
        return FALSE;
    }
private:
    AUI::UTraceWindowP win;
    huys::ADT::UAutoPtr<UToolBar> toolbar;
    huys::ADT::UAutoPtr<UListBox> listbox;
};

UDLGAPP_T(UDialogExt, IDD_TEST);


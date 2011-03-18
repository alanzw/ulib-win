//
// HTTPUI.C        Hyper Text Transfer Protocol Server
//
//                User Interface Module
//
//                Multi-Threaded Model
//                One thread handles the user interface
//                Another handles incoming connections
//                Other threads are created to handle
//                simultaneous connections
//

//
// Application/User Interface include
//
#include "httpui.h"

//
// Message for HTTPSERV informational notifications
//
#define UM_HTTP     WM_USER+2


char gszAppName[]  = {"HTTPSRVM"};
char gszPortKey[]  = {"PORT"};
char gszDirKey[]   = {"ROOTDIR"};


////////////////////////////////////////////////////////////

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    WORD wVersionRequested = MAKEWORD(2,2);
    WSADATA wsaData;
    int nRet;

    //
    // Initialize WinSock
    //
    nRet = WSAStartup(wVersionRequested, &wsaData);
    if (nRet)
    {
        MessageBox(NULL,
                   "Initialize WinSock Failed",
                   gszAppName,
                   MB_OK);
        return 1;
    }
    // Check version
    if (wsaData.wVersion != wVersionRequested)
    {
        MessageBox(NULL,
                   "Wrong WinSock Version",
                   gszAppName,
                   MB_OK);
        return 2;
    }

    //
    // Use new common controls
    //
    InitCommonControls();

    //
    // Use a dialog as a main window
    //
    DialogBox(hInstance,
              MAKEINTRESOURCE(IDD_MAINWND),
              NULL,
              MainWndProc);

    //
    // Release WinSock
    //
    WSACleanup();
    return(0);
}

////////////////////////////////////////////////////////////

BOOL CALLBACK MainWndProc(HWND hwnd,
                          UINT uMsg,
                          WPARAM wParam,
                          LPARAM lParam)
{
    switch (uMsg)
    {
        HANDLE_DLG_MSG(hwnd, WM_INITDIALOG, OnInitDialog);
        HANDLE_DLG_MSG(hwnd, WM_COMMAND,    OnCommand);

        //
        // Handle messages from HTTPMT.C
        //
        case UM_HTTP:
            switch(wParam)
            {
                case HTTP_FILEOK_MSG:
                    FileRequested(hwnd, 0, (LPCSTR)lParam);
                    break;
                case HTTP_FILENOTFOUND_MSG:
                    FileRequested(hwnd, 404, (LPCSTR)lParam);
                    break;
                case HTTP_STATS_MSG:
                    Statistics(hwnd, (LPHTTPSTATS)lParam);
                    break;
                case HTTP_EVENT_MSG:
                    ShowEvent(hwnd, (LPCSTR)lParam);
                    break;
            }
            return TRUE;
    }
    return FALSE;
}

////////////////////////////////////////////////////////////

BOOL OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
    LV_COLUMN col;
    HWND hwndList;
    TEXTMETRIC tm;

    //
    // Associate an icon with the dialog box.
    //
    SetClassLong(hwnd,
                 GCL_HICON,
                 (LONG)LoadIcon(GetWindowInstance(hwnd),
                 MAKEINTRESOURCE(IDI_APP)));

    //
    // Setup List Control
    //
    hwndList = GetDlgItem(hwnd, IDC_LIST);
    GetTextMetrics(GetDC(hwnd), &tm);

    col.mask    = LVCF_FMT|LVCF_TEXT|LVCF_WIDTH;
    col.fmt     = LVCFMT_LEFT;
    col.pszText = "File";
    col.cx      = tm.tmAveCharWidth*50;
    ListView_InsertColumn(hwndList, 0, &col);

    col.fmt     = LVCFMT_RIGHT;
    col.pszText = "Hits";
    col.cx      = tm.tmAveCharWidth*5;
    ListView_InsertColumn(hwndList, 1, &col);

   return(TRUE);
}

////////////////////////////////////////////////////////////

void OnCommand(HWND hwnd, int nId, HWND hwndCtl, UINT codeNotify)
{
    static BOOL  fStarted = FALSE;
    HWND         hwndButton;
    HTTPSERVINFO si;
    char         szRootDir[_MAX_PATH];
    int          nRet;

    switch (nId)
    {
        //
        // Start/Stop HTTP Server
        //
        case IDC_START_STOP:
            hwndButton = GetDlgItem(hwnd, IDC_START_STOP);

            if (!fStarted)
            {
                //
                // Get user preferences
                //
                nRet = DialogBox(GetWindowInstance(hwnd),
                          MAKEINTRESOURCE(IDD_SERVINFO),
                          hwnd,
                          InfoWndProc);
                if (nRet == IDOK)
                {
                    // Fill in the server info
                    si.nPort = GetProfileInt(gszAppName,
                                     gszPortKey,
                                     0);
                    GetProfileString(gszAppName,
                             gszDirKey,
                             "",
                             szRootDir,
                             sizeof(szRootDir));
                    si.lpRootDir = szRootDir;
                    si.hwnd      = hwnd;
                    si.uMsgApp   = UM_HTTP;
                    if (StartHTTP(&si))
                    {
                        SetWindowText(hwndButton, "&Stop");
                        fStarted = TRUE;
                    }
                }
            }
            else
            {
                StopHTTP();
                SetWindowText(hwndButton, "&Start");
                fStarted = FALSE;
            }
            break;

        case IDCANCEL:
            //
            // If HTTP is running, stop it
            //
            if (fStarted)
                StopHTTP();
            fStarted = FALSE;
            EndDialog(hwnd, 0);
            break;
   }
}

////////////////////////////////////////////////////////////

void FileRequested(HWND hwnd, int nError, LPCSTR lpFileName)
{
    char szBuf[256];
    char szDisplayName[_MAX_PATH+10];
    DWORD dwHitCount;
    HWND hwndList = GetDlgItem(hwnd, IDC_LIST);
    LV_FINDINFO fi;
    LV_ITEM        lvItem;
    int nItem;

    //
    // Was the file found?
    //
    if (!nError)
        strcpy(szDisplayName, lpFileName);
    else
        wsprintf(szDisplayName,
                 "%d - %s",
                 nError,
                 lpFileName);

    //
    // See if the item is already in the list
    //
    fi.flags = LVFI_STRING;
    fi.psz   = szDisplayName;
    nItem = ListView_FindItem(hwndList, -1, &fi);

    //
    // If it is, increment the hit count
    //
    if (nItem != -1)
    {
        lvItem.mask = LVIF_TEXT;
        lvItem.iItem = nItem;
        lvItem.iSubItem = 1;
        lvItem.pszText = szBuf;
        lvItem.cchTextMax = sizeof(szBuf);
        if (ListView_GetItem(hwndList, &lvItem))
        {
            dwHitCount = atol(szBuf);
            dwHitCount++;
            wsprintf(szBuf, "%ld", dwHitCount);
            ListView_SetItem(hwndList, &lvItem);
            return;
        }
    }

    //
    // Add a new item to the list
    //
    lvItem.mask = LVIF_TEXT;
    lvItem.iItem = 0;
    lvItem.iSubItem = 0;
    lvItem.pszText = szDisplayName;
    nItem = ListView_InsertItem(hwndList, &lvItem);
    lvItem.iItem = nItem;
    lvItem.iSubItem = 1;
    lvItem.pszText = "1";
    ListView_SetItem(hwndList, &lvItem);
}

////////////////////////////////////////////////////////////

void Statistics(HWND hwnd, LPHTTPSTATS lpStats)
{
    static DWORD dwTransferred = 0L;
    static DWORD dwConnections = 0L;
    static DWORD dwReceived    = 0L;
    static DWORD dwRate        = 0L;
    DWORD dwX;
    char szBuf[sizeof("4294967295 ")];

    #define UPDATENUMBER(n, id)    \
        wsprintf(szBuf,"%ld", n); \
        SetDlgItemText(hwnd, id, szBuf);

    dwConnections++;
    UPDATENUMBER(dwConnections, IDC_CONNECTIONS);

    //
    // Convert elapsed time to seconds
    //
    dwX = lpStats->dwElapsedTime/1000L;
    if (dwX > 1)
        dwX = (lpStats->dwSend+lpStats->dwRecv) / dwX;
    else
        dwX = (lpStats->dwSend+lpStats->dwRecv);
    if (dwRate > 0)
        dwRate = (dwRate+dwX)/2;
    else
        dwRate = dwX;

    UPDATENUMBER(dwRate, IDC_RATE);
    dwReceived += lpStats->dwRecv;
    UPDATENUMBER(dwReceived, IDC_RECEIVED);
    dwTransferred += lpStats->dwSend;
    UPDATENUMBER(dwTransferred, IDC_TRANSFERRED);
}

////////////////////////////////////////////////////////////

void ShowEvent(HWND hwnd, LPCSTR lpEvent)
{
    char szBuf[284];
    SYSTEMTIME st;
    HWND hwndEvents = GetDlgItem(hwnd, IDC_EVENTLOG);

    GetLocalTime(&st);
    wsprintf(szBuf,"%02d:%02d:%02d.%03d\t%s",
            st.wHour,   st.wMinute,
            st.wSecond, st.wMilliseconds,
            lpEvent);
    ListBox_AddString(hwndEvents, szBuf);
    if (ListBox_GetCount(hwndEvents) > 500)
        ListBox_DeleteString(hwndEvents, 0);
}

////////////////////////////////////////////////////////////

BOOL CALLBACK InfoWndProc(HWND hwnd,
                          UINT uMsg,
                          WPARAM wParam,
                          LPARAM lParam)
{
    char szText[256];
    BOOL fRet = 0;
    switch (uMsg)
    {
        case WM_INITDIALOG:
            //
            // Fill the controls with previous entries
            //
            GetProfileString(gszAppName,
                             gszPortKey,
                             "",
                             szText,
                             sizeof(szText));
            SetDlgItemText(hwnd,
                           IDC_PORT,
                           szText);

            GetProfileString(gszAppName,
                             gszDirKey,
                             "",
                             szText,
                             sizeof(szText));
            SetDlgItemText(hwnd,
                           IDC_ROOTDIR,
                           szText);
            fRet = TRUE;
        case WM_COMMAND:
            switch(wParam)
            {
                case IDOK:
                    //
                    // Save the preferences
                    //
                    GetDlgItemText(hwnd,
                                   IDC_PORT,
                                   szText,
                                   sizeof(szText));
                    WriteProfileString(gszAppName,
                                       gszPortKey,
                                       szText);

                    GetDlgItemText(hwnd,
                                   IDC_ROOTDIR,
                                   szText,
                                   sizeof(szText));
                    WriteProfileString(gszAppName,
                                       gszDirKey,
                                       szText);
                    fRet = TRUE;
                    EndDialog(hwnd, IDOK);
                    break;
                case IDCANCEL:
                    fRet = TRUE;
                    EndDialog(hwnd, IDCANCEL);
                    break;
            }
            break;
    }
    return fRet;
}

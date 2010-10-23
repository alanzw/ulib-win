#include "wshout.h"

#include <stdio.h>
#include <io.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAJOR_VERSION 2
#define MINOR_VERSION 2
#define WSA_MAKEWORD(x,y) ((y) * 256 + (x)) /* HI:Minor, LO:Major */


HANDLE hInst; /* ����ʵ�����  */
HWND hOurWnd; /* �����ھ��  */
HWND hMainDlg; /* ���Ի�����  */
int ret; /* ��������  */
char prbuf[PRBUF_LEN]; /* ������ʾ�ı��Ĺ���������  */
SOCKET sd; /* ���ڼ������ӵ��׽ӿ�������  */
long temporary_option      = 0L; /* ȱʡΪ����ģʽ  */
long blocking_option        = 0L; /* ����ģʽ��ȫ�ֱ�ʶ  */
int run_cancelled = 0; /* ָʾ��ʱ������ȡ����ť  */
int len = 1024; /* һ��д���ֽ���  */
BOOL running = FALSE; /* ���������״̬  */
const int iTCP = 1; /* ָ��ΪTCP Shout */
const int iUDP = 2; /* ָ��ΪUDP Shout */
int iProto = 1; /* ȱʡΪTCP Shout */
int iPortNo = SOCK_SHOUT;
int temporary_protocol    = 1; /* ��Settings()��ʹ��  */
int iShout    = 1;

int iListen = 2;
int iClientOrServer = 1; /* ȱʡΪShout���ͻ�����*/
int tClientOrServer = 1; /* ��Settings()��ʹ��  */
char HostModeBuf[20];/* ����ģʽ�ַ���  */
WORD VersionReqd;
LPWSADATA lpmyWSAData;

int PASCAL WinMain (HANDLE hInstance,HANDLE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
    HWND hWnd;
    MSG msg;
    BOOL InitApp(hInstance);
    if (!hPrevInstance)
    if (!InitApp(hInstance))
    return (NULL);
    hInst = hInstance;
    hWnd = CreateWindow("MainMenu",
    "Windows Shout",
    WS_OVERLAPPEDWINDOW | WS_SYSMENU | WS_MINIMIZEBOX,
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    NULL,
    NULL,
    hInstance,
    NULL);
    if (!hWnd)
    return (NULL);
    hOurWnd = hWnd;
    
    while (GetMessage(&msg, NULL, NULL, NULL)) {
        TranslateMessage(&msg); /* �����������  */
        DispatchMessage(&msg);
    }
    return (msg.wParam);
}

BOOL InitApp(HANDLE hInstance)
{
    HANDLE hMemory;
    PWNDCLASS pWndClass;
    BOOL bSuccess;
    hMemory = LocalAlloc(LPTR, sizeof(WNDCLASS));
    pWndClass = (PWNDCLASS) LocalLock(hMemory);
    pWndClass->hCursor = LoadCursor(NULL, IDC_ARROW);
    pWndClass->hIcon = LoadIcon(hInstance, (LPSTR) "SHOUT");
    pWndClass->lpszMenuName = (LPSTR) "MainMenu";
    pWndClass->lpszClassName = (LPSTR) "MainMenu";
    pWndClass->hbrBackground = GetStockObject(WHITE_BRUSH);
    pWndClass->hInstance = hInstance;
    pWndClass->style = NULL;
    pWndClass->lpfnWndProc = ShoutWndProc;
    bSuccess = RegisterClass(pWndClass);
    LocalUnlock(hMemory);
    LocalFree(hMemory);
    return (bSuccess);
}
long FAR PASCAL ShoutWndProc(HWND hWnd, WORD message,WORD wParam, LONG lParam)
{
    FARPROC lpDialogBoxProc;
    switch (message){
    case WM_CREATE:
        /* Put up the dialog box */
        lpDialogBoxProc = MakeProcInstance(DialogProc, hInst);
        DialogBox (hInst, (LPSTR) "MainDialog", hWnd, lpDialogBoxProc) ;
        FreeProcInstance(lpDialogBoxProc);
        PostMessage(hWnd, WM_DESTROY, 0, 0L);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return(DefWindowProc(hWnd, message, wParam, lParam));
    }
    return NULL;
}

BOOL  FAR  PASCAL  DialogProc(HWND  hOurDlg,  WORD  message,  WORD  wParam,  LONG
lParam)
{
    FARPROC lpProcAbout;
    FARPROC lpProcSettings;
    long lret;
    WORD wMajorVersion, wMinorVersion;
    char hostnm[64]; /* �����������Ĺ���������  */

    switch (message) {
    case WM_INITDIALOG:
        /* ѡ��ȱʡ����  */
        SetDlgItemText(hOurDlg, IDD_HNAME, "");
        SendDlgItemMessage(hOurDlg, /* �Ի�����  */
        IDD_HNAME, /* ��δ����� msg */
        EM_SETSEL, /* ѡ���ַ�  */
        NULL, /* ������Ϣ  */
        MAKELONG(0, 0x7fff)); /* ȫ������  */
        SetFocus(GetDlgItem(hOurDlg, IDD_HNAME));
        /* ��ʼ��  */
        hMainDlg = hOurDlg;    /* �����Լ��Ĵ��ھ��  */
        SetDlgItemText(hOurDlg, IDD_COHOST,"Shout to:");
        wMajorVersion = MAJOR_VERSION;
        wMinorVersion = MINOR_VERSION;
        VersionReqd=WSA_MAKEWORD(wMajorVersion, wMinorVersion);
        lpmyWSAData = (LPWSADATA)_calloc(1, sizeof(WSADATA));
        ret = WSAStartup(VersionReqd, lpmyWSAData);
        if (ret != 0){
            wshout_err (hOurDlg, WSAGetLastError(), "WSAStartup()");
        }
        return (TRUE);
    case WM_CLOSE:
        PostMessage(hOurDlg, WM_COMMAND, IDM_EXIT, 0L);
        break;
    case WM_SYSCOMMAND:
        SendMessage(hOurWnd, message, wParam, lParam);
        break;
    case WM_COMMAND:
        switch (wParam) {
        case IDD_CONNECT: /* �������Ӱ�ť  */
        case IDM_START: /* ѡ���� Start�˵���  */
            run_cancelled = FALSE;
            /* ��������  */
            if (running){
                MessageBox(hOurWnd,"Shout is already running !",
                "Shout", MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
                return FALSE;
            }
            ClearBoxes(hOurDlg);
            running = TRUE;
            if (iClientOrServer == iShout) {
                /* ȷ����������  */
                if (GetDlgItemText (hOurDlg, IDD_HNAME, hostnm, 80) < 2) {
                    MessageBeep(0);
                    SetDlgItemText(hOurDlg,
                    IDD_COMMENT,"No hostname specified");
                    running = FALSE;
                    break;
                }
                sd = ResolveAndConnectHost((char FAR *)hostnm,hOurDlg,iProto,
                iPortNo);
                if (sd == SOCKET_ERROR) {/* �޷������׽ӿ�  */
                    running = FALSE;
                    break;
                }
            }
            else {
                sd = GetSocketAndBind(hOurDlg, iProto, iPortNo);
                if (sd == SOCKET_ERROR) {
                    running = FALSE;
                    break;
                }
            }
            /* Set the I/O mode of the socket */
            if (blocking_option) {
                lret = 1L;    /* ������ģʽ  */
                ioctlsocket(sd, FIONBIO, (u_long FAR *) &lret);
            }
            else {
                lret = 0L; /* ����ģʽ  */
                ioctlsocket(sd, FIONBIO, (u_long FAR *) &lret);
            }
            if (iClientOrServer == iShout) {    /* SHOUT */
                /* �������ݲ�д���׽ӿ�  */
                if (iProto == iTCP)
                lret = TWriteData(sd, hOurDlg, len);
                else /* UDP */
                lret = UWriteData(sd, hOurDlg, len);
            }
            else { /* LISTEN */
                if (iProto == iTCP)
                lret = TReadData(sd,hOurDlg, len);
                else /* UDP */
                lret = UReadData(sd,hOurDlg, len);
            }
            closesocket(sd);
            running = FALSE;
            break;
        case IDD_CANCEL:
            if (running) {
                /* ֹͣ  */
                ret = WSACancelBlockingCall();
                run_cancelled = TRUE;
                if (ret == SOCKET_ERROR) {
                    /* WSANOTINITIALISED or WSAENETDOWN or WSAEINVAL */
                    if (h_errno == WSAENETDOWN) {
                        /* Watch out for hAcceptSock! */
                        /* close what is left of the connection */
                        closesocket(sd);
                    }
                }
            }
            break;
        case IDM_EXIT:
            ret = WSACleanup();
            if (ret == SOCKET_ERROR && h_errno == WSAEINPROGRESS){
                MessageBox(hOurWnd,
                "Data transfer in progress.\nStop transfer first.",
                "WndProc()", MB_OK | MB_APPLMODAL|MB_ICONINFORMATION);
                break; /* һ���׽ӿ�����������״̬  */
            }
            _free((char NEAR *) lpmyWSAData);
            EndDialog(hOurDlg, TRUE) ; /* �˳�  */
            break;
        case IDM_ABOUT:
            lpProcAbout = MakeProcInstance(About, hInst);
            DialogBox(hInst, "AboutBox", hOurDlg, lpProcAbout);
            FreeProcInstance(lpProcAbout);
            break;
        case IDM_SETTINGS:
            lpProcSettings = MakeProcInstance(Settings, hInst);
            DialogBox(hInst, "SettingsDialog", hOurDlg, lpProcSettings);
            FreeProcInstance(lpProcSettings);
            break;
        default:
            break;
        } /* switch (wParam) */
        break;
    } /* switch (message) */
    return FALSE;
}

/* �˺�������About�Ի���  */
BOOL FAR PASCAL About(HWND hDlg, WORD message, WORD wParam, LONG lParam)
{
    char tempBuf[15];
    
    switch (message) {
    case WM_INITDIALOG:
        SetDlgItemText(hDlg, IDA_COPYRIGHT,(LPSTR)lpmyWSAData->szDescription);
        wsprintf(tempBuf, "%d.%2d\n",MAJOR_VERSION, MINOR_VERSION);
        SetDlgItemText(hDlg, IDA_APP_VERSION, (LPSTR) tempBuf);
        wsprintf(tempBuf, "%d.%2d\n",
        lpmyWSAData->wVersion%256,lpmyWSAData->wVersion/256);
        SetDlgItemText (hDlg, IDA_DLL_VERSION, (LPSTR) tempBuf);
        return (FALSE);
    case WM_COMMAND:
        if (wParam == IDOK
                || wParam == IDCANCEL) {
            EndDialog(hDlg, TRUE);
            return (TRUE);
        }
        break;
    }
    return (FALSE);
}

/* �˺�������Settings �Ի���  */
BOOL FAR PASCAL Settings(HWND hDlg, WORD message, WORD wParam, LONG lParam)
{
    int buffer_len = len;
    int port_no        = iPortNo;
    
    switch (message) {
    case WM_INITDIALOG:
        /* Select a default send() buffer length */
        SetDlgItemInt(hDlg, IDS_BUFFLEN, len, 0);
        /* Select a default port number */
        SetDlgItemInt(hDlg, IDS_PORTNO, iPortNo, 0);
        if (iClientOrServer == iShout) /* ��������  */
        CheckThisProgBoxOn(hDlg, IDS_CLIENT);
        else
        CheckThisProgBoxOn(hDlg, IDS_SERVER);
        if (iProto == iTCP) /* Э������  */
        CheckThisProtoBoxOn(hDlg, IDS_TCP);
        else
        CheckThisProtoBoxOn(hDlg, IDS_UDP);
        if (!blocking_option) /* ����ģʽ  */
        CheckThisBoxOn(hDlg, IDS_BLOCK);
        else
        CheckThisBoxOn(hDlg, IDS_NOBLOCK);
        SendDlgItemMessage(hDlg, /* dialog handle */
        IDS_PORTNO, /* where to send msg */
        EM_SETSEL, /* select characters */
        NULL, /* additional info */
        MAKELONG(0, 0x7fff)); /* entire contents */
        SendDlgItemMessage(hDlg, /* dialog handle */
        IDS_BUFFLEN, /* where to send msg */
        EM_SETSEL, /* select characters */
        NULL, /* additional info */
        MAKELONG(0, 0x7fff)); /* entire contents */
        SetFocus(GetDlgItem(hDlg, IDS_BUFFLEN));
        return (TRUE);
    case WM_COMMAND:
        switch (wParam){
        case IDS_CLIENT:
            /* USer has set to Shout */
            CheckThisProgBoxOn(hDlg, IDS_CLIENT);
            tClientOrServer = iShout;
            SetDlgItemText(hMainDlg, IDD_COHOST,"Foreign host:");
            SetDlgItemText(hMainDlg, IDD_HNAME,"");
            break;
        case IDS_SERVER:
            /* USer has set to Listen */
            CheckThisProgBoxOn(hDlg, IDS_SERVER);
            tClientOrServer = iListen;
            SetDlgItemText(hMainDlg, IDD_COHOST,"Listening to:");
            SetDlgItemText(hMainDlg, IDD_HNAME,"[Hit 'Start']");
            break;
        case IDS_TCP:
            /* USer has set to TCP */
            CheckThisProtoBoxOn(hDlg, IDS_TCP);
            temporary_protocol = iTCP;
            break;
        case IDS_UDP:
            /* USer has set to UDP */
            CheckThisProtoBoxOn(hDlg, IDS_UDP);
            temporary_protocol = iUDP;
            break;
        case IDS_BLOCK:
            /* User has set to blocking mode */
            CheckThisBoxOn(hDlg, IDS_BLOCK);
            temporary_option = 0L;
            break;
        case IDS_NOBLOCK:
            /* User has set to nonblocking mode */
            CheckThisBoxOn(hDlg, IDS_NOBLOCK);
            temporary_option = 1L;
            break;
        case IDOK:
            /* �û�����ɶ����õ��޸�  */
            buffer_len = GetDlgItemInt(hDlg, IDS_BUFFLEN, NULL, 0);
            if (buffer_len == 0 || buffer_len > 8192) {
                MessageBox(hOurWnd, "Buffer length must be between 1 and 8K",
                "Settings()",
                MB_OK | MB_APPLMODAL | MB_ICONSTOP);
                return (FALSE);
            }
            port_no = GetDlgItemInt(hDlg, IDS_PORTNO, NULL, 0);
            if (port_no == 0) {
                MessageBox(hDlg, "Port number must be between 0 and 65,535",
                "Settings()",
                MB_OK | MB_APPLMODAL | MB_ICONSTOP);
                return (FALSE);
            }
            len          = buffer_len;
            iPortNo = port_no;
            blocking_option = temporary_option;
            iProto         = temporary_protocol;
            iClientOrServer = tClientOrServer;
        case IDCANCEL:
            /* �û�����ı�����  */
            EndDialog(hDlg, TRUE);
            return (TRUE);
            
        default:
            break;
        }
    default:
        break;
    }
    return (FALSE);
}

void CheckThisBoxOn(HWND hDlg, int ButtonID)
{
    switch (ButtonID) {
    case IDS_BLOCK:
        CheckDlgButton(hDlg, IDS_BLOCK, 1);
        CheckDlgButton(hDlg, IDS_NOBLOCK, 0);
        break;
    case IDS_NOBLOCK:
        CheckDlgButton(hDlg, IDS_BLOCK, 0);
        CheckDlgButton(hDlg, IDS_NOBLOCK, 1);
        break;
    default:
        break;
    }
    return;
}

void CheckThisProtoBoxOn(HWND hDlg, int ButtonID)
{
    switch (ButtonID) {
    case IDS_TCP:
        CheckDlgButton(hDlg, IDS_TCP, 1);
        CheckDlgButton(hDlg, IDS_UDP, 0);
        break;
    case IDS_UDP:
        CheckDlgButton(hDlg, IDS_TCP, 0);
        CheckDlgButton(hDlg, IDS_UDP, 1);
        break;
    default:
        break;
    }
    return;
}
void CheckThisProgBoxOn(HWND hDlg, int ButtonID)
{
    switch (ButtonID) {
    case IDS_CLIENT: /* Shout */
        CheckDlgButton(hDlg, IDS_CLIENT, 1);
        CheckDlgButton(hDlg, IDS_SERVER, 0);
        break;
    case IDS_SERVER: /* Listen */
        CheckDlgButton(hDlg, IDS_CLIENT, 0);
        CheckDlgButton(hDlg, IDS_SERVER, 1);
        break;
    default:
        break;
    }
    return;
}

/* ���¾���������δ���ģ���������������������Ϣ���У����������Ҫ�������Ϣ����
����һ������ֵ��*/
int
ShoutBlockingHook (void)
{
    MSG msg; /* lets us pull messages via PeekMessage */
    int ret = PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
    if (ret)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return ret;
}

void
ClearBoxes(HWND hOurDlg)
{
    wsprintf(prbuf,"                  \n");
    SetDlgItemText(hOurDlg, IDD_WRITE, (LPSTR) prbuf);
    SetDlgItemText(hOurDlg, IDD_SENT, (LPSTR) prbuf);
    SetDlgItemText(hOurDlg, IDD_TIME, (LPSTR) prbuf);
    SetDlgItemText(hOurDlg, IDD_WRITES,(LPSTR) prbuf);
    SetDlgItemText(hOurDlg, IDD_BYTES, (LPSTR) prbuf);
    SetDlgItemText(hOurDlg, IDD_BITS,    (LPSTR) prbuf);
    
    return;
}
/*
* wshout_err()����
* ����:
*
* ͨ����������ȡ��Ӧ�Ĵ��������ı������û��ṩ�Ĵ���ǰ׺��
* ��������ʾ�ڶԻ����С�
*/
void wshout_err (HWND hOurDlg, int wsa_err, char far *err_prefix)
{
    char errbuf[PRBUF_LEN];  /* ���������ַ���������  */
    /* ��ȡ���������ַ���  */
    WSAsperror(hInst, wsa_err, (LPSTR)errbuf, PRBUF_LEN);
    /* �ϲ����������ַ������û�����ǰ׺�ַ���  */
    wsprintf((LPSTR)prbuf, "%s:%s", (LPSTR) err_prefix, (LPSTR)errbuf);
    
    /* �ڶԻ�������ʾ�����ı�  */
    SetDlgItemText(hOurDlg, IDD_COMMENT, (LPSTR) prbuf);
}    /* end wshout_err() */

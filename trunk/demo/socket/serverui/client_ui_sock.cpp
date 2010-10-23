#include "resource.h"

#define USERPORT 10001
#define UM_SOCK  WM_USER+0x100

#include <windows.h>
#include <winsock2.h>
#include <stdio.h>

#define MAJOR_VERSION 2
#define MINOR_VERSION 2
#define WSA_MAKEWORD(x,y) ((y)*256+(x))

HANDLE hInst;
char server_address[256] = {0};
char buffer[1024];
char FAR* lpBuffer = &buffer[0];
SOCKET s = 0;
struct sockaddr_in dst_addr;
struct hostent far *hostaddr;
struct hostent hostnm;
struct servent far *sp;
int count;

BOOL InitApplication(HINSTANCE hInstance);
long FAR PASCAL ClientProc(HWND hWnd, unsigned message, UINT wParam, LONG lParam);
void AlertUser(HWND hWnd, char *message);

BOOL Client(HWND hWnd);
BOOL ReceivePacket(HWND hWnd);
BOOL SetSelect(HWND hWnd, long lEvent);
BOOL SendPacket(HWND hWnd, int len);

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    HWND hWnd;
    MSG msg;

    lstrcpy((LPSTR)server_address, lpCmdLine);
    if (!hPrevInstance)
    {
        if (!InitApplication(hInstance))
        {
            fprintf(stderr, "Error InitApplication!!");
            return FALSE;
        }
    }
    hInst = hInstance;
    hWnd = CreateWindow("ClientClass",
        "Windows ECHO Client",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance,
        NULL);
    
    if (!hWnd)
    {
        fprintf(stderr, "Error CreateWindow!!");
        return FALSE;
    }
    
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    PostMessage(hWnd, WM_USER, (WPARAM)0, (LPARAM)0);
    while (GetMessage(&msg, NULL, NULL, NULL))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return msg.wParam;
}

BOOL InitApplication(HINSTANCE hInstance)
{
    WNDCLASS WndClass;
    char *szAppName = "ClientClass";
    // fill in window class information
    WndClass.lpszClassName = (LPSTR)szAppName;
    WndClass.hInstance          = hInstance;
    WndClass.lpfnWndProc      = ClientProc;
    WndClass.hCursor              = LoadCursor(NULL, IDC_ARROW);
    WndClass.hIcon                  = LoadIcon(hInstance, NULL);
    WndClass.lpszMenuName    = MAKEINTRESOURCE(IDR_CLIENT_MENU);
    WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    WndClass.style                  = CS_HREDRAW | CS_VREDRAW;
    WndClass.cbClsExtra        = 0;
    WndClass.cbWndExtra        = 0;
    // register the class
    if (!RegisterClass(&WndClass))
    return(FALSE);
    return(TRUE);
}

long FAR PASCAL ClientProc(HWND hWnd, unsigned message, UINT wParam, LONG lParam)
{
    int length, i;
    WSADATA wsaData;
    int Status;
    switch (message)
    {
    case WM_USER:
        {
            WORD wMajorVersion, wMinorVersion;
            LPWSADATA lpmyWSAData;
            WORD  VersionReqd;
            int ret;
            wMajorVersion = MAJOR_VERSION;
            wMinorVersion = MINOR_VERSION;
            VersionReqd = WSA_MAKEWORD(wMajorVersion,wMinorVersion);
            lpmyWSAData = (LPWSADATA)malloc(sizeof(WSADATA));
            Status = WSAStartup(VersionReqd, lpmyWSAData);
            if (Status != 0)
            {
                AlertUser(hWnd, "WSAStartup() failed\n");
                PostQuitMessage(0);
            }
            hostaddr = gethostbyname(server_address);
            if (hostaddr == NULL)
            {
                AlertUser(hWnd, "gethostbyname ERROR!\n");
                WSACleanup();
                PostQuitMessage(0);
            }
            memcpy(&hostnm, hostaddr, sizeof(struct hostent));

        }
        break;
    case WM_COMMAND:
        switch (wParam)
        {
        case IDM_START:
            if (!Client(hWnd))
            {
                closesocket(s);
                AlertUser(hWnd, "Start Failed");
            }
            break;
        case IDM_EXIT:
            // WSACleanup();
            PostQuitMessage(0);
            break;
        }
        break;
    case UM_SOCK:
        switch (lParam)
        {
        case FD_CONNECT:
            if (!SetSelect(hWnd, FD_WRITE))
            closesocket(s);
            break;
        case FD_READ:
            if (!ReceivePacket(hWnd))
            {
                AlertUser(hWnd, "Receive Packet Failed.\n");
                closesocket(s);
                break;
            }
            if (!SetSelect(hWnd, FD_WRITE))
            closesocket(s);
            break;
        case FD_WRITE:
            for (i = 0; i < 1024; i ++)
            buffer[i] = (char)'A' + i % 26;
            length = 1024;
            if (!SendPacket(hWnd, length))
            {
                AlertUser(hWnd, "Packet Send Failed!\n");
                closesocket(s);
                break;
            }
            if (!SetSelect(hWnd, FD_READ))
            closesocket(s);
            break;
        case FD_CLOSE:
            if (WSAAsyncSelect(s, hWnd, 0, 0) == SOCKET_ERROR)
            AlertUser(hWnd, "WSAAsyncSelect Failed.\n");
            break;
        default:
            if (WSAGETSELECTERROR(lParam) != 0)
            {
                AlertUser(hWnd, "Socket Report Failure.");
                closesocket(s);
                break;
            }
            break;
        }
        break;
    case WM_DESTROY:
        closesocket(s);
        WSACleanup();
        PostQuitMessage(0);
        break;
    default:
        return (DefWindowProc(hWnd, message, wParam, lParam));
    }
    return(NULL);
}

void AlertUser(HWND hWnd, char *message)
{
    MessageBox(hWnd, (LPSTR)message, "Warning", MB_ICONEXCLAMATION);
    return;
}

BOOL Client(HWND hWnd)
{
    memset(&dst_addr,'\0', sizeof (struct sockaddr_in));
    memcpy((char FAR *)&dst_addr.sin_addr,(char FAR *)hostnm.h_addr,hostnm.h_length);
    dst_addr.sin_family = hostnm.h_addrtype;
    dst_addr.sin_port = htons(USERPORT);
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        AlertUser(hWnd, "Socket Failed");
        return (FALSE);
    }
    if (!SetSelect(hWnd, FD_CONNECT))
    return (FALSE);
    connect(s, (struct sockaddr FAR *)&dst_addr, sizeof(dst_addr));
    return (TRUE);
}

BOOL ReceivePacket(HWND hWnd)
{
    HDC hDc;
    int length;
    int i1,i2,i3;
    char line1[255], line2[255], line3[255];
    count ++;
    if ((length = recv(s, lpBuffer, 1024, 0)) == SOCKET_ERROR)
    return (FALSE);
    if (length == 0)
    return (FALSE);
    if (hDc = GetDC(hWnd))
    {
        i1 = wsprintf((LPSTR)line1, "TCP Echo Client No.%d", count);
        i2 = wsprintf((LPSTR)line2, "Receive %d bytes",length);
        i3 = wsprintf((LPSTR)line3, "Those are:%c, %c, %c, %c, %c,%c",
            buffer[0],buffer[1],buffer[2],buffer[100],buffer[1000],buffer[1023]);
        TextOut(hDc, 10, 2, (LPSTR)line1, i1);
        TextOut(hDc, 10, 22, (LPSTR)line2, i2);
        TextOut(hDc, 10, 42, (LPSTR)line3, i3);
        ReleaseDC(hWnd, hDc);
    }
    return (TRUE);
}

BOOL SetSelect(HWND hWnd, long lEvent)
{
    if (WSAAsyncSelect(s, hWnd, UM_SOCK, lEvent) == SOCKET_ERROR)
    {
        AlertUser(hWnd, "WSAAsyncSelect Failure.");
        return (FALSE);
    }
    return (TRUE);
}

BOOL SendPacket(HWND hWnd, int len)
{
    int length;
    if ((length = send(s, lpBuffer, len, 0)) == SOCKET_ERROR)
    return (FALSE);
    else
    if (length != len)
    {
        AlertUser(hWnd, "Send Length NOT Match!");
        return (FALSE);
    }
    return (TRUE);
}

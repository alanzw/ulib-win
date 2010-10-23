#ifndef _WSHOUT_INC_
#define _WSHOUT_INC_

#include <windows.h>
#include <winsock2.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

extern HANDLE hInst; /* Instance handle */
extern HWND hOurWnd; /* Main Window Handle */
extern int ret;      /* work variable */

#define PRBUF_LEN 50
extern char prbuf[PRBUF_LEN]; /* work buffer */

/* 程序控制IDs */
#define WM_SELECT WM_USER+16

/* 全局变量*/
#define SOCK_DISCARD 9 /* use the UDP ttytst source port for test */
#define SOCK_SHOUT 32766 /* TCP port used for SHOUT & LISTEN       */
#define BUF_SIZE 8192
#define WRITE_TIMER 1

/* 函数原型  */
int PASCAL WinMain(HANDLE, HANDLE, LPSTR, int);
long FAR PASCAL ShoutWndProc(HWND, WORD, WORD, LONG);
BOOL FAR PASCAL About(HWND, WORD, WORD, LONG);
BOOL FAR PASCAL DialogProc(HWND, WORD, WORD, LONG);

BOOL FAR PASCAL Settings(HWND, WORD, WORD, LONG);
BOOL InitApp(HANDLE);
void CheckThisBoxOn(HWND, int);
void CheckThisProtoBoxOn(HWND, int);
void CheckThisProgBoxOn(HWND, int);
void ClearBoxes(HWND);
SOCKET ResolveAndConnectHost(LPSTR, HWND, int, int);
SOCKET GetSocketAndBind(HWND, int, int);
long UWriteData(SOCKET, HWND, int);
long UReadData(SOCKET, HWND, int);
long TWriteData(SOCKET, HWND, int);
long TReadData(SOCKET, HWND, int);
int ShoutBlockingHook (void);
int PASCAL FAR WSAsperror (HANDLE, int, char far *, int);
void wshout_err (HWND, int, char far *);

#ifdef _cplusplus
}
#endif /* __cplusplus */

#endif /* ifndef _WSHOUT_INC_ */

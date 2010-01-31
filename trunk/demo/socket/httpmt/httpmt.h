//
// HTTPMT.H		HTTP Server Framework
//
//				Multi-Threaded Model
//

#include <winsock2.h>
#include <windowsx.h>
#include <process.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#ifndef _HTTPMT_H
#define _HTTPMT_H

#define HTTPPORT 80
#define METHOD_GET 0
#define METHOD_PUT 1

//
// Notification Messages
//
#define HTTP_STATS_MSG			WM_USER+1
#define HTTP_FILEOK_MSG			WM_USER+2
#define HTTP_FILENOTFOUND_MSG	WM_USER+3
#define HTTP_EVENT_MSG			WM_USER+4


//
// HTTPSERVINFO structure
//
typedef struct tagHTTPSERVINFO
{
	HWND		hwnd;
	UINT		uMsgApp;
	short		nPort;
	LPCSTR		lpRootDir;
}HTTPSERVINFO, *LPHTTPSERVINFO;

//
// CONNECTION structure
//
typedef struct tagREQUEST
{
  HANDLE		hExit;
  SOCKET		Socket;
  int			nMethod;
  DWORD			dwConnectTime;
  DWORD			dwRecv;
  DWORD			dwSend;
  HANDLE		hFile;
  char			szFileName[_MAX_PATH];
}REQUEST, *LPREQUEST;

//
// Statistics structure
//
typedef struct tagHTTPSTATS
{
	DWORD	dwElapsedTime;
	DWORD	dwRecv;
	DWORD	dwSend;
}HTTPSTATS, *LPHTTPSTATS;

//
// These numbers relate to their
// sequence in an array in
// SendError()
//
#define HTTP_STATUS_OK				0
#define HTTP_STATUS_CREATED			1
#define HTTP_STATUS_ACCEPTED		2
#define HTTP_STATUS_NOCONTENT		3
#define HTTP_STATUS_MOVEDPERM		4
#define HTTP_STATUS_MOVEDTEMP		5
#define HTTP_STATUS_NOTMODIFIED		6
#define HTTP_STATUS_BADREQUEST		7
#define HTTP_STATUS_UNAUTHORIZED	8
#define HTTP_STATUS_FORBIDDEN		9
#define HTTP_STATUS_NOTFOUND		10
#define HTTP_STATUS_SERVERERROR		11
#define HTTP_STATUS_NOTIMPLEMENTED	12
#define HTTP_STATUS_BADGATEWAY		13
#define HTTP_STATUS_UNAVAILABLE		14


//
// HTTPSERV.C
//
BOOL StartHTTP(LPHTTPSERVINFO lpInfo);
void StopHTTP(void);
unsigned __stdcall ListenThread(void *pVoid);
unsigned __stdcall ClientThread(void *pVoid);
BOOL RecvRequest(LPREQUEST lpReq, LPBYTE pBuf, DWORD dwBufSize);
int ParseRequest(LPREQUEST lpReq, LPBYTE lpBuf);
void CloseConnection(LPREQUEST lpReq);
void SendFile(LPREQUEST lpReq);
void SendFileContents(LPREQUEST lpReq);
BOOL SendBuffer(LPREQUEST lpReq, LPBYTE pBuf, DWORD dwBufSize);
void SendError(LPREQUEST lpReq, UINT uError);
void LogEvent(HWND hwnd, LPCSTR lpFormat, ...);
void LogWinSockError(HWND hwnd, LPCSTR lpText, int nErrorCode);
int GetLocalAddress(LPSTR lpStr, LPDWORD lpdwStrLen);

//
// COUNTC.C
//
HANDLE InitClientCount(void);
void IncrementClientCount(void);
void DecrementClientCount(void);
void DeleteClientCount(void);


#endif // _HTTPMT_H


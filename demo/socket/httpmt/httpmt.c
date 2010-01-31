//
// HTTPMT.C		HTTP Server Functions
//
//				Multi-Threaded Model
//

#include "httpmt.h"

//
// Internal linkage
//
static SOCKET listenSocket;
static char szWebRoot[_MAX_PATH];
static HWND	ghwnd;
static UINT guAppMsg;

HANDLE ghExit;
DWORD  gdwListenThread;

////////////////////////////////////////////////////////////

BOOL StartHTTP(LPHTTPSERVINFO lpInfo)
{
	SOCKADDR_IN		saServer;		
	LPSERVENT		lpServEnt;		
	unsigned		ThreadAddr;
	char			szBuf[256];		
	char			szAddress[80];
	DWORD			dwAddrStrLen;
    int				nRet;			

	//
	// Save the Window handle and message
	// ID for further use
	//
	ghwnd    = lpInfo->hwnd;
	guAppMsg = lpInfo->uMsgApp;
	if (lpInfo->lpRootDir != NULL)
		strcpy(szWebRoot, lpInfo->lpRootDir);
	else
		strcpy(szWebRoot, "/WebPages");

	//
	// Create the exit signal event object
	//
	ghExit = CreateEvent(NULL,		// Security
						 TRUE,		// Manual reset
						 FALSE,		// Initial State
						 NULL);		// Name
	if (ghExit == NULL)
		return FALSE;

 	//
	// Create a TCP/IP stream socket
	//
	listenSocket = socket(AF_INET, 
						  SOCK_STREAM, 
						  IPPROTO_TCP);
	if (listenSocket == INVALID_SOCKET)
	{
		LogWinSockError(ghwnd, 
						"Could not create listen socket",
						WSAGetLastError());
		return FALSE;
	}

	//
	// If a specific port number was specified
	// then use it
	//
	if (lpInfo->nPort != 0)
		saServer.sin_port = htons(lpInfo->nPort);
	else
	{
		//
		// Find a port number
		//
		lpServEnt = getservbyname("http", "tcp");
		if (lpServEnt != NULL)
			saServer.sin_port = lpServEnt->s_port;
		else
			saServer.sin_port = htons(HTTPPORT);
	}

	//
	// Fill in the rest of the address structure
	//
	saServer.sin_family = AF_INET;
	saServer.sin_addr.s_addr = INADDR_ANY;

	//
	// bind our name to the socket
	//
	nRet = bind(listenSocket, 
				(LPSOCKADDR)&saServer, 
				sizeof(struct sockaddr));
	if (nRet == SOCKET_ERROR)
	{
		LogWinSockError(ghwnd,
						 "bind() error",
						 WSAGetLastError());
		closesocket(listenSocket);
		return FALSE;
	}

	//
	// Set the socket to listen
	//
	nRet = listen(listenSocket, SOMAXCONN);
	if (nRet == SOCKET_ERROR)
	{
		LogWinSockError(ghwnd,
						 "listen() error",
						 WSAGetLastError());
		closesocket(listenSocket);
		return FALSE;
	}

	//
	// Create the listening thread
	//
	gdwListenThread = _beginthreadex(
							NULL,		 // Security
							0,			 // Stack size
							ListenThread,// Function address
							&ghExit,	 // Argument
							0,			 // Init flag
							&ThreadAddr);// Thread address
	if (!gdwListenThread)
	{
		LogEvent(ghwnd, 
				 "Could not create listening thread: %d",
				 GetLastError());
		closesocket(listenSocket);
		return FALSE;
	}

	//
	// Display the host name and address
	//
	gethostname(szBuf, sizeof(szBuf));
	dwAddrStrLen = sizeof(szAddress);
	GetLocalAddress(szAddress, &dwAddrStrLen);
	LogEvent(ghwnd, 
			 "HTTP Server Started: %s [%s] on port %d",
			 szBuf,
			 szAddress,
			 htons(saServer.sin_port));

	return TRUE;
}

////////////////////////////////////////////////////////////

void StopHTTP()
{
	int nRet;

	//
	// Signal the exit event
	//
	SetEvent(ghExit);

	//
	// Close the listening socket
	//
	nRet = closesocket(listenSocket);

	//
	// And wait for the listen thread to stop
	//
	nRet = WaitForSingleObject((HANDLE)gdwListenThread, 10000);
	if (nRet == WAIT_TIMEOUT)
		LogEvent(ghwnd, "TIMEOUT waiting for ListenThread");

	CloseHandle((HANDLE)gdwListenThread);
	CloseHandle(ghExit);
	LogEvent(ghwnd, "Server Stopped");
}

////////////////////////////////////////////////////////////

unsigned __stdcall ListenThread(void *pVoid)
{
	SOCKET		socketClient;
	unsigned	ThreadAddr;
	DWORD		dwClientThread;
	SOCKADDR_IN	SockAddr;
	LPREQUEST	lpReq;
	int			nLen;
	DWORD		dwRet;
	HANDLE		hNoClients;
	LPHANDLE	pHandle = (LPHANDLE)pVoid;

	//
	// Initialize client thread count to 0
	//
	hNoClients = InitClientCount();

	//
	// Loop forever accepting connections
	//
	while(1)
	{
		//
		// Block on accept()
		//
		nLen = sizeof(SOCKADDR_IN);
		socketClient = accept(listenSocket,
							  (LPSOCKADDR)&SockAddr,
							  &nLen);
		if (socketClient == INVALID_SOCKET)
		{
			//
			// StopHTTP() closes the listening socket
			// when it wants this thread to stop.
			break;
		}

		//
		// We have a connection
		// 
		LogEvent(ghwnd, 
				 "Connection accepted on socket:%d from:%s",
				 socketClient,
				 inet_ntoa(SockAddr.sin_addr));

		//
		// Allocate parms for client and fill in defaults
		// 
		lpReq = malloc(sizeof(REQUEST));
		if (lpReq == NULL)
		{
			LogEvent(ghwnd,
					 "No memory for client request");
			continue;
		}
		lpReq->hExit  = *pHandle;
		lpReq->Socket = socketClient;
		lpReq->dwConnectTime = GetTickCount();
		lpReq->hFile = INVALID_HANDLE_VALUE;
		lpReq->dwRecv = 0;;
		lpReq->dwSend = 0;


		//
		// Start a client thread to handle this request
		//
		dwClientThread = _beginthreadex(
							NULL,		  // Security
							0,			  // Stack size
							ClientThread, // Thread function
							lpReq,	      // Argument
						    0,			  // Init flag
						    &ThreadAddr); // Thread address
		if (!dwClientThread)
		{
			LogEvent(ghwnd, "Couldn't start client thread");
		}
		//
        // We won't be using client thread handles,
		// so we close them when they are created.
		// The thread will continue to execute.
        //
        CloseHandle((HANDLE)dwClientThread);

	}

	//
	// Wait for exit event
	//
	WaitForSingleObject((HANDLE)*pHandle, INFINITE);

	//
	// Wait for ALL clients to exit
	//
	dwRet = WaitForSingleObject(hNoClients, 5000);
	if (dwRet == WAIT_TIMEOUT)
	{
		LogEvent(ghwnd,
			"One or more client threads did not exit");
	}
	DeleteClientCount();
	return 0;
}

////////////////////////////////////////////////////////////

unsigned __stdcall ClientThread(void *pVoid)
{
	int nRet;
	BYTE buf[1024];
	LPREQUEST lpReq = (LPREQUEST)pVoid;

	//
	// Count this client
	//
	IncrementClientCount();

	//
	// Recv the request data
	//
	if (!RecvRequest(lpReq, buf, sizeof(buf)))
	{
		CloseConnection(lpReq);
		free(lpReq);
		DecrementClientCount();
		return 0;
	}

	//
	// Parse the request info
	//
	nRet = ParseRequest(lpReq, buf);
	if (nRet)
	{
		SendError(lpReq, nRet);
		CloseConnection(lpReq);
		free(lpReq);
		DecrementClientCount();
		return 0;
	}

	//
	// Send the file to the client
	//
	SendFile(lpReq);

	//
	// Clean up
	CloseConnection(lpReq);
	free(pVoid);

	//
	// Subtract this client
	//
	DecrementClientCount();
	return 0;
}

////////////////////////////////////////////////////////////

BOOL RecvRequest(LPREQUEST lpReq, LPBYTE pBuf, DWORD dwBufSize)
{
	WSABUF			wsabuf;
	WSAOVERLAPPED	over;
	DWORD			dwRecv;
	DWORD			dwFlags;
	DWORD			dwRet;
	HANDLE			hEvents[2];
	BOOL			fPending;
	int				nRet;

	//
	// Zero the buffer so the recv is null-terminated
	//
	memset(pBuf, 0, dwBufSize);

	//
	// Setup the WSABUF and WSAOVERLAPPED structures
	//
	wsabuf.buf  = pBuf;
	wsabuf.len  = dwBufSize;
	over.hEvent = WSACreateEvent();

	dwFlags = 0;
	fPending = FALSE;
	nRet = WSARecv(lpReq->Socket,	// Socket
				   &wsabuf,			// WSABUF
				   1,				// Number of buffers
				   &dwRecv,			// Bytes received
				   &dwFlags,		// Flags
				   &over,			// WSAOVERLAPPED
				   NULL);			// Completion function
	if (nRet != 0)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			LogWinSockError(ghwnd, 
						    "WSARecv()", 
							WSAGetLastError());
			CloseHandle(over.hEvent);
			return FALSE;
		}
		else
			fPending = TRUE;
	}

	//
	// If the I/O isn't finished...
	//
	if (fPending)
	{
		//
		// Wait for the request to complete or the exit event 
		//
		hEvents[0]  = over.hEvent;
		hEvents[1]  = lpReq->hExit;
		dwRet = WaitForMultipleObjects(2,
									   hEvents,
									   FALSE,
									   INFINITE);
		//
		// Was the recv event signaled?
		//
		if (dwRet != 0)
		{
			CloseHandle(over.hEvent);
			return FALSE;
		}
		if (!WSAGetOverlappedResult(lpReq->Socket,
									&over,
									&dwRecv,
									FALSE,
									&dwFlags))
			CloseHandle(over.hEvent);
			return FALSE;
	}

	//
	// Recv event is complete -- keep statistics
	//
	lpReq->dwRecv += dwRecv;
	CloseHandle(over.hEvent);
	return TRUE;
}

////////////////////////////////////////////////////////////

int ParseRequest(LPREQUEST lpReq, LPBYTE lpBuf)
{
	char szSeps[] = " \n";
	char *cpToken;

	//
	// Don't let requests include ".." characters
	// in requests
	//
	if (strstr(lpBuf, "..") != NULL)
	{
		// Send "bad request" error
		return(HTTP_STATUS_BADREQUEST);
	}

	//
	// Determine request method
	//
	cpToken = strtok(lpBuf, szSeps);
	if (!_stricmp(cpToken, "GET"))
		lpReq->nMethod = METHOD_GET;
	else
	{
		// Send "not implemented" error
		return(HTTP_STATUS_NOTIMPLEMENTED);
	}

	//
	// Get the file name
	//
	cpToken = strtok(NULL, szSeps);
	if (cpToken == NULL)
	{
		// Send "bad request" error
		return(HTTP_STATUS_BADREQUEST);
	}
	strcpy(lpReq->szFileName, szWebRoot);
	if (strlen(cpToken) > 1)
		strcat(lpReq->szFileName, cpToken);
	else
		strcat(lpReq->szFileName, "/index.html");
	return 0;
}

////////////////////////////////////////////////////////////

void CloseConnection(LPREQUEST lpReq)
{
	HTTPSTATS	stats;
	int			nRet;

	//
	// Log the event and close the socket
	//
	LogEvent(ghwnd, 
			 "Closing socket: %d",
			 lpReq->Socket);

	nRet = closesocket(lpReq->Socket);
	if (nRet == SOCKET_ERROR)
	{
		LogWinSockError(ghwnd, 
			 "closesocket()",
			 WSAGetLastError());
	}

	//
	// Give the user interface the stats
	//
	stats.dwElapsedTime = 
			(GetTickCount() - lpReq->dwConnectTime);
	stats.dwRecv = lpReq->dwRecv;
	stats.dwSend = lpReq->dwSend;
	SendMessage(ghwnd,
				guAppMsg,
				HTTP_STATS_MSG,
				(LPARAM)&stats);
}

////////////////////////////////////////////////////////////

void SendFile(LPREQUEST lpReq)
{
	//
	// Open the file for reading
	//
	lpReq->hFile = CreateFile(lpReq->szFileName, 
							  GENERIC_READ,
							  FILE_SHARE_READ,
							  NULL,
							  OPEN_EXISTING,
							  FILE_ATTRIBUTE_NORMAL,
							  NULL);
	if (lpReq->hFile == INVALID_HANDLE_VALUE)
	{
		SendMessage(ghwnd,
					guAppMsg,
					HTTP_FILENOTFOUND_MSG,
					(LPARAM)(LPCSTR)lpReq->szFileName);
		// Send "404 Not Found" error
		SendError(lpReq, HTTP_STATUS_NOTFOUND);
		return;
	}

	//
	// Tell the user interface about the file hit
	// (Sending just the request portion -- without
	// the root web directory portion
	//
	SendMessage(ghwnd,
				guAppMsg,
				HTTP_FILEOK_MSG,
				(LPARAM)(LPCSTR)&lpReq->szFileName[strlen(szWebRoot)]);
	//
	// Send the file contents to the client
	//
	SendFileContents(lpReq);

	//
	// Close the file
	//
	if (CloseHandle(lpReq->hFile))
		lpReq->hFile = INVALID_HANDLE_VALUE;
	else
		LogEvent(ghwnd,
				 "Error closing file: %d",
				 GetLastError());
}

////////////////////////////////////////////////////////////

void SendFileContents(LPREQUEST lpReq)
{
	static BYTE buf[1024];
	DWORD  dwRead;
	BOOL   fRet;

	//
	// Read and send data until EOF
	//
	while(1)
	{
		//
		// Read a buffer full from the file
		//
		fRet = ReadFile(lpReq->hFile,
						buf,
						sizeof(buf),
						&dwRead,
						NULL);
		if (!fRet)
		{
			// Send "404 Not Found" error
			SendError(lpReq, HTTP_STATUS_SERVERERROR);
			break;
		}

		if (dwRead == 0)
			break;

		//
		// Send this buffer to the client
		//
		if (!SendBuffer(lpReq, buf, dwRead))
			break;

		//
		// Add for statistics
		//
		lpReq->dwSend += dwRead;
	}
}

////////////////////////////////////////////////////////////

BOOL SendBuffer(LPREQUEST lpReq, LPBYTE pBuf, DWORD dwBufSize)
{
	WSABUF			wsabuf;
	WSAOVERLAPPED	over;
	DWORD			dwRecv;
	DWORD			dwFlags;
	DWORD			dwRet;
	HANDLE			hEvents[2];
	BOOL			fPending;
	int				nRet;

	//
	// Setup the WSABUF and WSAOVERLAPPED structures
	//
	wsabuf.buf  = pBuf;
	wsabuf.len  = dwBufSize;
	over.hEvent = WSACreateEvent();

	fPending = FALSE;
	nRet = WSASend(lpReq->Socket,	// Socket
				   &wsabuf,			// WSABUF
				   1,				// Number of buffers
				   &dwRecv,			// Bytes received
				   0,				// Flags
				   &over,			// WSAOVERLAPPED
				   NULL);			// Completion function
	if (nRet != 0)
	{
		if (WSAGetLastError() == WSA_IO_PENDING)
			fPending = TRUE;
		else
		{
			LogWinSockError(ghwnd, 
							"WSASend()", 
							WSAGetLastError());
			CloseHandle(over.hEvent);
			return FALSE;
		}
	}

	//
	// If the I/O isn't finished...
	//
	if (fPending)
	{
		//
		// Wait for the request to complete 
		// or the exit event to be signaled
		//
		hEvents[0]  = over.hEvent;
		hEvents[1]  = lpReq->hExit;
		dwRet = WaitForMultipleObjects(2,
									   hEvents,
									   FALSE,
									   INFINITE);
		//
		// Was the recv event signaled?
		//
		if (dwRet != 0)
		{
			CloseHandle(over.hEvent);
			return FALSE;
		}

		//
		// Get I/O result
		//
		if (!WSAGetOverlappedResult(lpReq->Socket,
									&over,
									&dwRecv,
									FALSE,
									&dwFlags))
		{
			LogWinSockError(ghwnd, 
							"WSAGetOverlappedResult()", 
							WSAGetLastError());
			CloseHandle(over.hEvent);
			return FALSE;
		}
	}

	CloseHandle(over.hEvent);
	return TRUE;
}

////////////////////////////////////////////////////////////

void SendError(LPREQUEST lpReq, UINT uError)
{
	static char szMsg[512];
	static char *szStatMsgs [] = {
								"200 OK",
								"201 Created",
								"202 Accepted",
								"204 No Content",
								"301 Moved Permanently",
								"302 Moved Temporarily",
								"304 Not Modified",
								"400 Bad Request",
								"401 Unauthorized",
								"403 Forbidden",
								"404 Not Found",
								"500 Internal Server Error",
								"501 Not Implemented",
								"502 Bad Gateway",
								"503 Service Unavailable"
								};
	#define NUMSTATMSGS sizeof(szStatMsgs) / sizeof(szStatMsgs[0])

	if (uError < 0 || uError > NUMSTATMSGS)
		return;

	wsprintf(szMsg, "<body><h1>%s</h1></body>",
				 szStatMsgs[uError]);
	send(lpReq->Socket,
		 szMsg,
		 strlen(szMsg),
		 0);
}

////////////////////////////////////////////////////////////

void LogEvent(HWND hwnd, LPCSTR lpFormat, ...)
{
	va_list Marker;
	char szBuf[256];
	
	// Write text to string
	// and append to edit control
	va_start(Marker, lpFormat);
	vsprintf(szBuf, lpFormat, Marker);
	va_end(Marker);
	SendMessage(ghwnd,
				guAppMsg,
				HTTP_EVENT_MSG,
				(LPARAM)szBuf);
}

////////////////////////////////////////////////////////////

void LogWinSockError(HWND hwnd, LPCSTR lpText, int nErrorCode)
{
	char szBuf[256];

	szBuf[0] = '\0';
	LoadString(GetWindowInstance(hwnd),
			   nErrorCode,
			   szBuf,
			   sizeof(szBuf));
	LogEvent(hwnd, "%s : %s", lpText, szBuf);
}

////////////////////////////////////////////////////////////

int GetLocalAddress(LPSTR lpStr, LPDWORD lpdwStrLen)
{
    struct in_addr *pinAddr;
    LPHOSTENT	lpHostEnt;
	int			nRet;
	int			nLen;

	//
	// Get our local name
	//
    nRet = gethostname(lpStr, *lpdwStrLen);
	if (nRet == SOCKET_ERROR)
	{
		lpStr[0] = '\0';
		return SOCKET_ERROR;
	}

	//
	// "Lookup" the local name
	//
	lpHostEnt = gethostbyname(lpStr);
    if (lpHostEnt == NULL)
	{
		lpStr[0] = '\0';
		return SOCKET_ERROR;
	}

	//
    // Format first address in the list
	//
	pinAddr = ((LPIN_ADDR)lpHostEnt->h_addr);
	nLen = strlen(inet_ntoa(*pinAddr));
	if ((DWORD)nLen > *lpdwStrLen)
	{
		*lpdwStrLen = nLen;
		WSASetLastError(WSAEINVAL);
		return SOCKET_ERROR;
	}

	*lpdwStrLen = nLen;
	strcpy(lpStr, inet_ntoa(*pinAddr));
    return 0;
}

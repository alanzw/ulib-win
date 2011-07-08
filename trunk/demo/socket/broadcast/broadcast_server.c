// ·þÎñ¶Ë
// BoardCast_Server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <WinSock2.h>
#include <Windows.h>
#pragma comment(lib, "WS2_32.lib")

int _tmain(int argc, _TCHAR* argv[])
{
	WORD wVersionRequested = MAKEWORD(2, 2);
	WSADATA wsaData;
	if(0 != WSAStartup(wVersionRequested, &wsaData))
	{
		printf("WSAStartup failed with error: %d\n", GetLastError());
		return EXIT_FAILURE;
	}
	if(2 != HIBYTE(wsaData.wVersion) || 2 != LOBYTE(wsaData.wVersion))
	{
		printf("Socket version not supported.\n");
		WSACleanup();
		return EXIT_FAILURE;
	}
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(INVALID_SOCKET == sock)
	{
		printf("socket failed with error: %d\n", WSAGetLastError());
		WSACleanup();
		return EXIT_FAILURE;
	}
	SOCKADDR_IN addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = htonl(INADDR_BROADCAST);
	addr.sin_port = htons(5150);
	BOOL bBoardcast = TRUE;
	if(SOCKET_ERROR == setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char*)&bBoardcast, sizeof(bBoardcast)))
	{
		printf("setsockopt failed with error code: %d\n", WSAGetLastError());
		if(INVALID_SOCKET != sock)
		{
			closesocket(sock);
			sock = INVALID_SOCKET;
		}
		WSACleanup();
	}
	printf("Server start, start to boardcast ...\n");
	char buf[] = {"Hello, this is boardcast!"};
	while(TRUE)
	{
		if(SOCKET_ERROR == sendto(sock, buf, sizeof(buf), 0, (LPSOCKADDR)&addr, sizeof(addr)))
		{
			printf("sendto failed with error: %d\n", WSAGetLastError());
			continue;
		}
		Sleep(1000);
	}
	WSACleanup();
	return 0;
}



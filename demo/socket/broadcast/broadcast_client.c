// ¿Í»§¶Ë
// BoardCast_Client.cpp : Defines the entry point for the console application.
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
	addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(5150);
	if(SOCKET_ERROR == bind(sock, (LPSOCKADDR)&addr, sizeof(addr)))
	{
		printf("bind failed with error: %d\n", WSAGetLastError());
		if(INVALID_SOCKET != sock)
		{
			closesocket(sock);
			sock = INVALID_SOCKET;
		}
		WSACleanup();
		return EXIT_FAILURE;
	}
#define MAX_BUFFER 256
	char szBuf[MAX_BUFFER] = {0};
	SOCKADDR_IN remote;
	memset(&remote, 0, sizeof(remote));
	int len = sizeof(remote);
	while(TRUE)
	{
		if(SOCKET_ERROR == recvfrom(sock, szBuf, MAX_BUFFER-1, 0, (LPSOCKADDR)&remote, &len))
		{
			printf("recvfrom failed with error: %d\n", WSAGetLastError());
			continue;
		}
		printf("recv data: <%s : %d> %s\n", inet_ntoa(remote.sin_addr), ntohs(remote.sin_port), szBuf);
	}
	return 0;
}


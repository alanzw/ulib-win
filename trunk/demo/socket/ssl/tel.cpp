#define STRICT
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <process.h>
#include <commctrl.h>

#include "ssl.h"

SOCKET s;
SSL_SOCKET* sx = 0;
bool SSL = false;
sockaddr_in dA,aa;
int slen = sizeof(sockaddr_in);

#pragma warning(disable:4996)
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"Crypt32.lib")
#pragma comment(lib,"Secur32.lib")

void r(void*)
{
	// Receiving Thread
	// Uncomment the commented lines to allow logging

	//   FILE* fp = fopen("log.txt","a+b");
	for (;;)
	{
		char c;
		int rval = 0;

		if (SSL)
		{
			rval = sx->s_recv(&c,1);
		}
		else
		{
			rval = recv(s,&c,1,0);
		}

		if (rval == 0 || rval == -1)
		{
			printf("--- Disconnected !\r\n\r\n");
			//         fclose(fp);
			exit(0);
		}
		putc(c,stdout);
		//      fputc(c,fp);
	}
}

void Loop()
{
	getpeername(s,(sockaddr*)&aa,&slen);

	printf("OK , connected with %s:%u...\r\n\r\n",inet_ntoa(aa.sin_addr),ntohs(aa.sin_port));
	if (SSL)
	{
		if (__argc == 2)
		{
			sx = new SSL_SOCKET(s,1);
			sx->ServerInit();
		}
		else
		{
			sx = new SSL_SOCKET(s,0);
			sx->ClientInit();
		}
	}

	_beginthread(r,4096,0);
	for ( ; ; )
	{
		char c = getc(stdin);
		int rval = 0;
		if (SSL)
		{
			rval = sx->s_ssend(&c,1);
		}
		else
		{
			rval = send(s,&c,1,0);
		}
		if (rval == 0 || rval == -1)
		{
			printf("--- Disconnected !\r\n\r\n");
			exit(0);
		}
	}
}


int main(int argc,char**argv)
{
	InitCommonControls();
	OleInitialize(0);
	WSADATA wData;
	WSAStartup(MAKEWORD(2,2),&wData);
	printf("Tel 2.0 , Chourdakis Michael\r\n");
	if (argc < 2)
	{
		printf("Usage 1 : TEL <ip> <port>\r\n");
		printf("Usage 2 : TEL <port>\r\n");
		printf("Use * before the port to initiate a SSL session.\r\n");
		return 1;
	}
	if (argc == 3)
	{
		char port[100] = {0};
		if (argv[2][0] == '*')
		{
			SSL = true;
			strcpy(port,argv[2] + 1);
		}
		else
		strcpy(port,argv[2]);

		if (strstr(argv[1],":") != 0) // IP v.6
		{
			printf("Mode 3 - connect to %s [%s]...\r\n",argv[1],argv[2]);
			ADDRINFO a = {0};
			ADDRINFO* b = 0;

			sockaddr_in6 sA = {0};
			int salen = sizeof(sA);

			a.ai_flags = 0;
			a.ai_family = PF_INET6;
			a.ai_socktype = SOCK_STREAM;
			a.ai_protocol = IPPROTO_TCP;

			int FS = getaddrinfo(argv[1],port,&a,&b);
			s = socket(AF_INET6,SOCK_STREAM,0);
			if (connect(s,(sockaddr*)b->ai_addr,b->ai_addrlen) < 0)
			{
				printf("--- Cannot connect !\r\n");
				return 3;
			}
			Loop();
		}
		else
		{
			printf("Mode 1 - connect to %s:%s...\r\n",argv[1],argv[2]);
			hostent* hp;
			unsigned long inaddr;

			memset(&dA,0,sizeof(dA));
			dA.sin_family = AF_INET;
			inaddr = inet_addr(argv[1]);
			if (inaddr != INADDR_NONE)
			memcpy(&dA.sin_addr,&inaddr,sizeof(inaddr));
			else
			{
				hp = gethostbyname(argv[1]);
				if (!hp)
				{
					printf("--- Remote system not found !\r\n");
					return 2;
				}
				memcpy(&dA.sin_addr,hp->h_addr,hp->h_length);
			}
			dA.sin_port = htons(atoi(port));
			s = socket(AF_INET,SOCK_STREAM,0);
			if (connect(s,(sockaddr*)&dA,slen) < 0)
			{
				printf("--- Cannot connect !\r\n");
				return 3;
			}
			Loop();
		}
	}
	if (argc == 2)
	{
		printf("Mode 2 - accept %s...\r\n",argv[1]);

		memset(&dA,0,sizeof(dA));
		dA.sin_family = AF_INET;
		int Port = 0;
		if (argv[1][0] == '*')
		{
			Port = atoi(argv[1] + 1);
			SSL = true;
		}
		else
		Port = atoi(argv[1]);

		dA.sin_port = htons(Port);
		dA.sin_addr.s_addr = INADDR_ANY;
		s = socket(AF_INET,SOCK_STREAM,0);
		if (bind(s,(sockaddr*)&dA,slen) < 0)
		{
			printf("--- Cannot bind !\r\n");
			return 4;
		}
		if (listen(s,3) < 0)
		{
			printf("--- Cannot listen !\r\n");
			return 5;
		}
		SOCKET x = accept(s,(sockaddr*)&aa,&slen);
		if (x == INVALID_SOCKET)
		{
			printf("--- Cannot accept !\r\n");
			return 6;
		}
		closesocket(s);
		s = x;
		Loop();
	}
}

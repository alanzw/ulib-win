#include <windows.h>
#include <winsock2.h>
#include <stdio.h>

#define USERPORT 10001

#define HOST_IP_ADDR "127.0.0.1"

int main(int argc, char **argv)
{
    char buf[1024];
    struct sockaddr_in client;
    struct sockaddr_in server;
    int s;
    int ns;
    int namelen;
    int pktlen;

    WSADATA wsadata;
    int nRet;
    if ( (nRet = ::WSAStartup(MAKEWORD(2, 2), &wsadata)) != 0 )
    {
        printf("WSAStartup failed: %d ", (unsigned int)GetLastError());
        return nRet;
    }

    //Did we get the right Winsock version?
    if (wsadata.wVersion != 0x0202)
    {
        return -1;
    }

    if ((s=socket(AF_INET, SOCK_STREAM, 0))<0)
    {
        fprintf(stderr, "Socket()");
        return 1;
    }
    ZeroMemory((char *)&server,sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(USERPORT);
    server.sin_addr.s_addr = INADDR_ANY;
    if (bind(s, (struct sockaddr *)&server, sizeof(server))<0)
    {
        fprintf(stderr, "Bind()");
        return 1;
    }
    if (listen(s,1)!=0)
    {
        fprintf(stderr, "Listen()");
        return 1;
    }
    namelen = sizeof(client);
    if ((ns = accept(s, (struct sockaddr *)&client, &namelen)) ==-1)
    {
        fprintf(stderr, "Accept()");
        return 1;
    }
    for (;;)
    {
        if ((pktlen = recv(ns,buf,1024,0))<0)
        {
            fprintf(stderr, "Recv()");
            break;
        }
        else
        {
            if (pktlen == 0)
            {
                printf("Recv():return FAILED,connection is shut down!\n");
                break;
            }
            else
            {
                printf("Recv():return SUCCESS,packet length = %d\n",pktlen);
            }
        }
        Sleep(1);
        if (send(ns,buf,pktlen,0)<0)
        {
            fprintf(stderr, "Send()");
            break;
        }
        else
        {
            printf("Send():return SUCCESS,packet length = %d\n",pktlen);
        }
    }
    closesocket(ns);
    closesocket(s);
    printf("Server ended successfully\n");

    return 0;
}

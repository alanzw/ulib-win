#include <windows.h>
#include <winsock2.h>

#include <stdio.h>
#include <stdlib.h>

#include "helper.h"
#include "servreq.h"

#define SERVER_PORT (80)

int main(int argc, char *argv[])
{
    SOCKET listener, conn;
    UINT  pid;
    struct sockaddr_in servaddr;

    WSADATA wsadata;
    int nRet;

    if ( (nRet = WSAStartup(MAKEWORD(2, 2), &wsadata)) != 0 )
    {
        printf("WSAStartup failed: %d ", (unsigned int)GetLastError());
        return nRet;
    }

    /* Did we get the right Winsock version? */
    if (wsadata.wVersion != 0x0202)
    {
        WSACleanup();
        return -1;
    }


    /*  Create socket  */
    if ( (listener = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
    {
        Error_Quit("Couldn't create listening socket.");
    }

    /*  Populate socket address structure  */
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(SERVER_PORT);

    /*  Assign socket address to socket  */
    if ( bind(listener, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0 )
    {
        Error_Quit("Couldn't bind listening socket.");
    }

    /*  Make socket a listening socket  */
    if ( listen(listener, LISTENQ) < 0 )
    {
        Error_Quit("Call to listen failed.");
    }

    /*  Loop infinitely to accept and service connections  */
    while (1)
    {
        /*  Wait for connection  */
        if ( (conn = accept(listener, NULL, NULL)) < 0 )
        {
            Error_Quit("Error calling accept()");
        }

        /*  Fork child process to service connection  */

        /*    if ( (pid = fork()) == 0 ) { */

        /*  This is now the forked child process, so
        close listening socket and service request   */
        /*
        if ( closesocket(listener) < 0 )
        {
            Error_Quit("Error closing listening socket in child.");
        }
        */
        Service_Request(conn);


        /*  Close connected socket and exit  */
		if ( closesocket(conn) < 0 )
        {
            Error_Quit("Error closing connection socket.");
        }
		
        /*WSACleanup();
        exit(EXIT_SUCCESS);*/
        /*    } */


        /*  If we get here, we are still in the parent process,
        so close the connected socket, clean up child processes,
        and go back to accept a new connection.                   */
		/*
        if ( closesocket(conn) < 0 )
        {
            Error_Quit("Error closing connection socket in parent.");
        }*/
        /*    waitpid(-1, NULL, WNOHANG); */
    }

	WSACleanup();
	exit(EXIT_SUCCESS);

    return EXIT_FAILURE;    /*  We shouldn't get here  */
}

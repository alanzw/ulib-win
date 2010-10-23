/*
 * =====================================================================================
 *
 *       Filename:  echoclient.cpp
 *
 *    Description:  echo server socket example.
 *
 *        Version:  1.0
 *        Created:  2009-8-2 21:47:58
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */
#include "resource.h"

#include <windows.h>
#include <winsock2.h>
#include <stdio.h>
#include <string.h>
#include <io.h>

#include "usocket.h"

#define PROTOPORT       6500            /* default protocol port number */

extern  int             errno;
char    localhost[] =   "localhost";    /* default host name            */

int main(int argc, char *argv[])
{
    struct  hostent  *ptrh;  /* pointer to a host table entry       */
    struct  protoent *ptrp;  /* pointer to a protocol table entry   */
    struct  sockaddr_in sad; /* structure to hold an IP address     */
    int     sd;              /* socket descriptor                   */
    int     port;            /* protocol port number                */
    char    *host;           /* pointer to host name                */
    int     n;               /* number of characters read           */
    char    buf[1000];       /* buffer for data from the server     */
    char    *text;           /* pointer to user's line of text      */

    USocket us;

    us.init();

    memset((char *)&sad,0,sizeof(sad)); /* clear sockaddr structure */
    sad.sin_family = AF_INET;         /* set family to Internet     */

    /* Check command-line argument for protocol port and extract    */
    /* port number if one is specified.  Otherwise, use the default */
    /* port value given by constant PROTOPORT                       */
    if (argc > 2) {                 /* if protocol port specified   */
        port = atoi(argv[2]);   /* convert to binary            */
    } else {
        port = PROTOPORT;       /* use default port number      */
    }
    if (port > 0)                   /* test for legal value         */
        sad.sin_port = htons((u_short)port);
    else {                          /* print error message and exit */
        fprintf(stderr,"Bad port number %s\n",argv[2]);
        exit(1);
    }

    /* Check host argument and assign host name. */
    if (argc > 1) {
        host = argv[1];         /* if host argument specified   */
    } else {
        host = localhost;
    }

    /* Convert host name to equivalent IP address and copy to sad. */

    ptrh = gethostbyname(host);
    if ( ((char *)ptrh) == NULL ) {
        fprintf(stderr,"Invalid host: %s\n", host);
        exit(1);
    }
    memcpy(&sad.sin_addr, ptrh->h_addr, ptrh->h_length);

    /* Map TCP transport protocol name to protocol number. */
    if ( ((int)(ptrp = getprotobyname("tcp"))) == 0) {
        fprintf(stderr, "Cannot map \"tcp\" to protocol number");
        exit(1);
    }

    /* Create a socket. */
    us.create(AF_INET, SOCK_STREAM, ptrp->p_proto);

    /* Connect the socket to the specified server. */
    if (SOCKET_ERROR == us.connect((struct sockaddr *)&sad))
    {
        exit(1);
    }

    /* Repeatedly read data from user and send it to server. */
    text = fgets(buf, sizeof(buf), stdin);
    while (text != NULL) {
        us.send(buf, strlen(buf));
        printf("send: %s", buf);
        if (strncmp(buf, "quit", 4) == 0) break;
        
        n = us.recv(buf, sizeof(buf));
        buf[n] = '\0';
        printf("recv: %s", buf);
        text = fgets(buf, sizeof(buf), stdin);
    }

    us.cleanup();

    printf("Press any key to continue...");
    getchar();

    return 0;
}


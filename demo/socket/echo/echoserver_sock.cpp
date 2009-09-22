/*
 * =====================================================================================
 *
 *       Filename:  echoserver.cpp
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
#include <io.h>

#include "usocket.h"

#define PROTOPORT       6500            /* default protocol port number */
#define QLEN            6               /* size of request queue        */

int     visits      =   0;              /* counts client connections    */

int main(int argc, char *argv[])
{
        struct  hostent  *ptrh;  /* pointer to a host table entry       */
        struct  protoent *ptrp;  /* pointer to a protocol table entry   */
        struct  sockaddr_in sad; /* structure to hold server's address  */
        struct  sockaddr_in cad; /* structure to hold client's address  */
        int     sd, sd2;         /* socket descriptors                  */
        int     port;            /* protocol port number                */
        int     alen;            /* length of address                   */
        char    buf[1000];       /* buffer for string the server sends  */
        int     n;               /* number of characters received       */
        int optval = 1;          /* options set by setsockopt           */

        int nState;

//#ifdef WIN32
//        WSADATA wsaData;
//        WSAStartup(0x0101, &wsaData);
//#endif

        USocket us;
        us.init();

        memset((char *)&sad, 0, sizeof(sad)); /* clear sockaddr structure */
        sad.sin_family = AF_INET;             /* set family to Internet     */
        sad.sin_addr.s_addr = INADDR_ANY;     /* set the local IP address   */

        /* Check command-line argument for protocol port and extract    */
        /* port number if one is specified.  Otherwise, use the default */
        /* port value given by constant PROTOPORT                       */

        if (argc > 1) {                 /* if argument specified        */
                port = atoi(argv[1]);   /* convert argument to binary   */
        } else {
                port = PROTOPORT;       /* use default port number      */
        }
        if (port > 0)                   /* test for illegal value       */
                sad.sin_port = htons((u_short)port);
        else {                          /* print error message and exit */
                fprintf(stderr,"Bad port number %s\n",argv[1]);
                exit(1);
        }

        /* Map TCP transport protocol name to protocol number */
        if ( ((int)(ptrp = getprotobyname("tcp"))) == 0) {
                fprintf(stderr, "cannot map \"tcp\" to protocol number");
                exit(1);
        }

        /* Create a socket */
        //sd = socket(AF_INET, SOCK_STREAM, ptrp->p_proto);
        //if (sd < 0) {
        //        fprintf(stderr, "Socket creation failed\n");
        //        exit(1);
        //}
        us.create(AF_INET, SOCK_STREAM, ptrp->p_proto);

        /* Eliminate "Address already in use" error from bind. */
        //if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR,
        //           (const void *)&optval , sizeof(int)) < 0)
        //return -1;
        us.setsockopt(SOL_SOCKET, SO_REUSEADDR, (const char *)&optval , sizeof(int));

        /* Bind a local address to the socket */
        //if (bind(sd, (struct sockaddr *)&sad, sizeof(sad)) < 0) {
        //        fprintf(stderr,"Bind failed\n");
        //        exit(1);
        //}
        us.bind(port);

        /* Specify size of request queue */
        //if (listen(sd, QLEN) < 0) {
        //        fprintf(stderr,"Listen failed\n");
        //        exit(1);
        //}
        us.listen(QLEN);

        /* Main server loop - accept and handle requests */

        while (1) {
                alen = sizeof(cad);
                //if ( (sd2 = accept(sd, (struct sockaddr *)&cad, &alen)) < 0) {
                //        fprintf(stderr, "Accept failed\n");
                //        exit(1);
                //}
                if (INVALID_SOCKET == (sd2 = us.accept((struct sockaddr *)&cad, &alen) ))
                {
                    fprintf(stderr, "Accept failed\n");
                    exit(1);
                }


                //n = recv(sd2, buf, sizeof(buf), 0);
                //while (n > 0)
                //{
                //    send(sd2, buf, n, 0);
                //    n = recv(sd2, buf, sizeof(buf), 0);
                //}
                n = us.recv(sd2, buf, sizeof(buf));
                while (n > 0)
                {
                    write(1, buf, n);
                    us.send(sd2, buf, n);
                    n = us.recv(sd2, buf, sizeof(buf));
                }

                // closesocket(sd2);
                us.close();
        }

    us.cleanup();

    getchar();

    return 0;
}


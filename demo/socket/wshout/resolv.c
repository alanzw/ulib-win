#include "wshout.h"
/* MSC Include files: */
#include <stdio.h>
#include <io.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

SOCKET ResolveAndConnectHost(LPSTR lpHostName,HWND hOurDlg,int iproto, int iSockPort)
{
    struct hostent FAR *host_ptr; /* Ptr to the host name */
    struct sockaddr_in dest; /* Addr of target host */
    SOCKET hSock; /* The socket to create */
    int iSockType;
    extern int iTCP;
    extern int iUDP;
    /* Internet family addressing */
    dest.sin_family = PF_INET;
    if (iproto == iTCP) {
        iSockType = SOCK_STREAM;
    }
    else if (iproto == iUDP) {
        iSockType = SOCK_DGRAM;
    }
    else {
        return (SOCKET) -1; /* Unknown protocol */
    }
    
    /* default port to connect to. Must be in network byte order */
    dest.sin_port = htons((u_int) iSockPort);
    
    SetDlgItemText(hOurDlg, IDD_COMMENT,"Resolving hostname...");
    
    /* Resolve the host name */
    host_ptr = gethostbyname(lpHostName);
    if (host_ptr == NULL) {
        wshout_err (hOurDlg, WSAGetLastError(), "gethostbyname()");
        return (SOCKET) -1;
    }
    /* Patch host address into struct describing conn: */
    bcopy(host_ptr->h_addr,&dest.sin_addr,host_ptr->h_length);
    
    /* Allocate a network (socket) descriptor:         */
    hSock = socket(PF_INET, iSockType, 0);
    if (hSock == INVALID_SOCKET) {
        wshout_err (hOurDlg, WSAGetLastError(), "socket()");
        return (SOCKET) -1;
    }
    
    /* Start connection process to host described in 'dest' *
    * struct.
    */
    SetDlgItemText(hOurDlg, IDD_COMMENT, "Connecting ...");
    ret=connect(hSock,(struct sockaddr FAR *)&dest,sizeof(dest));
    
    if (ret == SOCKET_ERROR) {
        wshout_err (hOurDlg, WSAGetLastError(), "connect()");
        closesocket(hSock);
        return (SOCKET) -1;
    }
    
    SetDlgItemText(hOurDlg, IDD_COMMENT, "...Connected");
    return hSock;
    OCKET GetSocketAndBind(HWND hOurDlg, int iProto, int iSockPort)
    SOCKET hSock; /* Connection socket descriptor     */
    struct sockaddr_in local; /* Local machine address structure*/
    int iSockType;
    extern int iTCP;
    extern int iUDP;
    
    /* Internet family addressing */
    if (iProto == iTCP) {
        iSockType = SOCK_STREAM;
    }
    else {
        iSockType = SOCK_DGRAM;
    }
    
    memset(&local, '\0', sizeof (local));
    local.sin_family = PF_INET;
    local.sin_port = htons((u_short)iSockPort);
    
    /* allocate a socket descriptor */
    hSock = socket(PF_INET, iSockType, 0);
    if (hSock == INVALID_SOCKET) { /* socket() failed */
        wshout_err (hOurDlg, WSAGetLastError(), "socket()");
        return (SOCKET) -1;
    }
    
    /* bind socket to a local addr */
    ret = bind(hSock, (struct sockaddr FAR *) &local, sizeof(local));
    if (ret == SOCKET_ERROR){         /* bind() failed         */
        wshout_err (hOurDlg, WSAGetLastError(), "bind()");
        return (SOCKET) -1;
    }
    
    if (iProto == iUDP)
    return (hSock);
    
    /* If iProto == iTCP, then must listen() and accept() also */
    ret = listen(hSock, 0);         /* listen on the socket */
    if (ret == SOCKET_ERROR){      /* listen() failed        */
        wshout_err (hOurDlg, WSAGetLastError(), "listen()");
        return (SOCKET) -1;
    }
    
    return(hSock);
}

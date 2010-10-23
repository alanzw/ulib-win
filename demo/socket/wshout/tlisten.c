#include "wshout.h"
/* MSC Include files: */
#include <stdio.h>
#include <io.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
/* Returns the number of bytes written */
long TReadData(SOCKET hSock, HWND hOurDlg, int read_len)
{
    static char ReadBuf[BUF_SIZE];
    SOCKET hAcceptSock;
    struct sockaddr_in local; /* Local machine address structure */
    long total_time = 0L;              /* variable to hold delta t */
    int tmp, len      = 0;
    int num_reads    = 0;
    long bytes_read = 0L;
    long last_time = 0L;
    long now = 0L;
    long ltemp;
    extern long blocking_option;
    extern int run_cancelled;
    struct linger AcceptLinger;
    BOOL running = FALSE;
    BOOL bTemp = TRUE;
    SetDlgItemText(hOurDlg, IDD_COMMENT, "Awaiting the TCP Data ...");
    SetDlgItemText(hOurDlg, IDD_HNAME, "                                            ");
    tmp = sizeof(local);
    if (!blocking_option) {
        hAcceptSock = accept(hSock,(struct sockaddr FAR *)&local,
        (int FAR *)&tmp);
    }
    else {
        for (;;) {
            do {
                ;
            } while (ShoutBlockingHook()); /* Dispatch messages if any */
            if (run_cancelled) {
                WSASetLastError(WSAEINTR);
                break; /* Non-blocking mode was cancelled */
            }
            hAcceptSock = accept(hSock,(struct sockaddr FAR *)&local,
            (int FAR *)&tmp);
            if (hAcceptSock == INVALID_SOCKET) {
                if (h_errno == WSAEWOULDBLOCK)
                /* Try again */
                ;
                else {
                    /* Fatal error -- pop out. */
                    break;
                }
            } /* end if ((hAcceptSock = .. */
            else {
                /* Success -- pop out. */
                break;
            }
        } /* end for */
    } /* end else */
    if (hAcceptSock == INVALID_SOCKET) {
        wshout_err (hOurDlg, WSAGetLastError(), "accept()");
        return 0;
    }
    /* Now, read the data as fast as we can until no more to read */
    time(&last_time);
    do {
        do {
            ;
        } while (ShoutBlockingHook()); /* Dispatch messages while available */
        if (run_cancelled) {
            WSASetLastError(WSAEINTR);
            break; /* Non-blocking mode was cancelled */
        }
        len = recv(hAcceptSock, ReadBuf, read_len, 0);
        if (len == SOCKET_ERROR) {
            if (h_errno == WSAEWOULDBLOCK)
            continue;
            else
            break;
        }
        else if (len == 0)
        break;
        num_reads++;
        bytes_read += len;
        wsprintf((LPSTR)prbuf,"%d\n",num_reads);
        SetDlgItemText(hOurDlg, IDD_WRITE, (LPSTR) prbuf);
        wsprintf((LPSTR)prbuf,"%ld\n",bytes_read);
        SetDlgItemText(hOurDlg, IDD_SENT, (LPSTR) prbuf);
        if (bTemp) { /* To update our main display once */
            /* Do not use wsprintf() or you will add an extra char */
            _fmemcpy(prbuf, inet_ntoa(local.sin_addr), 4*sizeof(u_long));
            SetDlgItemText(hOurDlg, IDD_HNAME, (LPSTR) prbuf);
            SetDlgItemText(hOurDlg, IDD_COMMENT, "Reading TCP Data ...");
            bTemp = FALSE;
        }
    } while ((len != 0) || (len != SOCKET_ERROR));
    time (&now);
    
    if (len == SOCKET_ERROR) {
        if ((h_errno == WSAESHUTDOWN) || (h_errno == WSAENOTCONN)) {
            /* nothing available for read. */
            wsprintf((LPSTR)prbuf,
            "Connection from %s closed.\n",inet_ntoa(local.sin_addr));
            SetDlgItemText(hOurDlg, IDD_COMMENT, prbuf);
        }
        else { /* Other error */
            wshout_err (hOurDlg, WSAGetLastError(), "recv()");
        }
    }
    else if (len == 0) {
        /* Other side shut down the connection */
        wsprintf((LPSTR)prbuf,
        "Connection from %s closed.\n",inet_ntoa(local.sin_addr));
        SetDlgItemText(hOurDlg, IDD_COMMENT, prbuf);
    }
    AcceptLinger.l_onoff = 1;
    AcceptLinger.l_linger = 0;
    ret = setsockopt(hAcceptSock, SOL_SOCKET, SO_LINGER,
    (char FAR *) &AcceptLinger, sizeof(AcceptLinger));
    if (ret == SOCKET_ERROR) {
        wshout_err (hOurDlg, WSAGetLastError(), "setsockopt()");
    }
    
    ret = closesocket(hAcceptSock);
    if (ret == SOCKET_ERROR) {
        wshout_err (hOurDlg, WSAGetLastError(), "closesocket()");
    }
    
    total_time = (long) difftime(now, last_time);
    if (total_time == 0)
    total_time = 1L;          /* Avoid dividing by zero */
    wsprintf((LPSTR)prbuf,"%ld\n",total_time);
    SetDlgItemText(hOurDlg, IDD_TIME, (LPSTR) prbuf);
    
    ltemp = num_reads/total_time;
    wsprintf((LPSTR)prbuf,"%ld\n", ltemp);
    SetDlgItemText(hOurDlg, IDD_WRITES,(LPSTR) prbuf);
    
    ltemp = bytes_read/total_time;
    wsprintf((LPSTR)prbuf,"%ld\n", ltemp);
    SetDlgItemText(hOurDlg, IDD_BYTES, (LPSTR) prbuf);
    
    ltemp = 8 * (bytes_read/total_time);
    wsprintf((LPSTR)prbuf,"%ld\n", ltemp);
    SetDlgItemText(hOurDlg, IDD_BITS,    (LPSTR) prbuf);
    
    if (bytes_read) {
        SetDlgItemText(hOurDlg, IDD_COMMENT, "...TCP Listen Done\n");
    } /* end: if (bytes_read) */
    
    return (bytes_read);
}

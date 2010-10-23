#include "wshout.h"

#include <stdio.h>
#include <io.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/* Returns the number of bytes written */
long TWriteData(SOCKET hSock, HWND hOurDlg, int send_len)
{
    int counter;
    static int DataBuffer[BUF_SIZE];     /* Buffer to hold generated data    */
    long total_len = 1024L*1024L;    /* Total # of bytes to generate      */
    long bytes_sent = 0L;     /* Counter of bytes on connection */
    int  tmp = 0;     /* holds count for bytes written    */
    long write_count = 0L;     /* number of times         */
    time_t start, end;     /* variables to hold read timing    */
    long total_time = 0L;     /* variable to hold delta t              */
    long ltemp = 0L;
    extern int run_cancelled;
    
    /* What makes shout unique is that it generates data*
        * in memory (as opposed to accessing the disk). *
        * This tests the 'raw' speed of the TCP connection *
        * as the rate-limiting access time is eliminated. *
        * First, generate the data and place it into an *
        * array, data_buffer: */
    
    for (counter = 0; counter < BUF_SIZE; counter++)
    DataBuffer[counter] = counter;
    
    /* Write data on the descriptor like a banshee,
        * careful to time the writes and count data
        * transmitted:
        */
    
    SetDlgItemText(hOurDlg, IDD_COMMENT, "...Sending TCP Data");
    time(&start);
    while ( bytes_sent < total_len) {      /* while still bytes to send... */
        do {
            ;
        } while (ShoutBlockingHook()); /* Dispatch messages if any */
        if (run_cancelled) {
            WSASetLastError(WSAEINTR);
            break; /* Non-blocking mode was cancelled */
        }
        tmp = send(hSock, (char FAR*) &DataBuffer, send_len, 0);
        if (tmp == SOCKET_ERROR) {
            if (h_errno == WSAEWOULDBLOCK)
            continue;
            else {
                wshout_err (hOurDlg, WSAGetLastError(), "send()");
            }
            /* Calc. time elapsed & stats about any data sent */
            time(&end);
            /* exit from the while loop */
            break;
        } /* end if (tmp == -1) */
        write_count++;         /* incr. counter of times written */
        bytes_sent += tmp;    /* total # of bytes placed on connection*/
        wsprintf(prbuf,"%ld\n",write_count);
        SetDlgItemText(hOurDlg, IDD_WRITE, (LPSTR) prbuf);
        wsprintf(prbuf,"%ld\n",bytes_sent);
        SetDlgItemText(hOurDlg, IDD_SENT, (LPSTR) prbuf);
    } /* end while */
    time(&end);
    
    if (total_time = (long) difftime(end, start)) {
        /* Print the statistics gathered        */
        wsprintf((LPSTR)prbuf,"%ld\n",total_time);
        SetDlgItemText(hOurDlg, IDD_TIME, (LPSTR) prbuf);
        ltemp = write_count/total_time;
        wsprintf((LPSTR)prbuf,"%ld\n", ltemp);
        SetDlgItemText(hOurDlg, IDD_WRITES,(LPSTR) prbuf);
        ltemp = bytes_sent/total_time;
        wsprintf((LPSTR)prbuf,"%ld\n", ltemp);
        SetDlgItemText(hOurDlg, IDD_BYTES, (LPSTR) prbuf);
        ltemp = 8 * (bytes_sent/total_time);
        wsprintf((LPSTR)prbuf,"%ld\n", ltemp);
        SetDlgItemText(hOurDlg, IDD_BITS,    (LPSTR) prbuf);
    } /* end if (total_time) */
    /* All done */
    SetDlgItemText(hOurDlg, IDD_COMMENT, "...TCP Shout Done");
    return bytes_sent;
}


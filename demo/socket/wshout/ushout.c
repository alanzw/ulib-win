#include <stdio.h>
#include <io.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
/* Returns the number of bytes written */
long UWriteData(SOCKET hSock, HWND hOurDlg, int send_len)
{
    int counter;
    static int DataBuffer[BUF_SIZE];    /* Buffer to hold generated data */
    static    char ReplyBuffer[512];        /* Buffer to hold any reply rcvd    */
    long bytes_sent = 0L;             /* Counter of bytes on connection */
    long total_len = 1024L*1024L;          /* Total # of bytes to generate */
    time_t start, end;             /* variables to hold read timing */
    long total_time = 0L;             /* variable to hold delta t */
    long write_count = 0L;             /* number of times */
    long tmp = 0L;             /* holds count for bytes written */
    long ltemp = 0L;
    int i_temp;
    extern int run_cancelled;
    
    struct sockaddr_in dest;    /* Destination machine address structure */
    
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
    
    SetDlgItemText(hOurDlg, IDD_COMMENT, "Sending UDP Data ...");
    time( &start );
    while (bytes_sent < total_len){/* while still bytes to send */
        do {
            ;
        } while (ShoutBlockingHook()); /* Dispatch messages if any */
        if (run_cancelled) {
            WSASetLastError(WSAEINTR);
            break; /* Non-blocking mode was cancelled */
        }
        tmp = send(hSock, (char FAR *) &DataBuffer, send_len, 0);
        if (tmp == SOCKET_ERROR) {
            if (h_errno == WSAEWOULDBLOCK) /* if no data, read again */
            continue;
            else {
                wshout_err (hOurDlg, WSAGetLastError(), "send()");
            }
            /* Calc. time elapsed & stats about any data sent */
            time(&end);
            if (total_time = (long) difftime(end, start)) {
                /* Print the statistics gathered        */
                wsprintf((LPSTR)prbuf,"%ld\n",write_count);
                SetDlgItemText(hOurDlg, IDD_WRITE, (LPSTR) prbuf);
                wsprintf((LPSTR)prbuf,"%ld\n",bytes_sent);
                SetDlgItemText(hOurDlg, IDD_SENT, (LPSTR) prbuf);
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
                /* exit from the while loop */
                break;
            } /* end if (total_time) */
            write_count++; /* incr. counter of times written    */
            bytes_sent += tmp;        /* # of bytes placed on connection */
            wsprintf((LPSTR)prbuf,"%ld\n",bytes_sent);
            SetDlgItemText(hOurDlg, IDD_SENT, (LPSTR) prbuf);
        } /* end if (tmp == -1) */
        write_count++; /* incr. counter of times written    */
        bytes_sent += tmp;        /* # of bytes placed on connection */
        wsprintf((LPSTR)prbuf,"%ld\n",write_count);
        SetDlgItemText(hOurDlg, IDD_WRITE, (LPSTR) prbuf);
        wsprintf((LPSTR)prbuf,"%ld\n",bytes_sent);
        SetDlgItemText(hOurDlg, IDD_SENT, (LPSTR) prbuf);
    } /* end while */
    
    /* Look for a reply ... NOTE: most hosts won't give
        * a 'reply', done to illustrate communication between
        * sockets. Our ulisten example will give a reply though.
        */
    
    SetDlgItemText(hOurDlg, IDD_COMMENT, "Waiting for reply from server..\n");
    while (1) {
        tmp = sizeof(dest);
        i_temp = recvfrom(hSock,(char FAR *) &ReplyBuffer,sizeof(ReplyBuffer),
        0, (struct sockaddr *) &dest, (int FAR *) &tmp);
        if (i_temp == SOCKET_ERROR) {
            if (h_errno == WSAEWOULDBLOCK) /* if no data, read again */
            continue;
            else {
                /* any error besides these. just punt */
                wshout_err (hOurDlg, WSAGetLastError(), "recvfrom()");
            }
            break;
        } /* end if (i_temp == SOCKET_ERROR) */
        /* else got a reply ...*/
        wsprintf((LPSTR)prbuf, "Server: %s\n", (LPSTR) ReplyBuffer);
        SetDlgItemText(hOurDlg, IDD_COMMENT, prbuf);
        break;
    } /* end while(1) */
    /* All done */
    return bytes_sent;
}

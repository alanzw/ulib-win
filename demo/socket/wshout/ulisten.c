#include <stdio.h>
#include <io.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
/* Returns the number of bytes written */
long UReadData(SOCKET hSock, HWND hOurDlg, int read_len)
{
    static char ReadBuf[BUF_SIZE];
    static char SendBuf[512];
    struct sockaddr_in local; /* Local machine address structure */
    int i; /* General purpose return code */
    long total_time = 0L;              /* variable to hold delta t */
    int tmp, len      = 0;
    int num_reads    = 0;
    long bytes_read = 0L;
    long last_time, now, timeout = 15L;
    long ltemp;
    extern int run_cancelled;
    BOOL bTemp = TRUE;
    
    SetDlgItemText(hOurDlg, IDD_COMMENT, "Awaiting the UDP Data ...");
    SetDlgItemText(hOurDlg, IDD_HNAME, "                                            ");
    
    time(&now); time(&last_time);
    while (last_time + timeout > now) {
        time(&now);
        tmp = sizeof(local);
        do {
            ;
        } while (ShoutBlockingHook()); /* Dispatch messages while available */
        if (run_cancelled) {
            WSASetLastError(WSAEINTR);
            break; /* Non-blocking mode was cancelled */
        }
        len = recvfrom(hSock, ReadBuf, read_len, 0,
        (struct sockaddr *) &local, &tmp);
        if (len == SOCKET_ERROR) {
            if (h_errno == WSAEWOULDBLOCK) {/* if no data, read again */
                continue;
            } /* end: if (errno == WSAEWOULDBLOCK) */
            else {
                if (bytes_read) {
                    wshout_err (hOurDlg, WSAGetLastError(), "recvfrom()");
                }
            } /* end else */
            break;
        } /* end: if (len == SOCKET_ERROR) */
        if (bTemp) { /* To update our main display once */
            /* Do not use wsprintf() or you will add an extra char */
            _fmemcpy(prbuf, inet_ntoa(local.sin_addr), 4*sizeof(u_long));
            SetDlgItemText(hOurDlg, IDD_HNAME, (LPSTR) prbuf);
            SetDlgItemText(hOurDlg, IDD_COMMENT, "Reading UDP Data ...");
            bTemp = FALSE;
        }
        
        num_reads++;
        if (len != SOCKET_ERROR)
        bytes_read += len;
        /* Print the statistics gathered        */
        wsprintf((LPSTR)prbuf,"%d\n",num_reads);
        SetDlgItemText(hOurDlg, IDD_WRITE, (LPSTR) prbuf);
        wsprintf((LPSTR)prbuf,"%ld\n",bytes_read);
        SetDlgItemText(hOurDlg, IDD_SENT, (LPSTR) prbuf);
        time(&last_time);
    } /* end: while */
    total_time = timeout;
    
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
        SetDlgItemText(hOurDlg, IDD_COMMENT, "...UDP Listen Done\n");
    } /* end: if (bytes_read) */
    else {
        wsprintf((LPSTR)prbuf, "Timed out. No data received.\n");
        SetDlgItemText(hOurDlg, IDD_COMMENT, prbuf);
        goto come_here;
    } /* end: else */
    
    /* send reply to 'client' */
    wsprintf((LPSTR)prbuf,
    "Replied to %s\n", inet_ntoa(local.sin_addr));
    SetDlgItemText(hOurDlg, IDD_COMMENT, prbuf);
    
    for (i=0; i<10; ++i) {
        sprintf(SendBuf, "Rec'd %ld bytes.\n", bytes_read);
        if(len = sendto(hSock, SendBuf, sizeof(SendBuf), 0,
                    (struct sockaddr FAR *)&local,sizeof(local)))
        {
            if (len == SOCKET_ERROR) { /* if could not send bec. */
                if (h_errno == WSAEWOULDBLOCK)
                continue;
                wshout_err (hOurDlg, WSAGetLastError(), "sendto()");
                break;
            } /* end: if (len == -1) */
        } /* end: if (len = sendto()) */
    } /* end for */
    come_here:
    return (bytes_read);
}

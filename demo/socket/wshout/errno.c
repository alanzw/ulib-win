#include <windows.h>
#include <winsock.h>

/*
  * Function: WSAsperror()
  *
  * Description:
  *
  * Copies string corresponding to the error code provided
  * into buf, maximum length len. Returns length actually
  * copied to buffer, or zero if error code is unknown.
  * String resources should be present for each error code
  * using the value of the code as the string ID (except for
  * error = 0, which is mapped to WSABASEERR to keep it with
  * the others). The DLL is free to use any string IDs that
  * are less than WSABASEERR for its own use.
  *
  */
int PASCAL FAR WSAsperror (HANDLE hInst,        /* Instance Handle */
      int errorcode,      /* WSA Error Number */
      char far * buf,    /* Buffer for error string */
      int len)                  /* Length of buffer */
{
int err_len;      /* length of error text */
         
                if (errorcode == 0)                          /* If error passed is 0, use the */
                                errorcode = WSABASEERR; /*    base resource file number */
                if (errorcode < WSABASEERR)          /* If invalid Error code */
                                return 0;                              /*    return string length of zero */
/* error string from the table in the Resource file into buffer */
                err_len = LoadString(hInst,errorcode,buf,len);
return (err_len);    /* return length of error string retrieved */
}    /* end WSAsperror() */

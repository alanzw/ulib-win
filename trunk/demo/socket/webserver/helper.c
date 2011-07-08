#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <ctype.h>

#include <windows.h>
#include <winsock2.h>

#include "helper.h"

/*  Prints an error message and quits  */
void Error_Quit(const char * msg)
{
    fprintf(stderr, "WEBSERV: %s\n", msg);
    exit(EXIT_FAILURE);
}

/*  Read a line from a socket  */
ssize_t Readline(int sockd, void *vptr, size_t maxlen)
{
    size_t n, rc;
    char    c, *buffer;

    buffer = vptr;

    for ( n = 1; n < maxlen; n++ )
    {
        if ( (rc = recv(sockd, &c, 1, 0)) == 1 )
        {
            printf("%c", c);
            *buffer++ = c;
            if ( c == '\n' )
            break;
        }
        else if ( rc == 0 )
        {
            if ( n == 1 )
            return 0;
            else
            break;
        }
        else
        {
            if ( errno == EINTR )
            {
                continue;
            }
            Error_Quit("Error in Readline()");
        }
    }

    *buffer = 0;
    return n;
}


/*  Write a line to a socket  */

ssize_t Writeline(int sockd, const void *vptr, size_t n)
{
    size_t      nleft;
    ssize_t     nwritten;
    const char *buffer;

    buffer = vptr;
    nleft  = n;

    while ( nleft > 0 )
    {
        if ( (nwritten = send(sockd, buffer, nleft, 0)) <= 0 )
        {
            if ( errno == EINTR )
            {
                nwritten = 0;
            }
            else
            {
                Error_Quit("Error in Writeline()");
            }
        }
        nleft  -= nwritten;
        buffer += nwritten;
    }

    return n;
}


/*  Removes trailing whitespace from a string  */

int Trim(char * buffer)
{
    int n = strlen(buffer) - 1;

    while ( !isalnum(buffer[n]) && n >= 0 )
    buffer[n--] = '\0';

    return 0;
}


/*  Converts a string to upper-case  */

int StrUpper(char * buffer)
{
    while ( *buffer )
    {
        *buffer = toupper(*buffer);
        ++buffer;
    }
    return 0;
}


/*  Cleans up url-encoded string  */

void CleanURL(char * buffer)
{
    char asciinum[3] = {0};
    int i = 0, c;

    while ( buffer[i] )
    {
        if ( buffer[i] == '+' )
        buffer[i] = ' ';
        else if ( buffer[i] == '%' )
        {
            asciinum[0] = buffer[i+1];
            asciinum[1] = buffer[i+2];
            buffer[i] = strtol(asciinum, NULL, 16);
            c = i+1;
            do
            {
                buffer[c] = buffer[c+2];
            }
            while ( buffer[2+(c++)] );
        }
        ++i;
    }
}

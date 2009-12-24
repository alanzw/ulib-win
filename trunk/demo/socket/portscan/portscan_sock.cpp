/*
 * =====================================================================================
 *
 *       Filename:  echo.cpp
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
#include <stdio.h>
#include "usocket.h"

int scan_port(const char *szhost, int iPort)
{
    USocket us;

    us.init();

    us.create(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (0 == us.connect(szhost, iPort))
    {
        printf("%s:%i is OK.\n", szhost, iPort);
    }
    else
    {
        //printf("Failed to connect to %s:%i.\n", szhost, iPort);
    }

    us.cleanup();

    return 0;
}

int main(int argc, char *argv[])
{
    char buf[256];
    for (int i=100; i<254; ++i)
    {
        sprintf(buf, "166.111.36.%d", i);
        scan_port(buf, 5900);
    }
    getchar();

    return 0;
}


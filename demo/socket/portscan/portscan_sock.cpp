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
    for (int i=1; i<100; ++i)
    {
        scan_port("166.111.36.100", i);
    }
    getchar();

    return 0;
}


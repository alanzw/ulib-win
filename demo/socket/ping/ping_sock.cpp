/*
 * =====================================================================================
 *
 *       Filename:  simple.cpp
 *
 *    Description:  simple socket example.
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

#include <windows.h>
#include <tchar.h>
#include <winsock2.h>
#include <stdio.h>

#include "usocket.h"

#include "ping.h"

int ping(const char *sIPAddr)
{
    return 0;
}

int main(int argc, char *argv[])
{
    USocket us;

    char buf[256];

    us.init();

    us.gethostname(buf, 256);

    printf(buf);

    us.cleanup();

    return 0;
}


/*
 * =====================================================================================
 *
 *       Filename:  simple.cpp
 *
 *    Description:  a simple socket example.
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

int main(int argc, char *argv[])
{
    USocket us;

    char buf[256];

    us.init();

    us.gethostname(buf, 256);

    // buf[strlen(buf)-2] = '\0';

    printf("hostname : %s\n", buf);

    us.cleanup();

    printf("Press any key to continue...");
    getchar();

    return 0;
}


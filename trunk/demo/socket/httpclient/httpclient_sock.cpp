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

#include "httpclient.h"

int main(int argc, char *argv[])
{
    UHTTPClient client;

    client.request("www.baidu.com", "/index.php");

    printf("Press any key to continue...");
    getchar();

    return 0;
}


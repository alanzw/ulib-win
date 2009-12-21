#include "resource.h"
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>

#ifndef BUF_SIZE
#define BUF_SIZE 256
#endif // BUF_SIZE

#include "upipe.h"
#include "uconsole.h"

#define  PIPE_NAME TEXT("\\\\.\\pipe\\mynamedpipe")

int main(int argc, char *argv[])
{

    UNamedPipe up(PIPE_NAME);

    up.create();

    if (up.connect())
    {
        TCHAR buf[BUF_SIZE];
        up.read(buf, BUF_SIZE);
        UConsole::PrintStdout(buf);
        TCHAR chBufReply[BUF_SIZE] = TEXT("Reply from NamedPipeServer!"); 
        up.write(chBufReply, sizeof(TCHAR) * (lstrlen(chBufReply) + 1));
        up.disconnect();
    }

    return 0;
}



#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hello_rpc.h"

int main(int argc, char* argv[])
{
    RPC_STATUS status = 0;

    unsigned int nMinCalls = 1;
    unsigned int nMaxCalls = 20;

    status = RpcServerUseProtseqEp(
        (unsigned char *)"ncacn_np",
        nMaxCalls,
        (unsigned char *)"\\pipe\\{a5194558-21a6-4978-9610-2072fcf1dc6e}",
        NULL );
    if ( status != 0 )
    {
        printf("RpcServerUseProtseqEp return  %d !\n", status);
        return 1;
    }

    status = RpcServerRegisterIf(
        HelloRPC_v1_0_s_ifspec,
        NULL,
        NULL );
    if ( status != 0 )
    {
        printf("RpcServerRegisterIf return %d !\n", status);
        return 1;
    }

    printf("Begin RPC server listen!\n");

    status = RpcServerListen(
        nMinCalls,
        nMaxCalls,
        FALSE );
    if ( status != 0 )
    {
        printf("RpcServerListen return  %d !\n", status);
        return 1;
    }

    getchar();
    return 0;
}

void * __RPC_USER MIDL_user_allocate(size_t len)
{
    return (malloc(len));
}

void __RPC_USER MIDL_user_free(void* ptr)
{
    free(ptr);
}

void HelloProc(unsigned char *pszString)
{
    printf("%s\n", pszString);
}

void ShutDown()
{
    RPC_STATUS status = 0;

    status = RpcMgmtStopServerListening(NULL);
    if ( status != 0 )
        printf("RpcMgmtStopServerListening return  %d !\n", status);

    status = RpcServerUnregisterIf(NULL, NULL, FALSE);
    if ( status != 0 )
        printf("RpcServerUnregisterIf return  %d !\n", status);
}

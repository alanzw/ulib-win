#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hello_rpc.h"

void doRpcCall();

int main(int argc, char* argv[])
{
    RPC_STATUS status;

    unsigned char * pszNetworkAddress = NULL;
    unsigned char * pszStringBinding = NULL;
    int i;

    for ( i = 1; i < argc; ++i )
    {
        if ( strcmp(argv[i], "-ip") == 0 )
        {
            pszNetworkAddress = (unsigned char *)argv[++i];
        }
    }

    status = RpcStringBindingCompose(
        NULL,
        (unsigned char *)"ncacn_np",
        pszNetworkAddress,
        (unsigned char *)"\\pipe\\{a5194558-21a6-4978-9610-2072fcf1dc6e}",
        NULL,
        &pszStringBinding );
    if ( status != 0 )
    {
        printf("RpcStringBindingCompose return  %d !\n", status);
        return 1;
    }

    printf("pszStringBinding = %s\n", pszStringBinding);

    status = RpcBindingFromStringBinding(
        pszStringBinding,
        &hello_Binding );
    if ( status != 0 )
    {
        printf("RpcBindingFromStringBinding return  %d !\n", status);
        return 1;
    }

    doRpcCall();

    status = RpcStringFree( &pszStringBinding );
    if ( status != 0 )
        printf("RpcStringFree return  %d !\n", status);

    status = RpcBindingFree( &hello_Binding );
    if ( status != 0 )
        printf("RpcBindingFree return  %d !\n", status);

    getchar();
    return 0;
}

void doRpcCall()
{
    char buff[1024];

    RpcTryExcept
    {
        while (1)
        {
            printf("Please input a string param for RPC call:\n");
            gets(buff);

            if ( strcmp(buff, "exit") == 0 || strcmp(buff, "quit") == 0 )
            {
                ShutDown();
                break;
            }
            else
            {
                HelloProc( (unsigned char *)buff );
                printf("RPC call <HelloProc> succeed!\n");
            }
        }
    }
    RpcExcept(1)
    {
        /*
        unsigned long ulCode = RpcExceptionCode();
        printf("RPC exception occured! code:  %ld\n", ulCode);
        */
    }
    RpcEndExcept
}

void * __RPC_USER MIDL_user_allocate(size_t len)
{
    return (malloc(len));
}

void __RPC_USER MIDL_user_free(void* ptr)
{
    free(ptr);
}

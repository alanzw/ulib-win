/* this ALWAYS GENERATED file contains the RPC server stubs */


/* File created by MIDL compiler version 5.01.0164 */
/* at Fri Apr 23 00:15:31 2010
 */
/* Compiler settings for hello_rpc.idl:
    Os (OptLev=s), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data
*/
//@@MIDL_FILE_HEADING(  )

#include <string.h>
#include "hello_rpc.h"

#define TYPE_FORMAT_STRING_SIZE   7
#define PROC_FORMAT_STRING_SIZE   9

typedef struct _MIDL_TYPE_FORMAT_STRING
{
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
} MIDL_TYPE_FORMAT_STRING;

typedef struct _MIDL_PROC_FORMAT_STRING
{
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
} MIDL_PROC_FORMAT_STRING;

static const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString =
{
    0,
    {
        0x4d,        /* FC_IN_PARAM */
#ifndef _ALPHA_
        0x1,        /* x86, MIPS & PPC Stack size = 1 */
#else
        0x2,        /* Alpha Stack size = 2 */
#endif
/*  2 */NdrFcShort( 0x2 ),    /* Type Offset=2 */
/*  4 */0x5b,        /* FC_END */
        0x5c,        /* FC_PAD */
/*  6 */0x5b,        /* FC_END */
        0x5c,        /* FC_PAD */
        0x0
    }
};

static const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString =
{
    0,
    {
        NdrFcShort( 0x0 ),    /* 0 */
/*  2 */
        0x11, 0x8,    /* FC_RP [simple_pointer] */
/*  4 */
        0x22,        /* FC_C_CSTRING */
        0x5c,        /* FC_PAD */
        0x0
        }
    };

/* Standard interface: HelloRPC, ver. 1.0,
   GUID={0xf97fa259,0xb638,0x454a,{0xa0,0x52,0x75,0x61,0x4b,0x0d,0xa8,0x5b}} */


extern RPC_DISPATCH_TABLE HelloRPC_v1_0_DispatchTable;

static const RPC_SERVER_INTERFACE HelloRPC___RpcServerInterface =
    {
    sizeof(RPC_SERVER_INTERFACE),
    {{0xf97fa259,0xb638,0x454a,{0xa0,0x52,0x75,0x61,0x4b,0x0d,0xa8,0x5b}},{1,0}},
    {{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}},
    &HelloRPC_v1_0_DispatchTable,
    0,
    0,
    0,
    0,
    0
    };

RPC_IF_HANDLE HelloRPC_v1_0_s_ifspec = (RPC_IF_HANDLE)& HelloRPC___RpcServerInterface;

/*extern static const MIDL_STUB_DESC HelloRPC_StubDesc;*/
static const MIDL_STUB_DESC HelloRPC_StubDesc =
{
    (void __RPC_FAR *)& HelloRPC___RpcServerInterface,
        MIDL_user_allocate,
        MIDL_user_free,
        0,
        0,
        0,
        0,
        0,
        __MIDL_TypeFormatString.Format,
        1, /* -error bounds_check flag */
        0x10001, /* Ndr library version */
        0,
        0x50100a4, /* MIDL Version 5.1.164 */
        0,
        0,
        0,  /* notify & notify_flag routine table */
        1,  /* Flags */
        0,  /* Reserved3 */
        0,  /* Reserved4 */
        0   /* Reserved5 */
};


void __RPC_STUB
HelloRPC_HelloProc(
    PRPC_MESSAGE _pRpcMessage )
{
    MIDL_STUB_MESSAGE _StubMsg;
    unsigned char __RPC_FAR *pszString;
    RPC_STATUS _Status;

    ((void)(_Status));
    NdrServerInitializeNew(
                          _pRpcMessage,
                          &_StubMsg,
                          &HelloRPC_StubDesc);

    /* ( unsigned char __RPC_FAR * )pszString = 0; */
    pszString = 0;
    RpcTryFinally
        {
        RpcTryExcept
            {
            if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[0] );

            NdrConformantStringUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                           (unsigned char __RPC_FAR * __RPC_FAR *)&pszString,
                                           (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[4],
                                           (unsigned char)0 );

            if(_StubMsg.Buffer > _StubMsg.BufferEnd)
                {
                RpcRaiseException(RPC_X_BAD_STUB_DATA);
                }
            }
        RpcExcept( RPC_BAD_STUB_DATA_EXCEPTION_FILTER )
            {
            /* RpcRaiseException(RPC_X_BAD_STUB_DATA); */
            }
        RpcEndExcept

        HelloProc(pszString);

        }
    RpcFinally
        {
        }
    RpcEndFinally
    _pRpcMessage->BufferLength =
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);

}

void __RPC_STUB
HelloRPC_ShutDown(
    PRPC_MESSAGE _pRpcMessage )
{
    MIDL_STUB_MESSAGE _StubMsg;
    RPC_STATUS _Status;

    ((void)(_Status));
    NdrServerInitializeNew(
                          _pRpcMessage,
                          &_StubMsg,
                          &HelloRPC_StubDesc);

    RpcTryFinally
        {
        RpcTryExcept
            {
            if(_StubMsg.Buffer > _StubMsg.BufferEnd)
                {
                RpcRaiseException(RPC_X_BAD_STUB_DATA);
                }
            }
        RpcExcept( RPC_BAD_STUB_DATA_EXCEPTION_FILTER )
            {
            RpcRaiseException(RPC_X_BAD_STUB_DATA);
            }
        RpcEndExcept

        ShutDown();

        }
    RpcFinally
        {
        }
    RpcEndFinally
    _pRpcMessage->BufferLength =
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);

}


static RPC_DISPATCH_FUNCTION HelloRPC_table[] =
    {
    HelloRPC_HelloProc,
    HelloRPC_ShutDown,
    0
    };
RPC_DISPATCH_TABLE HelloRPC_v1_0_DispatchTable =
    {
    2,
    HelloRPC_table
    };

#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif


/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Fri Apr 23 00:15:31 2010
 */
/* Compiler settings for hello_rpc.idl:
    Os (OptLev=s), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __hello_rpc_h__
#define __hello_rpc_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __HelloRPC_INTERFACE_DEFINED__
#define __HelloRPC_INTERFACE_DEFINED__

/* interface HelloRPC */
/* [implicit_handle][version][uuid] */ 

void HelloProc( 
    /* [string][in] */ unsigned char __RPC_FAR *pszString);

void ShutDown( void);


extern handle_t hello_Binding;


extern RPC_IF_HANDLE HelloRPC_v1_0_c_ifspec;
extern RPC_IF_HANDLE HelloRPC_v1_0_s_ifspec;
#endif /* __HelloRPC_INTERFACE_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif


#ifndef _MSC_VER
#define RpcTryFinally
#define RpcTryExcept
#define RpcExcept(x)
#define RpcEndExcept
#define RpcFinally
#define RpcEndFinally
#endif // _MSC_VER

#endif

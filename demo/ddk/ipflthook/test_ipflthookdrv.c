/*
* For x86/EWindows XP SP1 & VC 7
* cl ipflthookdrvtest.c /nologo /Os /G6 /Gz /Gs65536 /W3 /WX /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /link /RELEASE
*/

/*
* #define _WIN32_WINNT 0x0501
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
/*
* 用到了CTL_CODE宏
*/
#include <winioctl.h>

/************************************************************************
*                                                                      *
*                               Macro                                  *
*                                                                      *
************************************************************************/

#pragma comment( linker, "/INCREMENTAL:NO"    )
#pragma comment( linker, "/subsystem:console" )

#define VERSION                                 "1.00"
#define EXTERNALNAME                            "\\\\.\\IpFilterHookExternal1"

/*
* 0x0800是最小可用值
*/
#define IPFILTERHOOK_INDEX                      0x0800
#define IOCTL_IPFILTERHOOK_GET_FIREWALLFLAGS    CTL_CODE    \
    (                                                           \
    FILE_DEVICE_NETWORK,                                    \
    IPFILTERHOOK_INDEX + 0,                                 \
    METHOD_BUFFERED,                                        \
    FILE_READ_ACCESS                                        \
    )
#define IOCTL_IPFILTERHOOK_SET_FIREWALLFLAGS    CTL_CODE    \
    (                                                           \
    FILE_DEVICE_NETWORK,                                    \
    IPFILTERHOOK_INDEX + 1,                                 \
    METHOD_BUFFERED,                                        \
    FILE_WRITE_ACCESS                                       \
    )
/*
* 置位 - Enable
* 复位 - Disable
*/
#define FIREWALLFLAGS_DISABLEALL                0x00000000
/*
* 所有ICMP报文，包括接收、发送
*/
#define FIREWALLFLAGS_ALLICMP                   0x00000001
/*
* 接收到的RST报文
*/
#define FIREWALLFLAGS_INBOUNDRST                0x00000002
#define FIREWALLFLAGS_ENABLEALL                 0xFFFFFFFF
/*
* 缺省设置是全部允许，即不丢弃任何报文
*/
#define FIREWALLFLAGS_DEFAULT                   FIREWALLFLAGS_ENABLEALL

/************************************************************************
*                                                                      *
*                            Function Prototype                        *
*                                                                      *
************************************************************************/

static void PrintWin32ErrorCLI
(
char   *message,
DWORD   dwMessageId
);
static void usage
(
char   *arg
);

/************************************************************************
*                                                                      *
*                            Static Global Var                         *
*                                                                      *
************************************************************************/

/************************************************************************/

static void PrintWin32ErrorCLI ( char *message, DWORD dwMessageId )
{
    char   *errMsg;

    FormatMessage
    (
    FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
    NULL,
    dwMessageId,
    MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
    ( LPTSTR )&errMsg,
    0,
    NULL
    );
    fprintf( stderr, "%s: %s", message, errMsg );
    LocalFree( errMsg );
    return;
}  /* end of PrintWin32ErrorCLI */

static void usage ( char *arg )
{
    fprintf
    (
    stderr,
    "Usage: %s [-h] [-v] [-f FirewallFlags]\n",
    arg
    );
    exit( EXIT_FAILURE );
}  /* end of usage */

int __cdecl main ( int argc, char * argv[] )
{
    int     c,
    ret                 = EXIT_FAILURE;
    HANDLE  Device              = INVALID_HANDLE_VALUE;
    ULONG   FirewallFlags       = FIREWALLFLAGS_DEFAULT;
    ULONG   OrigFirewallFlags;
    DWORD   BytesReturned;

#if 0
    if ( 1 == argc )
    {
        usage( argv[0] );
    }
#endif
    /*
    * 从argv[1]开始循环处理命令行参数
    */
    for ( c = 1; c < argc; c++ )
    {
        /*
        * 同时支持-和/两种引入命令行参数的方式
        */
        if ( ( ( argv[c][0] != '-' ) && ( argv[c][0] != '/' ) ) || ( strlen( argv[c] ) < 2 ) )
        {
            usage( argv[0] );
        }
        else
        {
            /*
            * 在这个字节上，大小写不敏感
            */
            switch ( tolower( argv[c][1] ) )
            {
            case 'f':
                if ( ( c + 1 ) >= argc )
                {
                    usage( argv[0] );
                }
                FirewallFlags   = strtoul( argv[++c], NULL, 0 );
                break;
            case 'v':
                fprintf( stderr, "%s ver "VERSION"\n", argv[0] );
                return( EXIT_SUCCESS );
            case 'h':
            case '?':
            default:
                usage( argv[0] );
                break;
            }  /* end of switch */
        }
    }  /* end of for */
    /*
    * HANDLE CreateFile
    * (
    *     LPCTSTR                 lpFileName,             // file name
    *     DWORD                   dwDesiredAccess,        // access mode
    *     DWORD                   dwShareMode,            // share mode
    *     LPSECURITY_ATTRIBUTES   lpSecurityAttributes,   // SD
    *     DWORD                   dwCreationDisposition,  // how to create
    *     DWORD                   dwFlagsAndAttributes,   // file attributes
    *     HANDLE                  hTemplateFile           // handle to template file
    * );
    *
    * If the function fails, the return value is INVALID_HANDLE_VALUE. To
    * get extended error information, call GetLastError.
    */
    Device  = CreateFile
    (
    EXTERNALNAME,
    GENERIC_READ | GENERIC_WRITE,
    0,
    NULL,           // If lpSecurityAttributes is NULL, the handle cannot be inherited
    OPEN_EXISTING,  // The function fails if the file does not exist
    FILE_ATTRIBUTE_NORMAL,
    NULL
    );
    if ( INVALID_HANDLE_VALUE == Device )
    {
        PrintWin32ErrorCLI( "CreateFile() failed", GetLastError() );
        goto main_exit;
    }
    /*
    * BOOL DeviceIoControl
    * (
    *     HANDLE          hDevice,
    *     DWORD           dwIoControlCode,
    *     LPVOID          lpInBuffer,
    *     DWORD           nInBufferSize,
    *     LPVOID          lpOutBuffer,
    *     DWORD           nOutBufferSize,
    *     LPDWORD         lpBytesReturned,
    *     LPOVERLAPPED    lpOverlapped
    * );
    */
    if
    (
    FALSE == DeviceIoControl
    (
    Device,
    IOCTL_IPFILTERHOOK_GET_FIREWALLFLAGS,
    NULL,
    0,
    &OrigFirewallFlags,
    sizeof( OrigFirewallFlags ),
    &BytesReturned,
    NULL
    )
    )
    {
        PrintWin32ErrorCLI( "DeviceIoControl( GET ) for OrigFirewallFlags failed", GetLastError() );
        goto main_exit;
    }
    printf( "OrigFirewallFlags   = 0x%08X\n", OrigFirewallFlags );
    if
    (
    FALSE == DeviceIoControl
    (
    Device,
    IOCTL_IPFILTERHOOK_SET_FIREWALLFLAGS,
    &FirewallFlags,
    sizeof( FirewallFlags ),
    NULL,
    0,
    &BytesReturned,
    NULL
    )
    )
    {
        PrintWin32ErrorCLI( "DeviceIoControl( SET ) failed", GetLastError() );
        goto main_exit;
    }
    if
    (
    FALSE == DeviceIoControl
    (
    Device,
    IOCTL_IPFILTERHOOK_GET_FIREWALLFLAGS,
    NULL,
    0,
    &FirewallFlags,
    sizeof( FirewallFlags ),
    &BytesReturned,
    NULL
    )
    )
    {
        PrintWin32ErrorCLI( "DeviceIoControl( GET ) for FirewallFlags failed", GetLastError() );
        goto main_exit;
    }
    printf( "FirewallFlags       = 0x%08X\n", FirewallFlags );
    ret     = EXIT_SUCCESS;

    main_exit:

    if ( INVALID_HANDLE_VALUE != Device )
    {
        CloseHandle( Device );
        Device  = INVALID_HANDLE_VALUE;
    }
    return( ret );
}  /* end of main */
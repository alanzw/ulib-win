/************************************************************************
 *                                                                      *
 *                               Head File                              *
 *                                                                      *
 ************************************************************************/

/*
 * #define _WIN32_WINNT 0x0501
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

/************************************************************************
 *                                                                      *
 *                               Macro                                  *
 *                                                                      *
 ************************************************************************/

#pragma comment( linker, "/INCREMENTAL:NO"    )
#pragma comment( linker, "/subsystem:console" )
#pragma comment( lib,    "kernel32.lib"       )
#pragma comment( lib,    "advapi32.lib"       )

#define VERSION "1.10"

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
        "Usage: %s [-h] [-v] [-t target] [-s servicename] [-d displayname]\n"
        "       [-c cmdline] [-p dependencies]\n",
        arg
    );
    exit( EXIT_FAILURE );
}  /* end of usage */

int __cdecl main ( int argc, char * argv[] )
{
    SC_HANDLE       scm             = ( SC_HANDLE )NULL,
                    sc_handle       = ( SC_HANDLE )NULL;
    unsigned char  *target          = NULL,
                   *servicename     = NULL,
                   *displayname     = NULL,
                   *cmdline         = NULL,
                   *dependencies    = NULL,
                   *p;
    int             c,
                    ret             = EXIT_FAILURE;

    if ( 1 == argc )
    {
        usage( argv[0] );
    }
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
            case 'c':
                if ( ( c + 1 ) >= argc )
                {
                    usage( argv[0] );
                }
                cmdline         = argv[++c];
                break;
            case 'd':
                if ( ( c + 1 ) >= argc )
                {
                    usage( argv[0] );
                }
                displayname     = argv[++c];
                break;
            case 'p':
                if ( ( c + 1 ) >= argc )
                {
                    usage( argv[0] );
                }
                dependencies    = argv[++c];
                break;
            case 's':
                if ( ( c + 1 ) >= argc )
                {
                    usage( argv[0] );
                }
                servicename     = argv[++c];
                break;
            case 't':
                if ( ( c + 1 ) >= argc )
                {
                    usage( argv[0] );
                }
                target          = argv[++c];
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
     * 检查参数有效性
     */
    if ( NULL == servicename )
    {
        fprintf( stderr, "Checking your [-s servicename]\n" );
        return( EXIT_FAILURE );
    }
    if ( NULL == displayname )
    {
        fprintf( stderr, "Checking your [-d displayname]\n" );
        return( EXIT_FAILURE );
    }
    if ( NULL == cmdline )
    {
        fprintf( stderr, "Checking your [-c cmdline]\n" );
        return( EXIT_FAILURE );
    }
    /*
     * Header : Declared in Winsvc.h; include Windows.h.
     * Library: Use Advapi32.lib.
     *
     * SC_HANDLE OpenSCManager
     * (
     *     LPCTSTR lpMachineName,      // computer name
     *     LPCTSTR lpDatabaseName,     // SCM database name
     *     DWORD   dwDesiredAccess     // access type
     * );
     *
     * 第一形参可以用target，也可用\\<target>。还应该尝试unicodeserver。
     */
    scm         = OpenSCManager
    (
        target,
        SERVICES_ACTIVE_DATABASE,
        SC_MANAGER_CREATE_SERVICE
    );
    if ( NULL == scm )
    {
        PrintWin32ErrorCLI( "OpenSCManager() failed", GetLastError() );
        goto main_exit;
    }
    /*
     * 要求用逗号做每个服务名的结尾
     */
    if ( NULL != dependencies )
    {
        p   = dependencies;
        while ( *p )
        {
            if ( ',' == *p )
            {
                *p  = '\0';
            }
            p++;
        }  /* end of while */
    }
    /*
     * SC_HANDLE CreateService
     * (
     *     SC_HANDLE   hSCManager,         // handle to SCM database
     *     LPCTSTR     lpServiceName,      // name of service to start
     *     LPCTSTR     lpDisplayName,      // display name
     *     DWORD       dwDesiredAccess,    // type of access to service
     *     DWORD       dwServiceType,      // type of service
     *     DWORD       dwStartType,        // when to start service
     *     DWORD       dwErrorControl,     // severity of service failure
     *     LPCTSTR     lpBinaryPathName,   // name of binary file
     *     LPCTSTR     lpLoadOrderGroup,   // name of load ordering group
     *     LPDWORD     lpdwTagId,          // tag identifier
     *     LPCTSTR     lpDependencies,     // array of dependency names
     *     LPCTSTR     lpServiceStartName, // account name
     *     LPCTSTR     lpPassword          // account password
     * );
     */
    sc_handle   = CreateService
    (
        scm,
        servicename,
        displayname,
        SERVICE_ALL_ACCESS,
        SERVICE_KERNEL_DRIVER,
        SERVICE_DEMAND_START,
        SERVICE_ERROR_NORMAL,
        cmdline,
        NULL,
        NULL,
        dependencies,
        NULL,
        NULL
    );
    if ( NULL == sc_handle )
    {
        PrintWin32ErrorCLI( "CreateService() failed", GetLastError() );
        goto main_exit;
    }
    ret         = EXIT_SUCCESS;

main_exit:

    if ( NULL != sc_handle )
    {
        CloseServiceHandle( sc_handle );
        sc_handle = ( SC_HANDLE )NULL;
    }
    if ( NULL != scm )
    {
        CloseServiceHandle( scm );
        scm = ( SC_HANDLE )NULL;
    }
    return( ret );
}  /* end of main */

/************************************************************************/
#include "resource.h"
#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#ifndef _MSC_VER

#define __out_ecount(size)
#define __out_ecount_full_opt(size)
#define __in_ecount(size)

#define __in
#define __out
#define __inout

#include "winhttp.h"

#endif // _MSC_VER

int main(int argc, char *argv[])
{
    DWORD data;
    DWORD dwSize = sizeof(DWORD);

    // Use WinHttpOpen to obtain an HINTERNET handle.
    HINTERNET hSession = WinHttpOpen(
        L"A WinHTTP Example Program/1.0",
        WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
        WINHTTP_NO_PROXY_NAME,
        WINHTTP_NO_PROXY_BYPASS,
        0);
    if (hSession)
    {


        // Use WinHttpQueryOption to retrieve internet options.
        if (WinHttpQueryOption( hSession,
                WINHTTP_OPTION_CONNECT_TIMEOUT,
                &data, &dwSize))
        {
            printf("Connection timeout: %u ms\n\n",data);
        }
        else
        {
            printf( "Error %u in WinHttpQueryOption.\n",
                GetLastError());
        }

        // When finished, release the HINTERNET handle.
        WinHttpCloseHandle(hSession);
    }
    else
    {
        printf("Error %u in WinHttpOpen.\n", GetLastError());
    }

    //DWORD dwSize = 0;
    DWORD dwDownloaded = 0;
    LPSTR pszOutBuffer;
    BOOL  bResults = FALSE;
    //HINTERNET  hSession = NULL;
    HINTERNET  hConnect = NULL;
    HINTERNET  hRequest = NULL;

    // Use WinHttpOpen to obtain a session handle.
    hSession = WinHttpOpen( L"WinHTTP Example/1.0",
        WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
        WINHTTP_NO_PROXY_NAME,
        WINHTTP_NO_PROXY_BYPASS, 0 );

    // Specify an HTTP server.
    if( hSession )
        hConnect = WinHttpConnect( hSession, L"www.microsoft.com",
            INTERNET_DEFAULT_HTTPS_PORT, 0 );

    // Create an HTTP request handle.
    if( hConnect )
        hRequest = WinHttpOpenRequest( hConnect, L"GET", NULL,
            NULL, WINHTTP_NO_REFERER,
            WINHTTP_DEFAULT_ACCEPT_TYPES,
            WINHTTP_FLAG_SECURE );

    // Send a request.
    if( hRequest )
        bResults = WinHttpSendRequest( hRequest,
            WINHTTP_NO_ADDITIONAL_HEADERS, 0,
            WINHTTP_NO_REQUEST_DATA, 0,
            0, 0 );


    // End the request.
    if( bResults )
        bResults = WinHttpReceiveResponse( hRequest, NULL );

    // Keep checking for data until there is nothing left.
    if( bResults )
    {
        do
        {
            // Check for available data.
            dwSize = 0;
            if( !WinHttpQueryDataAvailable( hRequest, &dwSize ) )
                printf( "Error %u in WinHttpQueryDataAvailable.\n",
                    GetLastError( ) );

            // Allocate space for the buffer.
            pszOutBuffer = new char[dwSize+1];
            if( !pszOutBuffer )
            {
                printf( "Out of memory\n" );
                dwSize=0;
            }
            else
            {
                // Read the data.
                ZeroMemory( pszOutBuffer, dwSize+1 );

                if( !WinHttpReadData( hRequest, (LPVOID)pszOutBuffer,
                        dwSize, &dwDownloaded ) )
                    printf( "Error %u in WinHttpReadData.\n", GetLastError( ) );
                else
                    printf( "%s", pszOutBuffer );

                // Free the memory allocated to the buffer.
                delete [] pszOutBuffer;
            }
        } while( dwSize > 0 );
    }


    // Report any errors.
    if( !bResults )
        printf( "Error %d has occurred.\n", GetLastError( ) );

    // Close any open handles.
    if( hRequest ) WinHttpCloseHandle( hRequest );
    if( hConnect ) WinHttpCloseHandle( hConnect );
    if( hSession ) WinHttpCloseHandle( hSession );

    return 0;
}



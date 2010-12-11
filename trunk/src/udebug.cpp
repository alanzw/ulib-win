#include <windows.h>
#include <stdio.h>
#include <ctype.h>

#include "udebug.h"

namespace UDebugger
{

void odprintf(const char *format, ...)
{
    char    buf[4096], *p = buf;
    va_list args;
    int     n;

    va_start(args, format);
    n = _vsnprintf(p, sizeof(buf) - 3, format, args); // buf-3 is room for CR/LF/NUL
    va_end(args);

    p += (n < 0) ? sizeof buf - 3 : n;

    while ( p > buf  &&  isspace(p[-1]) )
        *--p = '\0';

    *p++ = '\r';
    *p++ = '\n';
    *p   = '\0';

    ::OutputDebugStringA(buf);
}

#ifndef MUTEX_QUERY_STATE 
#define MUTEX_QUERY_STATE (1)
#endif // MUTEX_QUERY_STATE

HANDLE setup_mutex(void)
{
    SID_IDENTIFIER_AUTHORITY SIAWindowsNT = SECURITY_NT_AUTHORITY;
    SID_IDENTIFIER_AUTHORITY SIAWorld     = SECURITY_WORLD_SID_AUTHORITY;

    SID     *pSidSYSTEM   = 0,      // 
            *pSidAdmins   = 0,
            *pSidEveryone = 0;

    AllocateAndInitializeSid(&SIAWindowsNT,
                             1,
                             SECURITY_LOCAL_SYSTEM_RID,
                             0, 0, 0, 0, 0, 0, 0,
                             (void **)&pSidSYSTEM);

    AllocateAndInitializeSid(&SIAWindowsNT,
                                2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS,
                                0, 0, 0, 0, 0, 0,
                                (void **)&pSidAdmins);

    AllocateAndInitializeSid(&SIAWorld,
                                1, SECURITY_WORLD_RID,
                                0, 0, 0, 0, 0, 0, 0,
                                (void **)&pSidEveryone);

    const DWORD dwACLSize = GetLengthSid(pSidSYSTEM)
                              + GetLengthSid(pSidAdmins)
                              + GetLengthSid(pSidEveryone)
                              + 32;

    ACL *pACL = (ACL *) GlobalAlloc( 0, dwACLSize );

    InitializeAcl(pACL, dwACLSize, ACL_REVISION );

    AddAccessAllowedAce(pACL,
                            ACL_REVISION,
                            SYNCHRONIZE | READ_CONTROL | MUTEX_QUERY_STATE,
                            pSidEveryone);

    AddAccessAllowedAce(pACL,
                            ACL_REVISION,
                            MUTEX_ALL_ACCESS,
                            pSidSYSTEM);

    AddAccessAllowedAce(pACL,
                            ACL_REVISION,
                            MUTEX_ALL_ACCESS,
                            pSidAdmins);

    SECURITY_DESCRIPTOR sd;

    InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);

    SetSecurityDescriptorDacl(&sd, TRUE, pACL, FALSE);

    SECURITY_ATTRIBUTES sa;
    ZeroMemory(&sa, sizeof sa);

    sa.bInheritHandle       = FALSE;
    sa.nLength              = sizeof sa;
    sa.lpSecurityDescriptor = &sd;

    HANDLE hMutex = CreateMutex(&sa, FALSE, "DBWinMutex");

    FreeSid(pSidAdmins);
    FreeSid(pSidSYSTEM);
    FreeSid(pSidEveryone);
    GlobalFree(pACL);

    return hMutex;
}

struct dbwin_buffer {
        DWORD   dwProcessId;
        char    data[4096-sizeof(DWORD)];
};

static HANDLE hDbwinMutex = 0;

void UOutputDebugStringA(char *lpString)
{
    dbwin_buffer *pDBBuffer    = 0;
    HANDLE        hFileMap     = 0;
    HANDLE        hBufferEvent = 0;
    HANDLE        hDataEvent   = 0;

	// if we can't make or acquire the mutex, we're done

        if ( hDbwinMutex == 0 )
                hDbwinMutex = setup_mutex();

        if ( hDbwinMutex == 0) return;

        (void) WaitForSingleObject(hDbwinMutex, INFINITE);


        hFileMap = OpenFileMapping(FILE_MAP_WRITE, FALSE, "DBWIN_BUFFER");

        pDBBuffer = (dbwin_buffer *) MapViewOfFile(
                hFileMap,
                FILE_MAP_READ|FILE_MAP_WRITE,
                0,              // file offset high
                0,              // file offset low
                0 );            // # of bytes to map (entire file)

        hBufferEvent = OpenEvent( SYNCHRONIZE,        FALSE, "DBWIN_BUFFER_READY");
        hDataEvent   = OpenEvent( EVENT_MODIFY_STATE, FALSE, "DBWIN_DATA_READY");

        const char *p  = lpString;
        int        len = strlen(lpString);

        while ( len > 0 )
        {
                if ( WaitForSingleObject(hBufferEvent, 10*1000) != WAIT_OBJECT_0 )
		{
			/* ERROR: give up */
			break;
		}

		// populate the shared memory segment. The string
		// is limited to 4k or so.
       pDBBuffer->dwProcessId = GetCurrentProcessId();

       int n = min(len, sizeof(pDBBuffer->data)-1);

       memcpy(pDBBuffer->data, p, n);
       pDBBuffer->data[n] = '\0';

       len -= n;
       p   += n;

       SetEvent(hDataEvent);
    }

	// cleanup after ourselves
	CloseHandle(hBufferEvent);
	CloseHandle(hDataEvent);
	UnmapViewOfFile(pDBBuffer);
	CloseHandle(hFileMap);
}

} // namespace UDebugger

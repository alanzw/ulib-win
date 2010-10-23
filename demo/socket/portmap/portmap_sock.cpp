#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <Aclapi.h>
#include <comdef.h>
#include <winioctl.h>

#define NT_HANDLE_LIST16
#define MAX_HANDLE_LIST_BUF 0x200000

typedef struct _HandleInfo
{
    USHORT dwPid;
    USHORT CreatorBackTraceIndex; 
    BYTE ObjType;
    BYTE HandleAttributes;
    USHORT HndlOffset;
    DWORD dwKeObject;
    ULONG GrantedAccess;
} HANDLEINFO, *PHANDLEINFO;

typedef struct _IO_STATUS_BLOCK {
    DWORD  Status;
    ULONG  Information;
} IO_STATUS_BLOCK, *PIO_STATUS_BLOCK;

typedef struct _LSA_UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
    PWSTR Buffer;
} LSA_UNICODE_STRING, *PLSA_UNICODE_STRING; 

typedef LSA_UNICODE_STRING UNICODE_STRING, *PUNICODE_STRING;

typedef struct _OBJECT_ATTRIBUTES {
    ULONG  Length;
    HANDLE RootDirectory;
    UNICODE_STRING *ObjectName;
    ULONG  Attributes;
    PSECURITY_DESCRIPTOR SecurityDescriptor;
    PSECURITY_QUALITY_OF_SERVICE SecurityQualityOfService;
} OBJECT_ATTRIBUTES,*POBJECT_ATTRIBUTES;

//
typedef DWORD  (CALLBACK* NTQUERYSYSTEMINFORMATION)( DWORD , PDWORD , DWORD , PVOID );
NTQUERYSYSTEMINFORMATION NtQuerySystemInformation;

typedef VOID (CALLBACK* RTLINITUNICODESTRING)(PUNICODE_STRING,PCWSTR);
RTLINITUNICODESTRING RtlInitUnicodeString;

typedef DWORD  (CALLBACK* ZWOPENSECTION)(PVOID, DWORD ,POBJECT_ATTRIBUTES); 
ZWOPENSECTION ZwOpenSection;

typedef VOID(CALLBACK* INITIALIZEOBJECTATTRIBUTES)(POBJECT_ATTRIBUTES,PUNICODE_STRING,ULONG ,HANDLE,PSECURITY_DESCRIPTOR);
INITIALIZEOBJECTATTRIBUTES InitializeObjectAttributes;

typedef DWORD  (CALLBACK* ZWOPENFILE)(PHANDLE,DWORD ,POBJECT_ATTRIBUTES,PIO_STATUS_BLOCK,ULONG ,ULONG );
ZWOPENFILE ZwOpenFile;

DWORD  getmap(PHANDLEINFO get1,LPVOID addr,HANDLE pm,char * buf)
{
    DWORD  readset;
    LPVOID pmaddr1;
    int i;
    readset = (get1->dwKeObject>>0x16); 
    readset = *((LPDWORD )((DWORD )addr + 4*readset));
    if((readset&0x000000ff)<1)    return 0;
    if((readset&0x000000ff)<0x80)
    {
        pmaddr1 = MapViewOfFile(pm,4,0,readset&0xfffff000,0x1000);
        readset = (get1->dwKeObject>>0x0c) & 0x3ff; 
        readset = *((LPDWORD )((DWORD )pmaddr1 + 4*readset));
        UnmapViewOfFile(pmaddr1);
        readset = readset & 0x0FFFFF000; 
    }
    else
    {
        readset=(readset&0xfffff000)+(get1->dwKeObject&0x003ff000);
    }
    pmaddr1 =MapViewOfFile(pm,4,0,readset,0x1000);
    if(pmaddr1!=NULL)
    {
        readset = get1->dwKeObject&0x00000fff;
        readset = (DWORD )pmaddr1+readset;
        for(i=0;i<0x70;i++)buf[i] = *((char *)(readset + i));
        UnmapViewOfFile(pmaddr1);
    }
    else
        return 0;
    return readset;
}

int main( )
{
    DWORD  readset1;
    DWORD  readset2;
    DWORD  readset3;
    OVERLAPPED la;
    HMODULE hNtdll = NULL;
    DWORD  dwNumEntries;
    PHANDLEINFO pHandleInfo;
    HANDLE htcp;
    HANDLE pmy;
    HANDLE hudp;
    HANDLE myhand;
    HANDLE h1=NULL;
    hNtdll = LoadLibrary( "ntdll.dll" );
    DWORD  status;
    LPVOID pmaddr;
    TOKEN_PRIVILEGES NewState;
    DWORD  dwNumBytes = MAX_HANDLE_LIST_BUF;
    PDWORD  pdwHandleList;
    PDWORD  pdwHandInfo;
    DWORD  dwNumBytesRet; 
    HANDLE hToken;
    BOOL isok;
    UNICODE_STRING dn;
    IO_STATUS_BLOCK ch3;
    int port1;
    int port2;
    HANDLE hProc;
    wchar_t * ch1 = L"\\Device\\Tcp";
    wchar_t * ch2 = L"\\Device\\Udp";
    OBJECT_ATTRIBUTES ofs;
    DWORD  i;
    DWORD  p=0;
    char buf1[0x70];
    char buf2[0x70];
    char buf3[0x70];
    char in[0x18];
    char in1[0x18];
    char out[0x38];
    char out1[0x30];
    PHANDLEINFO tcpdnum;
    PHANDLEINFO udpdnum;
    if ( !hNtdll )
    {
    printf( "LoadLibrary( NTDLL.DLL ) Error:%d\n", GetLastError() );
    return false;
    }
    NtQuerySystemInformation = (NTQUERYSYSTEMINFORMATION)
    GetProcAddress( hNtdll, "NtQuerySystemInformation");
    RtlInitUnicodeString = (RTLINITUNICODESTRING)
    GetProcAddress( hNtdll, "RtlInitUnicodeString");
    ZwOpenSection = (ZWOPENSECTION)
    GetProcAddress( hNtdll, "ZwOpenSection");;
    InitializeObjectAttributes = (INITIALIZEOBJECTATTRIBUTES)
    GetProcAddress( hNtdll, "InitializeObjectAttributes");
    ZwOpenFile = (ZWOPENFILE)
    GetProcAddress( hNtdll, "ZwOpenFile");;
    RtlInitUnicodeString(&dn,L"\\Device\\PhysicalMemory");
    OBJECT_ATTRIBUTES udm= {
        sizeof(OBJECT_ATTRIBUTES),// Length
        NULL,// RootDirectory
        &dn,// ObjectName
        0,// Attributes
        NULL,// SecurityDescriptor
        NULL,// SecurityQualityOfService
    };
    status = ZwOpenSection(&h1,SECTION_MAP_READ,&udm);
    if(status == 0)
    {
        pmy = GetCurrentProcess();
        pmaddr =MapViewOfFile(h1,4,0,0x30000,0x1000);
        NewState.PrivilegeCount=1;
        NewState.Privileges[0].Attributes=2;
        NewState.Privileges[0].Luid.HighPart=0;
        NewState.Privileges[0].Luid.LowPart=0;
        isok=LookupPrivilegeValue(0,SE_DEBUG_NAME,&NewState.Privileges[0].Luid);
        isok=OpenProcessToken(pmy,0x20,&hToken);
        isok=AdjustTokenPrivileges(hToken,0,&NewState,0x10,0,0);
        CloseHandle(hToken);
        RtlInitUnicodeString(&dn,ch1);
        ofs.SecurityDescriptor = 0; 
        ofs.ObjectName = &dn; 
        ofs.Length =0x18;
        ofs.RootDirectory = 0; 
        ofs.Attributes =0x40;
        ofs.SecurityQualityOfService =0;
        status=ZwOpenFile(&htcp,0x100000,&ofs,&ch3,3,0);
        RtlInitUnicodeString(&dn,ch2);
        ofs.ObjectName = &dn; 
        status=ZwOpenFile(&hudp,0x100000,&ofs,&ch3,3,0);
        pdwHandleList = (PDWORD )malloc(dwNumBytes);
        pdwHandInfo = (PDWORD )malloc(2048);
        dwNumBytesRet = 0x10;
        isok = (*NtQuerySystemInformation)(0x10,pdwHandleList,dwNumBytes,&dwNumBytesRet);
        if( !isok)
        {
            dwNumEntries = pdwHandleList[0];
            pHandleInfo = (PHANDLEINFO)( pdwHandleList + 1 );
            for (i = 0; i < dwNumEntries; i++ )
            {
                if(pHandleInfo->dwPid == GetCurrentProcessId() && pHandleInfo->HndlOffset ==(int)htcp)
                {
                    tcpdnum = pHandleInfo;
                    break;
                }
                pHandleInfo++; 
            }
            pHandleInfo = (PHANDLEINFO)( pdwHandleList + 1 );
            for (i = 0; i < dwNumEntries; i++ )
            {
                if(pHandleInfo->dwPid == GetCurrentProcessId() && pHandleInfo->HndlOffset ==(int)hudp)
                {
                    udpdnum = pHandleInfo;
                    break;
                }
                pHandleInfo++; 
            }
            ZeroMemory(buf1,0x70);
            ZeroMemory(buf2,0x70);
            readset1 = getmap(tcpdnum,pmaddr,h1,buf1);
            if(readset1==0)
            {
                printf("map tcp faile\n");
                return 0;
            }
            readset2 = getmap(udpdnum,pmaddr,h1,buf2);
            if(readset2==0)
            {
                printf("map udp faile\n");
                return 0;
            }
            la.hEvent = CreateEvent(0,1,0,0);;
            la.Internal = 0;
            la.InternalHigh=0;
            la.Offset = 0;
            la.OffsetHigh = 0;
            pHandleInfo = (PHANDLEINFO)( pdwHandleList + 1 );
            for (i = 0; i < dwNumEntries; i++ )
            {
                ZeroMemory(buf3,0x70);
                if(pHandleInfo->ObjType == tcpdnum->ObjType)
                {
                readset3 = getmap(pHandleInfo,pmaddr,h1,buf3);
                if(readset3==0)
                {
                    pHandleInfo++;
                    continue;
                }
                if(buf3[4]==buf1[4] && buf3[5]==buf1[5]&& buf3[6]==buf1[6]&& buf3[7]==buf1[7])
                {
                    if((buf3[16]==1 || buf3[16]==2) && buf3[17]==0 && buf3[18]==0 && buf3[19]==0) 
                    {
                        hProc = OpenProcess(0x40,0,pHandleInfo->dwPid);
                        if(hProc==NULL)
                        {
                            pHandleInfo++;
                            continue;
                        }
                        DuplicateHandle(hProc,(HANDLE)pHandleInfo->HndlOffset,pmy,&myhand,0,0,2);
                        CloseHandle(hProc);
                        if(myhand==NULL)
                        {
                            pHandleInfo++;
                            continue;
                        }
                        ZeroMemory(out1,0x30);
                        ZeroMemory(out,0x38);
                        ZeroMemory(in,0x18);
                        ZeroMemory(in1,0x18);
                        in[0x10] = 4;
                        in1[0x10]=3;
                        if(buf3[16]==2)
                        {
                            p = 0;
                            isok = DeviceIoControl(myhand,0x210012,&in,0x18,&out,0x38,&p,&la);
                            if(isok==FALSE)
                            {
                                pHandleInfo++;
                                continue;
                            }
                        }
                        isok = DeviceIoControl(myhand,0x210012,&in1,0x18,&out1,0x30,&p,&la);
                        if(isok)
                        {
                            port1 = out1[12];
                            if(port1<0)
                                port1 = 256 + port1;
                            port1 = port1*256;
                            port2 = out1[13];
                            if(port2<0)
                                port1 = port1 + 256 + port2;
                            else
                                port1 = port1 + port2;
                            printf("TCPPID:%d;PORT:%d\n",pHandleInfo->dwPid,port1); 
                        }
                        CloseHandle(myhand);
                    }
                }
            }
            pHandleInfo++;
            }
            pHandleInfo = (PHANDLEINFO)( pdwHandleList + 1 );
            for (i = 0; i < dwNumEntries; i++ )
            {
                ZeroMemory(buf3,0x70);
                if(pHandleInfo->ObjType == udpdnum->ObjType)
                {
                    readset3 = getmap(pHandleInfo,pmaddr,h1,buf3);
                    if(readset3==0)
                    {
                        pHandleInfo++;
                        continue;
                    }
                    if(buf3[4]==buf2[4] && buf3[5]==buf2[5]&& buf3[6]==buf2[6]&& buf3[7]==buf2[7])
                    {
                        if((buf3[16]==1 || buf3[16]==2) && buf3[17]==0 && buf3[18]==0 && buf3[19]==0) 
                        {
                            hProc = OpenProcess(0x40,0,pHandleInfo->dwPid);
                            if(hProc==NULL)
                            {
                                pHandleInfo++;
                                continue;
                            }
                            DuplicateHandle(hProc,(HANDLE)pHandleInfo->HndlOffset,pmy,&myhand,0,0,2);
                            CloseHandle(hProc);
                            if(myhand==NULL)
                            {
                                pHandleInfo++;
                                continue;
                            }
                            ZeroMemory(out1,0x30);
                            ZeroMemory(out,0x38);
                            ZeroMemory(in,0x18);
                            ZeroMemory(in1,0x18);
                            in[0x10] = 4;
                            in1[0x10]=3;
                            if(buf3[16]==2)
                            {
                                p = 0;
                                isok = DeviceIoControl(myhand,0x210012,&in,0x18,&out,0x38,&p,&la);
                                if(isok==FALSE)
                                {
                                        pHandleInfo++;
                                        continue;
                                }
                            }
                            isok = DeviceIoControl(myhand,0x210012,&in1,0x18,&out1,0x30,&p,&la);
                            if(isok)
                            {
                                port1 = out1[12];
                                if(port1<0)    port1 = 256 + port1;
                                port1 = port1*256;
                                port2 = out1[13];
                                if(port2<0)
                                    port1 = port1 + 256 + port2;
                                else
                                    port1 = port1 + port2;
                                printf("UDPPID:%d;PORT:%d\n",pHandleInfo->dwPid,(unsigned short)port1); 
                            }
                            CloseHandle(myhand);
                        }
                    }
                }
                pHandleInfo++;
            }
            CloseHandle(la.hEvent);
            free(pdwHandleList);
            free(pdwHandInfo);
            CloseHandle(h1);
            CloseHandle(pmy);
            CloseHandle(htcp);
            CloseHandle(hudp);
            UnmapViewOfFile(pmaddr);
    }
    else
            printf("error open handlelist\n");
}
    else
        printf("error open physcalmemory\n");
    return true;
}




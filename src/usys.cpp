#define _WIN32_WINNT 0x0501

#include <windows.h>
#include <tchar.h>
#include <nb30.h>
#include <reason.h>

#include "usys.h"


USystem::USystem()
{
    m_pBuffer = new TCHAR[U_BUFSIZE];
}

USystem::~USystem()
{
    if (m_pBuffer)
    {
        delete[] m_pBuffer;
        m_pBuffer = 0;
    }
}

int USystem::getScreenWidth()
{
    return ::GetSystemMetrics(SM_CXSCREEN);
}

int USystem::getScreenHeight()
{
    return ::GetSystemMetrics(SM_CYSCREEN);
}

int USystem::getArrange()
{
    return ::GetSystemMetrics(SM_ARRANGE);
}

LPCTSTR USystem::getArrangeText()
{
    TCHAR tmp[U_BUFSIZE] = {0};

    switch (getArrange())
    {
    case ARW_DOWN:
        wsprintf(tmp, _T("%s"), _T("ARW_DOWN -- Arrange vertically, top to bottom."));
        break;
    case ARW_LEFT:
        wsprintf(tmp, _T("%s"), _T("ARW_LEFT -- Arrange horizontally, left to right."));
        break;
    //case ARW_RIGHT:
    //    wsprintf(tmp, _T("%s"), _T("ARW_RIGHT -- Arrange horizontally, right to left."));
    //    break;
    //case ARW_UP:
    //    wsprintf(tmp, _T("%s"), _T("ARW_UP -- Arrange vertically, bottom to top."));
    //    break;
    case ARW_HIDE:
        wsprintf(tmp, _T("%s"), _T("ARW_HIDE -- Hide minimized windows by moving them off the visible area of the screen."));
        break;
    default:
        break;
    }

    wsprintf(m_pBuffer, _T("%s"), tmp);

    return m_pBuffer;
}

int USystem::getBootMode()
{
    return ::GetSystemMetrics(SM_CLEANBOOT);
}

LPCTSTR USystem::getBootModeText()
{
    switch (getBootMode())
    {
        case 0:
            wsprintf(m_pBuffer, _T("Normal Boot"));
            break;
        case 1:
            wsprintf(m_pBuffer, _T("Fail-safe boot"));
            break;
        case 2:
            wsprintf(m_pBuffer, _T("Fail-safe with network boot"));
            break;
    }
    return m_pBuffer;
}

int USystem::getMonitorNum()
{
    return ::GetSystemMetrics(SM_CMONITORS);
}

int USystem::getMouseButtonNum()
{
    return ::GetSystemMetrics(SM_CMOUSEBUTTONS);
}

LPCTSTR USystem::getComputerName()
{
    DWORD dwSize = U_BUFSIZE-1;
    if (::GetComputerName(m_pBuffer, &dwSize))
    {
        return m_pBuffer;
    }
    return NULL;
}

int USystem::getWindowCaptionHeight()
{
    return ::GetSystemMetrics(SM_CYCAPTION);
}

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

// Message
void showMsg(const char *szMsg, const char *szTitle/*="Info"*/)
{
    ::MessageBox(NULL, szMsg, szTitle, MB_OK|MB_ICONINFORMATION);
}
//
void showError(const char *msg)
{
    DWORD eNum;
    TCHAR sysMsg[256];
    TCHAR* p;
    char str[1000];

    eNum = GetLastError( );
    FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, eNum,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        sysMsg, 256, NULL );

    // Trim the end of the line and terminate it with a null
    p = sysMsg;
    while( ( *p > 31 ) || ( *p == 9 ) )
        ++p;
    do { *p-- = 0; } while( ( p >= sysMsg ) &&
        ( ( *p == '.' ) || ( *p < 33 ) ) );

    // Display the message
    wsprintf(str, "%s failed with error %d (%s)",
        msg, eNum, sysMsg );
    showMsg(str);
}


void __stdcall showErrorInfo(DWORD eNum)
{
    TCHAR sysMsg[256];
    TCHAR* p;
    char str[1000];
    FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, eNum,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        sysMsg, 256, NULL );

    // Trim the end of the line and terminate it with a null
    p = sysMsg;
    while( ( *p > 31 ) || ( *p == 9 ) )
        ++p;
    do { *p-- = 0; } while( ( p >= sysMsg ) &&
        ( ( *p == '.' ) || ( *p < 33 ) ) );

    // Display the message
    wsprintf(str, "Failed with error %d (%s)",
        eNum, sysMsg );
    showMsg(str);
}
// Volume Information
void getVolSerialNum(char *sn, const char *vol/*="C:\\"*/)
{
    DWORD dwSn;
    GetVolumeInformation(vol, NULL, 0, &dwSn, NULL, NULL, NULL, 0);
    wsprintf(sn, "%X-%X", HIWORD(dwSn), LOWORD(dwSn));
}

int getVolumeList(char *vlmList)
{
    DWORD dwDrives;
    if (!(dwDrives = ::GetLogicalDrives()))
    {
        showError("GetLogicalDrives");
    }
    //wsprintf(vlmList, "%x", dwDrives);
    char letter = 'A';
    int count = sizeof(dwDrives)*8;
    int driverNum = 0;
    for (int i=0; i<count; i++)
    {
        if ((dwDrives&(1<<i)) && letter<='Z')
        {
            vlmList[i] = letter;
            driverNum++;
        }
        else
        {
            vlmList[i] = '0';
        }
        letter++;
    }
    vlmList[count] = '\0';
    return driverNum;
}

// XP cdkey
void getXPKey(char *cdkey)
{
    //get DigitalProductId
    HKEY hkey = 0;
    LONG lResult = 0L;
    LPCTSTR data_Set="SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\";
    long ret0=(::RegOpenKeyEx(HKEY_LOCAL_MACHINE, data_Set, 0, KEY_READ, &hkey));
    //
    DWORD dwType = 0 ;
    DWORD cbData = 0;
    lResult = ::RegQueryValueEx(hkey, "DigitalProductId", 0, &dwType, 0, &cbData);

    //char tmp[10];
    //wsprintf(tmp, "%d-%d", dwType, cbData);
    //MessageBox(NULL, tmp, "debug", MB_OK);

    LPBYTE digitalProductId = new BYTE[cbData];

    long ret1=::RegQueryValueEx(hkey, "DigitalProductId", NULL, &dwType, digitalProductId, &cbData);
    ::RegCloseKey(hkey);

    //decode
    const int keyStartIndex = 52;
    const int keyEndIndex = keyStartIndex + 15;
    char digits[] = {
        'B', 'C', 'D', 'F', 'G', 'H', 'J', 'K', 'M', 'P', 'Q', 'R',
        'T', 'V', 'W', 'X', 'Y', '2', '3', '4', '6', '7', '8', '9',
    };
    const int decodeLength = 29;
    const int decodeStringLength = 15;
    char decodedChars[decodeLength + 1];
    ::memset(decodedChars, 0, decodeLength + 1);
    byte hexPid[keyEndIndex - keyStartIndex + 1];
    for (int i = keyStartIndex; i <= keyEndIndex; i++)
    {
        hexPid[i - keyStartIndex] = digitalProductId[i];
    }
    //
    delete[] digitalProductId;
    //
    for (int i = decodeLength - 1; i >= 0; i--)
    {
        // Every sixth char is a separator.
        if ((i + 1) % 6 == 0)
        {
            decodedChars[i] = '-';
        }
        else
        {
            // Do the actual decoding.
            int digitMapIndex = 0;
            for (int j = decodeStringLength - 1; j >= 0; j--)
            {
                int byteValue = (digitMapIndex << 8) | hexPid[j];
                hexPid[j] = (byte)(byteValue / 24);
                digitMapIndex = byteValue % 24;
                decodedChars[i] = digits[digitMapIndex];
            }
        }
    }

    //
    //MessageBox(NULL, decodedChars, "Info", MB_OK);
    memcpy(cdkey, decodedChars, decodeLength+1);
}

//
void getComputerName(char *hostname)
{
    DWORD bufsize = MAX_COMPUTERNAME_LENGTH + 1;
    ::GetComputerName(hostname, &bufsize);
}

void getOSVersion(char *osver)
{
    DWORD dwVersion = 0;
    DWORD dwMajorVersion = 0;
    DWORD dwMinorVersion = 0;
    DWORD dwBuild = 0;

    dwVersion = GetVersion();

    // Get the Windows version.

    dwMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion)));
    dwMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion)));

    // Get the build number.

    if (dwVersion < 0x80000000)
        dwBuild = (DWORD)(HIWORD(dwVersion));


    wsprintf(osver, "%d.%d (%d)",
        dwMajorVersion,
        dwMinorVersion,
        dwBuild);
}

void getOSName(char *osname)
{
    DWORD dwVersion = ::GetVersion();
    //   Get   the   Windows   version.
    DWORD dwWindowsMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion)));
    DWORD dwWindowsMinorVersion   =   (DWORD)(HIBYTE(LOWORD(dwVersion)));
    //   Get   the   build   number   for   Windows   NT/Windows   2000   or   Win32s.
    if (dwVersion   <   0x80000000)   //   Windows   NT/2000
        wsprintf(osname, "Windows   NT/2000   %ld", (DWORD)(HIWORD(dwVersion)));
    else if (dwWindowsMajorVersion < 4)   //   Win32s
        wsprintf(osname, "Win32s   %ld", (DWORD)(HIWORD(dwVersion) & ~0x8000) );
    else   //   Windows   95/98   --   No   build   number
        wsprintf(osname, "Windows   95/98");
}

//
typedef struct _ASTAT_
{
    ADAPTER_STATUS adapt;
    NAME_BUFFER    NameBuff [30];
}ASTAT, * PASTAT;

void getMacAddress(char *macAddr)
{
    int lana_num = 6;
    NCB ncb;
    UCHAR uRetCode;
    //
    ASTAT adapter;
    //
    memset( &ncb, 0, sizeof(ncb) );
    ncb.ncb_command = NCBRESET;
    ncb.ncb_lana_num = lana_num;
    //
    uRetCode = Netbios( &ncb );
    //
    memset( &ncb, 0, sizeof(ncb) );
    ncb.ncb_command = NCBASTAT;
    ncb.ncb_lana_num = lana_num;     //
    //
    strcpy( (char *)ncb.ncb_callname, "*" );
    memset( &adapter, 0, sizeof(adapter) );
    ncb.ncb_buffer = (unsigned char *) &adapter;
    //
    //
    ncb.ncb_length = sizeof(adapter);

    //
    uRetCode = Netbios( &ncb );
    //
    if ( uRetCode == 0 )
    {

        // 00-10-A4-E4-58-02
        wsprintf( macAddr, "%02X-%02X-%02X-%02X-%02X-%02X",
            int(adapter.adapt.adapter_address[0]),
            adapter.adapt.adapter_address[1],adapter.adapt.adapter_address[2],
            adapter.adapt.adapter_address[3],adapter.adapt.adapter_address[4],
            adapter.adapt.adapter_address[5]);
    }

}

int getAdapterNum()
{
    NCB ncb;
    UCHAR uRetCode;
    LANA_ENUM lana_enum;

    memset( &ncb, 0, sizeof(ncb) );
    ncb.ncb_command = NCBENUM;

    ncb.ncb_buffer = (unsigned char *) &lana_enum;
    ncb.ncb_length = sizeof(lana_enum);
    uRetCode = Netbios( &ncb );

    if ( uRetCode == 0 )
        return lana_enum.length;
    else
        return 0;

}

//
int getUserName(char *uname)
{
    TCHAR  infoBuf[INFO_BUFFER_SIZE];
    DWORD  bufCharCount = INFO_BUFFER_SIZE;

    if( !GetUserName( infoBuf, &bufCharCount ) )
    {
        showMsg(TEXT("Cannot GetUserName"));
        return 1;
    }

    strcpy(uname, infoBuf);
    return 0;
}

int turnOffMonitor(HWND hwnd)
{
    ::SendMessage(hwnd, WM_SYSCOMMAND, SC_MONITORPOWER,   2);
}

int turnOnMonitor(HWND hwnd)
{
    ::SendMessage(hwnd, WM_SYSCOMMAND, SC_MONITORPOWER,   0);
}

int showScreenSave(HWND hwnd)
{
    ::SendMessage(hwnd, WM_SYSCOMMAND, SC_SCREENSAVE, 0);
}

int showStartMenu(HWND hwnd)
{
    ::SendMessage(hwnd, WM_SYSCOMMAND, SC_TASKLIST, 0);
}


int shutdownWindows()
{
    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;

    // Get a token for this process.
    if (!OpenProcessToken(GetCurrentProcess(),
        TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
        return 1;

    // Get the LUID for the shutdown privilege.
    LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME,
        &tkp.Privileges[0].Luid);

    tkp.PrivilegeCount = 1;  // one privilege to set
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    // Get the shutdown privilege for this process.

    AdjustTokenPrivileges(hToken, FALSE, &tkp, 0,
        (PTOKEN_PRIVILEGES)NULL, 0);

    if (GetLastError() != ERROR_SUCCESS)
        return 1;

    // Shut down the system and force all applications to close.

    if (!ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE,
        SHTDN_REASON_MAJOR_OPERATINGSYSTEM |
        SHTDN_REASON_MINOR_UPGRADE |
        SHTDN_REASON_FLAG_PLANNED))
        return 1;

    return 0;
}

int addIcon2Desktop()
{
    return 0;
}

int addIcon2MyComputer()
{
    return 0;
}


bool setDesktopWallpaper( LPTSTR sFilePath )
{
    return (FALSE != ::SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, sFilePath, SPIF_UPDATEINIFILE));
}

bool removeDesktopWallpaper()
{
    return setDesktopWallpaper(_T(""));
}

/*
* SPI_SETDESKWALLPAPER defined constants
*/
#ifndef SETWALLPAPER_DEFAULT
#define SETWALLPAPER_DEFAULT    ((LPWSTR)-1)
#endif

bool resetDesktopWallpaper()
{
    return setDesktopWallpaper(NULL);
    //return setDesktopWallpaper(SETWALLPAPER_DEFAULT);
}

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}


#endif


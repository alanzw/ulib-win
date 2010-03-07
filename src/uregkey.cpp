#include <windows.h>

#include "uregkey.h"

URegKey::URegKey()
: m_hSubKey(NULL)
{}

URegKey::~URegKey()
{
    this->close();
}

LONG URegKey::open(HKEY hKey, LPCTSTR lpSubKey, PHKEY phkResult)
{
    return ::RegOpenKey(hKey, lpSubKey, phkResult);
}

LONG URegKey::openClassRoot(LPCTSTR lpSubKey)
{
    m_hkey = U_HKCLSRT;
    return open(m_hkey, lpSubKey, &m_hSubKey);
}

LONG URegKey::openCurrentUser( LPCTSTR lpSubKey )
{
    m_hkey = U_HKCUUSR;
    return open(m_hkey, lpSubKey, &m_hSubKey);
}

LONG URegKey::openLocalMachine( LPCTSTR lpSubKey )
{
    m_hkey = U_HKLMACH;
    return open(m_hkey, lpSubKey, &m_hSubKey);
}

LONG URegKey::close()
{
    if (m_hSubKey != NULL)
        return ::RegCloseKey(m_hSubKey);
    else
        return -1;
}

LONG URegKey::openEx( HKEY hKey, LPCTSTR lpSubKey, PHKEY phkResult, AccessRight right )
{
    return ::RegOpenKeyEx(hKey, lpSubKey, 0, right, &m_hSubKey);
}

LONG URegKey::deleteKey( HKEY hKey, LPCTSTR lpSubKey )
{
    return ::RegDeleteKey(hKey, lpSubKey);
}

//LONG URegKey::getValue( DWORD &dwValueType, PVOID &pvData )
//{
//    return ::RegGetValue(m_hSubKey, 0, NULL, NULL, dwValueType, pvData, NULL);
//}

LONG URegKey::queryValue(TCHAR *lpBuffer, LONG &lSize)
{
    return ::RegQueryValue(m_hSubKey, NULL, lpBuffer, &lSize);
}

LONG URegKey::queryValueEx(DWORD &lpType, LPBYTE lpData, DWORD &dwSize)
{
    return ::RegQueryValueEx(m_hSubKey, NULL, NULL, &lpType, lpData, &dwSize);
}

 BOOL URegKey::createEx(HKEY hKey, LPCTSTR lpSubKey, DWORD dwOptions, REGSAM samDesired, PHKEY phkResult)
 {
    return ERROR_SUCCESS == ::RegCreateKeyEx(hKey, lpSubKey, 0, NULL, dwOptions, samDesired, NULL, phkResult, NULL);
 }
 
 BOOL URegKey::setValueEx(HKEY hKey, LPCTSTR lpValueName, DWORD dwType, const BYTE * lpData, DWORD cbData)
 {
    return ERROR_SUCCESS == ::RegSetValueEx(hKey, lpValueName, 0, dwType, lpData, cbData);
 }

 
BOOL URegKey::saveKey(LPCTSTR lpFile, LPSECURITY_ATTRIBUTES lpSecurityAttributes /*= NULL*/)
{
    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;

    // Get a token for this process.
    if (!OpenProcessToken(GetCurrentProcess(),
        TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
        return 1;

    // Get the LUID for the shutdown privilege.
    LookupPrivilegeValue(NULL, SE_BACKUP_NAME,
        &tkp.Privileges[0].Luid);

    tkp.PrivilegeCount = 1;  // one privilege to set
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    // Get the shutdown privilege for this process.

    AdjustTokenPrivileges(hToken, FALSE, &tkp, 0,
        (PTOKEN_PRIVILEGES)NULL, 0);

    if (GetLastError() != ERROR_SUCCESS)
        return 1;

    return ERROR_SUCCESS == ::RegSaveKey(m_hSubKey, lpFile, lpSecurityAttributes);
}
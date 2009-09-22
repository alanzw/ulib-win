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

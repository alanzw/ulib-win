#ifndef U_REGKEY_H
#define U_REGKEY_H

#include "ulib.h"

#define U_HKCLSRT HKEY_CLASSES_ROOT
#define U_HKCUCFG HKEY_CURRENT_CONFIG
#define U_HKCUUSR HKEY_CURRENT_USER
#define U_HKLMACH HKEY_LOCAL_MACHINE
#define U_HKUSERS HKEY_USERS

class ULIB_API URegKey
{
public:
    URegKey();
    ~URegKey();

    enum {
        MAX_KEY_LENGTH = 255,
        MAX_VALUE_NAME = 16383
    };

    typedef enum tagAccessRight {
        ACR_ALL_ACCESS = KEY_ALL_ACCESS,
        ACR_READ       = KEY_READ,
        ACR_WRITE      = KEY_WRITE
    } AccessRight;

    typedef enum tagRegValueType {
        RVT_BINARY = REG_BINARY,
        RVT_DWORD  = REG_DWORD
    } ValueType;

    LONG openClassRoot(LPCTSTR lpSubKey);
    LONG openCurrentUser(LPCTSTR lpSubKey);
    LONG openLocalMachine(LPCTSTR lpSubKey);

    LONG deleteKey(HKEY hKey, LPCTSTR lpSubKey);

    // LONG getValue(DWORD &dwValueType, PVOID &pvData);
    // LONG queryInfo();
    LONG queryValue(TCHAR *lpBuffer, LONG &lSize);
    LONG queryValueEx(DWORD &lpType, LPBYTE lpData, DWORD &dwSize);
    
    //
    BOOL createEx(HKEY hKey, LPCTSTR lpSubKey, DWORD dwOptions, REGSAM samDesired, PHKEY phkResult);
    BOOL setValueEx(HKEY hKey, LPCTSTR lpValueName, DWORD dwType, const BYTE * lpData, DWORD cbData);
protected:

private:
    LONG open(HKEY hKey, LPCTSTR lpSubKey, PHKEY phkResult);

    LONG openEx(HKEY hKey, LPCTSTR lpSubKey, PHKEY phkResult, AccessRight right);

    LONG close();

    HKEY m_hkey;
    HKEY m_hSubKey;
};

#endif // U_REGKEY_H

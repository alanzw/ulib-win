#ifndef U_VTBL_DLL_DB_H
#define U_VTBL_DLL_DB_H

#define DEF_EXPORT __declspec(dllexport)

#define  NO_ERROR 0

class IDB {
    // Interfaces
public:
    // Interface for data access.
    virtual HRESULT Read(short nTable, short nRow, LPWSTR lpszData) =0;
};

class IDBSrvFactory {
    // Interface
public:
    virtual HRESULT CreateDB(IDB** ppObject) =0;
    virtual ULONG  Release() =0;
};

HRESULT DEF_EXPORT DllGetClassFactoryObject(IDBSrvFactory ** ppObject);

class CDB : public IDB {
    // Interfaces
public:
    // Interface for data access.
    HRESULT Read(short nTable, short nRow, LPWSTR lpszData);
};

class CDBSrvFactory : public IDBSrvFactory {
    // Interface
public:
    HRESULT CreateDB(IDB** ppObject);
    ULONG  Release();
};

#endif // U_VTBL_DLL_DB_H

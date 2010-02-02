#ifndef U_VTBL_DLL_DB_H
#define U_VTBL_DLL_DB_H

#include <windows.h>
#include <tchar.h>

#include "adt/uvector.h"
#include "adt/ustring.h"

typedef long HRESULT;
typedef huys::ADT::UStringAnsi TString;
typedef huys::ADT::UVector<TString> CStringArray;
typedef huys::ADT::UVector<CStringArray *> CPtrArray;

#ifdef BUILD_DLL
#define DEF_EXPORT __declspec(dllexport)
#else
#define DEF_EXPORT __declspec(dllimport)
#endif

#ifndef NO_ERROR
#define  NO_ERROR 0
#endif

class IDB {
    // Interfaces
public:
    // Interface for data access.
    virtual HRESULT Read(short nTable, short nRow, LPWSTR lpszData) =0;
    
    virtual HRESULT  Release() = 0;
};

class IDBSrvFactory {
    // Interface
public:
    virtual HRESULT CreateDB(IDB** ppObject) =0;
    virtual HRESULT  Release() =0;
};

HRESULT DEF_EXPORT DllGetClassFactoryObject(IDBSrvFactory ** ppObject);

class CDB : public IDB {
    // Interfaces
public:
    // Interface for data access.
    HRESULT DEF_EXPORT Read(short nTable, short nRow, LPWSTR lpszData);
    
    HRESULT DEF_EXPORT Release();
};

class CDBSrvFactory : public IDBSrvFactory {
    // Interface
public:
    HRESULT DEF_EXPORT CreateDB(IDB** ppObject);
    HRESULT DEF_EXPORT Release();
};

#endif // U_VTBL_DLL_DB_H

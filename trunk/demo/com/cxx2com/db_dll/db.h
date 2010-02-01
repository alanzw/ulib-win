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

class CDB {
    // Interfaces
public:
    // Interface for data access
    HRESULT DEF_EXPORT Read(short nTable, short nRow, LPTSTR lpszData);

    HRESULT DEF_EXPORT Write(short nTable, short nRow, LPCTSTR lpszData);

    // Interface for database management
    HRESULT DEF_EXPORT Create(short &nTable, LPCTSTR lpszName);

    HRESULT DEF_EXPORT Delete(short nTable);

    // Interface for database information
    HRESULT DEF_EXPORT GetNumTables(short &nNumTables);

    HRESULT DEF_EXPORT GetTableName(short nTable, LPTSTR lpszName);

    HRESULT DEF_EXPORT GetNumRows(short nTable, short &nRows);

    ULONG DEF_EXPORT Release(); // Need to free an object from within the DLL.

    // Implementation
private:
    CPtrArray m_arrTables;// Array of pointers to CStringArray (the "database")
    CStringArray m_arrNames; // Array of table names
};

class CDBSrvFactory
{
    // Interface
public:
    HRESULT DEF_EXPORT CreateDB(CDB** ppObject);

    ULONG  DEF_EXPORT Release();
};

HRESULT DEF_EXPORT DllGetClassFactoryObject(CDBSrvFactory ** ppObject);

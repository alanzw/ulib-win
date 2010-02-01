#include "adt/uvector.h"
#include "adt/ustring.h"

typedef long HRESULT;
typedef huys::ADT::UStringAnsi TString;
typedef huys::ADT::UVector<TString> CStringArray;
typedef huys::ADT::UVector<CStringArray *> CPtrArray;

#define DEF_EXPORT __declspec(dllexport)

#define  NO_ERROR 0

class CDB {
    // Interfaces
public:
    // Interface for data access
    HRESULT DEF_EXPORT Read(short nTable, short nRow, LPTSTR lpszData)
    {
        return 0;
    }
   
    HRESULT DEF_EXPORT Write(short nTable, short nRow, LPCTSTR lpszData)
    {
        return 0;
    }
    
    // Interface for database management
    HRESULT Create(short &nTable, LPCTSTR lpszName)
    {
        return 0;
    }
   
    HRESULT Delete(short nTable)
    {
        return 0;
    }
   
    // Interface for database information
    HRESULT GetNumTables(short &nNumTables)
    {
        return 0;
    }
   
    HRESULT GetTableName(short nTable, LPTSTR lpszName)
    {
        return 0;
    }
   
    HRESULT GetNumRows(short nTable, short &nRows)
    {
        return 0;
    }
    
    ULONG DEF_EXPORT Release() // Need to free an object from within the DLL.
    {
        delete this;
        return 0;
    }
    
    // Implementation
private:
    CPtrArray m_arrTables;// Array of pointers to CStringArray (the "database")
    CStringArray m_arrNames; // Array of table names
};

class CDBSrvFactory
{
    // Interface
public:
    HRESULT DEF_EXPORT CreateDB(CDB** ppObject)
    {
        *ppObject=new CDB;
        return NO_ERROR;
    }
    
    ULONG  DEF_EXPORT Release()
    {
        delete this;
        return 0;
    }
};

HRESULT DEF_EXPORT DllGetClassFactoryObject(CDBSrvFactory ** ppObject)
{
    *ppObject=new CDBSrvFactory;
    return NO_ERROR;  
}
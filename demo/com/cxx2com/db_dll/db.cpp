#include "db.h"

HRESULT CDB::Read(short nTable, short nRow, LPTSTR lpszData)
{
    return 0;
}

HRESULT CDB::Write(short nTable, short nRow, LPCTSTR lpszData)
{
    return 0;
}

// Interface for database management
HRESULT CDB::Create(short &nTable, LPCTSTR lpszName)
{
    return 0;
}

HRESULT CDB::Delete(short nTable)
{
    return 0;
}

// Interface for database information
HRESULT CDB::GetNumTables(short &nNumTables)
{
    return 0;
}

HRESULT CDB::GetTableName(short nTable, LPTSTR lpszName)
{
    return 0;
}

HRESULT CDB::GetNumRows(short nTable, short &nRows)
{
    return 0;
}

ULONG CDB::CDB::Release() // Need to free an object from within the DLL.
{
    delete this;
    return 0;
}

HRESULT CDBSrvFactory::CreateDB(CDB** ppObject)
{
    *ppObject=new CDB;
    return NO_ERROR;
}

ULONG CDBSrvFactory::Release()
{
    delete this;
    return 0;
}

HRESULT DllGetClassFactoryObject(CDBSrvFactory ** ppObject)
{
    *ppObject=new CDBSrvFactory;
    return NO_ERROR;  
}

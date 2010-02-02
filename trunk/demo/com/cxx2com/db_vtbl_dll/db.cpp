#include "db.h"

HRESULT CDBSrvFactory::CreateDB(IDB** ppvDBObject)
{
    *ppvDBObject=(IDB*) new CDB;
    return NO_ERROR;
}

HRESULT CDBSrvFactory::Release()
{
    return NO_ERROR;
}

HRESULT DllGetClassFactoryObject(IDBSrvFactory ** ppObject)
{
   *ppObject=(IDBSrvFactory*) new CDBSrvFactory;
   return NO_ERROR;
}



HRESULT CDB::Read(short nTable, short nRow, LPWSTR lpszData)
{
    return NO_ERROR;
}

HRESULT CDB::Release()
{
    return 0;
}

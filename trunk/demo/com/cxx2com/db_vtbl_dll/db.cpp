#include <stdio.h>

#include "dbimpl.h"

HRESULT CDBSrvFactory::CreateDB(IDB** ppvDBObject)
{
    *ppvDBObject=(IDB*) new CDB;

    printf("CDBSrvFactory::CreateDB!!\n");

    return NO_ERROR;
}

HRESULT CDBSrvFactory::Release()
{
    printf("CDBSrvFactory::Release!!\n");
    return NO_ERROR;
}

HRESULT DllGetClassFactoryObject(IDBSrvFactory ** ppObject)
{
   *ppObject=(IDBSrvFactory*) new CDBSrvFactory;

   printf("DllGetClassFactoryObject!!\n");
   return NO_ERROR;
}

HRESULT CDB::Read(short nTable, short nRow, LPWSTR lpszData)
{
    printf("CDB::Read\n");
    return NO_ERROR;
}

HRESULT CDB::Release()
{
    printf("CDB::Release\n");
    return 0;
}

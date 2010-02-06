#include "db.h"

class CDB : public IDB
{
    // Interfaces
public:
    // Interface for data access.
    HRESULT  Read(short nTable, short nRow, LPWSTR lpszData);

    HRESULT  Release();
};

class CDBSrvFactory : public IDBSrvFactory
{
    // Interface
public:
    HRESULT  CreateDB(IDB** ppObject);
    HRESULT  Release();
};

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppObject) {
    if (rclsid!=CLSID_DBSAMPLE) { // Is this the number of our class?
        return CLASS_E_CLASSNOTAVAILABLE;
    }
    if (riid!=IID_IDBSrvFactory) { // Is this the number of our interface?
        return E_INVALIDARG;
    }
    *ppObject=(IDBSrvFactory*) new CDBSrvFactory;
    return NO_ERROR;
}

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

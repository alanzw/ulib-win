#include "db.h"

#define  NO_ERROR 0

HRESULT CDBSrvFactory::CreateDB(IDB** ppvDBObject) {
 *ppvDBObject=(IDB*) new CDB;
 return NO_ERROR;
}

HRESULT DllGetClassFactoryObject(IDBSrvFactory ** ppObject) {
   *ppObject=(IDBSrvFactory*) new CDBSrvFactory;
   return NO_ERROR;
}


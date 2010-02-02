#ifndef U_DB_IMPL_H
#define U_DB_IMPL_H

#include "db.h"

class CDB : public IDB {
    // Interfaces
public:
    // Interface for data access.
    HRESULT  Read(short nTable, short nRow, LPWSTR lpszData);
    
    HRESULT  Release();
};

class CDBSrvFactory : public IDBSrvFactory {
    // Interface
public:
    HRESULT  CreateDB(IDB** ppObject);
    HRESULT  Release();
};

#endif // U_DB_IMPL_H

#include "db.h"

HRESULT CreateDB(IDB** ppObj)
{
    *ppObj=(IDB*) new CDB; // Cast to abstract base class.
    return NO_ERROR;
}

HRESULT CDB::Read(short nTable, short nRow, LPTSTR lpszData)
{
    return NO_ERROR;
}

HRESULT CDB::Write(short nTable, short nRow, LPCTSTR lpszData)
{
    return NO_ERROR;
}

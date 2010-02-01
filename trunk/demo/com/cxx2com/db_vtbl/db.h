#ifndef U_INTERFACE_DB_H
#define U_INTERFACE_DB_H

#include <windows.h>
#include <tchar.h>

typedef long HRESULT;

#ifndef NO_ERROR
#define  NO_ERROR 0
#endif

class IDB {
    // Interfaces
public:
    // Interface for data access
    virtual HRESULT Read(short nTable, short nRow, LPTSTR lpszData) =0;
    virtual HRESULT Write(short nTable, short nRow, LPCTSTR lpszData) =0;

};

HRESULT CreateDB(IDB** ppObj);

typedef long HRESULT;

class CDB : public IDB {
    // Interfaces
public:
    // Interface for data access
    HRESULT Read(short nTable, short nRow, LPTSTR lpszData);
    HRESULT Write(short nTable, short nRow, LPCTSTR lpszData);
};

#endif // U_INTERFACE_DB_H

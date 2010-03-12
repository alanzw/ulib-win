#ifndef U_ODBC_H
#define U_ODBC_H

#include "ulib.h"

namespace UODBC
{

class ULIB_API DataBase
{
public:
    DataBase();
    ~DataBase();

    SQLRETURN init();

    SQLRETURN connect(char *dsnName, char *userId, char *passwd, int nTimeout = 5);
    SQLRETURN connect(const char *filename, int nTimeout = 5);
    SQLRETURN connectExcel(const char *filename, int nTimeout = 5);
    void disconnect();
    bool exec(const char *stmt);
    bool getData(int col, char *buf, int nBufsize, SQLINTEGER *cbData);
    //
    SQLLEN getRows();
    //
    SQLRETURN fetchScroll(SQLSMALLINT FetchOrientation, SQLLEN FetchOffset);
    //
    SQLRETURN describeCol( SQLSMALLINT ColumnNumber,
                           SQLCHAR *ColumnName,
                           SQLSMALLINT BufferLength,
                           SQLSMALLINT *  NameLengthPtr,
                           SQLSMALLINT *  DataTypePtr,
                           SQLULEN *      ColumnSizePtr,
                           SQLSMALLINT *  DecimalDigitsPtr,
                           SQLSMALLINT *  NullablePtr);
    //
    SQLSMALLINT getResultCols();

    //
    SQLRETURN closeCursor();

    //
    SQLRETURN setLoginTimeout(int nSec);

    //
    void extract_error(char *fn);
private:
    HENV _hEnv;
    HDBC _hConn;
    HSTMT _hStmt;
};

}; // namespace UODBC

#endif // U_ODBC_H

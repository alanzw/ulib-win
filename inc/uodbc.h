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
    SQLRETURN connect(char *dsnName, char *userId, char *passwd);
    SQLRETURN connect(const char *filename);
    void disconnect();
    bool exec(const char *stmt);
    bool getdata(char *buf, SQLINTEGER *cbData);
    SQLLEN getRows();
    
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

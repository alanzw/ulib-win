#include <windows.h>
#include <tchar.h>

#include <cstdio>

#include <sql.h>
#include <sqltypes.h>
#include <sqlext.h>

#include "uodbc.h"

namespace UODBC
{

DataBase::DataBase()
{

}

DataBase::~DataBase()
{

}

void DataBase::disconnect()
{
    SQLFreeStmt(_hStmt, SQL_DROP);
    SQLDisconnect(_hConn);
    SQLFreeConnect(_hConn);
    SQLFreeEnv(_hEnv);
}

bool DataBase::exec( const char *stmt )
{
    RETCODE ret = SQLExecDirect(_hStmt, (SQLCHAR*)stmt, SQL_NTS);
    return (ret == SQL_SUCCESS) || (ret == SQL_SUCCESS_WITH_INFO);
}

bool DataBase::getdata( char *buf, SQLINTEGER *cbData )
{
    while(SQLFetch(_hStmt) == SQL_SUCCESS)
    {
        SQLGetData(_hStmt, 0, SQL_C_CHAR, buf, 256, cbData);
    }
}

SQLLEN DataBase::getRows()
{
    SQLLEN nRows = -1;
    SQLRowCount(_hStmt, &nRows);
    return nRows;
}

SQLRETURN DataBase::connect(char *dsnName, char *userId, char *passwd)
{
    /* Step1: Connect
     *    SQLAllocHandle(ENV)
     *    SetEnvAttr
     *    SQLAllocHandle(DBC)
     *    SQLConnect
     *    SQLSetConnectAttr
     */
     SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &_hEnv);
     SQLSetEnvAttr(_hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
     SQLAllocHandle(SQL_HANDLE_DBC, _hEnv, &_hConn);
     SQLRETURN sRet = SQLConnect(_hConn, (SQLCHAR*)dsnName, SQL_NTS, (SQLCHAR*)userId, SQL_NTS, (SQLCHAR*)passwd, SQL_NTS);

     /* Step2: Initialize
      *    SQLGetInfo
      *    SQLAllocHandle(STMT)
      *    SQLSetStmtAttr
      */

    SQLAllocHandle(SQL_HANDLE_STMT, _hConn, &_hStmt);
      
     /* Step 3: Execute
      *    Catalog function
      *    SQLBindParameter
      *    SQLExecDirect
      */

     /* Step 4a: Fetch Result
      *
      */

     /* Step 4b: Fetch Row Count
      */

     /* Step 5: Commit
      */

     /* Step 6: Disconnect
      */
     return sRet;
}

void DataBase::extract_error(char *fn)
{
    SQLINTEGER i = 0;
    SQLINTEGER native;
    SQLCHAR state[7];
    SQLCHAR text[256];
    SQLSMALLINT len;
    SQLRETURN ret;

    fprintf(stderr, "The driver reported the following diagnostics while running  %s\n", fn);

    do
    {
        ret = SQLGetDiagRec( SQL_HANDLE_DBC, _hConn, ++i, state, &native, text, sizeof(text), &len );
        if (SQL_SUCCEEDED(ret))
        {
            fprintf(stderr, "%s : %d : %d : %s\n" , state, i, native, text);
        }
    }
    while( ret == SQL_SUCCESS ); 
}

}

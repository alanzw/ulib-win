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
: _hEnv(0), _hConn(0), _hStmt(0)
{

}

DataBase::~DataBase()
{

}

SQLRETURN DataBase::init()
{
    SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &_hEnv);
    ret = SQLSetEnvAttr(_hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);

    return ret;
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
    if (0 == _hStmt)
    {
        SQLAllocHandle(SQL_HANDLE_STMT, _hConn, &_hStmt);
        SQLSetStmtAttr(_hStmt, SQL_ATTR_CURSOR_TYPE, (SQLPOINTER) SQL_CURSOR_STATIC, 0);
    }

    RETCODE ret = SQLExecDirect(_hStmt, (SQLCHAR*)stmt, SQL_NTS);
    return (ret == SQL_SUCCESS) || (ret == SQL_SUCCESS_WITH_INFO);
}

bool DataBase::getData( int nCol, char *szBuf, int nBufSize, SQLINTEGER *cbData )
{
    //while(SQLFetch(_hStmt) == SQL_SUCCESS)
    //{
        SQLGetData(_hStmt, nCol, SQL_C_CHAR, szBuf, nBufSize, cbData);
    //}

    return true;
}

SQLLEN DataBase::getRows()
{
    SQLLEN nRows = 0;
    SQLRETURN status;
    while (true)                        // loop forever
    {
        status = SQLFetch(_hStmt);            // fetch a row of data
        if (status == SQL_NO_DATA_FOUND) break;        // if No Data was returned then exit the loop

        nRows++;                    // add 1 to the row counter
    }

    return nRows;
}

SQLRETURN DataBase::fetchScroll(SQLSMALLINT FetchOrientation, SQLLEN FetchOffset)
{
    return SQLFetchScroll(_hStmt, FetchOrientation, FetchOffset);
}

SQLSMALLINT DataBase::getResultCols()
{
    SQLSMALLINT cols = -1;
    SQLNumResultCols(_hStmt, &cols);
    return cols;
}

SQLRETURN DataBase::describeCol(
    SQLSMALLINT ColumnNumber,
    SQLCHAR *ColumnName,
    SQLSMALLINT BufferLength,
    SQLSMALLINT *NameLengthPtr,
    SQLSMALLINT *DataTypePtr,
    SQLULEN *ColumnSizePtr,
    SQLSMALLINT *DecimalDigitsPtr,
    SQLSMALLINT *NullablePtr)
{
    return SQLDescribeCol(_hStmt, ColumnNumber, ColumnName, BufferLength, NameLengthPtr, DataTypePtr, ColumnSizePtr, DecimalDigitsPtr, NullablePtr);
}

SQLRETURN DataBase::connect(char *dsnName, char *userId, char *passwd, int nTimeout)
{
    /* Step1: Connect
     *    SQLAllocHandle(ENV)
     *    SetEnvAttr
     *    SQLAllocHandle(DBC)
     *    SQLConnect
     *    SQLSetConnectAttr
     */
     //SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &_hEnv);
     //SQLSetEnvAttr(_hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
     SQLAllocHandle(SQL_HANDLE_DBC, _hEnv, &_hConn);
     SQLRETURN sRet = SQLConnect(_hConn, (SQLCHAR*)dsnName, SQL_NTS, (SQLCHAR*)userId, SQL_NTS, (SQLCHAR*)passwd, SQL_NTS);

     /* Step2: Initialize
      *    SQLGetInfo
      *    SQLAllocHandle(STMT)
      *    SQLSetStmtAttr
      */

    //SQLAllocHandle(SQL_HANDLE_STMT, _hConn, &_hStmt);

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

#define MAX_CONNECT_LEN  512        // Max size of Connect string
SQLRETURN DataBase::connect(const char *filename, int nTimeout)
{
    SQLRETURN status;

    //SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &_hEnv);
    //SQLSetEnvAttr(_hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
    SQLAllocHandle(SQL_HANDLE_DBC, _hEnv, &_hConn);

    setLoginTimeout(nTimeout);

    char tmpStr[1024];
    sprintf(tmpStr, "Driver={Microsoft Access Driver (*.mdb)};DBQ=%s", filename);
    char maxStr[MAX_CONNECT_LEN];
    SQLSMALLINT returnSize;
    status = SQLDriverConnect(_hConn, NULL, (SQLCHAR *)tmpStr, strlen(tmpStr), (SQLCHAR *)maxStr, sizeof(maxStr), &returnSize, SQL_DRIVER_NOPROMPT );

    return status;
}

SQLRETURN DataBase::connectExcel(const char *filename, int nTimeout)
{
    SQLRETURN status;

    //SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &_hEnv);
    //SQLSetEnvAttr(_hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
    SQLAllocHandle(SQL_HANDLE_DBC, _hEnv, &_hConn);

    setLoginTimeout(nTimeout);

    char tmpStr[1024];
    sprintf(tmpStr, "Driver={Microsoft Excel Driver (*.xls)}; DBQ=%s; DriverID=790", filename);
    char maxStr[MAX_CONNECT_LEN];
    SQLSMALLINT returnSize;
    status = SQLDriverConnect(_hConn, NULL, (SQLCHAR *)tmpStr, strlen(tmpStr), (SQLCHAR *)maxStr, sizeof(maxStr), &returnSize, SQL_DRIVER_NOPROMPT );

    return status;
}

SQLRETURN DataBase::setLoginTimeout(int nSec)
{
    return ::SQLSetConnectAttr(_hConn, SQL_LOGIN_TIMEOUT,(void *) nSec, 0);
}

SQLRETURN DataBase::closeCursor()
{
    return SQLCloseCursor(_hStmt);
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

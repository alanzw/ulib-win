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

bool DataBase::getdata( char *buf, int *cbData )
{
	while(SQLFetch(_hStmt) == SQL_SUCCEEDED)
	{
		SQLGetData(_hStmt, 0, DT_STRING, buf, 256, cbData);
	}	
}

SQLRETURN DataBase::connect(char *dsnName, char *userId, char *passwd);
{
	/* Step1: Connect
	 *    SQLAllocHandle(ENV)
	 *    SetEnvAttr
	 *    SQLAllocHandle(DBC)
	 *    SQLConnect
	 *    SQLSetConnectAttr
	 */
	 SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &_hEnv);
	 SQLSetEnvAttr(_hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC30);
	 SQLAllocHandle(SQL_HANDLE_DBC, _hEnv, &_hConn);
	 SQLRETURN sRet = SQLConnect(_hConn, (SQLCHAR*)dsnName, SQL_NTS, (SQLCHAR*)userId, SQL_NTS, (SQLCHAR*)passwd, SQL_NTS);

	 /* Step2: Initialize
	  *    SQLGetInfo
	  *    SQLAllocHandle(STMT)
	  *    SQLSetStmtAttr
	  */

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

}

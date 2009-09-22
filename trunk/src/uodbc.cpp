#include <sql.h>
#include <sqltypes.h>
#include <sqlext.h>

#include "uodbc.h"

SQLHandle hdlEnv, hdlConn, hdlStmt, hdlDbc
char* stmt = "SELECT * from NutHead"; //SQL statement? NutHead is the Table name

//for example
char *dsnName = "COLLECTOR"
char* userID = "eXceed";
char* passwd = "hole";
char* retVal[256];
unsigned int cbData;

SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hdlEnv);
SQLSetEnvAttr(hdlEnv,SQL_ATTR_ODBC_VERSION,(void*)SQL_OV_ODBC30);
SQLAllocHandle(SQL_HANDLE_DBC, hdlEnv, &hdlConn);
SQLConnect(hdlConn, (SQLCHAR*)dsnName,SQL_NTS,(SQLCHAR*)userID,SQL_NTS, (SQLCHAR*)passwd, SQL_NTS);
SQLAllocHandle(SQL_HANDLE_STMT, hdlDbc, &hdlStmt);
SQLExecDirect(hdlStmt, (SQLCHAR*)stmt, SQL_NTS);


//Initialize the database connection

while(SQLFetch(hdlStmt) == SQL_SUCCEEDED)
{
  SQLGetData(hdlStmt,0,DT_STRING,retVal,256,&cbData);
  std::cout << retVal << std::endl;
}
SQLFreeHandle(SQL_HANDLE_STMT, hdlStmt);
SQLFreeHandle(SQL_HANDLE_DBC, hdlConn);
SQLFreeHandle(SQL_HANDLE_ENV, hdlEnv);  //End the connection


#include "resource.h"

#include <windows.h>
#include <commctrl.h>
#include <tchar.h>

#include "ulib.h"
#include "udialogx.h"
#include "udlgapp.h"
#include "uprogressbar.h"
#include "ubutton.h"
#include "umsg.h"
#include "colors.h"

#include <sql.h>
#include <sqltypes.h>
#include <sqlext.h>

#include "uodbc.h"


void connect()
{
	SQLHANDLE hEnv;			// ODBC Environment handle
	SQLHANDLE hDbc;			// ODBC Connection handle
	SQLHANDLE hStmt;		// ODBC Statement handle

	SQLRETURN status;		// SQL return variable to test success or failure

	// 1.	Create Environment Handle
	status = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);

	// 2.	Set the ODBC Version we are using to 3.0
	status = SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (void *) SQL_OV_ODBC3, 0);

	// 3.	Create a Connection Handle.
	status = SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);

	// 4.	Set Connection Login Timeout to 5 seconds.
	status = SQLSetConnectAttr(hDbc, SQL_LOGIN_TIMEOUT,(void *) 5, 0);

	// 5.	Open a Connection To Datasource (DSN
	//status =   	SQLConnect(hDbc,(SQLCHAR *) "Northwind", SQL_NTS,
	//				(SQLCHAR *) "Admin", SQL_NTS,
	//				(SQLCHAR *) "", SQL_NTS);		// connect to DSN
#define MAX_CONNECT_LEN  512        // Max size of Connect string
    char tmpStr[] = "Driver={Microsoft Access Driver (*.mdb)};DBQ=.\\db1.mdb";
    char maxStr[MAX_CONNECT_LEN];
    SQLSMALLINT returnSize;
    status = SQLDriverConnect(hDbc, NULL, (SQLCHAR *)tmpStr, strlen(tmpStr), (SQLCHAR *)maxStr, sizeof(maxStr), &returnSize, SQL_DRIVER_NOPROMPT );


	// 6.	Create SQL Statement Handle.
	status = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

	// 7.	Set Cursor Type to Static.
	status = SQLSetStmtAttr(hStmt, SQL_ATTR_CURSOR_TYPE, (SQLPOINTER) SQL_CURSOR_STATIC, 0);

	// 8.	Execute a SQL Statement. Here we get the employee records from Northwind database.
	status = SQLExecDirect(hStmt,(SQLCHAR *) "Select * from User", SQL_NTS);

	// 9.	Do whatever you will with the returned data.  First, check to see that something
	//		was returned.

	// count the number of rows in the returned data
	int rowcnt;						// variable to hold row count

	rowcnt = 0;						// initialize row counter

	while (true)						// loop forever
	{
		status = SQLFetch(hStmt);			// fetch a row of data
		if (status == SQL_NO_DATA_FOUND) break;		// if No Data was returned then exit the loop

		rowcnt++;					// add 1 to the row counter
	}

	showMsgFormat("db", "There were %d rows of data returned by the SQL Statement.\r\n", rowcnt);

	// Now, reset the cursor to the first row of data

	status = SQLFetchScroll(hStmt,SQL_FETCH_FIRST,0);

	if (rowcnt > 0) {
		SQLSMALLINT numcols;				// variable to store number of SQL Columns

		status = SQLNumResultCols(hStmt, &numcols);	// retrieve number of SQL Columns

		if (status == SQL_SUCCESS || status == SQL_SUCCESS_WITH_INFO) {
			showMsgFormat("db", "Number of returned Columns is: %d. \r\n",numcols);
		}
		else {
			showMsgFormat("db", "Error returning number of Columns...\r\n");
		}

		// Describe the Name of a Column in the returned data

		# define SQL_MAX_NAME_SIZE 128

		SQLSMALLINT iCol;				// variable to hold column number to Describe

		iCol = 3;					// Column Number to Describe
	
		SQLCHAR namebuffer[SQL_MAX_NAME_SIZE + 1];	// buffer to receive column name

		SQLSMALLINT buffersize;				// returns actual size of column name returned

		SQLSMALLINT typeOfData;				// returns type of data in the column

		SQLUINTEGER columnsize;				// returns size of the column 

		SQLSMALLINT decimaldigits;			// returns number of decimal digits for the column

		SQLSMALLINT nullable;				// returns - does column allow NULLs? 1 = yes, 0 = No

		memset(namebuffer, 0, SQL_MAX_NAME_SIZE + 1);	// initialize name buffer

		status = SQLDescribeCol(hStmt, iCol, namebuffer, SQL_MAX_NAME_SIZE, &buffersize, &typeOfData, &columnsize, 
					&decimaldigits, &nullable);	// Get Description of Column

		showMsgFormat("db", "Column name of column number %d is %s.\r\n",iCol, namebuffer);


		// Last, but not least, return the Column Data from the column number iCol shown above.

		int icol = iCol;			// same column number as above
		long nchars;				// number of characters returned

		char buffer[512];			// 512 character buffer

		memset(buffer,0,sizeof(buffer));	// initialize buffer

		// Get the Column Data and convert it to Character.
		status = SQLGetData(hStmt, icol, SQL_C_CHAR, buffer, sizeof(buffer) - 1, &nchars);

		showMsgFormat("db", "Data from Column %d is '%s'.\r\n",icol, buffer);
	}

	// 10.	Close the Cursor if one was created...
	status = SQLCloseCursor(hStmt);

	if (status == SQL_SUCCESS || status == SQL_SUCCESS_WITH_INFO) {
	}
	else {
		showMsgFormat("db", "Error with SQLCloseCursor...\r\n");
	}

	// 11.	Release the SQL Statement Handle.
	status = SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

	// 12.	Close the Connection
	status = SQLDisconnect(hDbc);

	// 13.  Release the Connection Handle
	status = SQLFreeHandle(SQL_HANDLE_DBC, hDbc);

	// 14.  Release the Environment Handle
	status = SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
}

using huys::UDialogBox;

class UDialogExt : public UDialogBox
{
    enum {
        ID_BN_OK = 1123
    };
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
    : UDialogBox(hInst, nID) {}

    ~UDialogExt()
    {
        CHECK_PTR(m_pUBtnOK);
    }
    
    virtual BOOL onInit()
    {
        m_pUBtnOK = new UPushButton(m_hDlg, ID_BN_OK, m_hInst);
        m_pUBtnOK->setPos(100, 100, 100, 100);
        m_pUBtnOK->create();
        m_pUBtnOK->setWindowText(_T("View"));
        
        return TRUE;
    }
    
    virtual BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch (LOWORD (wParam))
        {
            case ID_BN_OK:
                {
                    return onBnOK();
                }
            default:
                return UDialogBox::onCommand(wParam, lParam);
        }
    }
private:
    UPushButton *m_pUBtnOK;
    
    BOOL onBnOK()
    {
        //UODBC::DataBase db;
        //db.connect("db1.mdb");
        //db.exec("SELECT * FROM Users");
        //SQLLEN nRow = db.getRows();
        //db.disconnect();
        //showMsgFormat("db", "%d in Database!", nRow);
        
        connect();
        return TRUE;
    }
};

UDLGAPP_T(UDialogExt, IDD_TEST);


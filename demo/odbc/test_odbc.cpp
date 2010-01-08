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

void myconnect()
{
	SQLRETURN status;		// SQL return variable to test success or failure

    UODBC::DataBase db;

    status = db.init();
    status = db.connect("db1.mdb");
    db.exec("Select * from User");

	// count the number of rows in the returned data
	int rowcnt = 0;						// initialize row counter
    rowcnt = db.getRows();
    showMsgFormat("db", "There were %d rows of data returned by the SQL Statement.\r\n", rowcnt);

	// Now, reset the cursor to the first row of data

	//status = SQLFetchScroll(hStmt,SQL_FETCH_FIRST,0);
    status = db.fetchScroll(SQL_FETCH_FIRST, 0);

	if (rowcnt > 0) {
		SQLSMALLINT numcols;				// variable to store number of SQL Columns

		//status = SQLNumResultCols(hStmt, &numcols);	// retrieve number of SQL Columns
        numcols = db.getResultCols();
		showMsgFormat("db", "Number of returned Columns is: %d. \r\n",numcols);

		// Describe the Name of a Column in the returned data

		# define SQL_MAX_NAME_SIZE 128

		SQLSMALLINT iCol;				// variable to hold column number to Describe

		iCol = 1;					// Column Number to Describe
	
		SQLCHAR namebuffer[SQL_MAX_NAME_SIZE + 1];	// buffer to receive column name

		SQLSMALLINT buffersize;				// returns actual size of column name returned

		SQLSMALLINT typeOfData;				// returns type of data in the column

		SQLUINTEGER columnsize;				// returns size of the column 

		SQLSMALLINT decimaldigits;			// returns number of decimal digits for the column

		SQLSMALLINT nullable;				// returns - does column allow NULLs? 1 = yes, 0 = No

		memset(namebuffer, 0, SQL_MAX_NAME_SIZE + 1);	// initialize name buffer

		//status = SQLDescribeCol(hStmt, iCol, namebuffer, SQL_MAX_NAME_SIZE, &buffersize, &typeOfData, &columnsize, 
		//			&decimaldigits, &nullable);	// Get Description of Column
		status = db.describeCol(iCol, namebuffer, SQL_MAX_NAME_SIZE, &buffersize, &typeOfData, &columnsize, 
					&decimaldigits, &nullable);	// Get Description of Column
		showMsgFormat("db", "Column name of column number %d is %s.\r\n",iCol, namebuffer);


		// Last, but not least, return the Column Data from the column number iCol shown above.

		int icol = iCol;			// same column number as above
		long nchars;				// number of characters returned

		char buffer[512];			// 512 character buffer

		memset(buffer, 0, sizeof(buffer));	// initialize buffer

		// Get the Column Data and convert it to Character.
		//status = SQLGetData(hStmt, icol, SQL_C_CHAR, buffer, sizeof(buffer) - 1, &nchars);
        db.getData(iCol, buffer, sizeof(buffer) - 1, &nchars);

		showMsgFormat("db", "Data from Column %d is '%s'.\r\n",icol, buffer);
	}

    status = db.closeCursor();
	if (status == SQL_SUCCESS || status == SQL_SUCCESS_WITH_INFO) {
	}
	else {
		showMsgFormat("db", "Error with SQLCloseCursor...\r\n");
	}

    db.disconnect();
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
        myconnect();
        return TRUE;
    }
};

UDLGAPP_T(UDialogExt, IDD_TEST);


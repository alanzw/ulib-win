// RADatabase.h: interface for the CRADatabase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RADATABASE_H__1ED0D67C_5A19_4695_9E6D_3E51D86E5AC9__INCLUDED_)
#define AFX_RADATABASE_H__1ED0D67C_5A19_4695_9E6D_3E51D86E5AC9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <comdef.h>
#include "msado15.tlh"

class CRADatabase  
{
public:
	HRESULT RAConnect(const char *ConnectionString);
	HRESULT RAExecuteNoRs(const char *SqlStatement);
	_RecordsetPtr RAExecuteRs(const char *SqlStatement);
	_RecordsetPtr RAGetDisconnectedRs(const char *SqlStatement);
	CRADatabase();
	virtual ~CRADatabase();

private:
	char m_ConnectionString[MAX_PATH];
	_CommandPtr m_Command;
	_RecordsetPtr m_Recordset;
	_ConnectionPtr m_Connection;
};

#endif // !defined(AFX_RADATABASE_H__1ED0D67C_5A19_4695_9E6D_3E51D86E5AC9__INCLUDED_)

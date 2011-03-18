// RADatabase.cpp: implementation of the CRADatabase class.
//
//////////////////////////////////////////////////////////////////////
#include "RADatabase.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRADatabase::CRADatabase()
{
	try
	{
		// Initialize COM
		CoInitialize(NULL);
		// Initialize ADO
		m_Command.CreateInstance(__uuidof(Command));
		m_Recordset.CreateInstance(__uuidof(Recordset));
		m_Connection.CreateInstance(__uuidof(Connection));	
	}
	catch(_com_error &e)
	{
		printf("Description = %s\n", (char*) e.Description());   
	}
	catch(...)
	{
	}
}

CRADatabase::~CRADatabase()
{
	// Clean up ADO objects
	try
	{
		if (m_Connection)
		{
			if(m_Connection->State == adStateOpen)
			{
				m_Connection->Close();
				m_Connection.Release();			
			}
		}
		if (m_Recordset)
		{
			if(m_Recordset->State == adStateOpen)
			{
				m_Recordset->Close();
				m_Recordset.Release();			
			}
		}		

		m_Connection = NULL;
		m_Command = NULL;
		m_Recordset = NULL;

		CoUninitialize();
	}
	catch(_com_error &e)
	{
		printf("Description = %s\n", (char*) e.Description());   
	}
	catch(...)
	{
	}
}

HRESULT CRADatabase::RAConnect(const char *ConnectionString)
{
	// Attemp a connect, reconnect, or leave if already connected
	if (strlen(ConnectionString) > 0)
	{
		try
		{
			if (m_Connection == NULL)
				m_Connection.CreateInstance(__uuidof(Connection));
			if (0 == _stricmp(ConnectionString, m_ConnectionString) && 
				m_Connection->State == adStateOpen)
				return S_OK;
			else
			{
				if (m_Connection->State == adStateOpen)
					m_Connection->Close();
				sprintf(m_ConnectionString, ConnectionString);
			}
			// Perform the connect
			if (S_OK == m_Connection->Open((char*)m_ConnectionString, L"", L"", adModeUnknown))
				return S_OK;
		}
		catch(_com_error &e)
		{
			printf("Description = %s\n", (char*) e.Description());   
		}
		catch(...)
		{
		}
	}
	return E_FAIL;
}

_RecordsetPtr CRADatabase::RAGetDisconnectedRs(const char *SqlStatement)
{
	// Attempt get a disconnected RS and terminate the connection to the DB
	if (strlen(SqlStatement) > 0)
	{		
		try
		{
			if (m_Connection == NULL || m_Connection->State == adStateClosed)
				RAConnect(m_ConnectionString);

			if (m_Recordset)
			{
				if (m_Recordset->State == adStateOpen)
					m_Recordset->Close();
			}
			else
			{				
				m_Recordset.CreateInstance(__uuidof(Recordset));
			}

			m_Recordset->CursorLocation = adUseClient;
			
			m_Recordset->Open(SqlStatement,
				m_Connection.GetInterfacePtr(),
				adOpenForwardOnly, 
				adLockBatchOptimistic, 
				-1);

			m_Recordset->PutRefActiveConnection(NULL); 
			
			if(m_Connection->GetState() == adStateOpen)
				m_Connection->Close();
			//m_Connection.Release();
			//m_Connection = NULL;		
						
			return m_Recordset.Detach();			
		}
		catch(_com_error &e)
		{
			printf("Description = %s\n", (char*) e.Description());   
		}
		catch(...)
		{
		}
	}
	return NULL;
}


HRESULT CRADatabase::RAExecuteNoRs(const char *SqlStatement)
{
	// Attempt to execute a SQL statement and leave connection open
	if (strlen(SqlStatement) > 0)
	{		
		try
		{
			if (m_Connection == NULL || m_Connection->State == adStateClosed)
				RAConnect(m_ConnectionString);

			m_Connection->Execute(SqlStatement, NULL, adExecuteNoRecords );
			
			return S_OK;
		}
		catch(_com_error &e)
		{
			printf("Description = %s\n", (char*) e.Description());   
		}
		catch(...)
		{
		}
	}
	return E_FAIL;
}

_RecordsetPtr CRADatabase::RAExecuteRs(const char *SqlStatement)
{
	// Attempt to execute a SQL statement, return the RS, and leave connection open
	if (strlen(SqlStatement) > 0)
	{		
		try
		{
			if (m_Connection == NULL || m_Connection->State == adStateClosed)
				RAConnect(m_ConnectionString);
			if (m_Recordset)
			{
				if (m_Recordset->State == adStateOpen)
					m_Recordset->Close();
			}
			else
			{				
				m_Recordset.CreateInstance(__uuidof(Recordset));
			}
			
			m_Command->ActiveConnection = m_Connection;
			m_Command->CommandType = adCmdText;
			m_Command->CommandText = SqlStatement;
			
			_variant_t vNull;
			vNull.vt = VT_ERROR;
			vNull.scode = DISP_E_PARAMNOTFOUND;
			
			m_Recordset = m_Command->Execute( &vNull, &vNull, adCmdText );
		
			
			return m_Recordset.Detach();
		}
		catch(_com_error &e)
		{
			printf("Description = %s\n", (char*) e.Description());   
		}
		catch(...)
		{
		}
	}

	return NULL;
}

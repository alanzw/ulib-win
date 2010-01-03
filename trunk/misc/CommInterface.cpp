/************************************************************************************
 * �ļ���: CommInterface.cpp
 * ��Ȩ: Copyright (C) DavidHowe
 * ����: David Howe
 * E-mail: davidhowe@sina.com
 * Mobile Phone: ***********
 * ����ʱ��: 2003-4-23 10:44:41 AM;  �ص�: Xi'an
 * �ļ�˵��: ���������, ʹ�ø��ֳ���ͨ�ŷ�ʽ��ʹ��ʱ�ﵽͳһ.
 * �޸ļ�¼: -------------- +: ����  o: �޸�  -: ɾ�� --------------
       2003-4-23: + �����ļ�.
	   2003-4-30: + ��������˴��ڷ�ʽ�Ĵ���(m_nCommMode=0).
	   2003-5-2:  + ���������TCP��ʽ�Ĵ���(m_nCommMode=1,2).
       2003-6-?:  o �����˶�WSAEISCONN���ж�.
       2003-7-?:  o �����˷����߳�����ʱ����ٵ�bug.
       2003-11-20: o ������һ�����ܻ�ʹ����ѻ���bug.
       2003-1-7:  0 �޸�E1��ʽ��ʱ��ʱ϶������, ����ʱ϶����-1
       2004-3-31: �޸����¼�m_hEvtCanGetCmdToSend�Ĵ�����ʽ, ��Ϊ�Զ�Reset.
                  ���ڼ����ط�ɾ���˲���Ҫ��ResetEvent()
       2004-4-2:  �޸����ж�TCP/IP��ʽ�·�������ʧ�ܵĴ���.
       2004-6-21: ʹ��OnSend()���ж��Ƿ��ܹ�������������.
       2004-8-11:
               ��Ӷ�::PostThreadMessage()�ķ����жϴ���, ��������Dll������
                ʹ��CCommInterfaceʱ�����������������̵߳�bug.
                {F598F388-14D8-464F-BE35-2CA67E1A978C}
       2004-8-12: ��Ӻ��ж�, ʹ��������E1�Ĵ���.
       2005-6-14: ΪCloseInterface()�������Ĭ�ϲ���, �����Ƿ���Ҫ���ûص�.
 ************************************************************************************/


#include "stdafx.h"
#include "CommInterface.h"

// D.H 2004-8-12 14:27:36
// Plz note!!
// If you wanna use E1 communication mode, (which is rarely used)
//  you must define the macro below, or, the mode does not work.
#ifdef C7A43868_D29A_4404_9BFD_698A970F8614
    #include "ntddser.h"
    #include "cardtioctl.h"
    #include "WINIOCTL.h"
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


///////////////////////////////////////////////////////////
// Some non-member functions
///////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------------
// ����˵��: ���������̺߳���
// ����˵��: �ӿ�ʵ��ָ��
// ����ֵ: No use
// ������: DavidHowe;  ����ʱ��: 2003-4-28 4:10:42 PM;  �ص�: Xi'an
// ��ע/�޸ļ�¼: 
//-----------------------------------------------------------------------------------
UINT ReadCommThread(LPVOID pParam)
{
    ASSERT(pParam != NULL);
	CCommInterface *pInfc = (CCommInterface *) pParam;

	// Allocate receive buffer
	BYTE byRecvBuf[RECVBUF_SIZE];

	DWORD dwBytesRead = 0;
	DWORD dwError, dwEvent;

	// Overlapped structure
	OVERLAPPED olRead;
	memset(&olRead, 0, sizeof(OVERLAPPED));
	olRead.hEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);

	///////////////////////////////////////////////////////////
	// David 2003-4-28 3:05:07 PM
	// Save the olRead in the interface, when the thread is
	//  terminated, close the olRead.hEvent's handle.
	// Add this line to fix a "handle leak" bug.
	///////////////////////////////////////////////////////////
	pInfc->SaveOverlappedReadComm(&olRead);

	// Get the comm's handle
	HANDLE hComm = pInfc->GetCommPortHandle();

	// Set comm mask
	::SetCommMask(hComm, EV_RXCHAR);

	// Get the data-proc callback function's pointer and the "pParamCallback"
	//  param of current interface instance
	void (*pfnDataProc)(BYTE [], int, void *) =
		(void(*)(BYTE [], int, void *)) pInfc->GetDataProcAddr();
    ASSERT(pfnDataProc != NULL);
	void *pParamCallback = pInfc->GetCallbackParam();
    ASSERT(pParamCallback != NULL);

	///////////////////////////////////////////////////////////
	// Read-comm infinite loop
	///////////////////////////////////////////////////////////
	while(TRUE)
	{
		if(! ::WaitCommEvent(hComm, &dwEvent, &olRead))
		{
			if(GetLastError() != ERROR_IO_PENDING)
			{
				::ClearCommError(hComm, &dwError, NULL);
				continue;
			}
			::WaitForSingleObject(olRead.hEvent, INFINITE);
			::ResetEvent(olRead.hEvent);
			if(dwEvent & EV_RXCHAR)
			{
				if(! ::ReadFile(hComm, byRecvBuf, RECVBUF_SIZE, &dwBytesRead,
					&olRead))
				{
					if(! ::GetOverlappedResult(hComm, &olRead, &dwBytesRead, TRUE))
						continue;
					// Process Data
					pfnDataProc(byRecvBuf, dwBytesRead, pParamCallback);
				}
				else
				{
					// Process Data
					pfnDataProc(byRecvBuf, dwBytesRead, pParamCallback);
				}
			}
		}
		else
		{
			::ResetEvent(olRead.hEvent);
			if(dwEvent & EV_RXCHAR)
			{
				if(! ::ReadFile(hComm, byRecvBuf, RECVBUF_SIZE, &dwBytesRead,
					&olRead))
				{
					if(! ::GetOverlappedResult(hComm, &olRead, &dwBytesRead, TRUE))
						continue;
					// Process Data
					pfnDataProc(byRecvBuf, dwBytesRead, pParamCallback);
				}
				else
				{
					// Process Data
					pfnDataProc(byRecvBuf, dwBytesRead, pParamCallback);
				}
			}
		}
	}

	return 0L; // Never reaches here
}

/*************************************************************************************
** Description: E1���߳�
** Parameters: 
** Return: 
** Author: lipf;  Date: 2003-12-23 14:49:18;  Location: Office
** Misc: 
**************************************************************************************/
UINT ReadE1Thread(LPVOID pParam)
{
#ifndef C7A43868_D29A_4404_9BFD_698A970F8614
    return 0L;
#else
    CCommInterface *pInfc = (CCommInterface *) pParam;
    DWORD dwEvent, dwBytesRead=0;
    BYTE byRecvBuf[RECVBUF_SIZE];
    ULONG rxMask = EV_RXCHAR;
    ULONG nOutput;
    OVERLAPPED olE1Read;
    memset(&olE1Read, 0, sizeof(OVERLAPPED));
    olE1Read.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);	// �ص��ṹ��ʼ��
    pInfc->SaveOverlappedReadE1(&olE1Read);    
    // Get the E1's handle
    HANDLE hE1 = pInfc->GetE1Handle();

    if(!DeviceIoControl(hE1, IOCTL_SERIAL_SET_WAIT_MASK, &rxMask,
        sizeof(ULONG),NULL, 0,&nOutput,NULL))
    {
        TRACE("E1SCSetEvent ʧ��\n");
    }
    // Get the data-proc callback function's pointer and the "pParamCallback"
    //  param of current interface instance
    void (*pfnDataProc)(BYTE [], int, void *) =
        (void(*)(BYTE [], int, void *)) pInfc->GetDataProcAddr();
    
    void *pParamCallback = pInfc->GetCallbackParam();

    while(TRUE)
    {       
        if (!DeviceIoControl(hE1, IOCTL_SERIAL_WAIT_ON_MASK,NULL,0,&dwEvent,
            sizeof(DWORD), &nOutput,&olE1Read))
        {
            if(GetLastError()!=ERROR_IO_PENDING)	// �����쳣����
            {
                continue;
            }			
            
            ::WaitForSingleObject(olE1Read.hEvent, INFINITE);	// �ȴ����ݵ�
            ::ResetEvent(olE1Read.hEvent);
                      
            if(dwEvent & EV_RXCHAR)
            {
                if (!ReadFile(hE1, byRecvBuf, RECVBUF_SIZE, &dwBytesRead, &olE1Read))	// ���������ݰ�
                {
//DEL                     if (GetLastError()!=ERROR_IO_PENDING) 
//DEL                         continue;
                    if (!GetOverlappedResult(hE1, &olE1Read,
                        &dwBytesRead, TRUE))
                        continue;
                    
                    pfnDataProc(byRecvBuf, dwBytesRead, pParamCallback);
                }
                else
                {
                    pfnDataProc(byRecvBuf, dwBytesRead, pParamCallback);
                }
            }
        }
        else
        {
            ::ResetEvent(olE1Read.hEvent);
            if(dwEvent & EV_RXCHAR)
            {
                if (!ReadFile(hE1, byRecvBuf, RECVBUF_SIZE, &dwBytesRead, &olE1Read))	// ���������ݰ�
                {
                    if (!GetOverlappedResult(hE1, &olE1Read,
                        &dwBytesRead, TRUE))
                        continue;
                    
                    pfnDataProc(byRecvBuf, dwBytesRead, pParamCallback);
                }
                else
                {
                    pfnDataProc(byRecvBuf, dwBytesRead, pParamCallback);
                }
            }
        }
    }
    return 0l;
#endif // C7A43868_D29A_4404_9BFD_698A970F8614
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCommInterface::CCommInterface(int nCommMode)
{
	//
	// Initial vars
	//
	ASSERT(nCommMode >= INTERFACE_MINTYPE && nCommMode <= INTERFACE_MAXTYPE);
	m_nCommMode = nCommMode;
	m_pSendCmdThrd = NULL;
	m_hCommPort = NULL;
	m_pfnInterfaceOpenCallback = NULL;
	m_pfnInterfaceCloseCallback = NULL;
	m_pfnDataProcCallback = NULL;
	m_pParamCallback = NULL;
	m_pReadCommThrd = NULL;
	m_polReadComm = NULL;
	m_hEvtCanGetCmdToSend = ::CreateEvent(NULL, FALSE, TRUE, NULL);
	m_hEvtCanAddCmdToSend = ::CreateEvent(NULL, TRUE, TRUE, NULL);
	m_dwMinIntervalBetweenCmds = 50; // A default value, developer should modify
									 //  this by calling SetMinIntervalBetweenCmds(..)
	m_pListenSock = NULL;
	m_pDataSock = NULL;

    // E1 Param/* lipf 2003-12-23 */
    m_hE1Handle = NULL;
    m_pReadE1Thrd = NULL;
    m_polReadE1 = NULL;

    m_nUdpPort = 0;
    m_pUdpSock = NULL;
    m_csTargetHost = _T("");
    m_nTargetPort = 0;

	//
	// �����������������
	//
	m_pCmdListHead = new COMMAND_LIST_NODE;
	m_pCmdListHead->pbyCommand = NULL;
	m_pCmdListHead->nLenOfCmd = 0;
	m_pCmdListHead->pNext = NULL;
	m_pCmdListTail = m_pCmdListHead; // ��ʼΪ��, βָ��ͷ
}

CCommInterface::~CCommInterface()
{
	// �رսӿ�
	CloseInterface();

	//
	// �ͷ��������
	//
	COMMAND_LIST_NODE *p;
	while(m_pCmdListHead->pNext)
	{
		p = m_pCmdListHead;
		m_pCmdListHead = m_pCmdListHead->pNext;

		if(p->pbyCommand) delete p->pbyCommand;
		delete p;
	}
	if(m_pCmdListHead->pbyCommand) delete m_pCmdListHead->pbyCommand;
	delete m_pCmdListHead;

	// ɾ��
	::CloseHandle(m_hEvtCanGetCmdToSend);
	::CloseHandle(m_hEvtCanAddCmdToSend);
}

//-----------------------------------------------------------------------------------
// ����˵��: ��ȡ�汾��Ϣ
// ����˵��: �Ƿ���ʾ�汾��Ϣ
// ����ֵ: �汾��(��16bits�����汾��, ��16bits�Ǵΰ汾��)
// ������: DavidHowe;  ����ʱ��: 2003-5-4 12:56:06;  �ص�: Xi'an
// ��ע/�޸ļ�¼: 
//-----------------------------------------------------------------------------------
DWORD CCommInterface::GetVersionInfo(BOOL bShowMsgbox) const
{
	if(bShowMsgbox)
	{
		CString csMsg, csTmp;
		csTmp.Format("Last build: %s %s     \n\n", __DATE__, __TIME__);
		csMsg = "Module: CCommInterface\n";
		csMsg += "Version: 1.5\n";
		csMsg += csTmp;
		csMsg += "Author: David Howe\n";
		csMsg += "E-mail: davidhowe@sina.com\n";
		AfxMessageBox(csMsg, MB_ICONINFORMATION | MB_OK);
	}

	return 0x00010005;
}

//-----------------------------------------------------------------------------------
// ����˵��: �رսӿ�(���ݽӿ����͵�����Ӧ�ĺ���)
// ����˵��: 
// ����ֵ: �������
// ������: DavidHowe;  ����ʱ��: 2003-4-30 9:42:31 AM;  �ص�: Xi'an
// ��ע/�޸ļ�¼: ��ͳһ�ĺ����رսӿ�.
//-----------------------------------------------------------------------------------
BOOL CCommInterface::CloseInterface(BOOL bInvokeCallback)
{
	if(m_nCommMode == INTERFACE_COMMPORT) return CloseInterface_CommPort(bInvokeCallback);
	else if(m_nCommMode == INTERFACE_TCPSERVER) return CloseInterface_TcpServer(bInvokeCallback);
	else if(m_nCommMode == INTERFACE_TCPCLIENT) return CloseInterface_TcpClient(bInvokeCallback);
    else if (m_nCommMode == INTERFACE_E1) return CloseInterface_E1(bInvokeCallback);
    else if (m_nCommMode == INTERFACE_UDP) return CloseInterface_Udp(bInvokeCallback);

	return FALSE;
}

//-----------------------------------------------------------------------------------
// ����˵��: ��Ӹ�������Ͷ��еĶ�β
// ����˵��: ����buf, �����
// ����ֵ: �������
// ������: DavidHowe;  ����ʱ��: 2003-4-23 3:53:44 PM;  �ص�: Xi'an
// ��ע/�޸ļ�¼: 
//-----------------------------------------------------------------------------------
BOOL CCommInterface::AppendCmdToSend(const BYTE byBuf[], int nLen)
{
	if(byBuf == NULL)
	{
		TRACE("Interface::Command buffer is empty, add command failed.\n");
        ASSERT(FALSE);
		return FALSE;
	}
	if(nLen <= 0 || nLen > SENDBUF_SIZE)
	{
		TRACE("Interface::Command must be short than %d bytes.\n", SENDBUF_SIZE);
        ASSERT(FALSE);
		return FALSE;
	}

    //
    // Copy the command
	//
    COMMAND_LIST_NODE *pCmdNode = NULL;
    if (m_nCommMode == INTERFACE_UDP) // For UDP comm mode
    {
        if (nLen > SENDBUF_SIZE - 32)
        {
            TRACE("Interface::Out of send buffer in udp mode!\n");
            ASSERT(FALSE);
            return FALSE;
        }
        if (m_nTargetPort == 0)
        {
            TRACE("Interface::You must set the host address and host port first!\n");
            ASSERT(FALSE);
            return FALSE;
        }

        // D.H 2004-8-13 9:50:44
        // ע��, ����ÿ������, ��Ҫ���淢��Ŀ����Ϣ
        pCmdNode = new COMMAND_LIST_NODE;
        pCmdNode->pbyCommand = new BYTE[nLen + m_csTargetHost.GetLength() + 1 +
            sizeof(DWORD)];
        memcpy(pCmdNode->pbyCommand, (LPCTSTR) m_csTargetHost, m_csTargetHost.GetLength());
        pCmdNode->pbyCommand[m_csTargetHost.GetLength()] = '\0';
        memcpy(pCmdNode->pbyCommand + m_csTargetHost.GetLength() + 1, &m_nTargetPort,
            sizeof(DWORD));
        memcpy(pCmdNode->pbyCommand + m_csTargetHost.GetLength() + 1 + sizeof(DWORD),
            byBuf, nLen);
        pCmdNode->nLenOfCmd = nLen + m_csTargetHost.GetLength() + 1 + sizeof(DWORD);
        pCmdNode->pNext = NULL;
    }
    else // All comm mode but UDP
    {
        pCmdNode = new COMMAND_LIST_NODE;
        pCmdNode->pbyCommand = new BYTE[nLen];
        memcpy(pCmdNode->pbyCommand, byBuf, nLen);
        pCmdNode->nLenOfCmd = nLen;
        pCmdNode->pNext = NULL;
    }

    ::WaitForSingleObject(m_hEvtCanGetCmdToSend, INFINITE);

	// Wait until we can add the command
	::WaitForSingleObject(m_hEvtCanAddCmdToSend, INFINITE);
    ::ResetEvent(m_hEvtCanAddCmdToSend);

	///////////////////////////////////////////////////////////
	// ʹ���¼�, Ϊ�����������Ĺ����в��÷����߳�ȡ����
	///////////////////////////////////////////////////////////

    //
	// Append to list
	//
	if(m_pCmdListHead->pNext == NULL) // List is empty yet
	{
		m_pCmdListHead->pNext = pCmdNode;
		m_pCmdListTail = pCmdNode;
	}
	else // List is not empty
	{
		m_pCmdListTail->pNext = pCmdNode;
		m_pCmdListTail = pCmdNode;
	}

	// �¼����ź�̬
	::SetEvent(m_hEvtCanGetCmdToSend);
    ::SetEvent(m_hEvtCanAddCmdToSend);

	return TRUE;
}

//-----------------------------------------------------------------------------------
// ����˵��: ����һ�������ͷ, ���ȷ��͸�������
// ����˵��: ����buf, �����
// ����ֵ: �������
// ������: DavidHowe;  ����ʱ��: 2003-4-23 4:30:19 PM;  �ص�: Xi'an
// ��ע/�޸ļ�¼: 
//-----------------------------------------------------------------------------------
BOOL CCommInterface::InsertCmdToSend(const BYTE byBuf[], int nLen)
{
	if(byBuf == NULL)
	{
		TRACE("Interface::Command buffer is empty, add command failed.\n");
        ASSERT(FALSE);
		return FALSE;
	}
	if(nLen <= 0 || nLen > SENDBUF_SIZE)
	{
		TRACE("Interface::Command must be short than %d bytes.\n", SENDBUF_SIZE);
        ASSERT(FALSE);
		return FALSE;
	}

    //
    // Copy the command
    //
    COMMAND_LIST_NODE *pCmdNode = NULL;
    if (m_nCommMode == INTERFACE_UDP) // For UDP comm mode
    {
        if (nLen > SENDBUF_SIZE - 32)
        {
            TRACE("Interface::Out of send buffer in udp mode!\n");
            ASSERT(FALSE);
            return FALSE;
        }
        if (m_nTargetPort == 0)
        {
            TRACE("Interface::You must set the host address and host port first!\n");
            ASSERT(FALSE);
            return FALSE;
        }
        
        // D.H 2004-8-13 9:50:44
        // ע��, ����ÿ������, ��Ҫ���淢��Ŀ����Ϣ
        pCmdNode = new COMMAND_LIST_NODE;
        pCmdNode->pbyCommand = new BYTE[nLen + m_csTargetHost.GetLength() + 1 +
            sizeof(DWORD)];
        memcpy(pCmdNode->pbyCommand, (LPCTSTR) m_csTargetHost, m_csTargetHost.GetLength());
        pCmdNode->pbyCommand[m_csTargetHost.GetLength()] = '\0';
        memcpy(pCmdNode->pbyCommand + m_csTargetHost.GetLength() + 1, &m_nTargetPort,
            sizeof(DWORD));
        memcpy(pCmdNode->pbyCommand + m_csTargetHost.GetLength() + 1 + sizeof(DWORD),
            byBuf, nLen);
        pCmdNode->nLenOfCmd = nLen + m_csTargetHost.GetLength() + 1 + sizeof(DWORD);
        pCmdNode->pNext = NULL;
    }
    else // All comm mode but UDP
    {
        pCmdNode = new COMMAND_LIST_NODE;
        pCmdNode->pbyCommand = new BYTE[nLen];
        memcpy(pCmdNode->pbyCommand, byBuf, nLen);
        pCmdNode->nLenOfCmd = nLen;
        pCmdNode->pNext = NULL;
    }

    ::WaitForSingleObject(m_hEvtCanGetCmdToSend, INFINITE);

	// Wait until we can add the command
	::WaitForSingleObject(m_hEvtCanAddCmdToSend, INFINITE);
    ::ResetEvent(m_hEvtCanAddCmdToSend);

	///////////////////////////////////////////////////////////
	// ʹ���¼�, Ϊ�����������Ĺ����в��÷����߳�ȡ����
	///////////////////////////////////////////////////////////

	//
	// Insert into list
	//
	pCmdNode->pNext = m_pCmdListHead->pNext;
	m_pCmdListHead->pNext = pCmdNode;
	if(m_pCmdListTail == m_pCmdListHead) m_pCmdListTail = pCmdNode;

	// �¼����ź�̬
	::SetEvent(m_hEvtCanGetCmdToSend);
    ::SetEvent(m_hEvtCanAddCmdToSend);

	return TRUE;
}

//-----------------------------------------------------------------------------------
// ����˵��: �����������ȡ����һ������
// ����˵��: �������Ļ�����, �������ĳ���
// ����ֵ: �������
// ������: DavidHowe;  ����ʱ��: 2003-4-28 5:14:47 PM;  �ص�: Xi'an
// ��ע/�޸ļ�¼: 
//-----------------------------------------------------------------------------------
BOOL CCommInterface::GetCmdToSend(BYTE byBuf[], int& nLen)
{
    ::WaitForSingleObject(m_hEvtCanAddCmdToSend, INFINITE);
	// ��ʱ���ܽ��������������
	::ResetEvent(m_hEvtCanAddCmdToSend);

	if(m_pCmdListHead->pNext == NULL) // No command to send
	{
		nLen = 0;
		::SetEvent(m_hEvtCanAddCmdToSend);
		return FALSE;
	}
	else
	{
		memcpy(byBuf, m_pCmdListHead->pNext->pbyCommand,
			m_pCmdListHead->pNext->nLenOfCmd);
		nLen = m_pCmdListHead->pNext->nLenOfCmd;
		ASSERT(nLen <= SENDBUF_SIZE);
		::SetEvent(m_hEvtCanAddCmdToSend);
		return TRUE;
	}
}

//-----------------------------------------------------------------------------------
// ����˵��: ɾ����һ������ (�ѳɹ�����ʱ����һ��)
// ����˵��: 
// ����ֵ: �������
// ������: DavidHowe;  ����ʱ��: 2003-4-28 6:29:07 PM;  �ص�: Xi'an
// ��ע/�޸ļ�¼: 
//-----------------------------------------------------------------------------------
BOOL CCommInterface::DeleteFirstCmd(void)
{
	// No command to delete
	if(m_pCmdListHead->pNext == NULL)
	{
		TRACE("Interface::Delete command failed, sending-command list is empty.\n");
		return FALSE;
	}

	///////////////////////////////////////////////////////////
	// ɾ������Ĺ����в�����ӻ�ȡ������
	///////////////////////////////////////////////////////////

    ::WaitForSingleObject(m_hEvtCanAddCmdToSend, INFINITE);
	// �¼��÷��ź�̬
//DEL 	::ResetEvent(m_hEvtCanGetCmdToSend);
	::ResetEvent(m_hEvtCanAddCmdToSend);

	// Head move on
	COMMAND_LIST_NODE *p;
	p = m_pCmdListHead->pNext;
	m_pCmdListHead->pNext = m_pCmdListHead->pNext->pNext;
	if(p->pbyCommand) delete p->pbyCommand;
	delete p;

	// �¼����ź�̬
	::SetEvent(m_hEvtCanGetCmdToSend);
	::SetEvent(m_hEvtCanAddCmdToSend);

	return TRUE;
}

//-----------------------------------------------------------------------------------
// ����˵��: ɾ����������
// ����˵��: 
// ����ֵ: 
// ������: DavidHowe;  ����ʱ��: 2003-4-29 5:02:33 PM;  �ص�: Xi'an
// ��ע/�޸ļ�¼: 
//-----------------------------------------------------------------------------------
void CCommInterface::DeleteAllCmds(void)
{
	ASSERT(this != NULL);

	///////////////////////////////////////////////////////////
	// ɾ������Ĺ����в�����ӻ�ȡ������
	///////////////////////////////////////////////////////////

    ::WaitForSingleObject(m_hEvtCanAddCmdToSend, INFINITE);
	// �¼��÷��ź�̬
//DEL 	::ResetEvent(m_hEvtCanGetCmdToSend);
	::ResetEvent(m_hEvtCanAddCmdToSend);

	COMMAND_LIST_NODE *p, *q;
	p = m_pCmdListHead->pNext;

	if(! p) // Already empty
	{
		// �¼����ź�̬
		::SetEvent(m_hEvtCanGetCmdToSend);
		::SetEvent(m_hEvtCanAddCmdToSend);
		return;
	}

	while(p->pbyCommand != NULL)
	{
		q = p;
		p = p->pNext;

		delete q->pbyCommand;
		delete q;

		if(! p) break;
	}

	m_pCmdListHead->pNext = NULL;
	m_pCmdListTail = m_pCmdListHead;

	// �¼����ź�̬
	::SetEvent(m_hEvtCanGetCmdToSend);
	::SetEvent(m_hEvtCanAddCmdToSend);
}

//-----------------------------------------------------------------------------------
// ����˵��: ��������ģʽ��, �򿪽ӿڵĺ���
// ����˵��: ���ں�(��1��ʼ); �˴����յ�����ʱ�Ļص�����; ���ò���
// ����ֵ: �������
// ������: DavidHowe;  ����ʱ��: 2003-4-23 5:54:37 PM;  �ص�: Xi'an
// ��ע/�޸ļ�¼: Ҫ��֤���ֻ��һ�����ڱ���
//-----------------------------------------------------------------------------------
BOOL CCommInterface::CreateInterface_CommPort(int nCommPort,
											  const void *pfnInterfaceOpenCallback,
											  const void *pfnInterfaceCloseCallback,
											  const void *pfnDataProcCallback,
											  const void *pParamCallback)
{
	ASSERT(m_nCommMode == INTERFACE_COMMPORT);
	if(m_nCommMode != INTERFACE_COMMPORT) return FALSE;

	// Close previous if open
	CloseInterface();
	ASSERT(m_hCommPort == NULL);

	// The Interface Data Proc must be a valid pointer
	ASSERT(pfnDataProcCallback != NULL);

	if(nCommPort < 1 || nCommPort > 512)
	{
		TRACE("Interface::Commport index out of range: %d\n", nCommPort);
		return FALSE;
	}

	// Save the params
	m_nCommPort = nCommPort;
	m_pfnInterfaceOpenCallback = (void *) pfnInterfaceOpenCallback;
	m_pfnInterfaceCloseCallback = (void *) pfnInterfaceCloseCallback;
	m_pfnDataProcCallback = (void *) pfnDataProcCallback;
	m_pParamCallback = (void *) pParamCallback;

	//
	// (Attempt to) open the commport
	//
	CString csCommStr;
	csCommStr.Format("\\\\.\\COM%d", m_nCommPort);
	HANDLE hComm = ::CreateFile(csCommStr, GENERIC_READ | GENERIC_WRITE, 0,
		NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	if(hComm == (HANDLE) -1) // Open failure
	{
		m_hCommPort = NULL;
		TRACE("Interface::Open commport failed: COM%d\n", nCommPort);

		return FALSE;
	}
	else // Open success
	{
		m_hCommPort = hComm;

		//
		// Config the commport using default settings
		//
		DCB dcb;
		::GetCommState(m_hCommPort, &dcb);
		dcb.BaudRate = CBR_9600;
		dcb.ByteSize = 8;
		dcb.Parity = NOPARITY;
		dcb.StopBits = ONESTOPBIT;
		if(! ::SetCommState(m_hCommPort, &dcb))
		{
			TRACE("Interface::Failed to use the default commport settings: 9600,n,8,1\n");
			CloseInterface();
			return FALSE;
		}

		//
		// Config timeout settings using default settings
		//
		COMMTIMEOUTS tout;
		::GetCommTimeouts(m_hCommPort, &tout);
		tout.ReadIntervalTimeout = 20; // For CBR_9600
		tout.ReadTotalTimeoutConstant = 0;
		tout.ReadTotalTimeoutMultiplier = 0;
		tout.WriteTotalTimeoutConstant = 0;
		tout.WriteTotalTimeoutMultiplier = 0;
		if(! ::SetCommTimeouts(m_hCommPort, &tout))
		{
			TRACE("Interface::Failed to use the default commport timeout settings!\n");
			CloseInterface();
			return FALSE;
		}

		// Purge commport
		::PurgeComm(m_hCommPort, PURGE_RXCLEAR | PURGE_TXCLEAR);

		// Start the read-comm thread!
		m_pReadCommThrd = AfxBeginThread(ReadCommThread, this);

		//
		// Start the send-command thread!
		//
		m_pSendCmdThrd = (CInterfaceSendCmdThread *) AfxBeginThread(
			RUNTIME_CLASS(CInterfaceSendCmdThread), 0, CREATE_SUSPENDED);
		m_pSendCmdThrd->SetInterfaceHandle(this);
		::ResumeThread(m_pSendCmdThrd->m_hThread);
        BOOL bRet = ::PostThreadMessage(m_pSendCmdThrd->m_nThreadID,
            TM_START_SENDCOMMANDLOOP, 0, 0);
        // D.H 2004-8-11 15:59:59
        // ��Ӷ�::PostThreadMessage()�ķ����жϴ���, ��������Dll������
        //  ʹ��CCommInterfaceʱ�����������������̵߳�bug.
        // {F598F388-14D8-464F-BE35-2CA67E1A978C}
        while (! bRet)
        {
            Sleep(0);
            bRet = ::PostThreadMessage(m_pSendCmdThrd->m_nThreadID,
                TM_START_SENDCOMMANDLOOP, 0, 0);
        }

		// Call the Interface-Open callback
		if(m_pfnInterfaceOpenCallback)
		{
			void (*pfnCallback)(const void*) =
				(void(*)(const void*))m_pfnInterfaceOpenCallback;
			pfnCallback(m_pParamCallback);
		}

		return TRUE;
	}
}

//-----------------------------------------------------------------------------------
// ����˵��: ��������ģʽ��, �رմ���
// ����˵��: 
// ����ֵ: �������
// ������: DavidHowe;  ����ʱ��: 2003-4-24 6:10:00 PM;  �ص�: Xi'an
// ��ע/�޸ļ�¼: 
//-----------------------------------------------------------------------------------
BOOL CCommInterface::CloseInterface_CommPort(BOOL bInvokeCallback)
{
	BOOL bRet = TRUE;
	if(m_hCommPort != NULL)
	{
		// Terminate read-comm thread
		ASSERT(m_pReadCommThrd != NULL);
		if(m_pReadCommThrd)
		{
			if(m_pReadCommThrd->m_hThread)
			{
				::TerminateThread(m_pReadCommThrd->m_hThread, 0);
				delete m_pReadCommThrd;
				m_pReadCommThrd = NULL;
			}
		}

		// Terminate send-command thread
		ASSERT(m_pSendCmdThrd != NULL);
		if(m_pSendCmdThrd)
		{
			::TerminateThread(m_pSendCmdThrd->m_hThread, 0);
			::CloseHandle(m_pSendCmdThrd->m_olWrite.hEvent); // Prevent handle leak
			delete m_pSendCmdThrd;
			m_pSendCmdThrd = NULL;
		}

		// Close commport
		ASSERT(m_hCommPort != NULL);
		bRet = ::CloseHandle(m_hCommPort);
		m_hCommPort = NULL;

		// Close the event's handle (to prevent a handle leak)
		ASSERT(m_polReadComm != NULL);
		if(m_polReadComm) ::CloseHandle(m_polReadComm->hEvent);

		// Call the Interface-Close callback
		if(bInvokeCallback && m_pfnInterfaceCloseCallback)
		{
			void (*pfnCallback)(const void*) =
				(void(*)(const void*))m_pfnInterfaceCloseCallback;
			pfnCallback(m_pParamCallback);
		}

		// Have a rest, or the same commport cannot be opened immediately
		Sleep(50);
	}

	if(! bRet) TRACE("Interface::Close interface failed.\n");

	return bRet;
}

//-----------------------------------------------------------------------------------
// ����˵��: ���ô��ڲ���
// ����˵��: DCB
// ����ֵ: �������
// ������: DavidHowe;  ����ʱ��: 2003-4-28 4:32:20 PM;  �ص�: Xi'an
// ��ע/�޸ļ�¼: 
//-----------------------------------------------------------------------------------
BOOL CCommInterface::SetCommState(const DCB *pdcb) const
{
	ASSERT(m_nCommMode == INTERFACE_COMMPORT);
	ASSERT(m_hCommPort != NULL);
	ASSERT(pdcb != NULL);
	if(m_nCommMode != INTERFACE_COMMPORT) return FALSE;
	if(m_hCommPort == NULL) return FALSE;
	if(pdcb == NULL) return FALSE;

	// ��ͣ�߳�
	if(m_pReadCommThrd)
	{
		if(m_pReadCommThrd->m_hThread) ::SuspendThread(m_pReadCommThrd->m_hThread);
	}
	if(m_pSendCmdThrd)
	{
		if(m_pSendCmdThrd->m_hThread) ::SuspendThread(m_pSendCmdThrd->m_hThread);
	}

	// Call ::SetCommState(...)
	BOOL bRet = ::SetCommState(m_hCommPort, (DCB *) pdcb);
	if(! bRet) TRACE("Interface::Call SetCommState API failed.\n");

	// �ָ��߳�
	if(m_pReadCommThrd)
	{
		if(m_pReadCommThrd->m_hThread) ::ResumeThread(m_pReadCommThrd->m_hThread);
	}
	if(m_pSendCmdThrd)
	{
		if(m_pSendCmdThrd->m_hThread) ::ResumeThread(m_pSendCmdThrd->m_hThread);
	}

	return bRet;
}

//-----------------------------------------------------------------------------------
// ����˵��: ��ȡ���ڲ�����Ϣ
// ����˵��: ����һ��DCB��ָ��
// ����ֵ: �������
// ������: DavidHowe;  ����ʱ��: 2003-4-30 10:19:58 AM;  �ص�: Xi'an
// ��ע/�޸ļ�¼: 
//-----------------------------------------------------------------------------------
BOOL CCommInterface::GetCommState(const DCB *pdcb) const
{
	ASSERT(m_nCommMode == INTERFACE_COMMPORT);
	ASSERT(m_hCommPort != NULL);
	if(m_nCommMode != INTERFACE_COMMPORT) return FALSE;
	if(m_hCommPort == NULL) return FALSE;

	// Call ::GetCommState(...) and return
	BOOL bRet = ::GetCommState(m_hCommPort, (DCB *) pdcb);
	if(! bRet) TRACE("Interface::Call GetCommState API failed.\n");

	return bRet;
}

//-----------------------------------------------------------------------------------
// ����˵��: ���ô��ڳ�ʱ����
// ����˵��: COMMTIMEOUTS
// ����ֵ: �������
// ������: DavidHowe;  ����ʱ��: 2003-4-30 11:19:11 AM;  �ص�: Xi'an
// ��ע/�޸ļ�¼: 
//-----------------------------------------------------------------------------------
BOOL CCommInterface::SetCommTimeouts(const COMMTIMEOUTS *ptmo) const
{
	ASSERT(m_nCommMode == INTERFACE_COMMPORT);
	ASSERT(m_hCommPort != NULL);
	ASSERT(ptmo != NULL);
	if(m_nCommMode != INTERFACE_COMMPORT) return FALSE;
	if(m_hCommPort == NULL) return FALSE;
	if(ptmo == NULL) return FALSE;

	// ��ͣ�߳�
	if(m_pReadCommThrd)
	{
		if(m_pReadCommThrd->m_hThread) ::SuspendThread(m_pReadCommThrd->m_hThread);
	}
	if(m_pSendCmdThrd)
	{
		if(m_pSendCmdThrd->m_hThread) ::SuspendThread(m_pSendCmdThrd->m_hThread);
	}

	// Call ::SetCommTimeouts(...)
	BOOL bRet = ::SetCommTimeouts(m_hCommPort, (COMMTIMEOUTS *) ptmo);
	if(! bRet) TRACE("Interface::Call SetCommTimeouts API failed.\n");

	// �ָ��߳�
	if(m_pReadCommThrd)
	{
		if(m_pReadCommThrd->m_hThread) ::ResumeThread(m_pReadCommThrd->m_hThread);
	}
	if(m_pSendCmdThrd)
	{
		if(m_pSendCmdThrd->m_hThread) ::ResumeThread(m_pSendCmdThrd->m_hThread);
	}

	return bRet;
}

//-----------------------------------------------------------------------------------
// ����˵��: ��ȡ���ڳ�ʱ����
// ����˵��: ����һ��COMMTIMEOUTSָ��
// ����ֵ: �������
// ������: DavidHowe;  ����ʱ��: 2003-4-30 11:25:52 AM;  �ص�: Xi'an
// ��ע/�޸ļ�¼: 
//-----------------------------------------------------------------------------------
BOOL CCommInterface::GetCommTimeouts(const COMMTIMEOUTS *ptmo) const
{
	ASSERT(m_nCommMode == INTERFACE_COMMPORT);
	ASSERT(m_hCommPort != NULL);
	if(m_nCommMode != INTERFACE_COMMPORT) return FALSE;
	if(m_hCommPort == NULL) return FALSE;

	// Call ::GetCommTimeouts(...) and return
	BOOL bRet = ::GetCommTimeouts(m_hCommPort, (COMMTIMEOUTS *) ptmo);
	if(! bRet) TRACE("Interface::Call GetCommTimeouts API failed.\n");

	return bRet;
}

//-----------------------------------------------------------------------------------
// ����˵��: TCP Serverģʽ��, �򿪽ӿڵĺ���
// ����˵��: �����˿�; �ӿڴ�ʱ�Ļص�����; �ӿڹر�ʱ�Ļص�����;
//			 �ӿ��յ�����ʱ�Ļص�����; ���ò���;
// ����ֵ: �������
// ������: DavidHowe;  ����ʱ��: 2003-4-30 2:26:08 PM;  �ص�: Xi'an
// ��ע/�޸ļ�¼: 
//-----------------------------------------------------------------------------------
BOOL CCommInterface::CreateInterface_TcpServer(unsigned int nListenPort,
											   const void *pfnInterfaceOpenCallback,
											   const void *pfnInterfaceCloseCallback,
											   const void *pfnDataProcCallback,
											   const void *pParamCallback)
{
	ASSERT(m_nCommMode == INTERFACE_TCPSERVER);
	if(m_nCommMode != INTERFACE_TCPSERVER) return FALSE;

	// Close the previous Interface
	CloseInterface();
	ASSERT(m_pListenSock == NULL && m_pDataSock == NULL);

	// The Interface Data Proc must be a valid pointer
	ASSERT(pfnDataProcCallback != NULL);

	// Save the params
	m_nListenPort = nListenPort;
	m_pfnInterfaceOpenCallback = (void *) pfnInterfaceOpenCallback;
	m_pfnInterfaceCloseCallback = (void *) pfnInterfaceCloseCallback;
	m_pfnDataProcCallback = (void *) pfnDataProcCallback;
	m_pParamCallback = (void *) pParamCallback;

	// Create listen socket for the Tcp Server
	m_pListenSock = new CInterfaceListenSocket(this);
	ASSERT(m_pListenSock != NULL);

	if(! m_pListenSock->Create(nListenPort))
	{
		TRACE("Interface::Create listen socket failed, error code = %d\n", GetLastError());
		delete m_pListenSock;
		m_pListenSock = NULL;
		return FALSE;
	}
	if(! m_pListenSock->Listen())
	{
		TRACE("Interface::Socket listen failed, error code = %d\n", GetLastError());
		m_pListenSock->Close();
		delete m_pListenSock;
		m_pListenSock = NULL;
		return FALSE;
	}

	return TRUE;
}

//-----------------------------------------------------------------------------------
// ����˵��: �رսӿ� (Tcp Server ģʽ)
// ����˵��: 
// ����ֵ: 
// ������: DavidHowe;  ����ʱ��: 2003-4-30 4:44:33 PM;  �ص�: Xi'an
// ��ע/�޸ļ�¼: 
//-----------------------------------------------------------------------------------
BOOL CCommInterface::CloseInterface_TcpServer(BOOL bInvokeCallback)
{
	// Terminate send-command thread
	if(m_pSendCmdThrd)
	{
		::TerminateThread(m_pSendCmdThrd->m_hThread, 0);
		delete m_pSendCmdThrd;
		m_pSendCmdThrd = NULL;
	}

	if(m_pListenSock)
	{
		m_pListenSock->Close();
		delete m_pListenSock;
		m_pListenSock = NULL;
	}

	if(m_pDataSock)
	{
		m_pDataSock->Close();
		delete m_pDataSock;
		m_pDataSock = NULL;

		// Call the Interface-Close callback
		if(bInvokeCallback && m_pfnInterfaceCloseCallback)
		{
			void (*pfnCallback)(const void*) =
				(void(*)(const void*))m_pfnInterfaceCloseCallback;
			pfnCallback(m_pParamCallback);
		}
	}

	return TRUE;
}

//-----------------------------------------------------------------------------------
// ����˵��: �������socket��OnAccept(..)
// ����˵��: 
// ����ֵ: ������
// ������: DavidHowe;  ����ʱ��: 2003-4-30 5:39:50 PM;  �ص�: Xi'an
// ��ע/�޸ļ�¼: 
//-----------------------------------------------------------------------------------
BOOL CCommInterface::ProcessSocketAccept(void)
{
    if (m_pListenSock == NULL || m_pDataSock != NULL)
    {
        ASSERT(FALSE);
        return FALSE;
    }

	m_pDataSock = new CInterfaceDataSocket(this);
	if (! m_pDataSock)
    {
        ASSERT(FALSE);
        return FALSE;
    }

	if(! m_pListenSock->Accept(*m_pDataSock))
	{
		TRACE("Interface::Socket accept failed.\n");

		// Accept failed, delete the data socket we just created above
		delete m_pDataSock;
		m_pDataSock = NULL;

		//
		// Recreate the listen socket and relisten
		//
		m_pListenSock->Close();
		if(! m_pListenSock->Create(m_nListenPort))
		{
			TRACE("Interface::Recreate listen socket failed, error code = %d\n",
                GetLastError());
			return FALSE;
		}
		if(! m_pListenSock->Listen())
			TRACE("Interface::Relisten failed, error code = %d\n", GetLastError());

		return FALSE;
	}
	
	// liep 2003-11-24
//DEL 	m_pListenSock->GetSockName(m_csClientIp, m_nClientPort);

	// Delete the listen socket
	m_pListenSock->Close();
	delete m_pListenSock;
	m_pListenSock = NULL;

	//
	// Start the send-command thread!
	//
	m_pSendCmdThrd = (CInterfaceSendCmdThread *) AfxBeginThread(
		RUNTIME_CLASS(CInterfaceSendCmdThread), 0, CREATE_SUSPENDED);
	m_pSendCmdThrd->SetInterfaceHandle(this);
	::ResumeThread(m_pSendCmdThrd->m_hThread);
    BOOL bRet = ::PostThreadMessage(m_pSendCmdThrd->m_nThreadID,
        TM_START_SENDCOMMANDLOOP, 0, 0);
    while (! bRet)
    {
        Sleep(0);
        bRet = ::PostThreadMessage(m_pSendCmdThrd->m_nThreadID,
            TM_START_SENDCOMMANDLOOP, 0, 0);
    }

    // Call the Interface-Open callback
    if(m_pfnInterfaceOpenCallback)
    {
        void (*pfnCallback)(const void*) =
            (void(*)(const void*))m_pfnInterfaceOpenCallback;
        pfnCallback(m_pParamCallback);
    }

	return TRUE;
}

//-----------------------------------------------------------------------------------
// ����˵��: ��������socket��OnReceive(..)
// ����˵��: 
// ����ֵ: ������
// ������: DavidHowe;  ����ʱ��: 2003-4-30 10:27:28 PM;  �ص�: Xi'an
// ��ע/�޸ļ�¼: 
//-----------------------------------------------------------------------------------
BOOL CCommInterface::ProcessSocketReceive(void) const
{
	ASSERT(m_pDataSock != NULL);

	BYTE byBuf[RECVBUF_SIZE];
	int nRecvLen = m_pDataSock->Receive(byBuf, RECVBUF_SIZE);

	if(nRecvLen > 0)
	{
		if (m_pfnDataProcCallback)
        {
            void (*pfnDataProc)(BYTE [], int, void *) =
                (void(*)(BYTE [], int, void *)) m_pfnDataProcCallback;
            pfnDataProc(byBuf, nRecvLen, m_pParamCallback);
        }
        else ASSERT(FALSE);

		return TRUE;
	}
	else return FALSE;
}

//-----------------------------------------------------------------------------------
// ����˵��: ��������socket��OnClose(..)
// ����˵��: 
// ����ֵ: ������
// ������: DavidHowe;  ����ʱ��: 2003-5-1 12:19:30 AM;  �ص�: Xi'an
// ��ע/�޸ļ�¼: 
//-----------------------------------------------------------------------------------
BOOL CCommInterface::ProcessSocketClose(void)
{
	ASSERT(m_pDataSock != NULL);

	// Terminate send-command thread
	if(m_pSendCmdThrd)
	{
		::TerminateThread(m_pSendCmdThrd->m_hThread, 0);
		delete m_pSendCmdThrd;
		m_pSendCmdThrd = NULL;
	}

	m_pDataSock->Close();
	delete m_pDataSock;
	m_pDataSock = NULL;

	// Call the Interface-Close callback
	if(m_pfnInterfaceCloseCallback)
	{
		void (*pfnCallback)(const void*) =
			(void(*)(const void*))m_pfnInterfaceCloseCallback;
		pfnCallback(m_pParamCallback);
	}

	return TRUE;
}

//-----------------------------------------------------------------------------------
// ����˵��: TCP Clientģʽ��, �򿪽ӿڵĺ���
// ����˵��: �����IP; ����˼����˿�; �ӿڴ�ʱ�Ļص�����; �ӿڹر�ʱ�Ļص�����;
//			 �ӿ��յ�����ʱ�Ļص�����; ���ò���;
// ����ֵ: �������
// ������: DavidHowe;  ����ʱ��: 2003-5-1 6:59:38 PM;  �ص�: Xi'an
// ��ע/�޸ļ�¼: 
//-----------------------------------------------------------------------------------
BOOL CCommInterface::CreateInterface_TcpClient(CString csServerIp, // Server's IP addr
											   unsigned int nListenPort, // Server's listen port
											   const void *pfnInterfaceOpenCallback,
											   const void *pfnInterfaceCloseCallback,
											   const void *pfnDataProcCallback,
											   const void *pParamCallback)
{
	ASSERT(m_nCommMode == INTERFACE_TCPCLIENT);
	if(m_nCommMode != INTERFACE_TCPCLIENT) return FALSE;

	// Close the previous Interface
	CloseInterface();
	ASSERT(m_pDataSock == NULL);

	// The Interface Data Proc must be a valid pointer
	ASSERT(pfnDataProcCallback != NULL);

	// Save the params
	m_csServerIp = csServerIp;
	m_nListenPort = nListenPort;
	m_pfnInterfaceOpenCallback = (void *) pfnInterfaceOpenCallback;
	m_pfnInterfaceCloseCallback = (void *) pfnInterfaceCloseCallback;
	m_pfnDataProcCallback = (void *) pfnDataProcCallback;
	m_pParamCallback = (void *) pParamCallback;

	// Create data socket and try to connect server side
	m_pDataSock = new CInterfaceDataSocket(this);
	ASSERT(m_pDataSock != NULL);
	if(! m_pDataSock->Create())
	{
		TRACE("Interface::Create client socket failed, error code = %d\n",
            GetLastError());
		delete m_pDataSock;
		m_pDataSock = NULL;
		return FALSE;
	}

	m_pDataSock->Connect(m_csServerIp, m_nListenPort);

	return TRUE;
}

//-----------------------------------------------------------------------------------
// ����˵��: �رսӿ� (Tcp Client ģʽ)
// ����˵��: 
// ����ֵ: �������
// ������: DavidHowe;  ����ʱ��: 2003-5-1 10:55:33 PM;  �ص�: Xi'an
// ��ע/�޸ļ�¼: 
//-----------------------------------------------------------------------------------
BOOL CCommInterface::CloseInterface_TcpClient(BOOL bInvokeCallback)
{
	// Terminate send-command thread
	if(m_pSendCmdThrd)
	{
		::TerminateThread(m_pSendCmdThrd->m_hThread, 0);
		delete m_pSendCmdThrd;
		m_pSendCmdThrd = NULL;
	}

	if(m_pDataSock)
	{
		m_pDataSock->Close();
		delete m_pDataSock;
		m_pDataSock = NULL;

		// Call the Interface-Close callback
		if(bInvokeCallback && m_pfnInterfaceCloseCallback)
		{
			void (*pfnCallback)(const void*) =
				(void(*)(const void*))m_pfnInterfaceCloseCallback;
			pfnCallback(m_pParamCallback);
		}
	}

	return TRUE;
}

//-----------------------------------------------------------------------------------
// ����˵��: ��������socket��OnConnect(..)
// ����˵��: 
// ����ֵ: �������
// ������: DavidHowe;  ����ʱ��: 2003-5-1 10:57:26 PM;  �ص�: Xi'an
// ��ע/�޸ļ�¼: 
//-----------------------------------------------------------------------------------
BOOL CCommInterface::ProcessSocketConnect(void)
{
    //
    // Start the send-command thread!
    //
    m_pSendCmdThrd = (CInterfaceSendCmdThread *) AfxBeginThread(
        RUNTIME_CLASS(CInterfaceSendCmdThread), 0, CREATE_SUSPENDED);
    m_pSendCmdThrd->SetInterfaceHandle(this);
    ::ResumeThread(m_pSendCmdThrd->m_hThread);
    BOOL bRet = ::PostThreadMessage(m_pSendCmdThrd->m_nThreadID,
        TM_START_SENDCOMMANDLOOP, 0, 0);
    while (! bRet)
    {
        Sleep(0);
        bRet = ::PostThreadMessage(m_pSendCmdThrd->m_nThreadID,
            TM_START_SENDCOMMANDLOOP, 0, 0);
    }
    
	// Call the Interface-Open callback
	if(m_pfnInterfaceOpenCallback)
	{
		void (*pfnCallback)(const void*) =
			(void(*)(const void*))m_pfnInterfaceOpenCallback;
		pfnCallback(m_pParamCallback);
	}

	return TRUE;
}

//-----------------------------------------------------------------------------------
// ����˵��: ��ȡ���ӵĿͻ��˵� IP �� port.
// ����˵��: 
// ����ֵ: 
// ������: liep;  ����ʱ��: 2003-11-24 15:55:45;  �ص�: xi'an
// ��ע/�޸ļ�¼: 
//-----------------------------------------------------------------------------------
void CCommInterface::GetSockName(CString& csIP, UINT& nPort) const
{
    m_pDataSock->GetPeerName(csIP, nPort);
}

//-----------------------------------------------------------------------------------
// Description: Create udp interface
// Parameters: 
// Return: 
// Author: David Howe;  Date: 2004-8-12 16:15:41;  Location: Office
// Misc: 
//-----------------------------------------------------------------------------------
BOOL CCommInterface::CreateInterface_Udp(UINT nPort,
                                         const void *pfnInterfaceOpenCallback,
                                         const void *pfnInterfaceCloseCallback,
                                         const void *pfnDataProcCallback,
                                         const void *pParamCallback)
{
    // Close prev
    CloseInterface();
    ASSERT(m_pUdpSock == NULL);

    // The Interface Data Proc must be a valid pointer
	ASSERT(pfnDataProcCallback != NULL);
    
    // Save the params
    m_nUdpPort = nPort;
    m_pfnInterfaceOpenCallback = (void *) pfnInterfaceOpenCallback;
    m_pfnInterfaceCloseCallback = (void *) pfnInterfaceCloseCallback;
    m_pfnDataProcCallback = (void *) pfnDataProcCallback;
	m_pParamCallback = (void *) pParamCallback;
    
    // Create the udp socket
    m_pUdpSock = new CInterfaceUdpSocket(this);
    ASSERT(m_pUdpSock);
    
    if (! m_pUdpSock->Create(m_nUdpPort, SOCK_DGRAM))
    {
        TRACE("Interface::Create udp socket failed, error code = %d\n", GetLastError());
        delete m_pUdpSock;
        m_pUdpSock = NULL;
        return FALSE;
    }

    //
    // Start the send-command thread!
    //
    m_pSendCmdThrd = (CInterfaceSendCmdThread *) AfxBeginThread(
        RUNTIME_CLASS(CInterfaceSendCmdThread), 0, CREATE_SUSPENDED);
    m_pSendCmdThrd->SetInterfaceHandle(this);
    ::ResumeThread(m_pSendCmdThrd->m_hThread);
    BOOL bRet = ::PostThreadMessage(m_pSendCmdThrd->m_nThreadID,
        TM_START_SENDCOMMANDLOOP, 0, 0);
    // D.H 2004-8-11 15:59:59
    // ��Ӷ�::PostThreadMessage()�ķ����жϴ���, ��������Dll������
    //  ʹ��CCommInterfaceʱ�����������������̵߳�bug.
    // {F598F388-14D8-464F-BE35-2CA67E1A978C}
    while (! bRet)
    {
        Sleep(0);
        bRet = ::PostThreadMessage(m_pSendCmdThrd->m_nThreadID,
            TM_START_SENDCOMMANDLOOP, 0, 0);
    }
    
    return TRUE;
}

//-----------------------------------------------------------------------------------
// Description: Close udp interface
// Parameters: 
// Return: 
// Author: David Howe;  Date: 2004-8-12 16:42:42;  Location: Office
// Misc: 
//-----------------------------------------------------------------------------------
BOOL CCommInterface::CloseInterface_Udp(BOOL bInvokeCallback)
{
    // Terminate send-command thread
    if(m_pSendCmdThrd)
    {
        ::TerminateThread(m_pSendCmdThrd->m_hThread, 0);
        delete m_pSendCmdThrd;
        m_pSendCmdThrd = NULL;
    }
    
    if(m_pUdpSock)
    {
        m_pUdpSock->Close();
        delete m_pUdpSock;
        m_pUdpSock = NULL;
        
        // Call the Interface-Close callback
        if(bInvokeCallback && m_pfnInterfaceCloseCallback)
        {
            void (*pfnCallback)(const void*) =
                (void(*)(const void*))m_pfnInterfaceCloseCallback;
            pfnCallback(m_pParamCallback);
        }
    }
    
    return TRUE;
}

//-----------------------------------------------------------------------------------
// Description: Set target host of udp socket
// Parameters: lpszTargetHost: new host address
// Return: previous host address
// Author: David Howe;  Date: 2004-8-12 17:32:41;  Location: Office
// Misc: 
//-----------------------------------------------------------------------------------
LPCTSTR CCommInterface::SetUdpTargetHost(LPCTSTR lpszTargetHost)
{
    CString csPrevHost = m_csTargetHost;
    m_csTargetHost = lpszTargetHost;

    return csPrevHost;
}

//-----------------------------------------------------------------------------------
// Description: Set target port of udp socket
// Parameters: 
// Return: 
// Author: David Howe;  Date: 2004-8-12 17:34:29;  Location: Office
// Misc: 
//-----------------------------------------------------------------------------------
UINT CCommInterface::SetUdpTargetPort(UINT nTargetPort)
{
    UINT nPrevPort = m_nTargetPort;
    m_nTargetPort = nTargetPort;

    return nPrevPort;
}

//-----------------------------------------------------------------------------------
// Description: ����Udp socket�����ݵ�����Ϣ
// Parameters: 
// Return: 
// Author: David Howe;  Date: 2004-8-13 11:10:25;  Location: Office
// Misc: 
//-----------------------------------------------------------------------------------
BOOL CCommInterface::ProcessUdpSocketReceiveFrom(void) const
{
    ASSERT(m_pUdpSock != NULL);

    BYTE byBuf[RECVBUF_SIZE];
    CString csTargetHost = _T("");
    UINT nTargetPort = 0;
    int nRecvLen = m_pUdpSock->ReceiveFrom(byBuf, RECVBUF_SIZE, csTargetHost, nTargetPort);

    if (nRecvLen > 0)
    {
        if (m_pfnDataProcCallback)
        {
            // ������Ҫ�ڷ��ص�����������Ͻ��շ�λ����Ϣ
            memmove(byBuf + csTargetHost.GetLength() + 1 + sizeof(DWORD), byBuf, nRecvLen);
            memcpy(byBuf, (LPCTSTR) csTargetHost, csTargetHost.GetLength());
            byBuf[csTargetHost.GetLength()] = '\0';
            memcpy(&byBuf[csTargetHost.GetLength() + 1], &nTargetPort, sizeof(DWORD));

            // Call callback function
            void (*pfnDataProc)(BYTE [], int, void *) =
                (void(*)(BYTE [], int, void *)) m_pfnDataProcCallback;
            pfnDataProc(byBuf, nRecvLen + csTargetHost.GetLength() + 1 + sizeof(DWORD),
                m_pParamCallback);
        }
        else ASSERT(FALSE);

        return TRUE;
    }
    else return FALSE;
}

/*************************************************************************************
** Description: ����E1ͨ�Žӿ�
** Parameters: 
** Return: 
** Author: lipf;  Date: 2003-12-23 13:53:05;  Location: Office
** Misc: 
**************************************************************************************/
BOOL CCommInterface::CreateInterface_E1(unsigned int nCardID,// E1 Card Num
                                        unsigned int nSlotID, // E1's Time Slot
                                        const void *pfnInterfaceOpenCallback,
                                        const void *pfnInterfaceCloseCallback,
                                        const void *pfnDataProcCallback,
                                        const void *pParamCallback)
{
#ifndef C7A43868_D29A_4404_9BFD_698A970F8614
    return FALSE;
#else
    ASSERT(m_nCommMode == INTERFACE_E1);
    if(m_nCommMode != INTERFACE_E1) return FALSE;
    // Close previous if open
    CloseInterface();
    ASSERT(m_hE1Handle == NULL);
    ASSERT(pfnDataProcCallback != NULL);
    if(nCardID < 1 || nCardID > 4) // nCardID is unsigned int , so it is always > 0
    {
        TRACE("Interface::E1 Card ID out of range: %d\n", nCardID);
        return FALSE;
    }
    if(nSlotID <= 0 || nSlotID > 31)
    {
        TRACE("Interface::E1 Time Slot ID out of range: %d\n", nSlotID);
        return FALSE;
    }
    // Save the params
    m_nCardID = nCardID;
    m_nSlotID = nSlotID;
    m_pfnInterfaceOpenCallback = (void *) pfnInterfaceOpenCallback;
    m_pfnInterfaceCloseCallback = (void *) pfnInterfaceCloseCallback;
    m_pfnDataProcCallback = (void *) pfnDataProcCallback;
    m_pParamCallback = (void *) pParamCallback;
    //
    // (Attempt to) open the E1 Card and Set the Param
    //
    CString csE1Card;
    csE1Card.Format("\\\\.\\CardtDevice%d",nCardID-1);
    m_hE1Handle = CreateFile(csE1Card,GENERIC_READ|GENERIC_WRITE,0,NULL,
        OPEN_EXISTING,FILE_FLAG_OVERLAPPED,NULL);
    if(m_hE1Handle == INVALID_HANDLE_VALUE)
    {
        TRACE("E1SC#%d����ʧ�ܡ�",nCardID);
        m_hE1Handle = NULL;
        return FALSE;
    }

    BYTE bChannelIndex = nSlotID-1; // ���ó�������ʱ϶1�� ʵ�ʴ�0
    unsigned long wLen;
    if (!DeviceIoControl(m_hE1Handle,IOCTL_SET_CHANNEL,&bChannelIndex,1,NULL,0,&wLen,NULL))
    {
        TRACE("E1SC#%d������ͨ��%dʧ�ܡ�",nCardID, nSlotID);
        CloseInterface();
        return FALSE;
    }
        
    SERIAL_BAUD_RATE BaudRate;
    BaudRate.BaudRate = 9600;
    if(!DeviceIoControl(m_hE1Handle, IOCTL_SERIAL_SET_BAUD_RATE, &BaudRate, sizeof(SERIAL_BAUD_RATE),NULL,0,&wLen,NULL))
    {
        TRACE("E1SC#%d������������ʧ�ܡ�",nCardID);
        CloseInterface();
        return false;
    }

    SERIAL_LINE_CONTROL slc;
    slc.WordLength = 8;
    slc.Parity = NO_PARITY;
    slc.StopBits = 1;
    if(!DeviceIoControl(m_hE1Handle, IOCTL_SERIAL_SET_LINE_CONTROL, &slc, sizeof(SERIAL_LINE_CONTROL), NULL,0,&wLen,NULL))
        
        //	if(!dsiSetChannelLineControl(hE1[nCard-1],nChl,&dlc))
    {
        TRACE("E1SC#%d��ʱ϶�߿�����ʧ�ܡ�",nCardID);
        CloseInterface();
        return FALSE;
    }

    SERIAL_TIMEOUTS tmout ={0};
    tmout.ReadIntervalTimeout=30;
    tmout.ReadTotalTimeoutConstant=6000;
    tmout.WriteTotalTimeoutConstant=0;
    
    if(!DeviceIoControl(m_hE1Handle, IOCTL_SERIAL_SET_TIMEOUTS, &tmout, sizeof(SERIAL_TIMEOUTS), NULL,0,&wLen,NULL))
    {
        TRACE("E1SC#%d��ʱ϶��ʱ����ʧ�ܡ�",nCardID);
        CloseInterface();
        return FALSE;
    }
    //
    // begin the read E1 thread
    //
    m_pReadE1Thrd = AfxBeginThread(ReadE1Thread, this);

    // begin the send cmd thread
    
    m_pSendCmdThrd = (CInterfaceSendCmdThread *) AfxBeginThread(
        RUNTIME_CLASS(CInterfaceSendCmdThread), 0, CREATE_SUSPENDED);
    m_pSendCmdThrd->SetInterfaceHandle(this);
    ::ResumeThread(m_pSendCmdThrd->m_hThread);
    BOOL bRet = ::PostThreadMessage(m_pSendCmdThrd->m_nThreadID,
        TM_START_SENDCOMMANDLOOP, 0, 0);
    while (! bRet)
    {
        Sleep(0);
        bRet = ::PostThreadMessage(m_pSendCmdThrd->m_nThreadID,
            TM_START_SENDCOMMANDLOOP, 0, 0);
    }
    
    // Call the Interface-Open callback
    if(m_pfnInterfaceOpenCallback)
    {
        void (*pfnCallback)(const void*) =
            (void(*)(const void*))m_pfnInterfaceOpenCallback;
        pfnCallback(m_pParamCallback);
    }       
    return TRUE;
#endif // C7A43868_D29A_4404_9BFD_698A970F8614
}

/*************************************************************************************
** Description: 
** Parameters: 
** Return: 
** Author: lipf;  Date: 2003-12-23 15:49:19;  Location: 
** Misc: 
**************************************************************************************/
BOOL CCommInterface::CloseInterface_E1(BOOL bInvokeCallback)
{
#ifndef C7A43868_D29A_4404_9BFD_698A970F8614
    return FALSE;
#else
    BOOL bRet = TRUE;
    if(m_hE1Handle != NULL)
    {
        // Terminate read-comm thread
        ASSERT(m_pReadE1Thrd != NULL);
        if(m_pReadE1Thrd)
        {
            if(m_pReadE1Thrd->m_hThread)
            {
                ::TerminateThread(m_pReadE1Thrd->m_hThread, 0);
                delete m_pReadE1Thrd;
                m_pReadE1Thrd = NULL;
            }
        }
        
        // Terminate send-command thread
        ASSERT(m_pSendCmdThrd != NULL);
        if(m_pSendCmdThrd)
        {
            ::TerminateThread(m_pSendCmdThrd->m_hThread, 0);
            ::CloseHandle(m_pSendCmdThrd->m_olWrite.hEvent); // Prevent handle leak
            delete m_pSendCmdThrd;
            m_pSendCmdThrd = NULL;
        }
        
        // Close commport
        ASSERT(m_hE1Handle != NULL);
        bRet = ::CloseHandle(m_hE1Handle);
        m_hE1Handle = NULL;
        
        // Close the event's handle (to prevent a handle leak)
        ASSERT(m_polReadE1 != NULL);
        if(m_polReadE1) ::CloseHandle(m_polReadE1->hEvent);
        
        // Call the Interface-Close callback
        if(bInvokeCallback && m_pfnInterfaceCloseCallback)
        {
            void (*pfnCallback)(const void*) =
                (void(*)(const void*))m_pfnInterfaceCloseCallback;
            pfnCallback(m_pParamCallback);
        }
        // Have a rest, or the same commport cannot be opened immediately
        Sleep(50);
    }
    
    if(! bRet) TRACE("Interface::Close interface failed.\n");
    
    return bRet;
#endif // C7A43868_D29A_4404_9BFD_698A970F8614
}





//-----------------------------------------------------------------------------------
///////////////////////////////////////////////////////////
// CInterfaceSendCmdThread���ʵ�ִ���
///////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------
IMPLEMENT_DYNCREATE(CInterfaceSendCmdThread, CWinThread)

CInterfaceSendCmdThread::CInterfaceSendCmdThread()
{
	m_pInfc = NULL;
}

CInterfaceSendCmdThread::~CInterfaceSendCmdThread()
{
}

BOOL CInterfaceSendCmdThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CInterfaceSendCmdThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CInterfaceSendCmdThread, CWinThread)
	//{{AFX_MSG_MAP(CInterfaceSendCmdThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_THREAD_MESSAGE(TM_START_SENDCOMMANDLOOP, Start_SendCommandLoop)
	ON_THREAD_MESSAGE(TM_START_LOOP_MODE_0, SendCommandLoop_Mode0)
	ON_THREAD_MESSAGE(TM_START_LOOP_MODE_1, SendCommandLoop_Mode1)
    ON_THREAD_MESSAGE(TM_START_LOOP_MODE_3, SendCommandLoop_Mode3)
    ON_THREAD_MESSAGE(TM_START_LOOP_MODE_4, SendCommandLoop_Mode4)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInterfaceSendCmdThread message handlers

//-----------------------------------------------------------------------------------
// ����˵��: ��������ѭ���ܵ����, ���򿪽ӿں��ʹ���Ϣ���������
// ����˵��: δ��
// ����ֵ: 
// ������: DavidHowe;  ����ʱ��: 2003-4-28 5:10:57 PM;  �ص�: Xi'an
// ��ע/�޸ļ�¼: 
//-----------------------------------------------------------------------------------
LRESULT CInterfaceSendCmdThread::Start_SendCommandLoop(WPARAM wParam, LPARAM lParam)
{
    UNUSED_ALWAYS(wParam);
    UNUSED_ALWAYS(lParam);

	switch(m_pInfc->GetCommMode())
	{
	case INTERFACE_COMMPORT: // RS232
		PostThreadMessage(TM_START_LOOP_MODE_0, 0, 0);
		break;
	case INTERFACE_TCPSERVER: // Tcp Server
		PostThreadMessage(TM_START_LOOP_MODE_1, 0, 0);
		break;
	case INTERFACE_TCPCLIENT: // Tcp Client
		PostThreadMessage(TM_START_LOOP_MODE_2, 0, 0);
		break;
    case INTERFACE_E1:
        PostThreadMessage(TM_START_LOOP_MODE_3, 0, 0);
        break;
    case INTERFACE_UDP:
        PostThreadMessage(TM_START_LOOP_MODE_4, 0, 0);
        break;
	default: break;
	}

    return 0L;
}

//-----------------------------------------------------------------------------------
// ����˵��: ����0 (RS232) �ķ���ѭ��
// ����˵��: δ��
// ����ֵ: 
// ������: DavidHowe;  ����ʱ��: 2003-4-28 5:09:51 PM;  �ص�: Xi'an
// ��ע/�޸ļ�¼: 
//-----------------------------------------------------------------------------------
LRESULT CInterfaceSendCmdThread::SendCommandLoop_Mode0(WPARAM wParam, LPARAM lParam)
{
    UNUSED_ALWAYS(wParam);
    UNUSED_ALWAYS(lParam);
    
	ASSERT(m_pInfc != NULL);
	HANDLE hCommPort = m_pInfc->GetCommPortHandle();
	ASSERT(hCommPort != NULL);

	BYTE byBuf[SENDBUF_SIZE];
	int nLen = 0;
	DWORD dwBytesWritten = 0;

	memset(&m_olWrite, 0, sizeof(OVERLAPPED));
	m_olWrite.hEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);

	///////////////////////////////////////////////////////////
	// Send-command loop
	///////////////////////////////////////////////////////////
	while(TRUE)
	{
		///////////////////////////////////////////////////////////
		// ȷ��������в������ڱ�����ʱ, ������ȡ�������
		///////////////////////////////////////////////////////////
		::WaitForSingleObject(m_pInfc->m_hEvtCanGetCmdToSend, INFINITE);

		if(m_pInfc->GetCmdToSend(byBuf, nLen))
		{
			if(! ::WriteFile(hCommPort, byBuf, nLen, &dwBytesWritten, &m_olWrite))
			{
				if(GetLastError() == ERROR_IO_PENDING)
				{
					if(::GetOverlappedResult(hCommPort, &m_olWrite, &dwBytesWritten, TRUE))
						::ResetEvent(m_olWrite.hEvent);
				}
			}

			// If sent successfully, delete the command
			if(dwBytesWritten == (DWORD) nLen)
			{
				if(! m_pInfc->DeleteFirstCmd())
					TRACE("Interface::Delete command failed.\n");
			}
			else
			{
				TRACE("Interface::Send data failed.\n");
			}
		}

        /* D.H 2003-11-20 I forgot this before */
        ::SetEvent(m_pInfc->m_hEvtCanGetCmdToSend);

		// Sleep for a while before sending next command (if any),
		//  to let the thread cool down.
		Sleep(m_pInfc->GetMinIntervalBetweenCmds());
	}

    return 0L;
}

//-----------------------------------------------------------------------------------
// ����˵��: ����1 (Tcp Server/Client) �ķ���ѭ��
// ����˵��: 
// ����ֵ: 
// ������: DavidHowe;  ����ʱ��: 2003-4-30 11:35:52 PM;  �ص�: Xi'an
// ��ע/�޸ļ�¼: 
//-----------------------------------------------------------------------------------
LRESULT CInterfaceSendCmdThread::SendCommandLoop_Mode1(WPARAM wParam, LPARAM lParam)
{
    UNUSED_ALWAYS(wParam);
    UNUSED_ALWAYS(lParam);
    
	ASSERT(m_pInfc != NULL);
	CInterfaceDataSocket *pSock = m_pInfc->GetDataSocket();
	ASSERT(pSock != NULL);

	BYTE byBuf[SENDBUF_SIZE];
	int nLen = 0;
	DWORD dwBytesWritten = 0;

	///////////////////////////////////////////////////////////
	// Send-command loop
	///////////////////////////////////////////////////////////
	while(TRUE)
	{
		///////////////////////////////////////////////////////////
		// ȷ��������в������ڱ�����ʱ, ������ȡ�������
		///////////////////////////////////////////////////////////
		::WaitForSingleObject(m_pInfc->m_hEvtCanGetCmdToSend, INFINITE);

        // D.H 2004-6-21 11:15:18
        // ����һ��������ɷ���ʧ�ܵ�����, ��Ӧ����OnSend()��������
        //  �¼����ж��Ƿ��ܹ���������, �Ա���socket����.
        // {A4F90344-73F1-48fa-8D18-760C1846B9EF}
		if(m_pInfc->GetCmdToSend(byBuf, nLen))
		{
            // D.H 2004-6-21 11:39:25
            // Wait for the event, untill we can send data.
            ::WaitForSingleObject(pSock->GetEvtHandle_SocketCanSend(), 500/* ms */);

			// Send data out
			// David 2003-5-2 1:08:26 AM
			dwBytesWritten = pSock->Send(byBuf, nLen);
            
			// If sent successfully, delete the command
			if(dwBytesWritten != SOCKET_ERROR)
			{
                if (dwBytesWritten != (DWORD) nLen) // ���ͳ��Ȳ���
                {
                    TRACE("Interface(TCP/IP)::Send %d bytes, while wanna send %d bytes.\n",
                        dwBytesWritten, nLen);
                }
				if(! m_pInfc->DeleteFirstCmd())
                {
					TRACE("Interface(TCP/IP)::Delete sent-out command failed.\n");
                }

                // If sent successfully, set the event so that we can send next packet.
                ::SetEvent(pSock->GetEvtHandle_SocketCanSend());
			}
			else // error sending!
			{
				TRACE("Interface(TCP/IP)::Sends data failed, GetLastError()=%d.\n",
                    GetLastError());
            }
		}
        
        /* D.H 2003-11-20 I forgot this before */
        ::SetEvent(m_pInfc->m_hEvtCanGetCmdToSend);

		// Sleep for a while before sending next command (if any),
		//  to let the thread cool down.
		Sleep(m_pInfc->GetMinIntervalBetweenCmds());
	}

    return 0L;
}

/*************************************************************************************
** Description: E1��������
** Parameters: 
** Return: 
** Author: lipf;  Date: 2003-12-23 15:42:46;  Location: Office
** Misc: 
**************************************************************************************/
LRESULT CInterfaceSendCmdThread::SendCommandLoop_Mode3(WPARAM wParam, LPARAM lParam)
{
#ifndef C7A43868_D29A_4404_9BFD_698A970F8614
    return 0L;
#else
    UNUSED_ALWAYS(wParam);
    UNUSED_ALWAYS(lParam);
    
    ASSERT(m_pInfc != NULL);
    HANDLE hE1 = m_pInfc->GetE1Handle();
    ASSERT(hE1 != NULL);
    
    BYTE byBuf[SENDBUF_SIZE];
    int nLen = 0;
    DWORD dwBytesWritten = 0;
    
    memset(&m_olWrite, 0, sizeof(OVERLAPPED));
    m_olWrite.hEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
    
    ///////////////////////////////////////////////////////////
    // Send-command loop
    ///////////////////////////////////////////////////////////
    while(TRUE)
    {
        ///////////////////////////////////////////////////////////
        // ȷ��������в������ڱ�����ʱ, ������ȡ�������
        ///////////////////////////////////////////////////////////
        ::WaitForSingleObject(m_pInfc->m_hEvtCanGetCmdToSend, INFINITE);
        
        if(m_pInfc->GetCmdToSend(byBuf, nLen))
        {
            if(! ::WriteFile(hE1, byBuf, nLen, &dwBytesWritten, &m_olWrite))
            {
                if(GetLastError() == ERROR_IO_PENDING)
                {
                    if(::GetOverlappedResult(hE1, &m_olWrite, &dwBytesWritten, TRUE))
                        ::ResetEvent(m_olWrite.hEvent);
                }
            }
            
            // If sent successfully, delete the command
            if(dwBytesWritten == (DWORD) nLen)
            {
                if(! m_pInfc->DeleteFirstCmd())
                    TRACE("Interface::Delete command failed.\n");
            }
            else
            {
                TRACE("Interface::Send data failed.\n");
            }
        }

        /* D.H 2003-11-20 I forgot this before */
        ::SetEvent(m_pInfc->m_hEvtCanGetCmdToSend);
        
        // Sleep for a while before sending next command (if any),
        //  to let the thread cool down.
        Sleep(m_pInfc->GetMinIntervalBetweenCmds());
    }

    return 0L;
#endif // C7A43868_D29A_4404_9BFD_698A970F8614
}

//-----------------------------------------------------------------------------------
// Description: Udp��������
// Parameters: 
// Return: 
// Author: David Howe;  Date: 2004-8-12 16:20:36;  Location: Office
// Misc: 
//-----------------------------------------------------------------------------------
LRESULT CInterfaceSendCmdThread::SendCommandLoop_Mode4(WPARAM wParam, LPARAM lParam)
{
    UNUSED_ALWAYS(wParam);
    UNUSED_ALWAYS(lParam);

    ASSERT(m_pInfc != NULL);
    CInterfaceUdpSocket *pUdpSock = m_pInfc->GetUdpSocket();
    ASSERT(pUdpSock != NULL);

    BYTE byBuf[SENDBUF_SIZE];
    int nLen = 0;
    DWORD dwBytesWritten = 0;

    ////////////////////////////////////////////////////////////
    // Send command loop
    ////////////////////////////////////////////////////////////
    while (TRUE)
    {
        ///////////////////////////////////////////////////////////
        // ȷ��������в������ڱ�����ʱ, ������ȡ�������
        ///////////////////////////////////////////////////////////
		::WaitForSingleObject(m_pInfc->m_hEvtCanGetCmdToSend, INFINITE);

        if (m_pInfc->GetCmdToSend(byBuf, nLen))
        {
            // ��byBuf�н������͵�Ŀ����Ϣ
            CString csTargetHost;
            UINT nTargetPort = 0;
            csTargetHost.Format("%s", byBuf);
            memcpy(&nTargetPort, &byBuf[csTargetHost.GetLength() + 1], sizeof(DWORD));

            // Send data out
            dwBytesWritten = pUdpSock->SendTo(byBuf + csTargetHost.GetLength() + 1 +
                sizeof(DWORD), nLen - csTargetHost.GetLength() - 1 - sizeof(DWORD),
                nTargetPort, csTargetHost.IsEmpty() ? NULL : (LPCTSTR) csTargetHost);

            // If sent successfully, delete the command
            if(dwBytesWritten != SOCKET_ERROR)
            {
                if (dwBytesWritten != 
                    (DWORD) nLen - csTargetHost.GetLength() - 1 - sizeof(DWORD)) // ���ͳ��Ȳ���
                {
                    TRACE("Interface(UDP)::Send %d bytes, while wanna send %d bytes.\n",
                        dwBytesWritten, nLen - csTargetHost.GetLength() - 1 - sizeof(DWORD));
                }
                if(! m_pInfc->DeleteFirstCmd())
                {
                    TRACE("Interface(UDP)::Delete sent-out command failed.\n");
                }
            }
            else // error sending!
            {
                TRACE("Interface(UDP)::Sends data failed, GetLastError()=%d.\n",
                    GetLastError());
            }
        }

        ::SetEvent(m_pInfc->m_hEvtCanGetCmdToSend);

        Sleep(m_pInfc->GetMinIntervalBetweenCmds());
    }

    return 0L;
}








//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////
// CInterfaceListenSocket���ʵ�ִ���
///////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------

CInterfaceListenSocket::CInterfaceListenSocket()
{
    // Call another constructor
    ASSERT(FALSE);
}

CInterfaceListenSocket::CInterfaceListenSocket(CCommInterface *pInfc)
{
	ASSERT(pInfc != NULL);
	m_pInfc = pInfc;
}

CInterfaceListenSocket::~CInterfaceListenSocket()
{
}

// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CInterfaceListenSocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CInterfaceListenSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CInterfaceListenSocket member functions

void CInterfaceListenSocket::OnAccept(int nErrorCode) 
{
	if(! nErrorCode)
	{
		m_pInfc->ProcessSocketAccept();
	}

	CAsyncSocket::OnAccept(nErrorCode);
}








//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
///////////////////////////////////////////////////////////
// CInterfaceDataSocket���ʵ�ִ���
///////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------

CInterfaceDataSocket::CInterfaceDataSocket()
{
    // Do not call this, call another constructor
    ASSERT(FALSE);
}

CInterfaceDataSocket::CInterfaceDataSocket(CCommInterface *pInfc)
{
	ASSERT(pInfc != NULL);
	m_pInfc = pInfc;

    // D.H 2004-6-21 11:25:53
    // Init event
    m_hEvtSocketCanSend = ::CreateEvent(NULL, FALSE, FALSE, NULL);
}

CInterfaceDataSocket::~CInterfaceDataSocket()
{
    ::CloseHandle(m_hEvtSocketCanSend);
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CInterfaceDataSocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CInterfaceDataSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

void CInterfaceDataSocket::OnReceive(int nErrorCode) 
{
	if(! nErrorCode)
	{
		m_pInfc->ProcessSocketReceive();
	}

	CAsyncSocket::OnReceive(nErrorCode);
}

void CInterfaceDataSocket::OnConnect(int nErrorCode) 
{
	if(! nErrorCode || nErrorCode == WSAEISCONN)
	{
		m_pInfc->ProcessSocketConnect();
	}
	
	CAsyncSocket::OnConnect(nErrorCode);
}

void CInterfaceDataSocket::OnClose(int nErrorCode) 
{
	m_pInfc->ProcessSocketClose();

	CAsyncSocket::OnClose(nErrorCode);
}

// D.H 2004-6-21 11:32:58
// To know when I could send data with a Send() call.
void CInterfaceDataSocket::OnSend(int nErrorCode)
{
    // �¼����ź�̬, �������̷߳���
    SetEvent(m_hEvtSocketCanSend);

    CAsyncSocket::OnSend(nErrorCode);
}






//-------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////
// D.H 2004-8-12
// CInterfaceUdpSocket���ʵ�ִ���
////////////////////////////////////////////////////////////

CInterfaceUdpSocket::CInterfaceUdpSocket()
{
    // Do not call this, call another constructor
    ASSERT(FALSE);
}

CInterfaceUdpSocket::CInterfaceUdpSocket(CCommInterface *pInfc)
{
    ASSERT(pInfc != NULL);
    m_pInfc = pInfc;
}

CInterfaceUdpSocket::~CInterfaceUdpSocket()
{
}

////////////////////////////////////////////////////////////
// Do not edit the following lines, which are needed by ClassWizard.
#if 0
    BEGIN_MESSAGE_MAP(CInterfaceUdpSocket, CAsyncSocket)
    //{{AFX_MSG_MAP(CInterfaceUdpSocket)
    //}}AFX_MSG_MAP
    END_MESSAGE_MAP()
#endif	// 0

void CInterfaceUdpSocket::OnReceive(int nErrorCode) 
{
    if(! nErrorCode)
    {
        m_pInfc->ProcessUdpSocketReceiveFrom();
    }
    
    CAsyncSocket::OnReceive(nErrorCode);
}


//-----------------------------------------------------------------------------------
// End of file

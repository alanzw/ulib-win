/************************************************************************************
 * 文件名: CommInterface.cpp
 * 版权: Copyright (C) DavidHowe
 * 作者: David Howe
 * E-mail: davidhowe@sina.com
 * Mobile Phone: ***********
 * 创建时间: 2003-4-23 10:44:41 AM;  地点: Xi'an
 * 文件说明: 创建这个类, 使得各种常用通信方式在使用时达到统一.
 * 修改记录: -------------- +: 新增  o: 修改  -: 删除 --------------
       2003-4-23: + 创建文件.
	   2003-4-30: + 基本完成了串口方式的代码(m_nCommMode=0).
	   2003-5-2:  + 基本完成了TCP方式的代码(m_nCommMode=1,2).
       2003-6-?:  o 增加了对WSAEISCONN的判断.
       2003-7-?:  o 修正了发数线程启动时间过迟的bug.
       2003-11-20: o 修正了一个可能会使命令堆积的bug.
       2003-1-7:  0 修改E1方式打开时的时隙号问题, 配置时隙号码-1
       2004-3-31: 修改了事件m_hEvtCanGetCmdToSend的创建方式, 改为自动Reset.
                  并在几个地方删掉了不必要的ResetEvent()
       2004-4-2:  修改了判断TCP/IP方式下发送数据失败的代码.
       2004-6-21: 使用OnSend()来判断是否能够继续发送数据.
       2004-8-11:
               添加对::PostThreadMessage()的返回判断处理, 以修正在Dll工程中
                使用CCommInterface时不能启动发送数据线程的bug.
                {F598F388-14D8-464F-BE35-2CA67E1A978C}
       2004-8-12: 添加宏判断, 使可以屏蔽E1的代码.
       2005-6-14: 为CloseInterface()函数添加默认参数, 决定是否需要调用回调.
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
// 函数说明: 串口收数线程函数
// 参数说明: 接口实例指针
// 返回值: No use
// 创建人: DavidHowe;  创建时间: 2003-4-28 4:10:42 PM;  地点: Xi'an
// 备注/修改记录: 
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
** Description: E1读线程
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
    olE1Read.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);	// 重叠结构初始化
    pInfc->SaveOverlappedReadE1(&olE1Read);    
    // Get the E1's handle
    HANDLE hE1 = pInfc->GetE1Handle();

    if(!DeviceIoControl(hE1, IOCTL_SERIAL_SET_WAIT_MASK, &rxMask,
        sizeof(ULONG),NULL, 0,&nOutput,NULL))
    {
        TRACE("E1SCSetEvent 失败\n");
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
            if(GetLastError()!=ERROR_IO_PENDING)	// 串口异常处理
            {
                continue;
            }			
            
            ::WaitForSingleObject(olE1Read.hEvent, INFINITE);	// 等待数据到
            ::ResetEvent(olE1Read.hEvent);
                      
            if(dwEvent & EV_RXCHAR)
            {
                if (!ReadFile(hE1, byRecvBuf, RECVBUF_SIZE, &dwBytesRead, &olE1Read))	// 读整个数据包
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
                if (!ReadFile(hE1, byRecvBuf, RECVBUF_SIZE, &dwBytesRead, &olE1Read))	// 读整个数据包
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
	// 创建待发送命令队列
	//
	m_pCmdListHead = new COMMAND_LIST_NODE;
	m_pCmdListHead->pbyCommand = NULL;
	m_pCmdListHead->nLenOfCmd = 0;
	m_pCmdListHead->pNext = NULL;
	m_pCmdListTail = m_pCmdListHead; // 初始为空, 尾指向头
}

CCommInterface::~CCommInterface()
{
	// 关闭接口
	CloseInterface();

	//
	// 释放命令队列
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

	// 删除
	::CloseHandle(m_hEvtCanGetCmdToSend);
	::CloseHandle(m_hEvtCanAddCmdToSend);
}

//-----------------------------------------------------------------------------------
// 函数说明: 获取版本信息
// 参数说明: 是否显示版本消息
// 返回值: 版本号(高16bits是主版本号, 低16bits是次版本号)
// 创建人: DavidHowe;  创建时间: 2003-5-4 12:56:06;  地点: Xi'an
// 备注/修改记录: 
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
// 函数说明: 关闭接口(根据接口类型调用相应的函数)
// 参数说明: 
// 返回值: 操作结果
// 创建人: DavidHowe;  创建时间: 2003-4-30 9:42:31 AM;  地点: Xi'an
// 备注/修改记录: 用统一的函数关闭接口.
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
// 函数说明: 添加该命令到发送队列的队尾
// 参数说明: 命令buf, 命令长度
// 返回值: 操作结果
// 创建人: DavidHowe;  创建时间: 2003-4-23 3:53:44 PM;  地点: Xi'an
// 备注/修改记录: 
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
        // 注意, 对于每条命令, 都要保存发送目的信息
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
	// 使用事件, 为了在添加命令的过程中不让发送线程取命令
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

	// 事件置信号态
	::SetEvent(m_hEvtCanGetCmdToSend);
    ::SetEvent(m_hEvtCanAddCmdToSend);

	return TRUE;
}

//-----------------------------------------------------------------------------------
// 函数说明: 插入一条命令到队头, 优先发送该条命令
// 参数说明: 命令buf, 命令长度
// 返回值: 操作结果
// 创建人: DavidHowe;  创建时间: 2003-4-23 4:30:19 PM;  地点: Xi'an
// 备注/修改记录: 
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
        // 注意, 对于每条命令, 都要保存发送目的信息
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
	// 使用事件, 为了在添加命令的过程中不让发送线程取命令
	///////////////////////////////////////////////////////////

	//
	// Insert into list
	//
	pCmdNode->pNext = m_pCmdListHead->pNext;
	m_pCmdListHead->pNext = pCmdNode;
	if(m_pCmdListTail == m_pCmdListHead) m_pCmdListTail = pCmdNode;

	// 事件置信号态
	::SetEvent(m_hEvtCanGetCmdToSend);
    ::SetEvent(m_hEvtCanAddCmdToSend);

	return TRUE;
}

//-----------------------------------------------------------------------------------
// 函数说明: 从命令队列中取出第一条命令
// 参数说明: 输出命令的缓冲区, 输出命令的长度
// 返回值: 操作结果
// 创建人: DavidHowe;  创建时间: 2003-4-28 5:14:47 PM;  地点: Xi'an
// 备注/修改记录: 
//-----------------------------------------------------------------------------------
BOOL CCommInterface::GetCmdToSend(BYTE byBuf[], int& nLen)
{
    ::WaitForSingleObject(m_hEvtCanAddCmdToSend, INFINITE);
	// 此时不能进行增加命令操作
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
// 函数说明: 删掉第一条命令 (已成功发送时调用一次)
// 参数说明: 
// 返回值: 操作结果
// 创建人: DavidHowe;  创建时间: 2003-4-28 6:29:07 PM;  地点: Xi'an
// 备注/修改记录: 
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
	// 删除命令的过程中不能添加或取出命令
	///////////////////////////////////////////////////////////

    ::WaitForSingleObject(m_hEvtCanAddCmdToSend, INFINITE);
	// 事件置非信号态
//DEL 	::ResetEvent(m_hEvtCanGetCmdToSend);
	::ResetEvent(m_hEvtCanAddCmdToSend);

	// Head move on
	COMMAND_LIST_NODE *p;
	p = m_pCmdListHead->pNext;
	m_pCmdListHead->pNext = m_pCmdListHead->pNext->pNext;
	if(p->pbyCommand) delete p->pbyCommand;
	delete p;

	// 事件置信号态
	::SetEvent(m_hEvtCanGetCmdToSend);
	::SetEvent(m_hEvtCanAddCmdToSend);

	return TRUE;
}

//-----------------------------------------------------------------------------------
// 函数说明: 删除所有命令
// 参数说明: 
// 返回值: 
// 创建人: DavidHowe;  创建时间: 2003-4-29 5:02:33 PM;  地点: Xi'an
// 备注/修改记录: 
//-----------------------------------------------------------------------------------
void CCommInterface::DeleteAllCmds(void)
{
	ASSERT(this != NULL);

	///////////////////////////////////////////////////////////
	// 删除命令的过程中不能添加或取出命令
	///////////////////////////////////////////////////////////

    ::WaitForSingleObject(m_hEvtCanAddCmdToSend, INFINITE);
	// 事件置非信号态
//DEL 	::ResetEvent(m_hEvtCanGetCmdToSend);
	::ResetEvent(m_hEvtCanAddCmdToSend);

	COMMAND_LIST_NODE *p, *q;
	p = m_pCmdListHead->pNext;

	if(! p) // Already empty
	{
		// 事件置信号态
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

	// 事件置信号态
	::SetEvent(m_hEvtCanGetCmdToSend);
	::SetEvent(m_hEvtCanAddCmdToSend);
}

//-----------------------------------------------------------------------------------
// 函数说明: 串口连接模式下, 打开接口的函数
// 参数说明: 串口号(从1开始); 此串口收到数据时的回调函数; 备用参数
// 返回值: 操作结果
// 创建人: DavidHowe;  创建时间: 2003-4-23 5:54:37 PM;  地点: Xi'an
// 备注/修改记录: 要保证最多只有一个串口被打开
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
        // 添加对::PostThreadMessage()的返回判断处理, 以修正在Dll工程中
        //  使用CCommInterface时不能启动发送数据线程的bug.
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
// 函数说明: 串口连接模式下, 关闭串口
// 参数说明: 
// 返回值: 操作结果
// 创建人: DavidHowe;  创建时间: 2003-4-24 6:10:00 PM;  地点: Xi'an
// 备注/修改记录: 
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
// 函数说明: 配置串口参数
// 参数说明: DCB
// 返回值: 操作结果
// 创建人: DavidHowe;  创建时间: 2003-4-28 4:32:20 PM;  地点: Xi'an
// 备注/修改记录: 
//-----------------------------------------------------------------------------------
BOOL CCommInterface::SetCommState(const DCB *pdcb) const
{
	ASSERT(m_nCommMode == INTERFACE_COMMPORT);
	ASSERT(m_hCommPort != NULL);
	ASSERT(pdcb != NULL);
	if(m_nCommMode != INTERFACE_COMMPORT) return FALSE;
	if(m_hCommPort == NULL) return FALSE;
	if(pdcb == NULL) return FALSE;

	// 暂停线程
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

	// 恢复线程
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
// 函数说明: 获取串口参数信息
// 参数说明: 传入一个DCB的指针
// 返回值: 操作结果
// 创建人: DavidHowe;  创建时间: 2003-4-30 10:19:58 AM;  地点: Xi'an
// 备注/修改记录: 
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
// 函数说明: 设置串口超时参数
// 参数说明: COMMTIMEOUTS
// 返回值: 操作结果
// 创建人: DavidHowe;  创建时间: 2003-4-30 11:19:11 AM;  地点: Xi'an
// 备注/修改记录: 
//-----------------------------------------------------------------------------------
BOOL CCommInterface::SetCommTimeouts(const COMMTIMEOUTS *ptmo) const
{
	ASSERT(m_nCommMode == INTERFACE_COMMPORT);
	ASSERT(m_hCommPort != NULL);
	ASSERT(ptmo != NULL);
	if(m_nCommMode != INTERFACE_COMMPORT) return FALSE;
	if(m_hCommPort == NULL) return FALSE;
	if(ptmo == NULL) return FALSE;

	// 暂停线程
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

	// 恢复线程
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
// 函数说明: 获取串口超时参数
// 参数说明: 传入一个COMMTIMEOUTS指针
// 返回值: 操作结果
// 创建人: DavidHowe;  创建时间: 2003-4-30 11:25:52 AM;  地点: Xi'an
// 备注/修改记录: 
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
// 函数说明: TCP Server模式下, 打开接口的函数
// 参数说明: 监听端口; 接口打开时的回调函数; 接口关闭时的回调函数;
//			 接口收到数据时的回调函数; 备用参数;
// 返回值: 操作结果
// 创建人: DavidHowe;  创建时间: 2003-4-30 2:26:08 PM;  地点: Xi'an
// 备注/修改记录: 
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
// 函数说明: 关闭接口 (Tcp Server 模式)
// 参数说明: 
// 返回值: 
// 创建人: DavidHowe;  创建时间: 2003-4-30 4:44:33 PM;  地点: Xi'an
// 备注/修改记录: 
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
// 函数说明: 处理监听socket的OnAccept(..)
// 参数说明: 
// 返回值: 处理结果
// 创建人: DavidHowe;  创建时间: 2003-4-30 5:39:50 PM;  地点: Xi'an
// 备注/修改记录: 
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
// 函数说明: 处理数据socket的OnReceive(..)
// 参数说明: 
// 返回值: 处理结果
// 创建人: DavidHowe;  创建时间: 2003-4-30 10:27:28 PM;  地点: Xi'an
// 备注/修改记录: 
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
// 函数说明: 处理数据socket的OnClose(..)
// 参数说明: 
// 返回值: 处理结果
// 创建人: DavidHowe;  创建时间: 2003-5-1 12:19:30 AM;  地点: Xi'an
// 备注/修改记录: 
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
// 函数说明: TCP Client模式下, 打开接口的函数
// 参数说明: 服务端IP; 服务端监听端口; 接口打开时的回调函数; 接口关闭时的回调函数;
//			 接口收到数据时的回调函数; 备用参数;
// 返回值: 操作结果
// 创建人: DavidHowe;  创建时间: 2003-5-1 6:59:38 PM;  地点: Xi'an
// 备注/修改记录: 
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
// 函数说明: 关闭接口 (Tcp Client 模式)
// 参数说明: 
// 返回值: 操作结果
// 创建人: DavidHowe;  创建时间: 2003-5-1 10:55:33 PM;  地点: Xi'an
// 备注/修改记录: 
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
// 函数说明: 处理数据socket的OnConnect(..)
// 参数说明: 
// 返回值: 操作结果
// 创建人: DavidHowe;  创建时间: 2003-5-1 10:57:26 PM;  地点: Xi'an
// 备注/修改记录: 
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
// 函数说明: 获取连接的客户端的 IP 和 port.
// 参数说明: 
// 返回值: 
// 创建人: liep;  创建时间: 2003-11-24 15:55:45;  地点: xi'an
// 备注/修改记录: 
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
    // 添加对::PostThreadMessage()的返回判断处理, 以修正在Dll工程中
    //  使用CCommInterface时不能启动发送数据线程的bug.
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
// Description: 处理Udp socket的数据到达消息
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
            // 我们需要在返回的数据里面带上接收方位置信息
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
** Description: 创建E1通信接口
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
        TRACE("E1SC#%d卡打开失败。",nCardID);
        m_hE1Handle = NULL;
        return FALSE;
    }

    BYTE bChannelIndex = nSlotID-1; // 配置程序配置时隙1， 实际打开0
    unsigned long wLen;
    if (!DeviceIoControl(m_hE1Handle,IOCTL_SET_CHANNEL,&bChannelIndex,1,NULL,0,&wLen,NULL))
    {
        TRACE("E1SC#%d卡设置通道%d失败。",nCardID, nSlotID);
        CloseInterface();
        return FALSE;
    }
        
    SERIAL_BAUD_RATE BaudRate;
    BaudRate.BaudRate = 9600;
    if(!DeviceIoControl(m_hE1Handle, IOCTL_SERIAL_SET_BAUD_RATE, &BaudRate, sizeof(SERIAL_BAUD_RATE),NULL,0,&wLen,NULL))
    {
        TRACE("E1SC#%d卡波特率设置失败。",nCardID);
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
        TRACE("E1SC#%d卡时隙线控设置失败。",nCardID);
        CloseInterface();
        return FALSE;
    }

    SERIAL_TIMEOUTS tmout ={0};
    tmout.ReadIntervalTimeout=30;
    tmout.ReadTotalTimeoutConstant=6000;
    tmout.WriteTotalTimeoutConstant=0;
    
    if(!DeviceIoControl(m_hE1Handle, IOCTL_SERIAL_SET_TIMEOUTS, &tmout, sizeof(SERIAL_TIMEOUTS), NULL,0,&wLen,NULL))
    {
        TRACE("E1SC#%d卡时隙超时设置失败。",nCardID);
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
// CInterfaceSendCmdThread类的实现代码
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
// 函数说明: 发送命令循环总的入口, 当打开接口后发送此消息激活命令发送
// 参数说明: 未用
// 返回值: 
// 创建人: DavidHowe;  创建时间: 2003-4-28 5:10:57 PM;  地点: Xi'an
// 备注/修改记录: 
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
// 函数说明: 类型0 (RS232) 的发送循环
// 参数说明: 未用
// 返回值: 
// 创建人: DavidHowe;  创建时间: 2003-4-28 5:09:51 PM;  地点: Xi'an
// 备注/修改记录: 
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
		// 确定命令队列不是正在被操作时, 才允许取命令操作
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
// 函数说明: 类型1 (Tcp Server/Client) 的发送循环
// 参数说明: 
// 返回值: 
// 创建人: DavidHowe;  创建时间: 2003-4-30 11:35:52 PM;  地点: Xi'an
// 备注/修改记录: 
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
		// 确定命令队列不是正在被操作时, 才允许取命令操作
		///////////////////////////////////////////////////////////
		::WaitForSingleObject(m_pInfc->m_hEvtCanGetCmdToSend, INFINITE);

        // D.H 2004-6-21 11:15:18
        // 发现一个可能造成发送失败的问题, 我应该在OnSend()里面利用
        //  事件来判断是否能够发送数据, 以避免socket阻塞.
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
                if (dwBytesWritten != (DWORD) nLen) // 发送长度不对
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
** Description: E1发送数据
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
        // 确定命令队列不是正在被操作时, 才允许取命令操作
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
// Description: Udp发送数据
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
        // 确定命令队列不是正在被操作时, 才允许取命令操作
        ///////////////////////////////////////////////////////////
		::WaitForSingleObject(m_pInfc->m_hEvtCanGetCmdToSend, INFINITE);

        if (m_pInfc->GetCmdToSend(byBuf, nLen))
        {
            // 从byBuf中解析发送的目的信息
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
                    (DWORD) nLen - csTargetHost.GetLength() - 1 - sizeof(DWORD)) // 发送长度不对
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
// CInterfaceListenSocket类的实现代码
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
// CInterfaceDataSocket类的实现代码
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
    // 事件置信号态, 允许发送线程发送
    SetEvent(m_hEvtSocketCanSend);

    CAsyncSocket::OnSend(nErrorCode);
}






//-------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////
// D.H 2004-8-12
// CInterfaceUdpSocket类的实现代码
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

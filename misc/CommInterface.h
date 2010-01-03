/************************************************************************************
 * 文件名: CommInterface.h
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
                    加入E1通信方式代码(m_nCommMode=3)
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


#if !defined(AFX_COMMINTERFACE_H__1F66DAE4_677B_43D0_8384_ED8D698B9892__INCLUDED_)
#define AFX_COMMINTERFACE_H__1F66DAE4_677B_43D0_8384_ED8D698B9892__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxsock.h>	// Support for windows socket

///////////////////////////////////////////////////////////
// Some macros
///////////////////////////////////////////////////////////
#define INTERFACE_MINTYPE		0	// Min interface type
#define INTERFACE_COMMPORT		0	// Commport interface
#define INTERFACE_TCPSERVER		1	// TcpServer interface
#define INTERFACE_TCPCLIENT		2	// TcpClient interface
#define INTERFACE_E1            3   // E1 interface /* lipf 2003-12-23 */
#define INTERFACE_UDP           4   // Udp interface
#define INTERFACE_MAXTYPE		4	// Max interface type

// ============ D.H 2004-11-5 15:45:03 ============
// Both the send/recv buffer size are defined as
//  8k, if you want larger, just modify this.
// ================================================
#define RECVBUF_SIZE	(8 * 1024)	// receive buffer size 8k
#define SENDBUF_SIZE	(8 * 1024)	// send buffer size 8k

///////////////////////////////////////////////////////////
// Some user-defined messages
///////////////////////////////////////////////////////////
#define TM_START_SENDCOMMANDLOOP	(WM_USER + 1000)	// Thread message
#define TM_START_LOOP_MODE_0		(WM_USER + 1001)	// Thread message
#define TM_START_LOOP_MODE_1		(WM_USER + 1002)	// Thread message
#define TM_START_LOOP_MODE_2		(WM_USER + 1002)	// Thread message
														// Equal to TM_START_LOOP_MODE_1
														//  same process function
#define TM_START_LOOP_MODE_3        (WM_USER + 1003)    // For E1 /* lipf 2003-12-23 */
#define TM_START_LOOP_MODE_4        (WM_USER + 1004)    // Thread message

// Declare some classes first
class CInterfaceSendCmdThread;
class CInterfaceListenSocket;
class CInterfaceDataSocket;
class CInterfaceUdpSocket;

///////////////////////////////////////////////////////////
// Communication Interface Class(Main class)
///////////////////////////////////////////////////////////
class CCommInterface
{
	///////////////////////////////////////////////////////////
	// Used by all types of interfaces
	///////////////////////////////////////////////////////////
protected:
	// 命令队列节点的数据结构
	typedef struct _COMMAND_LIST_NODE
	{
		BYTE *pbyCommand;
		int nLenOfCmd;
		struct _COMMAND_LIST_NODE *pNext;
	}COMMAND_LIST_NODE;

public:
	CCommInterface(int nCommMode);
	virtual ~CCommInterface();


	// 获取接口类型
	int GetCommMode(void) const { return m_nCommMode; }
	// 添加该命令到发送队列
	BOOL AppendCmdToSend(const BYTE byBuf[], int nLen);
	// 优先发送该条命令
	BOOL InsertCmdToSend(const BYTE byBuf[], int nLen);
	// 删除所有命令
	void DeleteAllCmds(void);
	// 关闭接口
	BOOL CloseInterface(BOOL bInvokeCallback = TRUE);
	// 设置命令的最小发送间隔
    void SetMinIntervalBetweenCmds(DWORD dwInterval) {
        m_dwMinIntervalBetweenCmds = dwInterval;
    }
	// 获取命令的最小发送间隔
	DWORD GetMinIntervalBetweenCmds(void) const	{
        return m_dwMinIntervalBetweenCmds;
    }

	// 获取版本信息
	DWORD GetVersionInfo(BOOL bShowMsgbox) const;


protected:
	// 取出第一条命令, 去发送
	BOOL GetCmdToSend(BYTE byBuf[], int& nLen);
	// 删掉第一条命令 (已成功发送时调用一次)
	BOOL DeleteFirstCmd(void);

	// 获取回调函数地址
	void* GetDataProcAddr(void) const { return m_pfnDataProcCallback; }
	// 获取回调函数的第三个参数
	void* GetCallbackParam(void) const { return m_pParamCallback; }

	HANDLE m_hEvtCanGetCmdToSend; // 可以从命令队列中取出命令去发送
	HANDLE m_hEvtCanAddCmdToSend; // 可以向命令队列中增加命令

protected:
	int m_nCommMode; // 接口类型: (3, 4 are not completed yet.)
					 // 0 - RS232 (Duplex);
					 // 1 - WinSock TCP/IP Server;
					 // 2 - WinSock TCP/IP Client;
					 // 3 - Modem(Dialed thru RS232, duplex);
					 // 4 - RS485 (Semiduplex);

	DWORD m_dwMinIntervalBetweenCmds;	// 命令的最小发送间隔

	COMMAND_LIST_NODE *m_pCmdListHead, *m_pCmdListTail;	// 待发命令队列的头尾指针
	CInterfaceSendCmdThread *m_pSendCmdThrd;	// 发送命令的线程

	void *m_pfnInterfaceOpenCallback;	// 接口打开回调函数的地址
	void *m_pfnInterfaceCloseCallback;	// 接口关闭回调函数的地址
	void *m_pfnDataProcCallback;		// 数据处理回调函数的地址
	void *m_pParamCallback;				// 回调函数的第三个参数


//-----------------------------------------------------------------------------------
	///////////////////////////////////////////////////////////
	// m_nCommMode = 0
	// Following code is used by RS232 mode
	///////////////////////////////////////////////////////////
public:
	// 打开串口, 同时传递回调参数
	BOOL CreateInterface_CommPort(int nCommPort,
								  const void *pfnInterfaceOpenCallback,
								  const void *pfnInterfaceCloseCallback,
								  const void *pfnDataProcCallback,
								  const void *pParamCallback);
	// 配置及获取串口参数
	BOOL SetCommState(const DCB *pdcb) const;
	BOOL GetCommState(const DCB *pdcb) const;
	// 配置及获取串口超时
	BOOL SetCommTimeouts(const COMMTIMEOUTS *ptmo) const;
	BOOL GetCommTimeouts(const COMMTIMEOUTS *ptmo) const;

protected:
	// 获取串口句柄
	HANDLE GetCommPortHandle(void) const { return m_hCommPort; }
	// 关闭串口
	BOOL CloseInterface_CommPort(BOOL bInvokeCallback);

private:
	// 保存读串口线程的OVERLAPPED结构指针, user should not call this!
	void SaveOverlappedReadComm(const OVERLAPPED *polRead) {
        m_polReadComm = (OVERLAPPED *) polRead;
    }

protected:
	int m_nCommPort;				// Commport index (of coz, 1-based integer)
	HANDLE m_hCommPort;				// The commport handle

	OVERLAPPED *m_polReadComm;		// 读串口线程的OVERLAPPED结构指针
	CWinThread *m_pReadCommThrd;	// 读串口线程


//-----------------------------------------------------------------------------------
	///////////////////////////////////////////////////////////
	// m_nCommMode = 1
	// Used by WinSock Server mode
	///////////////////////////////////////////////////////////
public:
	// 打开接口socket, 并启动监听
	BOOL CreateInterface_TcpServer(unsigned int nListenPort,
								   const void *pfnInterfaceOpenCallback,
								   const void *pfnInterfaceCloseCallback,
								   const void *pfnDataProcCallback,
								   const void *pParamCallback);
protected:
	// 获取DataSocket
	CInterfaceDataSocket* GetDataSocket(void) const { return m_pDataSock; }
	// 关闭接口 (Tcp Server 模式)
	BOOL CloseInterface_TcpServer(BOOL bInvokeCallback);
	// 处理监听socket的OnAccept(..)
	BOOL ProcessSocketAccept(void);
	// 处理数据socket的OnReceive(..)
	BOOL ProcessSocketReceive(void) const;
	// 处理数据socket的OnClose(..)
	BOOL ProcessSocketClose(void);

protected:
	unsigned int m_nListenPort; // 启动监听的端口
	CInterfaceListenSocket *m_pListenSock; // 监听连接的Socket
	CInterfaceDataSocket *m_pDataSock; // 数据Socket


//-----------------------------------------------------------------------------------
	///////////////////////////////////////////////////////////
	// m_nCommMode = 2
	// Used by WinSock Client mode
	// (与模式1共用一些函数和变量)
	///////////////////////////////////////////////////////////
public:
	// 打开接口socket, 并进行连接
	BOOL CreateInterface_TcpClient(CString csServerIp, // Server's IP addr
								   unsigned int nListenPort, // Server's listen port
								   const void *pfnInterfaceOpenCallback,
								   const void *pfnInterfaceCloseCallback,
								   const void *pfnDataProcCallback,
								   const void *pParamCallback);

	// liep 2003-11-24 15:05:29
	void GetSockName(CString& csIP, UINT& nPort) const;
protected:
	// 关闭接口 (Tcp Client 模式)
	BOOL CloseInterface_TcpClient(BOOL bInvokeCallback);
	// 处理数据socket的OnConnect(..)
	BOOL ProcessSocketConnect(void);

protected:
	CString m_csServerIp;	// 服务端的IP地址

//-----------------------------------------------------------------------------------
    //////////////////////////////////////////////////////////////////////////
    // m_nCommMode = 3
    // Following code is used by E1 mode/* lipf 2003-12-23 */
    //////////////////////////////////////////////////////////////////////////
public:
    BOOL CreateInterface_E1(unsigned int nCardID, // E1 Card Num 1--4
                            unsigned int nSlotID, // E1's Time Slot 0--31
                            const void *pfnInterfaceOpenCallback,
                            const void *pfnInterfaceCloseCallback,
                            const void *pfnDataProcCallback,
                            const void *pParamCallback);
protected:
    // 获取E1句柄
     HANDLE GetE1Handle(void) const { return m_hE1Handle; }
    // 关闭E1
    BOOL CloseInterface_E1(BOOL bInvokeCallback);
    void SaveOverlappedReadE1(const OVERLAPPED *polRead) {
         m_polReadE1 = (OVERLAPPED *) polRead;
    }
protected:
    int m_nCardID;                  // range of 1 to 4
    int m_nSlotID;                  // range of 0 to 31
    HANDLE m_hE1Handle;
    OVERLAPPED *m_polReadE1;		// 读E1线程的OVERLAPPED结构指针
    CWinThread *m_pReadE1Thrd;	// 读E1线程



//-----------------------------------------------------------------------------------
    ////////////////////////////////////////////////////////////
    // D.H 2004-8-12 15:49:24
    // m_nCommMode = 4
    // Following code is used by Udp mode
    // 备注: 因为Udp socket可能每条命令发给不同的对象(Host&Port),
    //  所以需要把每条命令的发送目的记录下来, 可是记录在哪儿呢?
    //  终于, 决定把这个信息记录在每个数据包里面, 所以需要对函数
    //  AppendCmdToSend()和InsertCmdToSend()做一些修改...
    ////////////////////////////////////////////////////////////
public:
    BOOL CreateInterface_Udp(UINT nPort,
                             const void *pfnInterfaceOpenCallback,
                             const void *pfnInterfaceCloseCallback,
                             const void *pfnDataProcCallback,
                             const void *pParamCallback);
    LPCTSTR SetUdpTargetHost(LPCTSTR lpszTargetHost);
    UINT SetUdpTargetPort(UINT nTargetPort);
    LPCTSTR GetUdpTargetHost(void) const { return m_csTargetHost; }
    UINT GetUdpTargetPort(void) const { return m_nTargetPort; }

protected:
    UINT m_nUdpPort; // the udp port we opened
    CInterfaceUdpSocket *m_pUdpSock;
    CString m_csTargetHost; // the target host we will send data to
    UINT m_nTargetPort; // the port of the target host

protected:
    BOOL CloseInterface_Udp(BOOL bInvokeCallback);
    CInterfaceUdpSocket* GetUdpSocket(void) const { return m_pUdpSock; }
    // 处理Udp socket数据到
    BOOL ProcessUdpSocketReceiveFrom(void) const;
    
	///////////////////////////////////////////////////////////
	// Some friend classes and functions
	// David 2003-4-30 5:49:57 PM
	///////////////////////////////////////////////////////////
	friend class CInterfaceListenSocket;
	friend class CInterfaceDataSocket;
	friend class CInterfaceSendCmdThread;
    friend class CInterfaceUdpSocket;
	friend UINT ReadCommThread(LPVOID pParam);
    friend UINT ReadE1Thread(LPVOID pParam);
};

//-----------------------------------------------------------------------------------
///////////////////////////////////////////////////////////
// 发送命令的线程类(辅助的类, 串口操作时使用)
///////////////////////////////////////////////////////////
class CInterfaceSendCmdThread : public CWinThread
{
	DECLARE_DYNCREATE(CInterfaceSendCmdThread)
protected:
	CInterfaceSendCmdThread();           // protected constructor used by dynamic creation

// Attributes
protected:
	OVERLAPPED m_olWrite; // 声明为public是为了能从外部将其删除, 避免句柄泄漏
	CCommInterface *m_pInfc; // 接口的指针

// Operations
protected:
	// 设置接口的指针
    void SetInterfaceHandle(const CCommInterface *pInfc) {
        m_pInfc = (CCommInterface *) pInfc;
    }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInterfaceSendCmdThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CInterfaceSendCmdThread();

protected:
	// Generated message map functions
	//{{AFX_MSG(CInterfaceSendCmdThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	///////////////////////////////////////////////////////////
	// 启动发送命令循环
	///////////////////////////////////////////////////////////
	afx_msg LRESULT Start_SendCommandLoop(WPARAM wParam, LPARAM lParam);
	// 不同接口类型对应的发送命令循环函数
	afx_msg LRESULT SendCommandLoop_Mode0(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT SendCommandLoop_Mode1(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT SendCommandLoop_Mode3(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT SendCommandLoop_Mode4(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	///////////////////////////////////////////////////////////
	friend class CCommInterface;
};

//-----------------------------------------------------------------------------------
///////////////////////////////////////////////////////////
// 监听socket类 (辅助的类, Tcp server模式下使用)
///////////////////////////////////////////////////////////
class CInterfaceListenSocket : public CAsyncSocket
{
// Attributes
protected:
	CCommInterface *m_pInfc;

// Operations
public:
	CInterfaceListenSocket();
	CInterfaceListenSocket(CCommInterface *pInfc);
	virtual ~CInterfaceListenSocket();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInterfaceListenSocket)
	public:
	virtual void OnAccept(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CInterfaceListenSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};


//-----------------------------------------------------------------------------------
///////////////////////////////////////////////////////////
// 数据socket类 (辅助的类, Tcp Server/Client 模式下使用)
///////////////////////////////////////////////////////////
class CInterfaceDataSocket : public CAsyncSocket
{
// Attributes
protected:
	CCommInterface *m_pInfc;
    
protected:
    // D.H 2004-6-21 11:24:32
    // 用来判断是否能够调用Send()发送数据的事件
    HANDLE m_hEvtSocketCanSend;
public:
    HANDLE GetEvtHandle_SocketCanSend(void) const { return m_hEvtSocketCanSend; }

// Operations
public:
	CInterfaceDataSocket();
	CInterfaceDataSocket(CCommInterface *pInfc);
	virtual ~CInterfaceDataSocket();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInterfaceDataSocket)
	public:
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
    virtual void OnSend(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CInterfaceDataSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};


//-------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////
// Udp socket类 (INTERFACE_UDP模式下使用)
////////////////////////////////////////////////////////////
class CInterfaceUdpSocket : public CAsyncSocket
{
// Attributes
protected:
    CCommInterface *m_pInfc;

// Operations
public:
    CInterfaceUdpSocket();
    CInterfaceUdpSocket(CCommInterface *pInfc);
    virtual ~CInterfaceUdpSocket();
    
// Overrides
public:
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CInterfaceDataSocket)
public:
    virtual void OnReceive(int nErrorCode);
    //}}AFX_VIRTUAL
    
    // Generated message map functions
    //{{AFX_MSG(CInterfaceDataSocket)
    // NOTE - the ClassWizard will add and remove member functions here.
    //}}AFX_MSG
    
    // Implementation
protected:
};

#endif // !defined(AFX_COMMINTERFACE_H__1F66DAE4_677B_43D0_8384_ED8D698B9892__INCLUDED_)

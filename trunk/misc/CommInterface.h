/************************************************************************************
 * �ļ���: CommInterface.h
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
                    ����E1ͨ�ŷ�ʽ����(m_nCommMode=3)
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
	// ������нڵ�����ݽṹ
	typedef struct _COMMAND_LIST_NODE
	{
		BYTE *pbyCommand;
		int nLenOfCmd;
		struct _COMMAND_LIST_NODE *pNext;
	}COMMAND_LIST_NODE;

public:
	CCommInterface(int nCommMode);
	virtual ~CCommInterface();


	// ��ȡ�ӿ�����
	int GetCommMode(void) const { return m_nCommMode; }
	// ��Ӹ�������Ͷ���
	BOOL AppendCmdToSend(const BYTE byBuf[], int nLen);
	// ���ȷ��͸�������
	BOOL InsertCmdToSend(const BYTE byBuf[], int nLen);
	// ɾ����������
	void DeleteAllCmds(void);
	// �رսӿ�
	BOOL CloseInterface(BOOL bInvokeCallback = TRUE);
	// �����������С���ͼ��
    void SetMinIntervalBetweenCmds(DWORD dwInterval) {
        m_dwMinIntervalBetweenCmds = dwInterval;
    }
	// ��ȡ�������С���ͼ��
	DWORD GetMinIntervalBetweenCmds(void) const	{
        return m_dwMinIntervalBetweenCmds;
    }

	// ��ȡ�汾��Ϣ
	DWORD GetVersionInfo(BOOL bShowMsgbox) const;


protected:
	// ȡ����һ������, ȥ����
	BOOL GetCmdToSend(BYTE byBuf[], int& nLen);
	// ɾ����һ������ (�ѳɹ�����ʱ����һ��)
	BOOL DeleteFirstCmd(void);

	// ��ȡ�ص�������ַ
	void* GetDataProcAddr(void) const { return m_pfnDataProcCallback; }
	// ��ȡ�ص������ĵ���������
	void* GetCallbackParam(void) const { return m_pParamCallback; }

	HANDLE m_hEvtCanGetCmdToSend; // ���Դ����������ȡ������ȥ����
	HANDLE m_hEvtCanAddCmdToSend; // �����������������������

protected:
	int m_nCommMode; // �ӿ�����: (3, 4 are not completed yet.)
					 // 0 - RS232 (Duplex);
					 // 1 - WinSock TCP/IP Server;
					 // 2 - WinSock TCP/IP Client;
					 // 3 - Modem(Dialed thru RS232, duplex);
					 // 4 - RS485 (Semiduplex);

	DWORD m_dwMinIntervalBetweenCmds;	// �������С���ͼ��

	COMMAND_LIST_NODE *m_pCmdListHead, *m_pCmdListTail;	// ����������е�ͷβָ��
	CInterfaceSendCmdThread *m_pSendCmdThrd;	// ����������߳�

	void *m_pfnInterfaceOpenCallback;	// �ӿڴ򿪻ص������ĵ�ַ
	void *m_pfnInterfaceCloseCallback;	// �ӿڹرջص������ĵ�ַ
	void *m_pfnDataProcCallback;		// ���ݴ���ص������ĵ�ַ
	void *m_pParamCallback;				// �ص������ĵ���������


//-----------------------------------------------------------------------------------
	///////////////////////////////////////////////////////////
	// m_nCommMode = 0
	// Following code is used by RS232 mode
	///////////////////////////////////////////////////////////
public:
	// �򿪴���, ͬʱ���ݻص�����
	BOOL CreateInterface_CommPort(int nCommPort,
								  const void *pfnInterfaceOpenCallback,
								  const void *pfnInterfaceCloseCallback,
								  const void *pfnDataProcCallback,
								  const void *pParamCallback);
	// ���ü���ȡ���ڲ���
	BOOL SetCommState(const DCB *pdcb) const;
	BOOL GetCommState(const DCB *pdcb) const;
	// ���ü���ȡ���ڳ�ʱ
	BOOL SetCommTimeouts(const COMMTIMEOUTS *ptmo) const;
	BOOL GetCommTimeouts(const COMMTIMEOUTS *ptmo) const;

protected:
	// ��ȡ���ھ��
	HANDLE GetCommPortHandle(void) const { return m_hCommPort; }
	// �رմ���
	BOOL CloseInterface_CommPort(BOOL bInvokeCallback);

private:
	// ����������̵߳�OVERLAPPED�ṹָ��, user should not call this!
	void SaveOverlappedReadComm(const OVERLAPPED *polRead) {
        m_polReadComm = (OVERLAPPED *) polRead;
    }

protected:
	int m_nCommPort;				// Commport index (of coz, 1-based integer)
	HANDLE m_hCommPort;				// The commport handle

	OVERLAPPED *m_polReadComm;		// �������̵߳�OVERLAPPED�ṹָ��
	CWinThread *m_pReadCommThrd;	// �������߳�


//-----------------------------------------------------------------------------------
	///////////////////////////////////////////////////////////
	// m_nCommMode = 1
	// Used by WinSock Server mode
	///////////////////////////////////////////////////////////
public:
	// �򿪽ӿ�socket, ����������
	BOOL CreateInterface_TcpServer(unsigned int nListenPort,
								   const void *pfnInterfaceOpenCallback,
								   const void *pfnInterfaceCloseCallback,
								   const void *pfnDataProcCallback,
								   const void *pParamCallback);
protected:
	// ��ȡDataSocket
	CInterfaceDataSocket* GetDataSocket(void) const { return m_pDataSock; }
	// �رսӿ� (Tcp Server ģʽ)
	BOOL CloseInterface_TcpServer(BOOL bInvokeCallback);
	// �������socket��OnAccept(..)
	BOOL ProcessSocketAccept(void);
	// ��������socket��OnReceive(..)
	BOOL ProcessSocketReceive(void) const;
	// ��������socket��OnClose(..)
	BOOL ProcessSocketClose(void);

protected:
	unsigned int m_nListenPort; // ���������Ķ˿�
	CInterfaceListenSocket *m_pListenSock; // �������ӵ�Socket
	CInterfaceDataSocket *m_pDataSock; // ����Socket


//-----------------------------------------------------------------------------------
	///////////////////////////////////////////////////////////
	// m_nCommMode = 2
	// Used by WinSock Client mode
	// (��ģʽ1����һЩ�����ͱ���)
	///////////////////////////////////////////////////////////
public:
	// �򿪽ӿ�socket, ����������
	BOOL CreateInterface_TcpClient(CString csServerIp, // Server's IP addr
								   unsigned int nListenPort, // Server's listen port
								   const void *pfnInterfaceOpenCallback,
								   const void *pfnInterfaceCloseCallback,
								   const void *pfnDataProcCallback,
								   const void *pParamCallback);

	// liep 2003-11-24 15:05:29
	void GetSockName(CString& csIP, UINT& nPort) const;
protected:
	// �رսӿ� (Tcp Client ģʽ)
	BOOL CloseInterface_TcpClient(BOOL bInvokeCallback);
	// ��������socket��OnConnect(..)
	BOOL ProcessSocketConnect(void);

protected:
	CString m_csServerIp;	// ����˵�IP��ַ

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
    // ��ȡE1���
     HANDLE GetE1Handle(void) const { return m_hE1Handle; }
    // �ر�E1
    BOOL CloseInterface_E1(BOOL bInvokeCallback);
    void SaveOverlappedReadE1(const OVERLAPPED *polRead) {
         m_polReadE1 = (OVERLAPPED *) polRead;
    }
protected:
    int m_nCardID;                  // range of 1 to 4
    int m_nSlotID;                  // range of 0 to 31
    HANDLE m_hE1Handle;
    OVERLAPPED *m_polReadE1;		// ��E1�̵߳�OVERLAPPED�ṹָ��
    CWinThread *m_pReadE1Thrd;	// ��E1�߳�



//-----------------------------------------------------------------------------------
    ////////////////////////////////////////////////////////////
    // D.H 2004-8-12 15:49:24
    // m_nCommMode = 4
    // Following code is used by Udp mode
    // ��ע: ��ΪUdp socket����ÿ���������ͬ�Ķ���(Host&Port),
    //  ������Ҫ��ÿ������ķ���Ŀ�ļ�¼����, ���Ǽ�¼���Ķ���?
    //  ����, �����������Ϣ��¼��ÿ�����ݰ�����, ������Ҫ�Ժ���
    //  AppendCmdToSend()��InsertCmdToSend()��һЩ�޸�...
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
    // ����Udp socket���ݵ�
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
// ����������߳���(��������, ���ڲ���ʱʹ��)
///////////////////////////////////////////////////////////
class CInterfaceSendCmdThread : public CWinThread
{
	DECLARE_DYNCREATE(CInterfaceSendCmdThread)
protected:
	CInterfaceSendCmdThread();           // protected constructor used by dynamic creation

// Attributes
protected:
	OVERLAPPED m_olWrite; // ����Ϊpublic��Ϊ���ܴ��ⲿ����ɾ��, ������й©
	CCommInterface *m_pInfc; // �ӿڵ�ָ��

// Operations
protected:
	// ���ýӿڵ�ָ��
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
	// ������������ѭ��
	///////////////////////////////////////////////////////////
	afx_msg LRESULT Start_SendCommandLoop(WPARAM wParam, LPARAM lParam);
	// ��ͬ�ӿ����Ͷ�Ӧ�ķ�������ѭ������
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
// ����socket�� (��������, Tcp serverģʽ��ʹ��)
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
// ����socket�� (��������, Tcp Server/Client ģʽ��ʹ��)
///////////////////////////////////////////////////////////
class CInterfaceDataSocket : public CAsyncSocket
{
// Attributes
protected:
	CCommInterface *m_pInfc;
    
protected:
    // D.H 2004-6-21 11:24:32
    // �����ж��Ƿ��ܹ�����Send()�������ݵ��¼�
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
// Udp socket�� (INTERFACE_UDPģʽ��ʹ��)
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

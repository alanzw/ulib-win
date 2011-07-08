

//����KeepAlive    
BOOL bKeepAlive   =   TRUE;    
nRet = ::setsockopt(m_sockDesc,   SOL_SOCKET,   SO_KEEPALIVE,   (char*)&bKeepAlive,   sizeof(bKeepAlive));    
if   (nRet   !=0)    
{    
	sprintf(m_pszError,   "Winsock   error   :   %s   (Error   Code   %d)\n ",   "Socket   SetOpt   failed ",   WSAGetLastError());    
	return   FALSE;    
}    

//����KeepAlive���ʱ��ʹ���    
TCP_KEEPALIVE   inKeepAlive   =   {0};   //�������    
unsigned   long   ulInLen   =   sizeof(TCP_KEEPALIVE);        

TCP_KEEPALIVE   outKeepAlive   =   {0};   //�������    
unsigned   long   ulOutLen   =   sizeof(TCP_KEEPALIVE);        

unsigned   long   ulBytesReturn   =   0;    

//����socket��keep   aliveΪ10�룬���ҷ��ʹ���Ϊ3��    
inKeepAlive.onoff   =   1;      
inKeepAlive.keepaliveinterval   =   10000;   //����KeepAlive̽����ʱ����    
inKeepAlive.keepalivetime   =   3;   //��ʼ�״�KeepAlive̽��ǰ��TCP�ձ�ʱ��    

nRet   =   WSAIoctl(m_sockDesc,      
SIO_KEEPALIVE_VALS,    
(LPVOID)&inKeepAlive,    
ulInLen,    
(LPVOID)&outKeepAlive,    
ulOutLen,    
&ulBytesReturn,    
NULL,    
NULL);    
if(SOCKET_ERROR   ==   nRet)    
{    
	sprintf(m_pszError,   "Winsock   error   :   %s   (Error   Code   %d)\n ",   "Nonblocking   socket   call   error ",   WSAGetLastError());    
	return   FALSE;    
}   

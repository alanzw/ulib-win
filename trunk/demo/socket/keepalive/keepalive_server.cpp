

//设置KeepAlive    
BOOL bKeepAlive   =   TRUE;    
nRet = ::setsockopt(m_sockDesc,   SOL_SOCKET,   SO_KEEPALIVE,   (char*)&bKeepAlive,   sizeof(bKeepAlive));    
if   (nRet   !=0)    
{    
	sprintf(m_pszError,   "Winsock   error   :   %s   (Error   Code   %d)\n ",   "Socket   SetOpt   failed ",   WSAGetLastError());    
	return   FALSE;    
}    

//设置KeepAlive检测时间和次数    
TCP_KEEPALIVE   inKeepAlive   =   {0};   //输入参数    
unsigned   long   ulInLen   =   sizeof(TCP_KEEPALIVE);        

TCP_KEEPALIVE   outKeepAlive   =   {0};   //输出参数    
unsigned   long   ulOutLen   =   sizeof(TCP_KEEPALIVE);        

unsigned   long   ulBytesReturn   =   0;    

//设置socket的keep   alive为10秒，并且发送次数为3次    
inKeepAlive.onoff   =   1;      
inKeepAlive.keepaliveinterval   =   10000;   //两次KeepAlive探测间的时间间隔    
inKeepAlive.keepalivetime   =   3;   //开始首次KeepAlive探测前的TCP空闭时间    

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

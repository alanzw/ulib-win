#include <windows.h>
#include <winsock2.h>
#include <stdio.h>

#include "usocket.h"



USocket::USocket()
: m_socket(0)
{}

USocket::~USocket()
{}

int USocket::init()
{
    WSADATA wsadata;
    int nRet;
    if ( (nRet = ::WSAStartup(MAKEWORD(2, 2), &wsadata)) != 0 )
    {
        printf("WSAStartup failed: %d ", GetLastError());
        return nRet;
    }

    //Did we get the right Winsock version?
    if (wsadata.wVersion != 0x0202)
    {
        this->cleanup();
        return -1;
    }

    return 0;
}

int USocket::close()
{
    if (m_socket)
    {
        ::closesocket(m_socket);
    }
    return 0;
}


int USocket::cleanup()
{
    ::WSACleanup();
    return 0;
}


int USocket::send(SOCKET sock, const char *pack, int size)
{
    if (SOCKET_ERROR == ::send(sock, pack, size, 0))
    {
        printf("send failed: %d ", GetLastError());
        return SOCKET_ERROR;
    }

    return 0;
}

int USocket::send( const char *pack, int size )
{
    if (SOCKET_ERROR == ::send(m_socket, pack, size, 0))
    {
        printf("send failed: %d ", GetLastError());
        return SOCKET_ERROR;
    }

    return 0;
}

int USocket::create( int af, int type, int protocol )
{
    m_socket = ::socket(af, type, protocol);
    if (INVALID_SOCKET == m_socket)
    {
        this->close();
        return 1;
    }
    return 0;
}

//int USocket::create( const struct addrinfo *ai )
//{
//    m_socket = ::socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
//    if (INVALID_SOCKET == m_socket)
//    {
//        this->close();
//        return 1;
//    }
//    return 0;
//}

int USocket::bind( const struct sockaddr *s )
{
    if (::bind(m_socket, s, sizeof(struct sockaddr)) == SOCKET_ERROR)
    {
        closesocket(m_socket);
        return INVALID_SOCKET;
    }
    return 0;
}

int USocket::bind( int nPort )
{
    struct sockaddr_in s = {0};
    s.sin_family = AF_INET;
    s.sin_port = htons (nPort);   // Assign port to this socket
    s.sin_addr.s_addr = htonl (INADDR_ANY);   // No destination
    if (::bind(m_socket, (LPSOCKADDR)&s, sizeof(struct sockaddr_in)) == SOCKET_ERROR)
    {
        closesocket(m_socket);
        return INVALID_SOCKET;
    }
    return 0;
}

int USocket::listen( int nBacklog )
{
    if (SOCKET_ERROR == ::listen(m_socket, nBacklog))
    {
        printf("listen failed: %d ", GetLastError());
        return SOCKET_ERROR;
    }
    return 0;
}

int USocket::recv(SOCKET sock, char *buf, int nLen )
{
    int nRet = 0;
    if (SOCKET_ERROR == (nRet = ::recv(sock, buf, nLen, 0)))
    {
        printf("recv failed: %d ", GetLastError());
        return SOCKET_ERROR;
    }
    return nRet;
}

int USocket::recv( char *buf, int nLen )
{
    return this->recv(m_socket, buf, nLen);
}

int USocket::shutdown( int how )
{
    if (SOCKET_ERROR == ::shutdown(m_socket, how))
    {
        printf("shutdown failed: %d ", GetLastError());
        return SOCKET_ERROR;
    }
    return 0;
}

int USocket::gethostname( char *buf, int len )
{
    if (SOCKET_ERROR == ::gethostname(buf, len))
    {
        printf("gethostname failed: %d ", GetLastError());
        return SOCKET_ERROR;
    }
    return 0;
}

struct hostent * USocket::gethostbyname(const char *name)
{
    return ::gethostbyname(name);
}

struct hostent * USocket::gethostbyaddr(const char *addr, int len, int type)
{
    return ::gethostbyaddr(addr, len, type);
}

unsigned long USocket::inet_addr(const char *cp)
{
    return ::inet_addr(cp);
}

int USocket::connect( const struct sockaddr *s )
{
    if (SOCKET_ERROR == ::connect(m_socket, s, sizeof(struct sockaddr)))
    {
        printf("connect failed: %d ", GetLastError());
        return SOCKET_ERROR;
    }
    return 0;
}

int USocket::connect( const char *szhost, int nPort )
{
    SOCKADDR_IN target;
    target.sin_family = AF_INET;
    target.sin_addr.s_addr = inet_addr(szhost),
    target.sin_port = htons(nPort);
}

SOCKET USocket::accept( struct sockaddr *s /*= 0*/, int *len /*= 0*/ )
{
    SOCKET aSocket = ::accept(m_socket, s, len);
    if (INVALID_SOCKET == m_socket)
    {
        this->close();
        return INVALID_SOCKET;
    }
    return aSocket;
}

int USocket::WSAAsyncSelect( HWND hwnd, UINT uMsg, long lEvent )
{
    if (SOCKET_ERROR == ::WSAAsyncSelect(m_socket, hwnd, uMsg, lEvent))
    {
        printf("WSAAsyncSelect failed: %d ",  WSAGetLastError());
        return SOCKET_ERROR;
    }
    return 0;
}

int USocket::setsockopt( int level, int optname, const char *optval, int optlen )
{
    if (SOCKET_ERROR == ::setsockopt(m_socket, level, optname, optval, optlen))
    {
        printf("WSAAsyncSelect failed: %d ",  WSAGetLastError());
        return SOCKET_ERROR;
    }
    return 0;
}


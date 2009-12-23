#ifndef U_SOCKET_H
#define U_SOCKET_H

#include "ulib.h"

class ULIB_API USocket
{
public:
    enum SockError {
        SERR_CREATE,
        SERR_INIT,
        SERR_CLOSE,
        SERR_CLEANUP,
        SERR_BIND,
        SERR_LISTEN,
        SERR_CONNECT,
        SERR_SEND,
        SERR_RECV,
        SERR_ACCEPT,
        SERR_SHUTDOWN
    };
public:
    USocket();
    ~USocket();

    operator SOCKET()
    {
        return m_socket;
    }

    int init();
    int close();
    int cleanup();

    int create(int af = AF_INET, int type = SOCK_STREAM, int protocol = IPPROTO_TCP);
    //int create(const struct addrinfo *ai);
    //
    int bind(const struct sockaddr *s);
    int bind(int nPort);
    int listen(int nBacklog = 1);
    //
    int connect(const struct sockaddr *s);
    int connect(const char *szhost, int nPort);

    int send(SOCKET sock, const char *pack, int size);

    int send(const char *pack, int size);

    int recv(SOCKET sock, char *buf, int nLen);
    int recv(char *buf, int nLen);

    SOCKET accept(struct sockaddr *s = 0, int *len = 0);

    int shutdown(int how);

    int gethostname(char *buf, int len);
    struct hostent* gethostbyname(const char *name);
    struct hostent* gethostbyaddr(const char *addr, int len, int type);

    //
    unsigned long inet_addr(const char *cp);
    char * inet_ntoa(struct in_addr in);
    char * inet_ntoa(unsigned long ip);
    //
    int WSAAsyncSelect(HWND hwnd, UINT uMsg, long lEvent);

    //
    int setsockopt(int level, int optname, const char *optval, int optlen);
protected:
private:
    SOCKET m_socket;

    SOCKADDR_IN m_sockaddr;
};

#endif // U_SOCKET_H


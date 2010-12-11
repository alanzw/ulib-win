#ifndef U_HTTPCLIENT_H
#define U_HTTPCLIENT_H

#define MAX_RECEIVABLE    80000
#define MAX_URL_LENGTH    256

#define HTTP    0

typedef struct {
    int protocol;
    int port;

    char host[MAX_URL_LENGTH];
    char url[MAX_URL_LENGTH];
} CONNECTSTRUCT, *LPCONNECTSTRUCT;

#define HTTP_WAITING                0
#define HTTP_OK                     1
#define HTTP_ERROR                  2
#define HTTP_CANCEL                 3
#define HTTP_TIMEOUT                4
#define MAX_RECEIVABLE              80000

class UHTTPClient
{
public:
    UHTTPClient();
    virtual ~UHTTPClient();

    void request(char *host, char *url);

    WSADATA WSAData;

    struct sockaddr_in server_addr;
    struct hostent *hostent;
    //CONNECTSTRUCT connect;

    BOOL _bReady;
    int HTTPsocket;
    HANDLE RecvThread;
    DWORD ThreadID;
    long basetime;

public:
    BOOL wait(int time);
    void cancel(void);

    int result;
    char buffer[MAX_RECEIVABLE+1];
};

#endif // U_HTTPCLIENT_H

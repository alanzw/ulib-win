#ifndef U_NETWORK_H
#define U_NETWORK_H


/*
 * =====================================================================================
 *        Class:  UTcpListener
 *  Description:  
 *      
 * =====================================================================================
 */

class UTcpListener
{
public:
    UTcpListener();

    UTcpListener(int port);

    ~UTcpListener();

    void listen(int port);
    void listen();

    void start();

    void accept();
};


/*
 * =====================================================================================
 *        Class:  UTcpClient
 *  Description:  
 *      
 * =====================================================================================
 */

class UTcpClient
{
public:
    UTcpClient();

    ~UTcpClient();

    void connect(const char * saddr, int port);
};

#endif // U_NETWORK_H

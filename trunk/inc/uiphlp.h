#ifndef U_IPHELP_H
#define U_IPHELP_H

#include "ulib.h"

class ULIB_API UIPHelp
{
public:
    UIPHelp();
    ~UIPHelp();

    bool getNetworkInfo();

    const char *hostname() const;
    const char *domainName() const;

    const char *dnsServer() const;
    const char *dnsServerExtra() const;

    bool getAdapterInfo();

    bool getInterfaceInfo();

    bool getIPAddressTable();
    
    bool releaseIPAddress();
    bool renewIPAddress();
    
    bool addIPAddress();
    bool deleteIPAddress();
protected:
private:
    FIXED_INFO *m_pFixedInfo;
    IP_ADAPTER_INFO *m_pAdapterInfo;
    IP_INTERFACE_INFO*  m_pInterfaceInfo;
    MIB_IPADDRTABLE  *m_pIPAddrTable;
};

class ULIB_API UIcmpUtility
{
public:
    UIcmpUtility();

    bool createFile();
private:
    HANDLE m_hIcmpFile;
};

#endif // U_IPHELP_H


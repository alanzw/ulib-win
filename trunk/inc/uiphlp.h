#ifndef U_IPHELP_H
#define U_IPHELP_H

#include "ulib.h"

class ULIB_API UIPHelp
{
public:
    UIPHelp();
    ~UIPHelp();

    bool getInfo();

    const char *hostname() const;
    const char *domainName() const;
protected:
private:
    FIXED_INFO *m_pFixedInfo;
    IP_ADDR_STRING *m_pIPAddr;
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


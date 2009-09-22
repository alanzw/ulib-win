#ifndef U_NT_SERVICE_H
#define U_NT_SERVICE_H

#include "ulib.h"

class ULIB_API UNTService
{
public:
    UNTService();
    ~UNTService();

protected:
private:
};

class ULIB_API UNTServiceMan
{
public:
    UNTServiceMan();
    ~UNTServiceMan();


    BOOL installSvc(LPCTSTR lpSvcName);
    BOOL startSvc(LPCTSTR lpSvcName);
    BOOL stopSvc(LPCTSTR lpSvcName);
    int queryState(LPCTSTR lpSvcName);
protected:
private:
};

#endif // U_NT_SERVICE_H


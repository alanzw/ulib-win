#ifndef U_TOOLHELP_H
#define U_TOOLHELP_H

#include "ulib.h"

class ULIB_API UToolHelp
{
public:
    UToolHelp();
    bool snapall();
    bool listProcesses();
    bool listProcessModules( DWORD dwPID );
    bool listProcessThreads( DWORD dwOwnerPID );
    bool listHeaps(DWORD dwPID);
protected:
private:
    HANDLE m_hSnapshot;
};

#endif // U_TOOLHELP_H


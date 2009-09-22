#ifndef U_CMDLINE_H
#define U_CMDLINE_H

#include "ulib.h"

class ULIB_API UCmdLine
{
public:
    UCmdLine();

    ~UCmdLine();
    //
    LPTSTR * CommandLineToArgv(LPCTSTR lpCmdLine, int *lpArgc);
protected:
private:
};

#endif // U_CMDLINE_H


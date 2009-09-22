#ifndef U_TASKDIALOG_H
#define U_TASKDIALOG_H

#include "ulib.h"

#if _WIN32_IE < 0x0600
#error Comctl32.dll version 6.0 and later is needed.
#endif // _WIN32_IE < 0x0600

#if _WIN32_IE >= 0x0600
class ULIB_API UTaskDialog
{
public:
    UTaskDialog();
    virtual UTaskDialog();
protected:
private:
};
#endif // _WIN32_IE >= 0x0600

#endif // U_TASKDIALOG_H


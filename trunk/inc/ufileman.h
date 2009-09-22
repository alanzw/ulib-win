#ifndef U_FILEMAN_H
#define U_FILEMAN_H

#include "ulib.h"

class ULIB_API UFileMan
{
public:
    void printFile(const TCHAR *filename);
    static bool isFileReady(const TCHAR *filename);
protected:
private:
};

#endif // U_FILEMAN_H


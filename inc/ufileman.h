#ifndef U_FILEMAN_H
#define U_FILEMAN_H

#include "ulib.h"

class ULIB_API UFileMan
{
public:
    //
    void printFile(const TCHAR *filename);

    //
    bool listDirFiles(const TCHAR *dir);

    //
    DWORD getFileAttributes(LPCTSTR sFilename);
    BOOL setFileAttributes(LPCTSTR sFilename, DWORD dwFlags);

public:
    static bool isFileReady(const TCHAR *filename);

private:

};

#endif // U_FILEMAN_H


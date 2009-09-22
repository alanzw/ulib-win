#ifndef U_PIPE_H
#define U_PIPE_H

#include "ulib.h"

class ULIB_API UPipe
{
public:
    UPipe();
    virtual ~UPipe();
    virtual BOOL create();
    BOOL startChild(char *lpszChildName = "child.exe");
    virtual void write(){};
    virtual void read(char *sOutFilename = "out.dat");
private:
    char m_szReadBuffer[256];
    DWORD m_nReadNum;
    HANDLE m_hWrite;
    HANDLE m_hRead;
    STARTUPINFO m_siChild;
    PROCESS_INFORMATION m_piChild;
};

#endif // U_PIPE_H

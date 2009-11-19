#ifndef U_PIPE_H
#define U_PIPE_H

#include "uobject.h"

class ULIB_API UPipe : public UKernelObject
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

class ULIB_API UNamedPipe : public UPipe
{
public:
	UNamedPipe();
    UNamedPipe(LPCTSTR sName);
    virtual ~UNamedPipe();
	virtual BOOL create();

	BOOL connect(LPOVERLAPPED lpOverlapped = NULL);
	BOOL disconnect();

	BOOL read(LPTSTR lpBuffer, DWORD dwBufSize);
private:
	LPCTSTR m_sName;
};

#endif // U_PIPE_H

#ifndef U_FILE_H
#define U_FILE_H

#include "uobject.h"

class ULIB_API UFile : public UKernelObject
{
public:
    UFile();
    ~UFile();

    //
    operator HFILE() const
    { return (HFILE)m_hObj; }

    //
    operator HANDLE() const
    { return m_hObj; }


    //
    bool open(LPCSTR lpFilename = _T("defalut"));
    bool create(LPCTSTR lpFilename = _T("default"));

    bool isOpen() const;

    bool write(LPBYTE lpData, DWORD dwNumOfBytes);
    bool read(LPBYTE lpBuffer, DWORD dwBufSize, LPDWORD dwNumOfBytesRead);

    DWORD seek(LONG lOffset, DWORD dwFrom);
    DWORD pos();

    //
    bool lock(DWORD dwOffset, DWORD dwSize);
    bool unlock(DWORD dwOffset, DWORD dwSize);

    //
    bool flush();

    //
    DWORD size() const;

    //
    DWORD type() const;


    //
    LPCTSTR name() const
    { return m_sFilename; }
protected:
private:
    TCHAR m_sFilename[MAX_PATH];
};


class ULIB_API UTempFile : public UFile
{
public:
    bool open();

    bool move(LPCTSTR lpNewFilename);
};

#include <stdio.h>

class ULIB_API UCFile
{
    typedef enum {
        UCFAM_READONLY  = 0x01,
        UCFAM_WRITEONLY = 0x02,
        UCFAM_APPEND    = 0x04,
        UCFAM_RW_UPDATE = 0x08,
        UCFAM_RW_CREATE = 0x10,
        UCFAM_RW_APPEND = 0x20
    } UCFAccessMode;

    typedef enum {
        UCFEM_TEXT,
        UCFEM_BINARY
    } UCFExtraMode;
public:
    UCFile(const char *sFilename = "default");

    ~UCFile();
    //
    bool create(bool bOverwriten = true);
    //
    bool open(const char *mode);
    bool close();

    bool reopen(const char *mode);

    bool isOpen() const;

    bool isEOF() const;

    bool read(void *buf, size_t size);
    bool write(const void *buf, size_t size);

    size_t size();

    bool directStdOut(const char *mode);

    bool directStdErr(const char *mode);

    bool directStdIn(const char *mode);

    bool flush();

    bool seek(long int offset, int origin);
    long tell();

    void rewind();

    FILE * assign(FILE * pfile)
    {
        m_pFile = pfile;
        return m_pFile;
    }
private:
    char m_sFilename[MAX_PATH];
    FILE *m_pFile;
};

class ULIB_API UCTempFile : public UCFile
{
public:
    UCTempFile();

    bool open();
};


#endif // U_FILE_H


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
protected:
private:
    TCHAR m_sFilename[MAX_PATH];
};

#ifdef USE_C_FILE

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
    UCFile(const char *sFilename = "default")
    {
        strcpy(m_sFilename, sFilename);
    }
    ~UCFile()
    {}
    //
    bool create(bool bOverwriten = true)
    {
        const char *sMode = (bOverwriten ? "w":"w+");
        m_pFile = fopen(m_sFilename, sMode);
        return true;
    }
    //
    bool open(const char *mode)
    {
        m_pFile = fopen(m_sFilename, mode);
        return true;
    }
    bool close()
    {
        fclose(m_pFile);
        return true;
    }

    bool reopen(const char *mode)
    {
        freopen(m_sFilename, mode, m_pFile);
        return true;
    }

    bool isOpen() const
    {
        return (NULL == m_pFile);
    }

    bool isEOF() const
    {
        return 0 != feof(m_pFile);
    }
    
    bool read(void *buf, size_t size)
    {
        return 1 == fread(buf, size, 1, m_pFile);
    }
    bool write(const void *buf, size_t size)
    {
        return 1 == fwrite(buf, size, 1, m_pFile);
    }
    
    size_t size()
    {
        size_t s = 0;
        this->seek(m_pFile, 0, SEEK_END);
        s = this->tell(m_pFile);
        this->rewind();
        return s;
    }

    bool directStdOut(const char *mode)
    {
        m_pFile = freopen(m_sFilename, mode, stdout);
        return true;
    }

    bool directStdErr(const char *mode)
    {
        m_pFile = freopen(m_sFilename, mode, stderr);
        return true;
    }

    bool directStdIn(const char *mode)
    {
        m_pFile = freopen(m_sFilename, mode, stdin);
        return true;
    }

    bool flush()
    {
        fflush(m_pFile);
        return true;
    }

    bool seek(long int offset, int origin)
    {
        return (0 != fseek(m_pFile, offset, origin) );
    }

    long tell()
    {
        return ftell(m_pFile);
    }
    
    void rewind()
    {
        ::rewind(m_pFile);
    }
private:
    char m_sFilename[MAX_PATH];
    FILE *m_pFile;
};

class ULIB_API UCTempFile
{

};

#endif // USE_C_FILE

#endif // U_FILE_H


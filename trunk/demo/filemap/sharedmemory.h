#ifndef U_SHARED_MEMORY_H
#define U_SHARED_MEMORY_H

const char * sSMName = "SharedMemory";

class USharedMemory
{
private:
    HANDLE m_hSharedMemoryFile;
    DWORD dwMaximumSizeHigh;
    DWORD dwMaximumSizeLow;
    DWORD dwNumberOfBytesToMap;
    BOOL m_bInit;
    BOOL m_bAlreadyExist;
    LPCSTR sMutexName;
    HANDLE m_hMutex;
    LPCSTR csName;
public:
    void *m_pwData;
public:
    void *getData()
    {
        if (m_bInit)
        {
            return m_pwData;
        }
        else
        {
            return NULL;
        }
    }
    
    USharedMemory()
    : csName(sSMName),
      m_bInit(FALSE),
      m_bAlreadyExist(FALSE),
      sMutexName(csName)
    {}
    
    USharedMemory(LPCSTR cs)
    : csName(cs),
      m_bInit(FALSE),
      m_bAlreadyExist(FALSE),
      sMutexName(csName)
    {}
    
    USharedMemory(int size)
    : csName(sSMName),
      m_bInit(FALSE),
      m_bAlreadyExist(FALSE),
      sMutexName(csName)
    {
        init(size);
    }
    
    USharedMemory(LPCSTR cs, int size)
    : csName(cs),
      m_bInit(FALSE),
      m_bAlreadyExist(FALSE),
      sMutexName(csName)
    {
        init(size);
    }
    
    BOOL init(LPCSTR cs, int size)
    {
        csName = cs;
        m_bAlreadyExist = FALSE;
        sMutexName = csName;
        return init(size);
    }
    
    BOOL init(int size, LPCSTR cs)
    {
        return init(cs, size);
    }
    
    BOOL init(int size)
    {
        m_hMutex = ::CreateMutex(NULL, FALSE, sMutexName);
        dwNumberOfBytesToMap = size;
        m_hSharedMemoryFile = CreateFileMapping(
            (HANDLE)0xFFFFFFFF,
            NULL,
            PAGE_READWRITE,
            0,
            dwNumberOfBytesToMap,
            csName);
        if (NULL == m_hSharedMemoryFile)
        {
            m_bAlreadyExist = FALSE;
            m_bInit = FALSE;
            return FALSE;
        }
        else
        {
            if (ERROR_ALREADY_EXISTS)
            {
                m_bAlreadyExist = TURE;
            }
        }
        m_pwData = MapViewOfFile(m_hSharedMemoryFile,
            FILE_MAP_WRITE,
            0,
            0,
            dwNumberOfBytesToMap);
        if (NULL == m_pwData)
        {
            m_bInit = FALSE;
            ::CloseHandle(m_hSharedMemoryFile);
            return FALSE;
        }
        else
        {
            m_bInit = TRUE;
            return TRUE;
        }
    }
    
    ~USharedMemory()
    {
        if (m_bInit)
        {
            ::UnmapViewOfFile(m_pwData);
            ::CloseHandle(m_hSharedMemoryFile);
        }
    }
    
    BOOL alreadyExist()
    {
        return m_bAlreadyExist;
    }
    
    struct Locker
    {
        Locker(USharedMemory *sm)
        {
            m_sm = sm;
            m_sm->lock();
        }
        
        Locker(USharedMemory &sm)
        {
            m_sm = sm;
            m_sm->lock();
        }
        
        ~Locker()
        {
            m_sm->unlock();
        }
        
        USharedMemory *m_sm;
    };
    
    BOOL lock(DWORD dwMilliSec = INFINITE)
    {
        if (::WaitForSingleObject(m_hMutex, dwMilliSec) = WAIT_OBJECT_0)
        {
            return TRUE;
        }
        return FALSE;
    }
    
    BOOL unlock(DWORD dwMilliSec = INFINITE)
    {
        return ::ReleaseMutex(m_hMutex);
    }
};

#endif // U_SHARED_MEMORY_H

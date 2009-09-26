#include <string>
#include <vector>

#include <windows.h>

#define ___USE_MT___

template <class T>
class ObjectPool
{
public:
    ObjectPool() {}
    virtual ~ObjectPool() {}

    #ifdef ___USE_MT___
    inline static CRITICAL_SECTION* get_cs()
    {
        static CRITICAL_SECTION __m_cs;
        return &__m_cs;
    }
    #endif

    static std::vector<T* >& get_list()
    {
        static std::vector<T* > m_free;
        return m_free;
    }

    static void __op_initialize()
    {
        #ifdef ___USE_MT___
        InitializeCriticalSection(get_cs());
        #endif
        init();
    }

    static void __op_finalize()
    {
        #ifdef ___USE_MT___
        DeleteCriticalSection(get_cs());
        #endif
        destroy();
    }


    static void init()
    {
        for (int i = 0; i < 1; ++i)
        {
            T* p = ::new T;
            T::get_list().push_back(p);
        }
    }

    inline void *operator new( size_t stAllocateBlock)
    {
        #ifdef ___USE_MT___
        EnterCriticalSection(get_cs());
        #endif
        if (T::get_list().size() <= 0)
        {
            T::init();
        }
        T* p = T::get_list().back();
        T::get_list().pop_back();
        #ifdef ___USE_MT___
        LeaveCriticalSection(get_cs());
        #endif
        return p;
    }

    inline void operator delete( void *p )
    {
        #ifdef ___USE_MT___
        EnterCriticalSection(get_cs());
        #endif
        T::get_list().push_back((T*)p);
        #ifdef ___USE_MT___
        LeaveCriticalSection(get_cs());
        #endif
    }

    static void destroy()
    {
        std::vector<T *>::iterator first = T::get_list().begin();
        std::vector<T *>::iterator last = T::get_list().end();
        while (first != last)
        {
            T *p = *first;
            ++first;
            ::delete p;
        }
        T::get_list().erase(T::get_list().begin(), T::get_list().end());
    }
};

namespace NOSTL
{

class clsCrit
{
private:
    CRITICAL_SECTION _Crit;

    clsCrit(const clsCrit &); // NO NO NO NO! No copy constructor!!!
    void operator=(const clsCrit &)

public:
    clsCrit(void)
    {
        InitializeCriticalSection(&_Crit);
    }
    ~clsCrit(void)
    {
        DeleteCriticalSection(&_Crit);
    }

    void Enter(void) const
    {
        EnterCriticalSection(const_cast<CRITICAL_SECTION*>(&_Crit));
    }
    void Leave(void) const
    {
        LeaveCriticalSection(const_cast<CRITICAL_SECTION*>(&_Crit));
    }
};

class clsCritObj
{
private:
    const clsCrit* _pCrit;
public:
    clsCritObj(const clsCrit& Sync)
    {
        (_pCrit = &Sync)->Enter();
    }
    ~clsCritObj(void)
    {
        _pCrit->Leave();
    }
};

typedef void* _CHK_PVOID;

template<int S>
class clsPointerArray
{
private:
    _CHK_PVOID* _Array;
    int _nEntriesAllocated;
    int _nEntriesUsed;

public:
    clsPointerArray(void)
    {
        _Array = NULL;
        _nEntriesUsed = 0;
        _nEntriesAllocated = 0;
    }
    virtual ~clsPointerArray(void)
    {
        if (_nEntriesUsed)
            _chk_utildebug(TEXT("WRN: objects left in pool!"));
    }

    void* Get(void)
    {
        if (_nEntriesUsed <= 0)
            return(NULL);
        return(_Array[--_nEntriesUsed]);
    }
    void Add(void* p)
    {
        if (_nEntriesUsed + 1 > _nEntriesAllocated)
        {
            _CHK_PVOID* pNew = ::new _CHK_PVOID[_nEntriesAllocated + S];
            memcpy(pNew,_Array,_nEntriesUsed * sizeof(_CHK_PVOID));
            _nEntriesAllocated += S;
            delete[] _Array;
            _Array = pNew;
        }
        _Array[_nEntriesUsed++] = p;
    }
};

template <class T, int S = 16> // S = size increment in pool allocation
class clsObjectPool
{
private:
    #if !_OBJPOOL_NO_MULTITHREADING
    static clsCrit& CS(void)
    {
        static clsCrit __Crit;
        return(__Crit);
    }
    #endif

    static clsPointerArray<S>& List(void)
    {
        static clsPointerArray<S> __FreeItems;
        return(__FreeItems);
    }

public:
    clsObjectPool()
    {
    }

    virtual ~clsObjectPool()
    {
    }

    void* operator new(size_t stAllocateBlock)
    {
        #if !_OBJPOOL_NO_MULTITHREADING
        clsCritObj CO(CS());
        #endif

        clsPointerArray<S>& VList(T::List());
        void* pNewObject(VList.Get());

        if (pNewObject == NULL)
        pNewObject = ::new T;
        return(pNewObject);
    }

    void operator delete(void *p)
    {
        #if !_OBJPOOL_NO_MULTITHREADING
        clsCritObj CO(CS());
        #endif

        clsPointerArray<S>& VList(T::List());

        VList.Add(p);
    }

    void Clear(void)
    {
        #if !_OBJPOOL_NO_MULTITHREADING
        clsCritObj CO(CS());
        #endif

        clsPointerArray<S>& VList(T::List());

        while (true)
        {
            void* pObject(VList.Get());
            if (pObject == NULL)
            break;
            ::delete reinterpret_cast<T*>(pObject);
        }
    }
};

}; // namespace NOSTL


int main()
{
    return 0;
}

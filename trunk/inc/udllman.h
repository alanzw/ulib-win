#ifndef U_DLLMAN_H
#define U_DLLMAN_H

#include "ulib.h"

#include <stdio.h>
#include <exception>

#define DEF_DLLFUNC(PROCNAME, RET_T, ...) \
    typedef RET_T (*PROCNAME)( __VA_ARGS__ );

class ULIB_API UDllMan
{
public:
    UDllMan();
    ~UDllMan();
    explicit UDllMan(const TCHAR *filepath);
    int load(const TCHAR *filepath);

    template <class TRet>
    struct DllFunc0
    {
        typedef TRet (*pfType)();
        static TRet call(LPCTSTR lpLibName, LPCTSTR lpProcName)
        {
            UDllMan udm;
            udm.load(lpLibName);
            pfType pfFoo = (pfType)udm.find(lpProcName);
            return pfFoo();
        }
        static TRet callEx(UDllMan &udm, LPCSTR lpProcName)
        {
            pfType pfFoo = (pfType)udm.find(lpProcName);
            return pfFoo();
        }
    };

    template <typename T>
    T callFunc(LPCTSTR lpProcName)
    {
        return DllFunc0<T>::callEx(*this, lpProcName);
    }

    template <class TRet, class TPara>
    struct DllFunc
    {
        typedef TRet (*pfType)(TPara);
        static TRet call(LPCTSTR lpLibName, LPCTSTR lpProcName, TPara para)
        {
            UDllMan udm;
            udm.load(lpLibName);
            pfType pfFoo = (pfType)udm.find(lpProcName);
            return pfFoo(para);
        }
        static TRet callEx(UDllMan &udm, LPCSTR lpProcName, TPara para)
        {
            pfType pfFoo = (pfType)udm.find(lpProcName);
            return pfFoo(para);
        }
    };

    template <typename T1, typename T2>
    T1 callFunc(LPCTSTR lpProcName, T2 para)
    {
        return DllFunc<T1, T2>::callEx(*this, lpProcName, para);
    }

    template <class TRet, class TPara1, class TPara2>
    struct DllFunc2
    {
        typedef TRet (*pfType)(TPara1, TPara2);
        static TRet call(LPCTSTR lpLibName, LPCTSTR lpProcName, TPara1 para1, TPara2 para2)
        {
            UDllMan udm;
            udm.load(lpLibName);
            pfType pfFoo = (pfType)udm.find(lpProcName);
            return pfFoo(para1, para2);
        }
        static TRet callEx(UDllMan &udm, LPCSTR lpProcName, TPara1 para1, TPara2 para2)
        {
            pfType pfFoo = (pfType)udm.find(lpProcName);
            return pfFoo(para1, para2);
        }
    };

    template <typename T, typename TP1, typename TP2>
    T callFunc(LPCTSTR lpProcName, TP1 para1, TP2 para2)
    {
        return DllFunc2<T, TP1, TP2>::callEx(*this, lpProcName, para1, para2);
    }

    FARPROC WINAPI find(const TCHAR *funcname);

    int free();

    class UDllException : public std::exception
    {
    public:
        enum {
            UDLL_ERROR_GET_PROC_ADDRESS = 0x0001,
            UDLL_ERROR_LOAD_LIBRARY     = 0x0002
        };

        UDllException(DWORD dwCode)
            : m_dwCode(dwCode)
        {}

        void report(const TCHAR *lpszStr)
        {
            const TCHAR *lpszError = NULL;

            switch(m_dwCode)
            {
                case UDLL_ERROR_LOAD_LIBRARY:
                    lpszError = _T("Error loading the library");
                    break;
                case UDLL_ERROR_GET_PROC_ADDRESS:
                    lpszError = _T("Error finding procedure address");
                default:
                    lpszError = _T("Unkown");
            }
            printf("%s : %s", lpszStr, lpszError);
        }
    private:
        DWORD m_dwCode;

    };
protected:
private:
    HINSTANCE m_hDll;
};

namespace huys
{

namespace DLLInject
{
    //! Using CreateRemoteThread
    ULIB_API BOOL injectDll(DWORD dwPid, char *dllname, char *injected_dllname);
    ULIB_API unsigned long GetTargetProcessIdFromProcname(char *procName);

    //! Using SetWindowsHookEx
    ULIB_API unsigned long GetTargetProcessIdFromWindow(char *className, char *windowName);
    ULIB_API unsigned long GetTargetThreadIdFromWindow(char *className, char *windowName);

	ULIB_API BOOL injectLibW(DWORD dwPid, LPCWSTR sLibFile);
	ULIB_API BOOL injectLibA(DWORD dwPid, LPCSTR sLibFile);
	ULIB_API BOOL ejectLibW(DWORD dwPid, LPCWSTR sLibFile);
}; // namespace DLLInject

}; // namespace huys

#endif // U_DLLMAN_H


/*
 * =====================================================================================
 *
 *       Filename:  uole.h
 *
 *    Description:  OLE
 *
 *        Version:  1.0
 *        Created:  2009-7-23 18:33:35
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#ifndef U_OLE_H
#define U_OLE_H

#include "udllman.h"

namespace huys
{
namespace UOle
{

inline HRESULT CoInitialize(void)
{
    return ::CoInitialize(NULL);
}

inline void CoUninitialize(void)
{
    ::CoUninitialize();
}

/*
typedef enum tagCLSCTX {
  CLSCTX_INPROC_SERVER            = 0x1,
  CLSCTX_INPROC_HANDLER           = 0x2,
  CLSCTX_LOCAL_SERVER             = 0x4,
  CLSCTX_INPROC_SERVER16          = 0x8,
  CLSCTX_REMOTE_SERVER            = 0x10,
  CLSCTX_INPROC_HANDLER16         = 0x20,
  CLSCTX_RESERVED1                = 0x40,
  CLSCTX_RESERVED2                = 0x80,
  CLSCTX_RESERVED3                = 0x100,
  CLSCTX_RESERVED4                = 0x200,
  CLSCTX_NO_CODE_DOWNLOAD         = 0x400,
  CLSCTX_RESERVED5                = 0x800,
  CLSCTX_NO_CUSTOM_MARSHAL        = 0x1000,
  CLSCTX_ENABLE_CODE_DOWNLOAD     = 0x2000,
  CLSCTX_NO_FAILURE_LOG           = 0x4000,
  CLSCTX_DISABLE_AAA              = 0x8000,
  CLSCTX_ENABLE_AAA               = 0x10000,
  CLSCTX_FROM_DEFAULT_CONTEXT     = 0x20000,
  CLSCTX_ACTIVATE_32_BIT_SERVER   = 0x40000,
  CLSCTX_ACTIVATE_64_BIT_SERVER   = 0x80000,
  CLSCTX_ENABLE_CLOAKING          = 0x100000,
  CLSCTX_PS_DLL                   = 0x80000000
} CLSCTX;
*/
inline HRESULT CoCreateInstance( REFCLSID rclsid,
                                 LPUNKNOWN pUnkOuter,
                                 DWORD dwClsContext,
                                 REFIID riid,
                                 LPVOID *ppv )
{
    return ::CoCreateInstance(rclsid, pUnkOuter, dwClsContext, riid, ppv);
}

inline HRESULT HuCoCreateInstance(
	  LPCTSTR szDllName,
	  REFCLSID rclsid,
	  IUnknown* pUnkOuter,
	  REFIID riid,
	  LPVOID FAR* ppv)
{
	  HRESULT hr = REGDB_E_KEYMISSING;

	  static UDllMan udm(szDllName);
	
	  //HMODULE hDll = ::LoadLibrary(szDllName);
	  //if (hDll == 0)
	  //	return hr;
	
	  //typedef HRESULT (__stdcall *pDllGetClassObject)( 
	  //		REFCLSID rclsid, 
      //		    REFIID riid,
	  //	    LPVOID FAR* ppv);
	
	  //pDllGetClassObject GetClassObject = 
	  //	 (pDllGetClassObject)::GetProcAddress(hDll, "DllGetClassObject");
	  //if (GetClassObject == 0)
	  //{
	  //	::FreeLibrary(hDll);
	  //	return hr;
	  //}

	  IClassFactory *pIFactory;
	
	  hr = udm.callFunc<HRESULT, REFCLSID, REFIID, LPVOID FAR*>("DllGetClassObject", rclsid, IID_IClassFactory, (LPVOID *)&pIFactory);
	
	  if (!SUCCEEDED(hr))
		return hr;
	
	  hr = pIFactory->CreateInstance(pUnkOuter, riid, ppv);
	  pIFactory->Release();
	
	  return hr;

}

//
class ULIB_API UCoObject
{
public:
    virtual void * acquireInterface (IID const & iid) = 0;
    virtual ~UCoObject(){}
};

//
class ULIB_API USmartObject: public UCoObject
{
public:
    USmartObject (CLSID const & classId, bool running = false);
    ~USmartObject ()
    {
        if(_iUnk)
            _iUnk->Release ();
    }
    void * acquireInterface (IID const & iid);

private:
    IUnknown * _iUnk;
};

//
template<class T>
class USmartFace
{
public:
    ~USmartFace ()
    {
        if (_i)
        {
            _i->Release ();
        }
    }
    T * operator-> () { return _i; }
protected:
    USmartFace () : _i (0) {}
    USmartFace (void * i)
    {
        _i = static_cast<T*> (i);
    }
protected:
    T * _i;
};

template<class T, IID const * iid>
class USmartObjFace: public USmartFace<T>
{
public:
    USmartObjFace (UCoObject & obj)
    : USmartFace<T> (obj.acquireInterface (*iid))
    {}
};

class HEx
{
public:
    HEx (HRESULT hr, char const * str = 0)
        : _hr (hr), _str (str)
    {}
private:
    HRESULT _hr;
    char const * _str;
};


class ULIB_API UDispObject: public UCoObject
{
public:
    UDispObject (CLSID const & classId)
        :_iDisp (0)
    {
        HRESULT hr = ::CoCreateInstance (
            classId,
            0,
            CLSCTX_ALL,
            IID_IDispatch,
            (void**)&_iDisp);

        if (FAILED (hr))
        {
            if (hr == E_NOINTERFACE)
                throw "No IDispatch interface";
            else
                throw HEx (hr, "Couldn't create DispObject");
        }
    }

    ~UDispObject ()
    {
        if (_iDisp)
            _iDisp->Release ();
    }

    operator bool () const { return _iDisp != 0; }
    bool operator ! () const { return _iDisp == 0; }

    DISPID getDispId (WCHAR * funName)
    {
        DISPID dispid;
        HRESULT hr = _iDisp->GetIDsOfNames (
                                 IID_NULL,
                                 &funName,
                                 1,
                                 GetUserDefaultLCID (),
                                 &dispid);
        return dispid;
    }

    void getProperty (DISPID propId, VARIANT & result)
    {
        // In parameters
        DISPPARAMS args = { 0, 0, 0, 0 };
        EXCEPINFO except;
        UINT argErr;
        HRESULT hr = _iDisp->Invoke (propId,
            IID_NULL,
            GetUserDefaultLCID (),
            DISPATCH_PROPERTYGET,
            &args,
            &result,
            &except,
            &argErr);
        if (FAILED (hr))
            throw HEx (hr, "Couldn't get property");
    }

    void * acquireInterface (IID const & iid)
    {
        void * p = 0;
        HRESULT hr = _iDisp->QueryInterface (iid, &p);
        if (FAILED (hr))
        {
            if (hr == E_NOINTERFACE)
                throw "No such interface";
            else
                throw HEx (hr, "Couldn't query interface");
        }
        return p;
    }

protected:
    UDispObject (IDispatch * iDisp) : _iDisp (iDisp) {}
    UDispObject () : _iDisp (0) {}
protected:
    IDispatch * _iDisp;
};

//
//
// Strings
//

class BString
{
    friend class CString;
public:
    BString ()
        :_str (0)
    {}
    BString (VARIANT & var)
    {
        if (var.vt != VT_BSTR)
        {
            throw "Variant type is not a BSTR";
        }
        _str = var.bstrVal;
    }
    BString (WCHAR * str)
        :_str (::SysAllocString (str))
    {}
    ~BString ()
    {
        // Works for null string, too.
        ::SysFreeString (_str);
    }
    BSTR * GetPointer () { return &_str; }
protected:
    BSTR _str;
};

class CString
{
public:
    CString (BString & bstr)
        :_len (::SysStringLen (bstr._str))
    {
        _str = new char [_len + 1];
        wcstombs (_str, bstr._str, _len);
        _str [_len] = '\0';
    }
    ~CString ()
    {
        delete []_str;
    }
    operator char const * () const { return _str; }
    int Len () { return _len; }
    bool IsEqual (char const * str)
    {
        return strcmp (str, _str) == 0;
    }
protected:
    char * _str;
    int    _len;
};


//
// Type Library from exe or dll
//
class TypeLibrary
{
    friend class TypeInfo;
public:
    // Can use name if found on the PATH
    TypeLibrary (WCHAR * path);
    ~TypeLibrary ()
    {
        if (_iLib != 0)
            _iLib->Release ();
    }
    bool IsOk () const { return _iLib != 0; }
    int GetCount ()
    {
        return _iLib->GetTypeInfoCount ();
    }
    void GetDocumentation (int idx, BString & name, BString & doc);
private:
    ITypeInfo * GetTypeInfo (int idx);
    ITypeInfo * GetTypeInfo (WCHAR * name);
private:
    ITypeLib * _iLib;
};

//
// Type info for a given element of the library
//
class TypeInfo: public USmartFace<ITypeInfo>
{
    friend class USmartObject;
public:
    TypeInfo (TypeLibrary & lib, int idx);
    TypeInfo (TypeLibrary & lib, WCHAR * name);
    ~TypeInfo ()
    {
        _i->ReleaseTypeAttr (_attr);
    }
    GUID & GetGuid () const { return _attr->guid; }
    void GetDocumentation (BString & name, BString & doc);
private:
    void * CreateInstance (IID const & iid);
private:
    TYPEATTR  * _attr;
};

}; // namespace UOle
}; // namespace huys

#endif // U_OLE_H


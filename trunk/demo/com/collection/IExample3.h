#ifndef _IEXAMPLE3_H_
#define _IEXAMPLE3_H_

#include <initguid.h>

#undef DEFINE_GUID

#ifdef INITGUID
#define DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
        EXTERN_C const GUID DECLSPEC_SELECTANY name \
                = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }
#else
#define DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
    EXTERN_C const GUID FAR name
#endif // INITGUID

// IExample3 type library's GUID
// {6C5A4C1A-87B3-44c1-8286-78000B2173EF}
DEFINE_GUID(CLSID_TypeLib, 0x6c5a4c1a, 0x87b3, 0x44c1, 0x82, 0x86, 0x78, 0x0, 0xb, 0x21, 0x73, 0xef);

// IExample3 object's GUID
// {6AFACEB2-9298-4d4b-80C1-F027C07B1A1E}
DEFINE_GUID(CLSID_IExample3, 0x6afaceb2, 0x9298, 0x4d4b, 0x80, 0xc1, 0xf0, 0x27, 0xc0, 0x7b, 0x1a, 0x1e);

// IExample3 VTable's GUID
// {CFADB388-9563-4591-AABB-BE7794AEC17C}
DEFINE_GUID(IID_IExample3, 0xcfadb388, 0x9563, 0x4591, 0xaa, 0xbb, 0xbe, 0x77, 0x94, 0xae, 0xc1, 0x7c);

// IExample3's VTable
#undef  INTERFACE
#define INTERFACE IExample3
DECLARE_INTERFACE_ (INTERFACE, IDispatch)
{
    // IUnknown functions
    STDMETHOD  (QueryInterface)        (THIS_ REFIID, void **) PURE;
    STDMETHOD_ (ULONG, AddRef)        (THIS) PURE;
    STDMETHOD_ (ULONG, Release)        (THIS) PURE;
    // IDispatch functions
    STDMETHOD_ (ULONG, GetTypeInfoCount)(THIS_ UINT *) PURE;
    STDMETHOD_ (ULONG, GetTypeInfo)        (THIS_ UINT, LCID, ITypeInfo **) PURE;
    STDMETHOD_ (ULONG, GetIDsOfNames)    (THIS_ REFIID, LPOLESTR *, UINT, LCID, DISPID *) PURE;
    STDMETHOD_ (ULONG, Invoke)            (THIS_ DISPID, REFIID, LCID, WORD, DISPPARAMS *, VARIANT *, EXCEPINFO *, UINT *) PURE;
    // Extra functions
    STDMETHOD  (SetString)            (THIS_ BSTR) PURE;
    STDMETHOD  (GetString)            (THIS_ BSTR *) PURE;
    STDMETHOD  (GetPorts)            (THIS_ IDispatch **) PURE;
};

#endif // _IEXAMPLE3_H_


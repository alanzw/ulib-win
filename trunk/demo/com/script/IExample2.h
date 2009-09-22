#ifndef _IEXAMPLE2_H_
#define _IEXAMPLE2_H_

#include <initguid.h>

// IExample2 type library's GUID
// {E1124082-5FCD-4a66-82A6-755E4D45A9FC}
DEFINE_GUID(CLSID_TypeLib, 0xe1124082, 0x5fcd, 0x4a66, 0x82, 0xa6, 0x75, 0x5e, 0x4d, 0x45, 0xa9, 0xfc);

// IExample2 object's GUID
// {520F4CFD-61C6-4eed-8004-C26D514D3D19}
DEFINE_GUID(CLSID_IExample2, 0x520f4cfd, 0x61c6, 0x4eed, 0x80, 0x4, 0xc2, 0x6d, 0x51, 0x4d, 0x3d, 0x19);

// IExample2 VTable's GUID
// {B6127C55-AC5F-4ba0-AFF6-7220C95EEF4D}
DEFINE_GUID(IID_IExample2, 0xb6127c55, 0xac5f, 0x4ba0, 0xaf, 0xf6, 0x72, 0x20, 0xc9, 0x5e, 0xef, 0x4d);

// IExample2's VTable
#undef  INTERFACE
#define INTERFACE IExample2
DECLARE_INTERFACE_ (INTERFACE, IDispatch)
{
    // IUnknown functions
    STDMETHOD  (QueryInterface)          (THIS_ REFIID, void **) PURE;
    STDMETHOD_ (ULONG, AddRef)           (THIS) PURE;
    STDMETHOD_ (ULONG, Release)          (THIS) PURE;
    // IDispatch functions
    STDMETHOD_ (ULONG, GetTypeInfoCount) (THIS_ UINT *) PURE;
    STDMETHOD_ (ULONG, GetTypeInfo)      (THIS_ UINT, LCID, ITypeInfo **) PURE;
    STDMETHOD_ (ULONG, GetIDsOfNames)    (THIS_ REFIID, LPOLESTR *, UINT, LCID, DISPID *) PURE;
    STDMETHOD_ (ULONG, Invoke)           (THIS_ DISPID, REFIID, LCID, WORD, DISPPARAMS *, VARIANT *, EXCEPINFO *, UINT *) PURE;
    // Extra functions
    STDMETHOD  (SetString) (THIS_ BSTR) PURE;
    STDMETHOD  (GetString) (THIS_ BSTR *) PURE;
};

#endif // _IEXAMPLE2_H_


#ifndef _IEXAMPLE_H_
#define _IEXAMPLE_H_
#define   INITGUID
#include <initguid.h>

#ifdef DEFINE_GUID
// Our IExample object's GUID
// {6899A2A3-405B-44d4-A415-E08CEE2A97CB}
DEFINE_GUID(CLSID_IExample, 0x6899a2a3, 0x405b, 0x44d4, 0xa4, 0x15, 0xe0, 0x8c, 0xee, 0x2a, 0x97, 0xcb);

// Our IExample VTable's GUID
// {74666CAC-C2B1-4fa8-A049-97F3214802F0}
DEFINE_GUID(IID_IExample, 0x74666cac, 0xc2b1, 0x4fa8, 0xa0, 0x49, 0x97, 0xf3, 0x21, 0x48, 0x2, 0xf0);
#endif  // DEFINE_GUID

// Defines IExample's functions (in a way that works both for our COM DLL
// and any app that #include's this .H file
#undef  INTERFACE
#define INTERFACE   IExample
DECLARE_INTERFACE_ (INTERFACE, IUnknown)
{
    STDMETHOD  (QueryInterface)       (THIS_ REFIID, void **) PURE;
    STDMETHOD_ (ULONG, AddRef)        (THIS) PURE;
    STDMETHOD_ (ULONG, Release)       (THIS) PURE;
    STDMETHOD  (SetString)            (THIS_ char *) PURE;
    STDMETHOD  (GetString)            (THIS_ char *, DWORD) PURE;
};

#endif // _IEXAMPLE_H_


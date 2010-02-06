#ifndef U_VTBL_DLL_DB_H
#define U_VTBL_DLL_DB_H

#include <ole2.h>

#define _AFX_NO_BSTR_SUPPORT

#include <windows.h>
#include <tchar.h>

#include "adt/uvector.h"
#include "adt/ustring.h"

typedef long HRESULT;
typedef huys::ADT::UStringAnsi TString;
typedef huys::ADT::UVector<TString> CStringArray;
typedef huys::ADT::UVector<CStringArray *> CPtrArray;

#ifdef BUILD_DLL
#define DEF_EXPORT __declspec(dllexport)
#else
#define DEF_EXPORT __declspec(dllimport)
#endif

#ifndef NO_ERROR
#define  NO_ERROR 0
#endif

// {30DF3430-0266-11cf-BAA6-00AA003E0EED}
static const GUID CLSID_DBSAMPLE =
{ 0x30df3430, 0x266, 0x11cf, { 0xba, 0xa6, 0x0, 0xaa, 0x0, 0x3e, 0xe, 0xed } };
// {30DF3431-0266-11cf-BAA6-00AA003E0EED}
static const GUID IID_IDBSrvFactory =
{ 0x30df3431, 0x266, 0x11cf, { 0xba, 0xa6, 0x0, 0xaa, 0x0, 0x3e, 0xe, 0xed } };
// {30DF3432-0266-11cf-BAA6-00AA003E0EED}
static const GUID IID_IDB =
{ 0x30df3432, 0x266, 0x11cf, { 0xba, 0xa6, 0x0, 0xaa, 0x0, 0x3e, 0xe, 0xed } };

class IDB : public IUnknown
{
    // Interfaces
public:
    // Interface for data access.
    virtual HRESULT _stdcall Read(short nTable, short nRow, LPTSTR lpszData) =0;
};

extern ULONG g_dwRefCount;

extern "C" HRESULT DEF_EXPORT DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppObject);

#endif // U_VTBL_DLL_DB_H

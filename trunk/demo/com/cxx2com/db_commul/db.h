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
// {BE02C6C4-8DD1-4e7d-A0F9-58E2EC34565A}
static const GUID IID_IDBAccess = 
{ 0xbe02c6c4, 0x8dd1, 0x4e7d, { 0xa0, 0xf9, 0x58, 0xe2, 0xec, 0x34, 0x56, 0x5a } };
// {90FB4160-3DDA-4d98-8E40-50BC459563D3}
static const GUID IID_IDBManage = 
{ 0x90fb4160, 0x3dda, 0x4d98, { 0x8e, 0x40, 0x50, 0xbc, 0x45, 0x95, 0x63, 0xd3 } };
// {80383F44-9764-4a54-B4BB-95B6F1000684}
static const GUID IID_IDBInfo = 
{ 0x80383f44, 0x9764, 0x4a54, { 0xb4, 0xbb, 0x95, 0xb6, 0xf1, 0x0, 0x6, 0x84 } };

class IDB : public IUnknown
{
    // Interfaces
public:
};

class IDBAccess : public IUnknown {
   public:
   // Interface for data access
   virtual HRESULT _stdcall Read(short nTable, short nRow, LPTSTR lpszData) =0;
   virtual HRESULT _stdcall Write(short nTable, short nRow, LPCTSTR lpszData) =0;
};
class IDBManage : public IUnknown {
   // Interface for database management
   public:
   virtual HRESULT _stdcall Create(short &nTable, LPCTSTR lpszName) =0;
   virtual HRESULT _stdcall Delete(short nTable) =0;
};
class IDBInfo : public IUnknown {
   // Interface for retrieval of information about the database.
   public:
   virtual HRESULT _stdcall GetNumTables(short &nNumTables) =0;
   virtual HRESULT _stdcall GetTableName(short nTable, LPTSTR lpszName) =0;
   virtual HRESULT _stdcall GetNumRows(short nTable, short &nRows) =0;
};


class IHyClassFactory : public IUnknown
{
   virtual HRESULT _stdcall CreateInstance(IUnknown *pUnkOuter,
         REFIID riid, void**  ppvObject) = 0;
   virtual HRESULT _stdcall LockServer(BOOL fLock) = 0;
};

extern ULONG g_dwRefCount;

extern "C" HRESULT DEF_EXPORT DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppObject);

#endif // U_VTBL_DLL_DB_H

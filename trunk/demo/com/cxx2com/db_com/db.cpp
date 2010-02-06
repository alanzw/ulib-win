#include <stdio.h>

#include "db.h"


class CDB : public IDB
{
    // Interfaces
public:
    // Interface for data access
    virtual HRESULT _stdcall Read(short nTable, short nRow, LPTSTR lpszData);

    virtual HRESULT _stdcall GetNumRows(short nTable, short &nRows);

    virtual HRESULT _stdcall QueryInterface(REFIID riid, void** ppObject);

    virtual ULONG  _stdcall AddRef();

    virtual ULONG  _stdcall Release();
    // Implementation
private:
    CPtrArray m_arrTables; // Array of pointers to CStringArray
    CStringArray m_arrNames; // Array of table names

    ULONG m_dwRefCount;

public:

    CDB();

    virtual ~CDB();
};


class CDBSrvFactory : public IHyClassFactory {
    // Interface
public:

    HRESULT _stdcall QueryInterface(REFIID riid, void** ppObject);

    ULONG  _stdcall AddRef();

    ULONG  _stdcall Release();

    HRESULT _stdcall CreateInstance(IUnknown *pUnkOuter, REFIID riid, void** ppObject);

    HRESULT   _stdcall LockServer(BOOL fLock);

    // Implementation

private:

    ULONG m_dwRefCount;

public:

    CDBSrvFactory();

};

CDB::CDB() {

    m_dwRefCount=0;

}

CDB::~CDB()
{
}

HRESULT CDB::Read(short nTable, short nRow, LPTSTR lpszData)
{
    printf("CDB::Read\n");
    return NO_ERROR;
}

HRESULT CDB::GetNumRows(short nTable, short &nRows)
{
    return NO_ERROR;
}

HRESULT CDB::QueryInterface(REFIID riid, void** ppObject)
{
    if (riid==IID_IUnknown || riid==IID_IDB)
    {
        *ppObject=(IDB*) this;
    }
    else
    {
        return E_NOINTERFACE;
    }
    
    AddRef();

    return NO_ERROR;

}

ULONG CDB::AddRef() {

    g_dwRefCount++;

    m_dwRefCount++;

    return m_dwRefCount;

}

ULONG CDB::Release() {

    g_dwRefCount--;

    m_dwRefCount--;

    if (m_dwRefCount==0) {

        delete this;

        return 0;

    }

    return m_dwRefCount;

}

ULONG g_dwRefCount=0;

// Create a new database object and return a pointer to it.
HRESULT CDBSrvFactory::CreateInstance(IUnknown *pUnkOuter, REFIID riid, void** ppObject) {

    if (pUnkOuter!=NULL) {

        return CLASS_E_NOAGGREGATION;

    }

    CDB* pDB=new CDB;

    if (FAILED(pDB->QueryInterface(riid, ppObject))) {

        delete pDB;

        *ppObject=NULL;

        return E_NOINTERFACE;

    }

    return NO_ERROR;

}

HRESULT   CDBSrvFactory::LockServer(BOOL fLock) {

    if (fLock) {

        g_dwRefCount++;

    }

    else {

        g_dwRefCount--;

    }

    return NO_ERROR;

}

CDBSrvFactory::CDBSrvFactory() {

    m_dwRefCount=0;

}

HRESULT CDBSrvFactory::QueryInterface(REFIID riid, void** ppObject) {

    if (riid==IID_IUnknown || riid==IID_IClassFactory) {

        *ppObject=(IDB*) this;

    }

    else {

        return E_NOINTERFACE;

    }

    AddRef();

    return NO_ERROR;

}

ULONG CDBSrvFactory::AddRef() {

    g_dwRefCount++;

    m_dwRefCount++;

    return m_dwRefCount;

}

ULONG CDBSrvFactory::Release() {

    g_dwRefCount--;

    m_dwRefCount--;

    if (m_dwRefCount==0) {

        delete this;

        return 0;

    }

    return m_dwRefCount;

}

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppObject) {
    if (rclsid!=CLSID_DBSAMPLE) {
        return CLASS_E_CLASSNOTAVAILABLE;
    }

    CDBSrvFactory *pFactory= new CDBSrvFactory;

    if (FAILED(pFactory->QueryInterface(riid, ppObject))) {

        delete pFactory;

        *ppObject=NULL;

        return E_INVALIDARG;

    }

    return NO_ERROR;

}


HRESULT _stdcall DllCanUnloadNow() {
    if (g_dwRefCount) {
        return S_FALSE;
    }
    else {
        return S_OK;
    }
}

STDAPI DllRegisterServer(void)
{
    HKEY hKeyCLSID, hKeyInproc32;
    DWORD dwDisposition;
    if (RegCreateKeyEx(HKEY_CLASSES_ROOT, 
                "CLSID\\{30DF3430-0266-11cf-BAA6-00AA003E0EED}", 
            NULL, "", REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, 
            &hKeyCLSID, &dwDisposition)!=ERROR_SUCCESS) {
        return E_UNEXPECTED;
    }
 
    return NOERROR;
}
STDAPI DllUnregisterServer(void) {
    if (RegDeleteKey(HKEY_CLASSES_ROOT, 
                "CLSID\\{30DF3430-0266-11cf-BAA6-00AA003E0EED}\\InprocServer32") !=ERROR_SUCCESS) {
        return E_UNEXPECTED;
    }
    if (RegDeleteKey(HKEY_CLASSES_ROOT, 
                "CLSID\\{30DF3430-0266-11cf-BAA6-00AA003E0EED}") !=ERROR_SUCCESS) {
        return E_UNEXPECTED;
    }
    return NOERROR;
}

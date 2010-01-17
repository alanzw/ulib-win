#include <windows.h>
#include <tchar.h>
#include <objbase.h>
#include <unknwn.h>

template <typename T>
class UComPtr
{
public:
    typedef T Type;
    typedef T* TypePtr;
public:
    UComPtr()
    : _p(0)
    {
    }

    UComPtr(T * p)
    {
        if (0 != (_p=p))
        {
            _p->AddRef();
        }
    }

    ~UComPtr()
    {
        if(_p)
        {
            _p->Release();
        }
    }

    operator T*() const
    {
        return (T*)_p;
    }

    T& operator*() const
    {
        return *_p;
    }

    T* operator->() const
    {
        return (T*)_p;
    }
private:
    TypePtr _p;
};

IDBInitialize *g_pDBInitialize;

HRESULT InitComEntironment()
{
    HRESULT hres;
    hres =  CoInitializeEx(NULL,COINIT_MULTITHREADED);
    if (FAILED(hres))
        return hres;
    return S_OK;
}

HRESULT CreateSession()
{
    UComPtr<IDBCreateSession> pSession;

    HRESULT hResult = g_pDBInitialize->QueryInterface( IID_IDBCreateSession, ( void**)&pSession );
    if( FAILED( hResult ) )
    {
        return hResult;
    }

    hResult = pSession->CreateSession( NULL, IID_IOpenRowset, ( IUnknown** )&m_pIOpenRowset );
    if( FAILED( hResult ) )
    {
        return hResult;
    }
    return S_OK;
}

int _tmain()
{
    TCHAR szInitStr[1024];
    UINT uTimeout = 15U;
    swprintf( szInitStr,
              _T("Provider=SQLOLEDB;Data Source=%s;Initial Catalog=%s;UserId=%s;Password=%s;Connect Timeout=%u"),
              pszServerName,
              pszDataSource,
              pszUserName,
              pszPassword,
              uTimeout );
    UComPtr<IDataInitialize> pIDataInitialize;
    //
    HRESULT hResult = ::CoCreateInstance( CLSID_MSDAINITIALIZE, NULL,
        CLSCTX_INPROC_SERVER, IID_IDataInitialize, ( void** )&pIDataInitialize );

    if( FAILED( hResult ) )
        return hResult;
    //
    hResult = pIDataInitialize->GetDataSource( NULL, CLSCTX_INPROC_SERVER,
        (LPCOLESTR)szInitStr, IID_IDBInitialize, ( IUnknown** )&m_pDBInitialize);
    if( FAILED( hResult ) )
        return hResult;
    //
    hResult = m_pDBInitialize->Initialize();
    if( FAILED( hResult ) )
        return hResult;

    return 0;
}

#if 0

#define INITGUID
#define DBINITCONSTANTS
#define INC_OLE2

#include <windows.h>

#include <ole2ver.h>
#include <cguid.h>
#include <oledb.h>
#include <oledberr.h>

#include <stdio.h>

DEFINE_GUID(CLSID_SampProv, 0xE8CCCB79L,0x7C36,0x101B,0xAC,0x3A,0x00,0xAA,0x00,0x44,0x77,0x3D);

void abc( BYTE p )
{
    FILE *fp = fopen("z.txt", "a+");
    fprintf(fp, "%c", p);
    fclose(fp);
}

void abc1( char *p )
{
    FILE *fp=fopen("z.txt","a+");
    fprintf(fp,"%s",p);
    fclose(fp);
}

IDBInitialize* pIDBInitialize;IDBCreateSession *pIDBCreateSession;
DBID dbcolid;
IOpenRowset* pIOpenRowset;
IAccessor *pIAccessor;
HACCESSOR hAccessor;
DBBINDING rgBind;
BYTE* pRowData;
IRowset* pIRowset;
ULONG cRowsObtained,iRow;
HROW rghRows[100],*pRows=&rghRows[0];
char aa[200];

int main()
{
    OleInitialize( NULL );
    CoCreateInstance(CLSID_SampProv, 0, CLSCTX_INPROC_SERVER, IID_IDBInitialize,
           (void **)&pIDBInitialize);
    pIDBInitialize->Initialize();
    pIDBInitialize->QueryInterface(IID_IDBCreateSession, (void**)&pIDBCreateSession);
    pIDBCreateSession->CreateSession(0, IID_IOpenRowset, (IUnknown**)&pIOpenRowset);
    dbcolid.eKind = DBKIND_NAME;
    dbcolid.uName.pwszName = L"z.csv";
    pIOpenRowset->OpenRowset(0, &dbcolid, 0, IID_IRowset, 0, 0, (IUnknown**)&pIRowset);
    rgBind.dwPart = DBPART_VALUE|DBPART_LENGTH|DBPART_STATUS;
    rgBind.eParamIO = DBPARAMIO_NOTPARAM;
    rgBind.iOrdinal = 1;
    rgBind.wType = DBTYPE_STR;
    rgBind.obValue = 8;
    rgBind.obLength = 0;
    rgBind.obStatus = 4;
    rgBind.cbMaxLen = 7;
    pIRowset->QueryInterface( IID_IAccessor, (void**)&pIAccessor );
    pIAccessor->CreateAccessor(DBACCESSOR_ROWDATA, 1, &rgBind, 0, &hAccessor,0);
    pRowData = (BYTE *) malloc(1000);
    pIRowset->GetNextRows(NULL, 0, 100, &cRowsObtained, &pRows);
    for ( iRow=0; iRow < cRowsObtained; iRow++ )
    {
        pIRowset->GetData(rghRows[iRow], hAccessor, pRowData );
        for ( int j =0 ;j<=4;j++)
            abc(pRowData [j+8]);
            abc('\n');
        }
    }
}

#endif

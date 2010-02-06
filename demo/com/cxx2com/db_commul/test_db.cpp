#include <stdio.h>

#include "db.h"

int main()
{
    ::CoInitialize(NULL);

    IClassFactory *pDBFactory=NULL;
    IUnknown *pDB = NULL;

    HRESULT hRes;
    hRes=CoGetClassObject(CLSID_DBSAMPLE, CLSCTX_SERVER, NULL, IID_IClassFactory,
    (void**) &pDBFactory);
    if (FAILED(hRes)) {
        TString csError;
        csError.format(_T("Error %d creating DB Object : %d"), hRes, GetLastError());
        //AfxMessageBox(csError);
        //return FALSE;
        printf("%s\n", csError.c_str());
        //return FALSE;
    }
    hRes=pDBFactory->CreateInstance(NULL, IID_IUnknown, (void**) &pDB);
    if (FAILED(hRes)) {
        TString csError;
        csError.format(_T("Error %d creating DB Object : %d"), hRes, GetLastError());
        //AfxMessageBox(csError);
        //return FALSE;
        printf("%s\n", csError.c_str());
        //return FALSE;
    }
    
    IDBAccess *pDBAccess = 0;
    hRes = pDB->QueryInterface(IID_IDBAccess, (void **)&pDBAccess);
    if (FAILED(hRes)) {
        TString csError;
        csError.format(_T("Error %d QueryInterface : %d"), hRes, GetLastError());
        //AfxMessageBox(csError);
        //return FALSE;
        printf("%s\n", csError.c_str());
        return 1;
    }
    
    
    pDBAccess->Read(0, 0, "");
    //pDB->Release();
    //pDBFactory->Release(); // Do not need th


    ::CoUninitialize();
    return 0;
}

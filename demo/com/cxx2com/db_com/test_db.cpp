#include <stdio.h>

#include "db.h"

int main()
{
    ::CoInitialize(NULL);

    IClassFactory *pDBFactory=NULL;
    IDB *pDB;

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
    hRes=pDBFactory->CreateInstance(NULL, IID_IDB, (void**) &pDB);
    if (FAILED(hRes)) {
        TString csError;
        csError.format(_T("Error %d creating DB Object : %d"), hRes, GetLastError());
        //AfxMessageBox(csError);
        //return FALSE;
        printf("%s\n", csError.c_str());
        //return FALSE;
    }
    
    pDB->Read(0, 0, "");

    //pDB->Release();
    //pDBFactory->Release(); // Do not need th


    ::CoUninitialize();
    return 0;
}

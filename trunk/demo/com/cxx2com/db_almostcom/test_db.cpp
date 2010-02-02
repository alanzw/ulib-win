#include <stdio.h>

#include "db.h"

int main()
{
    IDBSrvFactory *pDBFactory = 0;

    ::CoInitialize(NULL);
    
    HRESULT hRes;
    hRes=CoGetClassObject(CLSID_DBSAMPLE, CLSCTX_SERVER, NULL, 
        IID_IDBSrvFactory, (void**) &pDBFactory);
    if (FAILED(hRes)) {
        TString csError;
        csError.format(_T("Error %d creating DB Object : %d"), hRes, GetLastError());
        //AfxMessageBox(csError);
        //return FALSE;
        printf("%s\n", csError.c_str());
    }

    ::CoUninitialize();
    return 0;
}

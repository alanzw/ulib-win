#include "db.h"

int main()
{
    IDB *pDB = NULL;

    // Create a database object through the exported function and class factory
    // object.
    IDBSrvFactory *pDBFactory=NULL;

    DllGetClassFactoryObject((IDBSrvFactory **)&pDBFactory);
    pDBFactory->CreateDB((IDB**)&pDB);

    pDBFactory->Release(); // We no longer need the factory.

    if (NULL != pDB)
    {
        pDB->Release();
    }

    return 0;

}
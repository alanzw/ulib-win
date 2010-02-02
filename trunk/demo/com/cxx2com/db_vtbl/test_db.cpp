#include "db.h"

int main()
{
    CDB *pDB = NULL;
     
    CreateDB((IDB **)&pDB);
    
    if (NULL != pDB)
    {
        delete pDB;
    }

    return 0;
}

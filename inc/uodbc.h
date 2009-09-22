#ifndef U_ODBC_H
#define U_ODBC_H

#include "ulib.h"

namespace UODBC
{

SQLRETURN connect(char *dsnName, char *userId, char *passwd);

};

#endif // U_ODBC_H

#ifndef PG_RESOURCE_H
#define PG_RESOURCE_H

#include "reqhead.h"         /*  for struct ReqInfo  */

/*  Function prototypes  */
int Return_Resource (int conn, FILE * resource, struct ReqInfo * reqinfo);
FILE * Check_Resource  (struct ReqInfo * reqinfo);
int Return_Error_Msg(int conn, struct ReqInfo * reqinfo);

#endif  /*  PG_RESOURCE_H  */

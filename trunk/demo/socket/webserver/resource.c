#include <string.h>
#include <stdio.h>

#include "resource.h"
#include "reqhead.h"
#include "helper.h"


/*  Change this string to change the root directory that
    the server will use, i.e. /index.html will translate
    here to /home/httpd/html/index.html                   */

#ifdef _WIN32
static char server_root[1000] = ".";
#else
static char server_root[1000] = "/home/httpd/html";
#endif /* _WIN32 */


/*  Returns a resource  */
int Return_Resource(int conn, FILE *resource, struct ReqInfo * reqinfo)
{
    int  i;
	int  n = 0;
	char buf[10001] = {0};

	while (!feof(resource))
	{

		memset(buf, 0, 10000);

		n = fread(buf, 10000, 1, resource);

		i = strlen(buf);

		printf(buf);

        if ( i < 0 )
        {
            Error_Quit("Error reading from file.");
        }

        if ( send(conn, buf, i, 0) < 1 )
        {
            Error_Quit("Error sending file.");
        }
    }

	fclose(resource);

    return 0;
}


/*  Tries to open a resource. The calling function can use
    the return value to check for success, and then examine
    errno to determine the cause of failure if neceesary.    */

FILE* Check_Resource(struct ReqInfo * reqinfo)
{

    /*  Resource name can contain urlencoded
    data, so clean it up just in case.    */
    CleanURL(reqinfo->resource);

    /*  Concatenate resource name to server root, and try to open  */
    strcat(server_root, reqinfo->resource);

    /* return open(server_root, O_RDONLY); */

    printf("%s\n", server_root);

    return fopen(server_root, "rb");

    /* return 0; */
}


/*  Returns an error message  */

int Return_Error_Msg(int conn, struct ReqInfo * reqinfo)
{

    char buffer[100];

    sprintf(buffer, "<HTML>\n<HEAD>\n<TITLE>Server Error %d</TITLE>\n"
                    "</HEAD>\n\n", reqinfo->status);
    Writeline(conn, buffer, strlen(buffer));

    sprintf(buffer, "<BODY>\n<H1>Server Error %d</H1>\n", reqinfo->status);
    Writeline(conn, buffer, strlen(buffer));

    sprintf(buffer, "<P>The request could not be completed.</P>\n"
                    "</BODY>\n</HTML>\n");
    Writeline(conn, buffer, strlen(buffer));

    return 0;

}

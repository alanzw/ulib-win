/*
 * =====================================================================================
 *
 *       Filename:  utcl.h
 *
 *    Description:  Class UTcl
 *
 *        Version:  1.0
 *        Created:  2009-8-28 18:11:45
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#ifndef U_TCL_H
#define U_TCL_H

#include <tcl.h>

class UTcl
{
public:
    UTcl();
    ~UTcl();
    //
    bool initialize();
    bool finalize();
private:
    Tcl_Interp * tcl_interp;
};

#endif // U_TCL_H


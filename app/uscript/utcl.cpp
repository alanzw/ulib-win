/*
 * =====================================================================================
 *
 *       Filename:  utcl.cpp
 *
 *    Description:  Class UTcl implement
 *
 *        Version:  1.0
 *        Created:  2009-8-28 18:13:23
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#include "utcl.h"

UTcl::UTcl()
: tcl_interp(NULL)
{}

UTcl::~UTcl()
{}

bool UTcl::initialize()
{
    tcl_interp = Tcl_CreateInterp();

    if ( tcl_interp == NULL )
    {
        return false;
    }

    return true;
}

bool UTcl::finalize()
{
    if ( NULL == tcl_interp )
    {
        return false;
    }
    Tcl_DeleteInterp(tcl_interp);
    return true;
}

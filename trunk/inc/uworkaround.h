/*
 * =====================================================================================
 *
 *       Filename:  uworkaround.h
 *
 *    Description:  Workaround for Variable Compilers
 *
 *        Version:  1.0
 *        Created:  2009-8-15 10:45:10
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#ifndef U_WORKAROUND_H
#define U_WORKAROUND_H

#include "udebug.h"

namespace huys
{

#if defined(_MSC_VER) && (_MSC_VER = 1200)

    class tracing_output_va
    {
    private:

    public:
        tracing_output_va()
        {

        }

        void operator()( const char* format, ... )
        {
            va_list args ;
            va_start( format, args ) ;
            UDebugger::odprintf(format, args) ;
            va_end( args );
        }
    };

#endif
};

#endif // U_WORKAROUND_H


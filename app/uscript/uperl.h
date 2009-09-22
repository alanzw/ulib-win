/*
 * =====================================================================================
 *
 *       Filename:  uperl.h
 *
 *    Description:  Class UPerl
 *
 *        Version:  1.0
 *        Created:  2009-8-5 3:42:33
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#ifndef U_PERL_H
#define U_PERL_H

#include <EXTERN.h>
#include <perl.h>

class UPerl
{
public:
    UPerl();
    ~UPerl();

    bool initialize();
    bool finalize();

    bool parse(const char* lpszText);
private:
    PerlInterpreter *my_perl;
};

#endif // U_PERL_H


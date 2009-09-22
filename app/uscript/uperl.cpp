/*
 * =====================================================================================
 *
 *       Filename:  uperl.cpp
 *
 *    Description:  UPerl Implement
 *
 *        Version:  1.0
 *        Created:  2009-8-5 3:45:49
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#include "uperl.h"

UPerl::UPerl()
{}

UPerl::~UPerl()
{}

bool UPerl::initialize()
{
    my_perl = perl_alloc();
    perl_construct(my_perl);
    return true;
}

bool UPerl::finalize()
{
    perl_destruct(my_perl);
    perl_free(my_perl);
    return 0;
}


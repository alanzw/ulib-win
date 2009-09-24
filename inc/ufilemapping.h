/*
 * =====================================================================================
 *
 *       Filename:  ufilemap.h
 *
 *    Description:  UFileMapping Class
 *
 *        Version:  1.0
 *        Created:  2009-9-24 22:49:21
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */
#ifndef U_FILEMAPPING_H
#define U_FILEMAPPING_H

#include "uobject.h"

class ULIB_API UFileMapping : public UKernelObject
{
public:
    UFileMapping();
    ~UFileMapping();
};

#endif // U_FILEMAPPING_H


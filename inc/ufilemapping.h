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
    enum {
        BUFFER_SIZE = 256
    };
public:
    UFileMapping();
    ~UFileMapping();

    //
    BOOL create(LPCTSTR sMapFilename);
    BOOL open(LPCTSTR sMapFilename);
    //
    BOOL mapViewOfFile();
    BOOL unmapViewOfFile();
    //
    BOOL write(LPCTSTR lpText, int cbBytes);
private:
    TCHAR m_sMapFilename[BUFFER_SIZE];
    //
    LPSTR m_lpBuffer;
};

#endif // U_FILEMAPPING_H


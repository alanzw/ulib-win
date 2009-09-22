/*
 * =====================================================================================
 *
 *       Filename:  ulog.h
 *
 *    Description:  class ULog
 *
 *        Version:  1.0
 *        Created:  2009-8-7 22:46:10
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#ifndef U_LOG_H
#define U_LOG_H

#include <stdio.h>
#include "ulib.h"

class ULIB_API ULog
{
public:
    ULog(const char *filename);
    ~ULog();
    void addLine(const char *sLine);
private:
    FILE *m_pFile;
};

#endif // U_LOG_H


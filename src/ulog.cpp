/*
 * =====================================================================================
 *
 *       Filename:  ulog.cpp
 *
 *    Description:  ULog implement
 *
 *        Version:  1.0
 *        Created:  2009-8-7 22:47:48
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#include <windows.h>
#include <tchar.h>

#include <assert.h>

#include "ulog.h"

ULog::ULog( const char *filename )
{
    m_pFile = fopen(filename, "a");
    if (NULL == m_pFile)
    {
        throw 22;
    }
}

ULog::~ULog()
{
    if ( NULL != m_pFile )
    {
        fclose(m_pFile);
        m_pFile = NULL;
    }
}

void ULog::addLine( const char *sLine )
{
    assert(NULL != m_pFile);
    fputs(sLine, m_pFile);
    fputs("\n", m_pFile);
}


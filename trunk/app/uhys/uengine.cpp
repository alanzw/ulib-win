/*
 * =====================================================================================
 *
 *       Filename:  uengine.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/01/2009 04:16:30 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#include "uengine.h"

namespace huys
{
namespace ScriptEngine
{

UEngine::UEngine()
{

}

UEngine::~UEngine()
{
}

void UEngine::doString(const char *str)
{
    m_parser.tokenize(str);
    m_parser.peek();
}

void UEngine::doFile(const char *file)
{
    FILE *pFile = NULL;
    pFile = fopen(file, "r");
    if (NULL == pFile)
    {
        return;
    }
    char buf[1024];

    while (!feof(pFile))
    {
        fgets(buf, 1024, pFile);
        buf[strlen(buf)-1] = '\0';
        this->doString(buf);
    }
    fclose(pFile);
}

}; // namespace ScriptEngine
}; // namespace huys


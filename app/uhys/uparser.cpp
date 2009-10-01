/*
 * =====================================================================================
 *
 *       Filename:  uparser.cpp
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  10/01/2009 02:50:40 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#include <iostream>
#include "uparser.h"

namespace huys
{

namespace ScriptEngine
{

UPaser::UPaser()
{
}

UPaser::~UPaser()
{}

bool UPaser::tokenize(const char *code)
{
    int nSpaceAt = 0;
    int nCommaAt = 0;
    int nCommentAt = 0;

    UStringA str = code;

    if (0 == str.size())
    {
        return false;
    }

    nSpaceAt = str.find(' ');
    nCommaAt = str.find(',');
    nCommentAt = str.find('%');

    // Comment or space line
    if (0 == nCommentAt || 0 == nSpaceAt)
    {
        return false;
    }

    //
    if (-1 == nSpaceAt)
    {
#ifdef HU_DEBUG
    std::cout << "1 : " << str.c_str() << std::endl;
#endif
        m_tokens.push_back(str);
        return true;
    }

#ifdef HU_DEBUG
    std::cout << "2 : " << str.substr(0, nSpaceAt).c_str() << std::endl;
#endif
    m_tokens.push_back(str.substr(0, nSpaceAt));

    if (-1 != nCommaAt)
    {
#ifdef HU_DEBUG
    std::cout << "3 : " << str.substr(nSpaceAt+1, nCommaAt).c_str() << std::endl;
    std::cout << "3 : " << str.substr(nCommaAt+1, str.size()).c_str() << std::endl;
#endif
        m_tokens.push_back(str.substr(nSpaceAt+1, nCommaAt));
        m_tokens.push_back(str.substr(nCommaAt+1, str.size()));
    }
    else
    {
#ifdef HU_DEBUG
    std::cout << "4 : " << str.substr(nSpaceAt+1, str.size()).c_str() << std::endl;
#endif
        m_tokens.push_back(str.substr(nSpaceAt+1, str.size()));
    }

    return true;
}

}; // ScriptEngine

}; // namespace huys

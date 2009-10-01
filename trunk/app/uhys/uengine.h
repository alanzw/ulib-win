/*
 * =====================================================================================
 *
 *       Filename:  uengine.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/01/2009 04:10:43 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#ifndef U_ENGINE_H
#define U_ENGINE_H

#include "uparser.h"

namespace huys
{
namespace ScriptEngine
{

class UEngine
{
public:
    UEngine();
    ~UEngine();

    void doString(const char *str);
    void doFile(const char *filename);
private:
    UPaser m_parser;
};

};
}; // namespace huys

#endif // U_ENGINE_H


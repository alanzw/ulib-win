/*
 * =====================================================================================
 *
 *       Filename:  upaser.h
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  10/01/2009 02:46:38 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#include "adt/ustring.h"
#include "adt/uvector.h"

namespace huys
{

namespace ScriptEngine
{

typedef huys::ADT::UStringAnsi UStringA;
typedef huys::ADT::UVector<UStringA> TokenVec;

class UPaser
{
public:
    UPaser();
    ~UPaser();
    //
    bool tokenize(const char *code);

    //
    void peek();
private:
    TokenVec m_tokens;
};

}; // namespace ScriptEngine

}; // namespace huys


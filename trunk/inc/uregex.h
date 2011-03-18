#ifndef U_REGEX_H
#define U_REGEX_H

#include "ulib.h"

/*
 * 1. Any character except
 *        [\^$.|?*+()
 *    match a single instance of themselves.
 * 2  A backslash escapes special characters to
 *    suppress their special meaning.
 * 3  \Q...\E
 *    Matches the characters between \Q and \E literally,
 *    suppressing the meaning of special characters.
 */

namespace URegex
{

class ULIB_API URegExp
{
public:
    URegExp();
    
    ~URegExp();
    
    bool isMatch(const char *s);
    
public:
    class URegExpMatch
    {
    public:
        URegExpMatch()
        {}
    };
private:
    
};

}; // namespace URegex

#endif // U_REGEX_H

/*
 * =====================================================================================
 *
 *       Filename:  usingleton.h
 *
 *    Description:  singleton
 *
 *        Version:  1.0
 *        Created:  2009-7-22 16:41:11
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#ifndef U_SINGLETON_H
#define U_SINGLETON_H

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName &);            \
    void operator=(const TypeName &)

#define DISALLOW_EVIL_CONSTRUCTOR(TypeName) \
    DISALLOW_COPY_AND_ASSIGN(TypeName)

namespace huys
{

namespace dp
{

namespace creational
{

class USingleton
{
public:
    static USingleton *instance()
    {
        if (!_current)
        {
            _current = new USingleton();
        }

        return _current;
    }
private:
    explicit USingleton() {}

    ~USingleton(){}

    static USingleton *_current;

    DISALLOW_EVIL_CONSTRUCTOR(USingleton);
};

}; // namespace creational
}; // namespace dp
}; // namespace huys

#endif // U_SINGLETON_H


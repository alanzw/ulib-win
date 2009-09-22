/*
 * =====================================================================================
 *
 *       Filename:  uobjectpool.h
 *
 *    Description:  Object Pool
 *
 *        Version:  1.0
 *        Created:  2009-7-22 20:13:02
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

namespace huys
{

namespace dp
{

namespace creational
{

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName &);            \
    void operator=(const TypeName &)

#define DISALLOW_EVIL_CONSTRUCTOR(TypeName) \
    DISALLOW_COPY_AND_ASSIGN(TypeName

template <typename T>
class UObjectPool
{
public:
    UObjectPool();
    ~UObjectPool();
private:
    DISALLOW_EVIL_CONSTRUCTOR(UObjectPool);
};

}; // namespace creational
}; // namespace dp
}; // namespace huys


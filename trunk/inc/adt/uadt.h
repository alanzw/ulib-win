#ifndef U_ADT_H
#define U_ADT_H

#include "../ulib.h"

namespace huys
{

namespace ADT
{

// Private copy constructor and copy assignment ensure classes derived from
// class noncopyable cannot be copied.
class noncopyable
{
protected:
    noncopyable() {}
    ~noncopyable() {}
private:
    noncopyable(const noncopyable&);
    const noncopyable & operator=(const noncopyable &);
};


}; // namespace ADT

}; // namespace huys


#endif // U_ADT_H


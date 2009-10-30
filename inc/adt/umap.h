#ifndef HU_MAP_H
#define HU_MAP_H

#include "adt/upair.h"

namespace huys
{

namespace adt
{

template <typename Tk, typename Ty, class _Alloc = std::allocator<Ty> >
class UMap
{
public:
    typedef Ty* iterator;
    typedef const Ty* const_iterator;
    typedef size_t size_type;
    typedef UPair<Tk, Ty> value_type;
    typedef Tk key_type;
    typedef Ty referent_type;
public:
    UMap()
    {

    }

    ~UMap()
    {
    }

private:

};

}; // namespace adt

}; // namespace huys

#endif // HU_MAP_H

#ifndef HU_MAP_H
#define HU_MAP_H

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
    typedef Ty value_type;
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

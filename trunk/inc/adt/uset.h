
#ifndef HU_SET_H
#define HU_SET_H

namespace huys
{

namespace ADT
{

template <class T>
class USet
{
public:
    typedef T* iterator;
    typedef const T* const_iterator;
    typedef size_t size_type;
    typedef T value_type;

    USet()
    {
    }

    ~USet()
    {
    }

    USet(const USet &s)
    {
    }

    USet& operator=(const USet &s)
    {
        return *this;
    }


private:
};

}; // namespace ADT

}; // namespace huys

#endif // HU_SET_H

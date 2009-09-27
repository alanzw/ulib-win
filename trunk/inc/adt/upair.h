#ifndef U_PAIR_H
#define U_PAIR_H

namespace huys
{

namespace ADT
{

template <class T1, class T2>
class UPair
{
public:
    typedef T1 first_type;
    typedef T2 second_type;
public:
    UPair()
    : _first(T1()), _second(T2())
    {}

    ~UPair()
    {}

    UPair(const T1 &v1, const T2 &v2)
    : _first(v1), _second(v2)
    {}

    UPair(const UPair &p)
    : _first(p._first), _second(p._second)
    {}

    T1 & first()
    { return _first; }

    const T1& first() const
    { return _first; }

    T2 & second()
    { return _second; }

    const T2& second() const
    { return _second; }

    bool operator == (const UPair &p)
    {
        return (_first == p._first && _second == p._second);
    }

    bool operator != (const UPair &p)
    {
        return !(*this == p);
    }

    explicit const UPair & operator=(const UPair &p)
    {
        _first = p._first;
        _second = p._second;
        return *this;
    }
private:
    T1 _first;
    T2 _second;
};

template <class T1, class T2>
inline UPair<T1, T2> make_pair(const T1 &x, const T2 &y)
{
    return (UPair<T1, T2>(x, y));
}

}; // namespace ADT

}; // namespace huys


#endif // U_PAIR_H

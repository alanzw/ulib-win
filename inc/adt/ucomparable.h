#ifndef U_COMPARABLE_H
#define U_COMPARABLE_H

namespace huys
{

namespace ADT
{

template <typename T>
class comparable
{
    friend bool operator==(T const &a, T const &b) { return  a.equal_to(b); }
    friend bool operator!=(T const &a, T const &b) { return !a.equal_to(b); }
    friend bool operator>(T const &a, T const &b) { return a.greater_than(b); }
    friend bool operator<(T const &a, T const &b) { return !a.greater_than(b) && !a.equal_to(b); }
    friend bool operator>=(T const &a, T const &b) { return  a.equal_to(b) || a.greater_than(b); }
    friend bool operator<=(T const &a, T const &b) { return  !a.greater_than(b); }
};

}; // namespace ADT

}; // namespace huys

#endif // U_COMPARABLE_H

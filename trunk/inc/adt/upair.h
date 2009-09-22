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
    UPair();
    ~UPair();
private:
    T1 _first;
    T2 _second;
};

}; // namespace ADT

}; // namespace huys


#endif // U_PAIR_H

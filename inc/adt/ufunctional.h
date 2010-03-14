#ifndef U_FUNCTIONAL_H
#define U_FUNCTIONAL_H

namespace huys
{

namespace ADT
{

template <class T>
void swap(T x1, T x2)
{
    T temp = x1;
    x1 = x2;
    x2 = temp;
}


template <class TI, class TC>
TI partition(TI _f, TI _l, TC _p)
{
    for ( ; ; ++_f)
    {
        for (; _f != _l && _p(*_f); ++_f)
            ;

        if (_f == _l)
        {
            break;
        }

        for (; _f != --_l && !_p(*_l);)
            ;

        if (_f == _l)
        {
            break;
        }

        swap(*_f, *_l);
    }

    return _f;
}


}; // namespace ADT

}; // namespace huys



#endif // U_FUNCTIONAL_H

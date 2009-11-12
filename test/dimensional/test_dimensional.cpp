#include <iostream>
//#include <string>
#include <cstdio>
#include <cstring>
using namespace std;

//typedef int dimension[7]; // m  l  t  ...
//dimension const mass      = {1, 0, 0, 0, 0, 0, 0};
//dimension const length    = {0, 1, 0, 0, 0, 0, 0};
//dimension const time      = {0, 0, 1, 0, 0, 0, 0};

//
//dimension const force = {1, 1, -2, 0, 0, 0, 0};

#define AUX_WRAPPER_VALUE_TYPE int

#define BOOST_PP_CAT(a, b) BOOST_PP_CAT_I(a, b)

#define BOOST_PP_CAT_I(a, b) a ## b

#define AUX_WRAPPER_NAME BOOST_PP_CAT(AUX_WRAPPER_VALUE_TYPE, _)

#define BOOST_MPL_AUX_NTTP_DECL(T, x) T x

#define BOOST_STATIC_CONSTANT(type, assignment) enum { assignment }

#define AUX_WRAPPER_PARAMS(N) BOOST_MPL_AUX_NTTP_DECL(AUX_WRAPPER_VALUE_TYPE, N)

namespace mpl
{

template< AUX_WRAPPER_PARAMS(N) >
struct AUX_WRAPPER_NAME
{
    BOOST_STATIC_CONSTANT(AUX_WRAPPER_VALUE_TYPE, value = N);
    typedef AUX_WRAPPER_NAME type;
    typedef AUX_WRAPPER_VALUE_TYPE value_type;
    operator AUX_WRAPPER_VALUE_TYPE() const { return static_cast<AUX_WRAPPER_VALUE_TYPE>(this->value); }
};

template <class T1, class T2, class T3, class T4, class T5, class T6, class T7>
class vector
{
};

}; // namespace mpl

template <class T0, class T1, class T2>
struct tiny_size
  : mpl::int_<3> {};

typedef mpl::vector<
   mpl::int_<1>, mpl::int_<0>, mpl::int_<0>, mpl::int_<0>
 , mpl::int_<0>, mpl::int_<0>, mpl::int_<0>
> mass;

typedef mpl::vector<
   mpl::int_<0>, mpl::int_<1>, mpl::int_<0>, mpl::int_<0>
 , mpl::int_<0>, mpl::int_<0>, mpl::int_<0>
> length;

template <class T, class Dimensions>
struct quantity
{
    explicit quantity(T x)
       : m_value(x)
    {}

    T value() const { return m_value; }
 private:
    T m_value;
};

template <class T, class D>
quantity<T,D>
operator+(quantity<T,D> x, quantity<T,D> y)
{
  return quantity<T,D>(x.value() + y.value());
}

template <class T, class D>
quantity<T,D>
operator-(quantity<T,D> x, quantity<T,D> y)
{
  return quantity<T,D>(x.value() - y.value());
}


int main()
{
    quantity<float,length> l( 1.0f );
    quantity<float,mass> m( 2.0f );

    //m = l;    // compile-time type error

    quantity<float,length> len1( 1.0f );
    quantity<float,length> len2( 2.0f );

    len1 = len1 + len2;   // OK

    // len1 = len2 + quantity<float,mass>( 3.7f ); // error

    return 0;
}

#include <iostream>
//#include <string>
#include <cstdio>
#include <cstring>
using namespace std;

template <typename T>
struct is_double
{
    enum { value = false };
};

template <>
struct is_double<double>
{
    enum { value = true };
};

template <typename T>
struct is_int
{
    enum { value = false };
};

template <>
struct is_int<int>
{
    enum { value = true };
};

template <typename T1, typename T2>
struct is_same
{
    enum { value = false };
};

template <typename T>
struct is_same<T, T>
{
    enum { value = true };
};


template <typename T>
struct is_refernce
{
    enum { value = false };
};

template <typename T>
struct is_refernce<T&>
{
    enum { value = true };
};

template <typename T>
struct add_const
{
    typedef const T type;
};

template <>
struct add_const<void>
{
    typedef void type;
};

template <typename T>
struct add_reference
{
    typedef T & type;
};

template <typename T>
struct add_const_ref
{
    typedef T const & type;
};

template <>
struct add_const_ref<void>
{
    typedef void type;
};

template <typename T1, typename T2>
bool test_type(T1 x, T2 y)
{
    return is_same<T1, T2>::value;
}

template <typename T>
void typeOf(T x)
{
    if (is_double<T>::value)
    {
        cout << "typeOf " << x << " :  double" << endl;
    }

    if (is_int<T>::value)
    {
        cout << "typeOf " << x << " :  int" << endl;
    }
}

template <typename c, typename x, typename y>
struct replace_type
{
    typedef c type;
};



template <typename T>
struct type_descriptor {
    static const char * name;
    operator const char *()
    { return name; }
};

template <typename T> const char* type_descriptor<T>::name = "Unknow type";
template<> const char* type_descriptor<char>::name = "char";
template<> const char* type_descriptor<const char>::name = "const char";
template<> const char* type_descriptor<volatile char>::name = "volatile char";
template<> const char* type_descriptor<const volatile char>::name = "const volatile char";
template<> const char* type_descriptor<unsigned char>::name = "unsigned char";
template<> const char* type_descriptor<const unsigned char>::name = "const unsigned char";
template<> const char* type_descriptor<volatile unsigned char>::name = "volatile unsigned char";
template<> const char* type_descriptor<const volatile unsigned char>::name = "const volatile unsigned char";
template<> const char* type_descriptor<signed char>::name = "signed char";
template<> const char* type_descriptor<const signed char>::name = "const signed char";
template<> const char* type_descriptor<volatile signed char>::name = "volatile signed char";
template<> const char* type_descriptor<const volatile signed char>::name = "const volatile signed char";
template<> const char* type_descriptor<short>::name = "short";
template<> const char* type_descriptor<const short>::name = "const short";
template<> const char* type_descriptor<volatile short>::name = "volatile short";
template<> const char* type_descriptor<const volatile short>::name = "const volatile short";
template<> const char* type_descriptor<unsigned short>::name = "unsigned short";
template<> const char* type_descriptor<const unsigned short>::name = "const unsigned short";
template<> const char* type_descriptor<volatile unsigned short>::name = "volatile unsigned short";
template<> const char* type_descriptor<const volatile unsigned short>::name = "const volatile unsigned short";
template<> const char* type_descriptor<int>::name = "int";
template<> const char* type_descriptor<const int>::name = "const int";
template<> const char* type_descriptor<volatile int>::name = "volatile int";
template<> const char* type_descriptor<const volatile int>::name = "const volatile int";
template<> const char* type_descriptor<unsigned int>::name = "unsigned int";
template<> const char* type_descriptor<const unsigned int>::name = "const unsigned int";
template<> const char* type_descriptor<volatile unsigned int>::name = "volatile unsigned int";
template<> const char* type_descriptor<const volatile unsigned int>::name = "const volatile unsigned int";
template<> const char* type_descriptor<long>::name = "long";
template<> const char* type_descriptor<const long>::name = "const long";
template<> const char* type_descriptor<volatile long>::name = "volatile long";
template<> const char* type_descriptor<const volatile long>::name = "const volatile long";
template<> const char* type_descriptor<unsigned long>::name = "unsigned long";
template<> const char* type_descriptor<const unsigned long>::name = "const unsigned long";
template<> const char* type_descriptor<volatile unsigned long>::name = "volatile unsigned long";
template<> const char* type_descriptor<const volatile unsigned long>::name = "const volatile unsigned long";
template<> const char* type_descriptor<long long>::name = "long long";
template<> const char* type_descriptor<const long long>::name = "const long long";
template<> const char* type_descriptor<volatile long long>::name = "volatile long long";
template<> const char* type_descriptor<const volatile long long>::name = "const volatile long long";
template<> const char* type_descriptor<unsigned long long>::name = "unsigned long long";
template<> const char* type_descriptor<const unsigned long long>::name = "const unsigned long long";
template<> const char* type_descriptor<volatile unsigned long long>::name = "volatile unsigned long long";
template<> const char* type_descriptor<const volatile unsigned long long>::name = "const volatile unsigned long long";
template<> const char* type_descriptor<float>::name = "float";
template<> const char* type_descriptor<const float>::name = "const float";
template<> const char* type_descriptor<volatile float>::name = "volatile float";
template<> const char* type_descriptor<const volatile float>::name = "const volatile float";
template<> const char* type_descriptor<double>::name = "double";
template<> const char* type_descriptor<const double>::name = "const double";
template<> const char* type_descriptor<volatile double>::name = "volatile double";
template<> const char* type_descriptor<const volatile double>::name = "const volatile double";
template<> const char* type_descriptor<long double>::name = "long double";
template<> const char* type_descriptor<const long double>::name = "const long double";
template<> const char* type_descriptor<volatile long double>::name = "volatile long double";
template<> const char* type_descriptor<const volatile long double>::name = "const volatile long double";

template<typename T>
struct type_descriptor<T*> {
    operator const char*()
    {
        static char name[1024];
        strcpy(name, "pointer to ");
        strcat(name, type_descriptor<T>());
        return name;
    }
};

template<typename T>
struct type_descriptor<T &> {
    operator const char*()
    {
        static char name[1024];
        strcpy(name, "reference to ");
        strcat(name, type_descriptor<T>());
        return name;
    }
};

template<typename T, int size>
struct type_descriptor<T[size]>{
    operator const char*()
    {
        static char name[1024];
        strcpy(name, "array of ");
        strcat(name, type_descriptor<T>());
        char temp[256] = {0};
        sprintf(temp, " which size = %d", size);
        strcat(name, temp);
        return name;
    }
};

template<typename retType>
struct type_descriptor<retType(*)()>
{
    operator const char*()
    {
        static char name[1024];
        strcpy(name, "function return ");
        strcat(name, type_descriptor<retType>());
        strcat(name, " and has no argument");
        return name;
    }
};

template<typename retType, typename argType>
struct type_descriptor<retType(*)(argType)>
{
    operator const char*()
    {
        static char name[1024];
        strcpy(name, "function return ");
        strcat(name, type_descriptor<retType>());
        strcat(name, " and with ");
        strcat(name, type_descriptor<argType>());
        strcat(name, " as first argument");
        return name;
    }
};

template<typename retType, typename arg1Type, typename arg2Type>
struct type_descriptor<retType(*)(arg1Type, arg2Type)>
{
    operator const char*(){
        static char name[1024];
        strcpy(name, "function return ");
        strcat(name, type_descriptor<retType>());
        strcat(name, " and with ");
        strcat(name, type_descriptor<arg1Type>());
        strcat(name, " as first argument, ");
        strcat(name, type_descriptor<arg2Type>());
        strcat(name, " as second argument");
        return name;
    }
};

template <typename T>
const char *describe(T x)
{
    return type_descriptor<T>();
}

int main()
{
    int a = 0;
    int b = 0;
    double c = 0.1;

    cout << "a -- " ; typeOf(a);
    cout << "b -- " ; typeOf(b);
    cout << "c -- " ; typeOf(c);

    cout << "a -- " << describe(a) << endl;
    cout << "b -- " << describe(b) << endl;
    cout << "c -- " << describe(c) << endl;
    cout << "int[] -- " << type_descriptor<int[]>() << endl;
    cout << "int[1] -- " << type_descriptor<int[1]>() << endl;
    cout << "int& -- " << type_descriptor<int&>() << endl;

    cout << "is_same<int, int>::value : " << is_same<int, int>::value << endl;
    cout << "is_same<int, char>::value : " << is_same<int, char>::value << endl;
    cout << "test_type(a, b) : " << test_type(a, b) << endl;
    cout << "test_type(a, c) : " << test_type(a, c) << endl;

    cout << "is_refernce<int> : " << is_refernce<int>::value << endl;
    cout << "is_refernce<int&> : " << is_refernce<int&>::value << endl;

    cout << "is_same<int &, add_reference<int>::type> : " << is_same<int &, add_reference<int>::type>::value  << endl;
    cout << "is_same<int &, add_reference<int &>::type> : " << is_same<int &, add_reference<int&>::type>::value  << endl;
    cout << "is_same<double &, add_reference<int &>::type> : " << is_same<double &, add_reference<int&>::type>::value  << endl;

    cout << "is_same<int &, add_const_ref<int>::type> : " << is_same<int &, add_const_ref<int>::type>::value  << endl;
    cout << "is_same<int const &, add_const_ref<int>::type> : " << is_same<int const &, add_const_ref<int>::type>::value  << endl;

    return 0;
}

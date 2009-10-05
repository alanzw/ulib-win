/*
Compile time function execution (or Compile-Time Function Evaluation, CTFE) is the ability of a compiler,
that would normally compile a function to machine code and execute it at run time, to execute the
function at compile time. This is possible if the arguments to the function are known at compile time,
and the function does not make any reference to or attempt to modify any global state (is a pure function).
 */

#include <iostream>

template <int N>
struct Factorial
{
    enum
    {
        value = N * Factorial<N - 1>::value
    };
};

template <>
struct Factorial<0>
{
    enum { value = 1 };
};

int main()
{

    std::cout << "Factorial(4) = " << Factorial<4>::value << std::endl;

    return 0;
}

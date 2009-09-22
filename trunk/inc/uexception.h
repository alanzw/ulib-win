#ifndef U_EXCEPTION_H
#define U_EXCEPTION_H

#include "ulib.h"

#include <exception>

class ULIB_API UException : public std::exception
{
public:
    UException()
        : std::exception()
    {};

    ~UException()
    {}

protected:
private:
};

#endif // U_EXCEPTION_H


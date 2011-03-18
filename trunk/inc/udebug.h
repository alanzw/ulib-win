#ifndef U_DEBUG_H
#define U_DEBUG_H

#include "ulib.h"

namespace UDebugger
{

/*! \fn void __cdecl odprintf(const char *format, ...)
 *  \brief .
 *  \param format a string format.
 *  \param ... rest arguments.
 *  \return void.
 */
void ULIB_API __cdecl odprintf(const char *format, ...);

/*! \fn void __cdecl odprintf(const char *format, ...)
 *  \brief .
 *  \param format a string format.
 *  \param ... rest arguments.
 *  \return void.
 */
void UOutputDebugStringA(char * lpText);

/*! \class UTrace
 *  \brief Trace Tool class.
 */
class UTrace
{
public:
    UTrace();
    
    ~UTrace();
};

};  // namespace UDebugger

#endif // U_DEBUG_H

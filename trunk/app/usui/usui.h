#ifndef U_SUI_H
#define U_SUI_H

#include "ulua.h"

class USui
{
public:
    USui();
    ~USui();
    
private:
    typedef lua_CFunction sui_cb;
    
    template <class T>
    struct _callback
    {
        
    };
private:
    ULua m_l;
};

#endif // U_SUI_H

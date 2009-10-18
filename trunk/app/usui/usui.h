#ifndef U_SUI_H
#define U_SUI_H

#include <map>

#include "ulua.h"

template <class T>
class USui
{
    enum {
        
    
    };
public:
    USui(T * p)
    : m_pClass(p)
    {
        m_l.initialize();
        m_l.registerCallback("USui_Event", &USui<T>::lua_event);
    }
    
    ~USui()
    {
        m_l.finalize();
    }
    
    typedef lua_CFunction sui_cb;
    typedef void (T::*cbFoo)(int param);
    static bool comp(int l, int r) {return false;}

    static int lua_event(lua_State* luaVM)
    {
        //USui<T> *p = (USui<T> *)lua_touserdata(luaVM, 1);
        
        //if (NULL == p)
        //{
        //    return -1;
        //}
        const char *fooName = lua_tostring(luaVM, 1);
        int param = lua_tonumber(luaVM, 2);
        //->call(fooName, param);
        
        return 0;
    }
    
    int call(const char *sNameCb, int param)
    {
        if (std::map::end == cb_table.find(sNameCb))
        {
            return -1;
        }
        
        (m_pClass->*cb_table[sNameCb])(param);
        return 0
    }
    
    int insert(const char *sNameCb, cbFoo f)
    {
        cb_table.insert(sNameCb, f);
        return 0;
    }
    
    int doString(const char *s)
    {
        return m_l.doString(s);
    }
    
    int doFile(const char *sFilename)
    {
        return m_l.doFile(sFilename);
    }

    
    template <class TFunc>
    struct _callback0
    {
        const char *name;
        TFunc f;
    };
private:
    ULua m_l;
    T * m_pClass;
    
    std::map<const char*, cbFoo, bool(*)(int, int)> cb_table(&USui<T>::comp);
};

#endif // U_SUI_H

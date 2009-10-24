#ifndef U_SUI_H
#define U_SUI_H

#include <utility>
#include <map>
#include <string>

#include "ulua.h"


template <class T>
class USui
{
public:
    typedef lua_CFunction sui_cb;
    typedef void (T::*cbFoo)(bool param);
    //static bool comp(int l, int r) {return false;}
    typedef bool (T::*cbFunc)();

    static int lua_event(lua_State* luaVM)
    {
        USui<T> *p = USui<T>::instance();

        //if (NULL == p)
        //{
        //    return -1;
        //}
        //T *p = (T *)lua_touserdata(luaVM, 1);
        const char *fooName = lua_tostring(luaVM, 1);
        bool param = lua_toboolean(luaVM, 2);
        p->call(fooName, param);

        return 0;
    }

    static USui<T> * instance()
    {
        static USui<T> usui;

        return &usui;
    }

    void set(T *p)
    {
        m_pClass = p;
    }


    int call(const char *sNameCb, int param)
    {
        std::string str = sNameCb;
        //if (std::map<std::string, cbFoo>::end == cb_table.find(str))
        //{
        //    return -1;
        //}

        (m_pClass->*cb_table[str])(param);
        return 0;
    }

    int insert(const char *sNameCb, cbFoo f)
    {
        cb_table.insert(std::pair<std::string, cbFoo>(sNameCb, f));
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

private:
    ULua m_l;
    T * m_pClass;

    std::map<std::string, cbFoo> cb_table;
private:
    USui()
    : m_pClass(NULL)
    {
        m_l.initialize();
        m_l.registerCallback("USui_Event", &USui<T>::lua_event);
    }

    ~USui()
    {
        m_l.finalize();
    }
};


#endif // U_SUI_H

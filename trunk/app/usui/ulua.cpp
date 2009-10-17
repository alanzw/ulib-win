/*
 * =====================================================================================
 *
 *       Filename:  ulua.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/28/2009 07:13:39 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */
#include "ulua.h"

bool ULua::initialize()
{
    m_interpreter = lua_open();

    if (NULL == m_interpreter)
    {
        return false;
    }
    
    /* load various Lua libraries */
    luaopen_base(m_interpreter);
    luaopen_table(m_interpreter);
    //luaopen_io(m_interpreter);
    luaopen_string(m_interpreter);
    luaopen_math(m_interpreter);

    return true;
}

bool ULua::finalize()
{
    lua_close(m_interpreter);
}

int ULua::doString(const char *sText)
{
    return luaL_dostring(m_interpreter, sText);
}

int ULua::doFile(const char *sFilename)
{
    return luaL_dofile(m_interpreter, sFilename);
}

int ULua::registerCallback(const char *name, lua_CFunction pfn)
{
    lua_register(m_interpreter, name, pfn);
    return 0;
}

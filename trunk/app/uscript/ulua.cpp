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

    /* load various Lua libraries */
    luaopen_base(m_interpreter);
    luaopen_table(m_interpreter);
    luaopen_io(m_interpreter);
    luaopen_string(m_interpreter);
    luaopen_math(m_interpreter);

    return true;
}

bool ULua::finalize()
{
    lua_close(m_interpreter);
}

int ULua::parse(const char *sText)
{
    return luaL_dostring(m_interpreter, sText);
}

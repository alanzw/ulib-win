/*
 * =====================================================================================
 *
 *       Filename:  ulua.h
 *
 *    Description:  ULua Class
 *
 *        Version:  1.0
 *        Created:  09/28/2009 01:02:27 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#ifndef U_LUA_H
#define U_LUA_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#ifdef __cplusplus
}
#endif // __cplusplus

class ULua
{
    //typedef int (*lua_CFunction) (lua_State *L);
    
public:
    bool initialize();
    bool finalize();
    
    int doString(const char *sText);
    int doFile(const char *sFilename);
    int registerCallback(const char *name, lua_CFunction pfn);
private:
    lua_State *m_interpreter;
};

#endif // U_LUA_H


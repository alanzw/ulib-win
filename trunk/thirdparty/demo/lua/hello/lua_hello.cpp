#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#ifdef __cplusplus
}
#endif // __cplusplus

int main(int argc, char *argv[])
{
    lua_State *L;

    L = lua_open();


    if (0 == L)
    {
        printf("Error lua_open\n");
        return -1;
    }

    /* load various Lua libraries */
    luaL_openlibs(L);

    luaL_dostring(L, "print 'Hello World!'");

    lua_close(L);

    return 0;
}

#include <windows.h>
#include <tchar.h>

#include "uwinapp.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>


BOOL APIENTRY DllMain(HANDLE module, DWORD reason, LPVOID reserved)
{
    return TRUE;
}

static int l_uxwindow(lua_State * L)
{
    //const char * s = luaL_checkstring(L, 1);
    //MessageBox(NULL, s, "messagebox", MB_OK);
    
    UWinApp app;

    app.init(::GetModuleHandle(NULL));

    return app.run();
    
    //return 0;
}

__declspec(dllexport) int luaopen_lua_cxlib(lua_State * L)
{
    lua_pushcfunction(L, l_uxwindow);
    return 1;
}

#ifdef __cplusplus
}
#endif // __cplusplus


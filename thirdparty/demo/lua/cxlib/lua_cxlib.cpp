#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include "uwinapp.h"

#include "aui/aui_aboutdlg.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

HMODULE g_hModule = NULL;

BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        g_hModule = hModule;
        break;
    case DLL_THREAD_ATTACH:
        g_hModule = hModule;
        break;
    case DLL_THREAD_DETACH:
        break;

    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;

    UNREFERENCED_PARAMETER(lpReserved);
}

static int l_uxwindow(lua_State * L)
{
    //const char * s = luaL_checkstring(L, 1);
    //MessageBox(NULL, s, "messagebox", MB_OK);
    
    UWinApp app;

    app.init(g_hModule);

    return app.run();
    
    //return 0;
}

static int l_uhelpwindow(lua_State * L)
{
    MessageBox(NULL, "xxx", "hello", MB_OK);
    return 0;
}

static const struct luaL_reg cx_lib[] = {
    {"uxwindow", l_uxwindow},
    {"uhelpwindow", l_uhelpwindow},
    {NULL, NULL}
};

__declspec(dllexport) int luaopen_lua_cxlib(lua_State * L)
{
    //lua_pushcfunction(L, l_uxwindow);
    //lua_pushcfunction(L, l_uhelpwindow);
    
    luaL_register(L, "lua_cxlib", cx_lib);
    
    return 1;
}

#ifdef __cplusplus
}
#endif // __cplusplus


#include "script.h"

LUALIB_API int (luaopen_cjson) (lua_State *L); 
void luaLoadLib(lua_State *L, const char *libname, lua_CFunction luafunc) {
    lua_pushcfunction(L, luafunc);
    lua_pushstring(L, libname);
    lua_call(L, 1, 0);
}

lua_State *
script_init(const char *filename)
{
    lua_State *L = luaL_newstate();

    luaL_openlibs(L);
    luaLoadLib(L, "cjson", luaopen_cjson);
    luaL_loadfile(L, filename);
    lua_pcall(L, 0,0,0);

    return L;
}

void 
script_release(lua_State *L)
{
    lua_close(L);
}

#include "script.h"

LUALIB_API int (luaopen_cjson) (lua_State *L); 

void
lua_loadlib(lua_State *L, const char *libname, lua_CFunction luafunc) {
    lua_pushcfunction(L, luafunc);
    lua_pushstring(L, libname);
    lua_call(L, 1, 0);
}

lua_State *
script_init(const char *filename)
{
    lua_State *L = luaL_newstate();

    luaL_openlibs(L);

    lua_loadlib(L, "cjson", luaopen_cjson);
    luaL_loadfile(L, filename);
    lua_pcall(L, 0,0,0);

    return L;
}

void 
script_release(lua_State *L)
{
    lua_close(L);
}

void
script_load_config (lua_State *L) {
}

int
script_check_func_exists(lua_State * L, const char *func_name) {
    int ret;

    lua_getglobal(L, func_name);  
    ret = lua_isfunction(L,lua_gettop(L));
    lua_pop(L,-1);

    return ret;
}

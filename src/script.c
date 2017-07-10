#include "tcpkit.h"
#include "local_addresses.h"
#include <stdlib.h>
#include <string.h>

LUALIB_API int (luaopen_cjson) (lua_State *L); 

void
lua_loadlib(lua_State *L, const char *libname, lua_CFunction luafunc) {
    lua_pushcfunction(L, luafunc);
    lua_pushstring(L, libname);
    lua_call(L, 1, 0);
}

void
script_pushtableinteger(lua_State* L , char* key , long value)
{
    lua_pushstring(L, key);
    lua_pushinteger(L, value);
    lua_settable(L, -3);
}

void
script_pushtablestring(lua_State* L , char* key , char* value)
{
    lua_pushstring(L, key);
    lua_pushstring(L, value);
    lua_settable(L, -3);
}

void
script_pushtablelstring(lua_State* L , char* key , char* value, int len)
{
    lua_pushstring(L, key);
    lua_pushlstring(L, value, len);
    lua_settable(L, -3);
}

lua_State *
script_init(const char *filename)
{
    lua_State *L = luaL_newstate();

    luaL_openlibs(L);

    lua_loadlib(L, "cjson", luaopen_cjson);
    if(luaL_dofile(L, filename)) {
         logger(ERROR,"%s", lua_tostring(L, -1));
    }

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

void
script_need_gc(lua_State* L)
{
#define LUA_GC_CYCLE_PERIOD 500
    static long gc_count = 0; 

    gc_count++;
    if (gc_count == LUA_GC_CYCLE_PERIOD) {
        lua_gc(L, LUA_GCSTEP, LUA_GC_CYCLE_PERIOD);
        gc_count = 0; 
    }  
}

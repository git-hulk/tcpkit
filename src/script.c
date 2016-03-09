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

static int
set_config(lua_State* L)
{
    struct tk_options *opts;
    opts = get_global_options();

    // discard any extra arguments passed in
    lua_settop(L, 1);
    // argument should be table
    luaL_checktype(L, 1, LUA_TTABLE);

    // stack state: | -1 => nil | index => table |
    lua_pushnil(L);
    while (lua_next(L, 1)) {
        // stack state: | -1 => value | -2 => key | index => table |
        lua_pushvalue(L, -2);
        // stack state: | -1 => key | -2 => value | -3 => key| index => table |
        const char *key = lua_tostring(L, -1);
        const char *val = lua_tostring(L, -2);
        if(!opts->server && strcmp(key, "server") == 0) {
            opts->server = strdup(val);
        } else if(!opts->port && strcmp(key, "port") == 0)  {
            opts->port = atoi(val);
        } else if(!opts->device && strcmp(key, "device") == 0)  {
            opts->device = strdup(val);
        } else if(!opts->device && strcmp(key, "log_file") == 0)  {
            opts->log_file= strdup(val);
        } else if(!opts->specified_addresses && strcmp(key, "local_address") == 0)  {
            if(!parse_addresses((char *)val)) {
                opts->specified_addresses = 1;
            }
        }

        lua_pop(L, 2);
        // stack state: | -1 => key | index => table |
    }
    // stack state: |index => table|
    return 1;
}

lua_State *
script_init(const char *filename)
{
    lua_State *L = luaL_newstate();

    luaL_openlibs(L);

    lua_register(L, "set_config", set_config);
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

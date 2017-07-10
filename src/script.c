#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "script.h"
#include "tcpkit.h"
#include "local_addresses.h"

LUALIB_API int (luaopen_cjson) (lua_State *);

void
lua_loadlib(lua_State *vm, const char *libname, lua_CFunction luafunc) {
    lua_pushcfunction(vm, luafunc);
    lua_pushstring(vm, libname);
    lua_call(vm, 1, 0);
}

void
script_pushtableinteger(lua_State *vm , char *key , long value) {
    lua_pushstring(vm, key);
    lua_pushinteger(vm, value);
    lua_settable(vm, -3);
}

void
script_pushtablestring(lua_State *vm , char *key , char *value) {
    lua_pushstring(vm, key);
    lua_pushstring(vm, value);
    lua_settable(vm, -3);
}

void
script_pushtablelstring(lua_State *vm , char *key, char *value, int len) {
    lua_pushstring(vm, key);
    lua_pushlstring(vm, value, len);
    lua_settable(vm, -3);
}

int
script_is_func_exists(lua_State *vm, const char *func_name) {
    int ret;
    lua_getglobal(vm, func_name);
    ret = lua_isfunction(vm, lua_gettop(vm));
    lua_pop(vm, -1);
    return ret;
}


lua_State *
script_create_vm(const char *filename) {
    int ret;
    lua_State *vm;

    vm  = luaL_newstate();
    luaL_openlibs(vm);
    lua_loadlib(vm, "cjson", luaopen_cjson);

    if(filename && access(filename, R_OK) == 0) {
        ret = luaL_dofile(vm, filename);
    } else {
        char *chunk = "\
        function process_packet(item) \
            if item.len > 0 then \
                local time_str = os.date('%Y-%m-%d %H:%M:%S', item.tv_sec)..'.'..item.tv_usec\
                local network_str = item.src .. ':' .. item.sport .. '=>' .. item.dst .. ':' .. item.dport \
                print(time_str, network_str, item.len, item.payload) \
            end \
        end \
        ";
        ret = luaL_dostring(vm, chunk);
    }
    if (ret != 0) { // load script file or chunk failed
        logger(ERROR,"%s", lua_tostring(vm, -1));
        return NULL;
    }
    if (!script_is_func_exists(vm, DEFAULT_CALLBACK)) {
        logger(ERROR,"%s\n", "function process_packet was not found");
        return NULL;
    }
    return vm;
}

void
script_release(lua_State *vm) {
    lua_close(vm);
}

void
script_need_gc(lua_State* vm) {
#define LUA_GC_CYCLE_PERIOD 500
    static long gc_count = 0; 

    gc_count++;
    if (gc_count == LUA_GC_CYCLE_PERIOD) {
        lua_gc(vm, LUA_GCSTEP, LUA_GC_CYCLE_PERIOD);
        gc_count = 0; 
    }  
}

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#include "lua.h"
#include "tcpkit.h"

lua_State *lua_state_create(const char *file, char *err) {
    lua_State *state;

    state = luaL_newstate();
    luaL_openlibs(state);

    if(access(file, R_OK) != 0) {
        snprintf(err, MAX_ERR_BUFF_SIZE, "open lua file(%s): %s ",
                file, strerror(errno));
        return NULL;
    }
    if (luaL_dofile(state, file) != 0) {
        snprintf(err, MAX_ERR_BUFF_SIZE, "%s", lua_tostring(state, -1));
        return NULL;
    }
    lua_getglobal(state, "process");
    if (!lua_isfunction(state, lua_gettop(state))) {
        lua_pop(state, -1);
        snprintf(err, MAX_ERR_BUFF_SIZE, "'process' function was not found");
        return NULL;
    }
    lua_pop(state, -1);
    return state;
}

void lua_table_push_boolean(lua_State *state, const char *key, int bool) {
    lua_pushstring(state, key);
    lua_pushboolean(state, bool);
    lua_settable(state, -3);
}

void lua_table_push_int(lua_State *state, const char *key, long value) {
    lua_pushstring(state, key);
    lua_pushinteger(state, value);
    lua_settable(state, -3);
}

void lua_table_push_string(lua_State *state, const char *key, char *value) {
    lua_pushstring(state, key);
    lua_pushlstring(state, value, strlen(value));
    lua_settable(state, -3);
}

void lua_table_push_cstring(lua_State *state, const char *key, const char *value, int size) {
    lua_pushstring(state, key);
    lua_pushlstring(state, value, size);
    lua_settable(state, -3);
}

void lua_need_gc(lua_State* state) {
#define LUA_GC_CYCLE_PERIOD 500
    static long gc_count = 0;

    gc_count++;
    if (gc_count == LUA_GC_CYCLE_PERIOD) {
        lua_gc(state, LUA_GCSTEP, LUA_GC_CYCLE_PERIOD);
        gc_count = 0;
    }
}

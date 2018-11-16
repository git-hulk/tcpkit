/**
 *   tcpkit --  toolkit to analyze tcp packet
 *   Copyright (C) 2018  @git-hulk
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 **/

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#include "vm.h"
#include "tcpikt.h"

lua_State *vm_open_with_script(const char *file, char *err_buf) {
    lua_State *vm;

    vm = luaL_newstate();
    luaL_openlibs(vm);

    if(access(file, R_OK) != 0) {
        snprintf(err_buf, MAX_ERR_BUFF_SIZE, "%s, path: %s", strerror(errno), file);
        return NULL;
    }
    if (luaL_dofile(vm, file) != 0) {
        snprintf(err_buf, MAX_ERR_BUFF_SIZE, "%s", lua_tostring(vm, -1));
        return NULL;
    }
    // check the callback function is exists
    lua_getglobal(vm, "process");
    if (!lua_isfunction(vm, lua_gettop(vm))) {
        lua_pop(vm, -1);
        snprintf(err_buf, MAX_ERR_BUFF_SIZE, "'process' function was not found in script file");
        return NULL;
    }
    lua_pop(vm, -1);
    return vm;
}

void vm_push_table_boolean(lua_State *vm, const char *key, int bool) {
    lua_pushstring(vm, key);
    lua_pushboolean(vm, bool);
    lua_settable(vm, -3);
}

void vm_push_table_int(lua_State *vm, const char *key, long value) {
    lua_pushstring(vm, key);
    lua_pushinteger(vm, value);
    lua_settable(vm, -3);
}

void vm_push_table_string(lua_State *vm, const char *key, char *value) {
    lua_pushstring(vm, key);
    lua_pushlstring(vm, value, strlen(value));
    lua_settable(vm, -3);
}

void vm_push_table_cstring(lua_State *vm, const char *key, char *value, int size) {
    lua_pushstring(vm, key);
    lua_pushlstring(vm, value, size);
    lua_settable(vm, -3);
}

void vm_need_gc(lua_State* vm) {
#define LUA_GC_CYCLE_PERIOD 500
    static long gc_count = 0;

    gc_count++;
    if (gc_count == LUA_GC_CYCLE_PERIOD) {
        lua_gc(vm, LUA_GCSTEP, LUA_GC_CYCLE_PERIOD);
        gc_count = 0;
    }
}
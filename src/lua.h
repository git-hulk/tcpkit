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

#ifndef TCPKIT_LUA_H
#define TCPKIT_LUA_H

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

lua_State *lua_state_create(const char *file, char *err);
void lua_table_push_boolean(lua_State *state, const char *key, int bool);
void lua_table_push_int(lua_State *state, const char *key, long value);
void lua_table_push_string(lua_State *state, const char *key, char *value);
void lua_table_push_cstring(lua_State *state, const char *key, const char *value, int size);
void lua_need_gc(lua_State* state);
void lua_close(lua_State *state);
#endif

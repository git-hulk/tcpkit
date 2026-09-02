/**
 *   tcpkit --  toolkit to analyze tcp packet
 *   Copyright (C) 2018  @git-hulk
 *
 *   SPDX-License-Identifier: MIT
 *
 *   Use of this source code is governed by the MIT license that can be found
 *   in the LICENSE file at the root of this repository.
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

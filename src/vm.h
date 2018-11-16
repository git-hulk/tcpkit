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

#ifndef TCPKIT_VM_H
#define TCPKIT_VM_H

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

lua_State *vm_open_with_script(const char *file, char *err_buf);
void vm_push_table_boolean(lua_State *vm, const char *key, int bool);
void vm_push_table_int(lua_State *vm, const char *key, long value);
void vm_push_table_string(lua_State *vm, const char *key, char *value);
void vm_push_table_cstring(lua_State *vm, const char *key, char *value, int size);
void vm_need_gc(lua_State* vm);
void vm_close(lua_State *state);

#endif //TCPKIT_VM_H

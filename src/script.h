#ifndef _SCRIPT_H_
#define _SCRIPT_H_

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

void script_release(lua_State *vm);
void script_need_gc(lua_State *vm);
lua_State *script_create_vm(const char *filename);

void script_pushtableinteger(lua_State *vm, char* key , long value);
void script_pushtablestring(lua_State *vm, char* key , char *value);
void script_pushtablelstring(lua_State *vm, char* key , char *value, int len);
int script_is_func_exists(lua_State *vm, const char *func_name);
#endif

#ifndef _SCRIPT_H_
#define _SCRIPT_H_

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

lua_State *script_init(const char *filename);
void script_release(lua_State *L);
void script_need_gc(lua_State *L);
int script_check_func_exists(lua_State * L, const char *func_name);
void script_load_config (lua_State *L);

void script_pushtableinteger(lua_State* L , char* key , long value);
void script_pushtablestring(lua_State* L , char* key , char* value);
void script_pushtablelstring(lua_State* L , char* key , char* value, int len);
#endif

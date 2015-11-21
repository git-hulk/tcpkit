#ifndef _SCRIPT_H_
#define _SCRIPT_H_

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

lua_State *script_init(const char *filename);
void script_release(lua_State *L);
#endif

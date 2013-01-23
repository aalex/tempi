#include "tempi/config.h" // include before check for HAVE_LUA
#ifdef HAVE_LUA

extern "C" {

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

int luaopen_mathx(lua_State *L);

} // extern "C"

#endif // HAVE_LUA


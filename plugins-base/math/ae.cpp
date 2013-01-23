#include "tempi/config.h" // include before check for HAVE_LUA
#ifdef HAVE_LUA

/*
* ae.c
* general purpose arithmetic expression package based on Lua
* Luiz Henrique de Figueiredo <lhf@tecgraf.puc-rio.br>
* 02 May 2012 21:41:57
* This code is hereby placed in the public domain.
*/

#include <string.h>
extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
} // extern "C"
#include "ae.h"
#include "lmathx.h"


extern "C" {

//extern int luaopen_mathx(lua_State *L);
static lua_State *L=NULL;

void ae_open(void)
{
 if (L!=NULL) return;
 L=luaL_newstate();
 luaopen_math(L);			/* open math libraries as globals */
 luaopen_mathx(L);
 lua_rawseti(L,LUA_REGISTRYINDEX,LUA_RIDX_GLOBALS);
 lua_settop(L,0);
 lua_pushnil(L);			/* slot for error message */
}

void ae_close(void)
{
 if (L==NULL) return;
 lua_close(L);
 L=NULL;
}

double ae_set(const char* name, double value)
{
 lua_pushnumber(L,value);
 lua_setglobal(L,name);
 return value;
}

typedef struct LoadS
{
 const char *text[3];
 size_t size[3];
 int i;
} LoadS;

static const char *getS (lua_State *L, void *data, size_t *size)
{
 LoadS *S=(LoadS*) data;
 int i=S->i++;
 *size=S->size[i];
 (void)L;
 return S->text[i];
}

#define RETURN		"return "

double ae_eval(const char* expression)
{
 double value;
 int error=0;
 lua_getglobal(L,expression);		/* is it cached? */
 switch (lua_type(L,-1))
 {
  case LUA_TNIL:			/* no: compile, cache, and call it */
  {
	LoadS S;
	S.text[0]=RETURN;	S.size[0]=sizeof(RETURN)-1;
	S.text[1]=expression;	S.size[1]=strlen(expression);
	S.text[2]=NULL;		S.size[2]=0;
	S.i=0;
	error=lua_load(L,getS,&S,"=ae","text"); if (error) break;
	lua_pushvalue(L,-1);
	lua_setglobal(L,expression);
  }
  case LUA_TFUNCTION:			/* yes: call it */
	if (!lua_iscfunction(L,-1)) error=lua_pcall(L,0,1,0);
 }
 if (error)
 {
  value=0.0;
  lua_pushstring(L,strchr(lua_tostring(L,-1),' ')+1);
 }
 else if (lua_type(L,-1)==LUA_TNUMBER)
 {
  value=lua_tonumber(L,-1);
  lua_pushnil(L);
 }
 else if (lua_type(L,-1)==LUA_TBOOLEAN)
 {
  value=lua_toboolean(L,-1);
  lua_pushnil(L);
 }
 else
 {
  value=0.0;
  lua_pushliteral(L,"not a number");
 }
 lua_replace(L,1);			/* save error message */
 lua_settop(L,1);
 return value;
}

const char* ae_error(void)
{
 return lua_tostring(L,1);
}

} // extern "C"

#endif // HAVE_LUA


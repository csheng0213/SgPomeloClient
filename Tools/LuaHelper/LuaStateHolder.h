#ifndef _LUA_STATE_HOLDER_H_
#define _LUA_STATE_HOLDER_H_

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

class LuaStateHolder
{
public:
    LuaStateHolder()
        : _ls(0)
    {
        _ls = luaL_newstate();
        luaL_openlibs(_ls);
    }

    virtual ~LuaStateHolder()
    {
        if ( _ls )
        {
            lua_close(_ls);
            _ls = 0;
        }
    }
    
    inline lua_State * get_lua_state()
    {
        return _ls;
    }
    
protected:
    lua_State *_ls;
};

#endif // _LUA_STATE_HOLDER_H_
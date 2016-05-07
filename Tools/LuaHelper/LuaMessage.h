#ifndef _LUA_MESSAGE_H_
#define _LUA_MESSAGE_H_

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include <stack>

#include "LuaStateHolder.h"

class LuaMessage : public LuaStateHolder
{
public:
    LuaMessage();

    virtual ~LuaMessage();

    void push_value(lua_Number  v);

	void push_value(lua_Integer v);

	void push_value(const char *v);

    void push_value(bool v);

    void push_table();

    void push_table(const char *k);

    void push_table_end();
    
    void push_key_value(int k, lua_Number v);
    
    void push_key_value(int k, lua_Integer v);

    void push_key_value(int k, const char *v);

    void push_key_value(int k, bool v);

    void push_key_value(const char *k, lua_Number v);

    void push_key_value(const char *k, lua_Integer v);

    void push_key_value(const char *k, const char *v);

    void push_key_value(const char *k, bool v);
    
    inline void set_lua_state(lua_State *L)
    {
        if (_ls) {
            lua_close(_ls);
            _write_idx.pop();
            _ls = nullptr;
        }
        
        _ls = L;
        lua_newtable(_ls);
        _write_idx.push(1);

    }
    
    inline void clear_lua_state()
    {
        if(_ls)
        {
            _write_idx.pop();
            _ls = nullptr;
        }
    }

    

private:
    std::stack<int>     _write_idx;
};

#endif // _LUA_MESSAGE_H_
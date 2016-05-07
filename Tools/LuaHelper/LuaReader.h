#ifndef _LUA_MESSAGE_READER_H_
#define _LUA_MESSAGE_READER_H_

#include "LuaBaseReader.h"

class LuaReader : public LuaBaseReader
{
    typedef void (*error_handler) (const char *);

public:
    LuaReader();
    
    LuaReader(const char *file);

    LuaReader(const char *data, size_t size);

    virtual ~LuaReader(){}
    
    void bind_err_handler(error_handler handler);

    virtual void handle_error(const char *msg);

    bool state();
    
    inline void set_lua_state(lua_State *L)
    {
        if (_ls) {
            lua_close(_ls);
            _ls = nullptr;
        }
        
        _ls = L;
        _table_level++;
    }
    
    inline void clear_lua_state()
    {
        if(_ls)
        {
            _ls = nullptr;
        }
    }


private:
    bool            _state;
    error_handler   _handler;
};

#endif // _LUA_MESSAGE_READER_H_
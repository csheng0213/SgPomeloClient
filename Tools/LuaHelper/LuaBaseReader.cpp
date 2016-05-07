#include "LuaBaseReader.h"
#include "string.h"
#include "memory"

#include "cocos2d.h"

void LuaBaseReader::open()
{
    this->handle_open();
}

int LuaBaseReader::load_file(const char *fn)
{
    int r = luaL_dofile(_ls, fn);

    if (r == 0) 
		return 0;
	else
	{
		 std::string fileName = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(fn);
		 r = luaL_dofile(_ls, fileName.c_str());
		 if (r == 0)
			 return 0;
	}

    this->emit_error();
    return -1;
}

int LuaBaseReader::load_string(const char *s)
{
    int r = luaL_dostring(_ls, s);

    if (r == 0)
        return 0;

    this->emit_error();
    return -1;
}

int LuaBaseReader::load_string_buffer(const char *s, int size)
{
    int r = (luaL_loadbuffer(_ls, s, size, s) ||
             lua_pcall(_ls, 0, LUA_MULTRET, 0));

    if (r == 0)
        return 0;

    this->emit_error();
    return -1;
}

void LuaBaseReader::close()
{
    _table_level = 0;

    if ( _value )
    {
        delete [] _value;
        _value = 0;
        _value_len = 0;
    }
}

const char *LuaBaseReader::get_string(int idx)
{
    if ( !lua_istable(_ls, this->get_tbl_idx(-1)) ) return 0;

    lua_pushinteger(_ls, idx);
    lua_gettable(_ls, this->get_tbl_idx(-2));

    const char *r = store_value( lua_tostring(_ls, -1) );

    lua_pop(_ls, 1);
    return r;
}

const char *LuaBaseReader::get_string(const char *key)
{
    if ( !lua_istable(_ls, this->get_tbl_idx(-1)) ) return 0;

    const char *r = 0;
    int to_pop = 0;
    char *key_buf = strdup(key);

    do
    {
        char *p = strtok(key_buf, ".");
        if ( !p ) break;

        lua_pushstring(_ls, p);
        lua_gettable(_ls, this->get_tbl_idx(-2));
        to_pop++;

        int e = 0;
        while ( (p = strtok(0, ".")) )
        {
            if ( !lua_istable(_ls, -1) )
            {
                e = -1;
                break;
            }

            lua_pushstring(_ls, p);
            lua_gettable(_ls, -2);
            to_pop++;
        }

        if (e == 0)
        {
            r = store_value( lua_tostring(_ls, -1) );
        }
    }
    while (0);

    lua_pop(_ls, to_pop);
    free(key_buf);

    return r;
}

int LuaBaseReader::get_int(int idx, int *err)
{
    if ( !lua_istable(_ls, this->get_tbl_idx(-1)) )
    {
        if (err) *err = -1;
        return 0;
    }

    int r;
    int e = 0;

    lua_pushinteger(_ls, idx);
    lua_gettable(_ls, this->get_tbl_idx(-2));

    r = lua_type(_ls, -1);
    if (r == LUA_TNUMBER)
    {
        r = (int)lua_tointeger(_ls, -1);
    }
    else if (r == LUA_TBOOLEAN)
    {
        r = lua_toboolean(_ls, -1);
    }
    else
    {
        r = 0;
        e = -1;
    }

    if (err) *err = e;

    lua_pop(_ls, 1);
    return r;
}

int LuaBaseReader::get_int(const char *key, int *err)
{
    if ( !lua_istable(_ls, this->get_tbl_idx(-1)) )
    {
        if (err) *err = -1;
        return 0;
    }

    int r = 0;
    int e = 0;
    int to_pop = 0;
    char *key_buf = strdup(key);

    do
    {
        char *p = strtok(key_buf, ".");
        if ( !p )
        {
            e = -1;
            break;
        }

        lua_pushstring(_ls, p);
        lua_gettable(_ls, this->get_tbl_idx(-2));
        to_pop++;

        while ( (p = strtok(0, ".")) )
        {
            if ( !lua_istable(_ls, -1) )
            {
                e = -1;
                break;
            }

            lua_pushstring(_ls, p);
            lua_gettable(_ls, -2);
            to_pop++;
        }

        if (e == 0)
        {
            r = lua_type(_ls, -1);
            if (r == LUA_TNUMBER)
            {
                r = (int)lua_tointeger(_ls, -1);
            }
            else if (r == LUA_TBOOLEAN)
            {
                r = lua_toboolean(_ls, -1);
            }
            else
            {
                r = 0;
                e = -1;
            }
        }
    }
    while (0);

    lua_pop(_ls, to_pop);
    free(key_buf);

    if (err) *err = e;
    return r;
}

double LuaBaseReader::get_double(int idx, int *err)
{
    if ( !lua_istable(_ls, this->get_tbl_idx(-1)) )
    {
        if (err) *err = -1;
        return 0;
    }

    double r;
    int e = 0;

    lua_pushinteger(_ls, idx);
    lua_gettable(_ls, this->get_tbl_idx(-2));

    r = lua_type(_ls, -1);
    if (r == LUA_TNUMBER)
    {
        r = lua_tonumber(_ls, -1);
    }
    else if (r == LUA_TBOOLEAN)
    {
        r = (double)lua_toboolean(_ls, -1);
    }
    else
    {
        r = 0;
        e = -1;
    }

    if (err) *err = e;

    lua_pop(_ls, 1);
    return r;
}

double LuaBaseReader::get_double(const char *key, int *err)
{
    if ( !lua_istable(_ls, this->get_tbl_idx(-1)) )
    {
        if (err) *err = -1;
        return 0;
    }

    double r = 0;
    int e = 0;
    int to_pop = 0;
    char *key_buf = strdup(key);

    do
    {
        char *p = strtok(key_buf, ".");
        if ( !p )
        {
            e = -1;
            break;
        }

        lua_pushstring(_ls, p);
        lua_gettable(_ls, this->get_tbl_idx(-2));
        to_pop++;

        while ( (p = strtok(0, ".")) )
        {
            if ( !lua_istable(_ls, -1) )
            {
                e = -1;
                break;
            }

            lua_pushstring(_ls, p);
            lua_gettable(_ls, -2);
            to_pop++;
        }

        if (e == 0)
        {
            r = lua_type(_ls, -1);
            if (r == LUA_TNUMBER)
            {
                r = lua_tonumber(_ls, -1);
            }
            else if (r == LUA_TBOOLEAN)
            {
                r = (double)lua_toboolean(_ls, -1);
            }
            else
            {
                r = 0;
                e = -1;
            }
        }
    }
    while (0);

    lua_pop(_ls, to_pop);
    free(key_buf);

    if (err) *err = e;
    return r;
}

int LuaBaseReader::enter_table(const char *key)
{
    if ( !lua_istable(_ls, this->get_tbl_idx(-1)) ) return -1;

    int r = 0;
    int to_pop = 0;

    char *key_buf = strdup(key);
    do
    {
        char *p = strtok(key_buf, ".");
        if ( !p )
        {
            r = -1;
            break;
        }

        lua_pushstring(_ls, p);
        lua_gettable(_ls, this->get_tbl_idx(-2));
        to_pop++;
        if ( !lua_istable(_ls, -1) )
        {
            r = -1;
            break;
        }

        while ( (p = strtok(0, ".")) )
        {
            lua_pushstring(_ls, p);
            lua_gettable(_ls, -2);
            to_pop++;
            if ( !lua_istable(_ls, -1) )
            {
                r = -1;
                break;
            }
        }

        if (r == 0)
        {
            if (to_pop > 1)
            {
                lua_replace(_ls, -to_pop);
                lua_pop(_ls, to_pop - 2);
            }
            _table_level++;
            _read_idx.push(1);
        }
    }
    while (0);

    if (r == -1)
    {
        lua_pop(_ls, to_pop);
    }

    free(key_buf);

    return r;
}

int LuaBaseReader::enter_table(int key)
{
    if ( !lua_istable(_ls, this->get_tbl_idx(-1)) ) return -1;

    lua_pushinteger(_ls, key);
    lua_gettable(_ls, this->get_tbl_idx(-2));

    if ( !lua_istable(_ls, -1) )
    {
        lua_pop(_ls, 1);
        return -1;
    }

    _table_level++;
    _read_idx.push(1);

    return 0;
}

int LuaBaseReader::enter_table()
{
    if ( !lua_istable(_ls, this->get_tbl_idx(-1)) ) return -1;

    lua_pushinteger(_ls, _read_idx.top()++);
    lua_gettable(_ls, this->get_tbl_idx(-2));

    if ( !lua_istable(_ls, -1) )
    {
        lua_pop(_ls, 1);
        return -1;
    }

    _table_level++;
    _read_idx.push(1);

    return 0;
}

int LuaBaseReader::leave_table()
{
    if (_table_level > 0)
    {
        _read_idx.pop();
        _table_level--;
        lua_pop(_ls, 1);

        return 0;
    }
    else
    {
        return -1;
    }
}

int LuaBaseReader::fetch(int &h)
{
    if (h)
    {
        lua_pop(_ls, 1);
    }
    else
    {
        if (!lua_istable(_ls, this->get_tbl_idx(-1)) ) return 0;

        lua_pushnil(_ls);
        h = 1;
    }

    return lua_next(_ls, this->get_tbl_idx(-2));
}

void LuaBaseReader::fetch_break()
{
    lua_pop(_ls, 2);
}

int LuaBaseReader::get_key_int(int *err)
{
    int r;
    int e;

    r = lua_type(_ls, -2);
    if (r == LUA_TNUMBER)
    {
        r = (int)lua_tointeger(_ls, -2);
    }
    else if (r == LUA_TBOOLEAN)
    {
        r = lua_toboolean(_ls, -2);
    }
    else
    {
        r = 0;
        e = -1;
    }

    if (err) *err = e;

    return r;
}

const char *LuaBaseReader::get_key_string()
{
    return lua_tostring(_ls, -2);
}

int LuaBaseReader::get_value_int(int *err)
{
    int r;
    int e;

    r = lua_type(_ls, -1);
    if (r == LUA_TNUMBER)
    {
        r = (int)lua_tointeger(_ls, -1);
    }
    else if (r == LUA_TBOOLEAN)
    {
        r = lua_toboolean(_ls, -1);
    }
    else
    {
        r = 0;
        e = -1;
    }

    if (err) *err = e;

    return r;
}

double LuaBaseReader::get_value_double(int *err)
{
    double r;
    int e;

    if ( lua_isnumber(_ls, -1) )
    {
        r = lua_tonumber(_ls, -1);
        e = 0;
    }
    else
    {
        r = 0;
        e = -1;
    }

    if (err) *err = e;

    return r;
}

const char *LuaBaseReader::get_value_string()
{
    return lua_tostring(_ls,-1);
}

void LuaBaseReader::handle_error(const char *msg)
{
    printf("lua error: %s\n", msg);
}

const char *LuaBaseReader::store_value(const char *v)
{
    if ( !v ) return 0;

    size_t len = (size_t)strlen(v) + 1;
    if (len > _value_len)
    {
        _value_len = len;
        if (_value_len < 256) _value_len = 256;

        if ( _value ) delete[] _value;
        _value = new char[_value_len];
    }

    memcpy(_value, v, len);

    return _value;
}

void LuaBaseReader::emit_error()
{
    const char *msg = lua_tostring(_ls, -1);
    if (msg)
    {
        handle_error(msg);
        lua_pop(_ls, 1);
    }
}

#ifndef _LUA_BASE_READER_H_
#define _LUA_BASE_READER_H_

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "LuaStateHolder.h"
#include <stack>

class LuaBaseReader : public LuaStateHolder
{
public:
    LuaBaseReader()
        : _table_level(0)
        , _value(0)
        , _value_len(0)
    {
        _read_idx.push(1);
        this->open();
    }

    virtual ~LuaBaseReader()
    {
        this->close();
    }

public:
    void open();

    int load_file(const char *fn);

    int load_string(const char *s);

    int load_string_buffer(const char *s, int size);

    void close();

    const char *get_string(int idx);

    const char *get_string(const char *key);

    int get_int(int idx, int *err = 0);

    int get_int(const char *key, int *err = 0);

    double get_double(int idx, int *err = 0);

    double get_double(const char *key, int *err = 0);

    int enter_table(const char *key);

    int enter_table(int key);

    int enter_table();

    int leave_table();

    // fetch key-value pair into stack, get them by follows api
    int fetch(int &h);

    // used to break a fetch loop
    void fetch_break();

    // get the "key" value of the fetched key-value pair
    int get_key_int(int *err = 0);
    const char *get_key_string();

    // get the "value" value of the fetched key-value pair
    int get_value_int(int *err = 0);
    double get_value_double(int *err = 0);
    const char *get_value_string();

protected:
    virtual void handle_open() {}

    virtual void handle_error(const char *msg);

private:
    inline int get_tbl_idx(int idx)
    {
        return _table_level > 0 ? idx : LUA_GLOBALSINDEX;
    }

    const char *store_value(const char *v);

    void emit_error();

protected:
    int             _table_level;
    std::stack<int> _read_idx;

private:
    char    *_value;
    size_t   _value_len;
};

#endif // _LUA_BASE_READER_H_

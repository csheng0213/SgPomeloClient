#include "LuaReader.h"

extern "C"
{
#include "ltser.h"
}

LuaReader::LuaReader()
    : _state(true)
    , _handler(0)
{
    if (!_ls)
        _state = false;
}

LuaReader::LuaReader(const char *file)
{
    if (!_ls && !load_file(file))
        _state = false;
}

LuaReader::LuaReader(const char *data, size_t size)
    : _state(true)
    , _handler(0)
{
    if (!_ls || unserializetable(_ls, data, &size) != SLT_OK)
    {
        _state = false;
        return;
    }

    _table_level++;
}

void LuaReader::bind_err_handler(error_handler handler)
{
    _handler = handler;
}

void LuaReader::handle_error(const char *msg)
{
    if (_handler)
        _handler(msg);
}

bool LuaReader::state()
{
    return _state;
}

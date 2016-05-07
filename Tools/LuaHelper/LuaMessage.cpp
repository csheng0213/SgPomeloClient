#include "LuaMessage.h"

LuaMessage::LuaMessage()
{
    lua_newtable(_ls);
    _write_idx.push(1);
}

LuaMessage::~LuaMessage()
{
}

void LuaMessage::push_value(lua_Number v)
{
    lua_pushnumber(_ls, v);
    lua_rawseti(_ls, -2, _write_idx.top()++);
}

void LuaMessage::push_value(lua_Integer v)
{
    lua_pushinteger(_ls, v);
    lua_rawseti(_ls, -2, _write_idx.top()++);
}

void LuaMessage::push_value(const char *v)
{
	lua_pushstring(_ls, v);
	lua_rawseti(_ls, -2, _write_idx.top()++);
}

void LuaMessage::push_value(bool v)
{
    lua_pushboolean(_ls, v ? 1 : 0);
    lua_rawseti(_ls, -2, _write_idx.top()++);
}

void LuaMessage::push_table()
{
    lua_pushinteger(_ls, _write_idx.top()++);
    lua_newtable(_ls);
    _write_idx.push(1);
}

void LuaMessage::push_table(const char *k)
{
    lua_pushstring(_ls, k);
    lua_newtable(_ls);
    _write_idx.push(1);
}

void LuaMessage::push_table_end()
{
    lua_rawset(_ls, -3);
    _write_idx.pop();
}

void LuaMessage::push_key_value(int k, lua_Number v)
{
    lua_pushnumber(_ls, v);
    lua_rawseti(_ls, -2, k);
}

void LuaMessage::push_key_value(int k, lua_Integer v)
{
    lua_pushinteger(_ls, v);
    lua_rawseti(_ls, -2, k);
}

void LuaMessage::push_key_value(int k, const char *v)
{
    lua_pushstring(_ls, v);
    lua_rawseti(_ls, -2, k);
}

void LuaMessage::push_key_value(int k, bool v)
{
    lua_pushboolean(_ls, v);
    lua_rawseti(_ls, -2, k);
}

void LuaMessage::push_key_value(const char *k, lua_Number v)
{
    lua_pushstring(_ls, k);
    lua_pushnumber(_ls, v);
    lua_rawset(_ls, -3);
}

void LuaMessage::push_key_value(const char *k, lua_Integer v)
{
    lua_pushstring(_ls, k);
    lua_pushinteger(_ls, v);
    lua_rawset(_ls, -3);
}

void LuaMessage::push_key_value(const char *k, const char *v)
{
    lua_pushstring(_ls, k);
    lua_pushstring(_ls, v);
    lua_rawset(_ls, -3);
}

void LuaMessage::push_key_value(const char *k, bool v)
{
    lua_pushstring(_ls, k);
    lua_pushboolean(_ls, v);
    lua_rawset(_ls, -3);
}

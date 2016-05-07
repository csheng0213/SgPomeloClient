#ifndef ltser_h
#define ltser_h

#include "lua.h"

// serialize return code
#define SLT_OK 0

#define SLT_NeedTable 1
#define SLT_KeyTypeError 2
#define SLT_KeyOutOfRange 3
#define SLT_KeyTooLong 4
#define SLT_ValueTypeError 5
#define SLT_ValueTooLong 6
#define SLT_BufferTooSmall 7

#define SLT_BufferError 8
#define SLT_DataError 9

LUA_API int serializetable(lua_State *L, int n, void *dst, size_t *size);
LUA_API int unserializetable(lua_State *L, const void *src, size_t *size);

#endif
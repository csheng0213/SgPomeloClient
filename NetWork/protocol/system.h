#ifndef __PROTOCOL_SYSTEM_H__
#define __PROTOCOL_SYSTEM_H__

#include "base.h"

const uint32_t MAX_LUA_MSG_LEN = 0xfff0;
BEGIN_PROTO(proto_lua_custom, PROTO_SYSTEM_LUACUSTOM)
	uint8_t data[MAX_LUA_MSG_LEN];
END_PROTO

#endif
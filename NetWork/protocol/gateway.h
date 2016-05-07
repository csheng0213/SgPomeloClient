#ifndef __PROTOCOL_GATEWAY_H__
#define __PROTOCOL_GATEWAY_H__

#include "base.h"

BEGIN_PROTO(proto_dispatch_player, PROTO_GATEWAY_DISPATCH_PLAYER)
	char account[MAX_ACCOUNT_LEN];
	uint32_t accountID;
	uint32_t playerid;
	uint32_t serverid;
END_PROTO

BEGIN_PROTO(proto_dispatch_player_r, PROTO_GATEWAY_DISPATCH_PLAYER)
	uint32_t playerid;
	uint32_t serverid;
END_PROTO

#endif
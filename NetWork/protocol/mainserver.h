#ifndef __PROTOCOL_MAINSERVER_H__
#define __PROTOCOL_MAINSERVER_H__

#include "player/playerdata.h"

BEGIN_PROTO(proto_mainserver_register, PROTO_REGIST)
	uint32_t id;
	uint32_t curPlayer;
	uint32_t maxPlayer;
END_PROTO

BEGIN_PROTO(proto_mainserver_syninfo, PROTO_SYSTEM_SYNINFO)
	uint32_t curPlayer;
	uint32_t maxPlayer;
END_PROTO

BEGIN_PROTO(proto_get_playerdata, PROTO_MAINSERVER_GETPLAYERDATA)
	char account[MAX_ACCOUNT_LEN];
	uint32_t playerid;
END_PROTO

BEGIN_PROTO(proto_get_playerdata_r, PROTO_MAINSERVER_GETPLAYERDATA)
	SPlayerData data;
END_PROTO

BEGIN_PROTO(proto_get_playerdata_err, PROTO_MAINSERVER_GETPLAYERDATAERR)
	uint32_t playerid;
	int32_t errcode;
END_PROTO

BEGIN_PROTO(proto_save_playerdata, PROTO_MAINSERVER_SAVEPLAYERDATA)
	uint32_t playerid;
	int32_t optex;
	SPlayerData data;
END_PROTO

BEGIN_PROTO(proto_save_playerdata_r, PROTO_MAINSERVER_SAVEPLAYERDATA)
	uint32_t playerid;
	int32_t result;
END_PROTO

BEGIN_PROTO(proto_player_not_online, PROTO_MAINSERVER_PLAYERNOTONLINE)
	uint32_t playerid;
END_PROTO
#endif
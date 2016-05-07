#ifndef __PROTOCOL_PLAYER_H__
#define __PROTOCOL_PLAYER_H__

#include "base.h"

BEGIN_PROTO(proto_player_login, PROTO_PLAYER_LOGIN)
	int serverID;
	char accountid[MAX_ACCOUNT_LEN];
	char token[MAX_TOKEN_LEN];
	char version[MAX_ACCOUNT_LEN];
END_PROTO

BEGIN_PROTO(proto_player_login_r, PROTO_PLAYER_LOGIN)
	enum LoginResult {
		R_LOGINOK,
		R_FAILED,
	};
	LoginResult result;
	uint32_t uid;
	uint32_t serverid;
	uint64_t token;
	uint32_t state;
END_PROTO

BEGIN_PROTO(proto_player_logout, PROTO_PLAYER_LOGOUT)
	uint32_t uid;
END_PROTO

BEGIN_PROTO(proto_player_logout_r, PROTO_PLAYER_LOGOUT)
	enum LogoutReason {
		R_ENTERSERVERFAILED,	// 进入游戏失败
		R_LOGINATOTHERPLACE,	// 在其他地方登录
		R_TOKENINVALID,			// TOKEN无效
		R_ACCOUNTERROR,			// 账号验证失败
		R_ACCOUNTCHANGED,		// 账号已更改
		R_PLAYERINVALID,		// 玩家已失效
		R_PLAYERFULL,			// 玩家数量已满
		R_NOTINSERVER,			// 不在服务器中
		R_SYSTEMERROR,			// 系统其他错误
        R_ACCOUNTLOCK,          // 玩家被封禁
	};
	LogoutReason reason;
END_PROTO

BEGIN_PROTO(proto_player_reconnect, PROTO_PLAYER_RECONNECT)
	uint32_t uid;
	uint64_t token;
END_PROTO

BEGIN_PROTO(proto_player_bindaccount, PROTO_PLAYER_BINDACCOUNT)
	char account[MAX_ACCOUNT_LEN];
	char password[MAX_PASSWORD_LEN];
	char channel[MAX_CHANNEL_LEN];
END_PROTO

BEGIN_PROTO(proto_player_bindaccount_r, PROTO_PLAYER_BINDACCOUNT)
	uint32_t ret;
END_PROTO

#endif
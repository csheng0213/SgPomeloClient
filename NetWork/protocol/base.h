#ifndef __PROTOCOLBASE_H__
#define __PROTOCOLBASE_H__

#define MAX_ACCOUNT_LEN 21
#define MAX_PASSWORD_LEN 16
#define MAX_CHANNEL_LEN 16
#define MAX_TOKEN_LEN 36

struct proto_base_s {
	proto_base_s(uint32_t proto) {
		_proto = proto;
	}

	uint32_t _proto;
};

#define BEGIN_PROTO(name, proto) \
struct name : public proto_base_s { \
	name() : proto_base_s(proto) { \
		memset((char *)this + sizeof(proto_base_s), 0, sizeof(*this) - sizeof(proto_base_s)); \
	}

#define END_PROTO };

enum ProtocolBase {
	// 系统保留
	PROTO_SYSTEM_BEGIN = 0x0000,
	PROTO_REGIST = PROTO_SYSTEM_BEGIN,	// 服务器注册
	PROTO_SYSTEM_LUACUSTOM,		// 脚本自定义消息
	PROTO_SYSTEM_SYNINFO,		// 服务器同步信息
	PROTO_SYSTEM_MAX,

	// 网关
	PROTO_GATEWAY_BEGIN = 0x0100,
	PROTO_GATEWAY_DISPATCH_PLAYER = PROTO_GATEWAY_BEGIN,	// 请求为角色分配服务器
	PROTO_GATEWAY_MAX,

	// 主服
	PROTO_MAINSERVER_BEGIN = 0x0200,
	PROTO_MAINSERVER_GETPLAYERDATA = PROTO_MAINSERVER_BEGIN,		// 获取角色数据
	PROTO_MAINSERVER_GETPLAYERDATAERR,	// 获取角色数据失败
	PROTO_MAINSERVER_SAVEPLAYERDATA,	// 保存角色数据
	PROTO_MAINSERVER_PLAYERNOTONLINE,	// 角色不在线
	PROTO_MAINSERVER_MAX,

	// 玩家
	PROTO_PLAYER_BEGIN = 0x0300,
	PROTO_PLAYER_LOGIN = PROTO_PLAYER_BEGIN,	// 请求登录
	PROTO_PLAYER_LOGOUT,		// 请求登出
	PROTO_PLAYER_RECONNECT,		// 断线重连
	PROTO_PLAYER_BINDACCOUNT,	// 绑定账号
	PROTO_PLAYER_MAX,

	// 消息同步
	PROTO_SYN_BEGIN = 0x0400,
	PROTO_SYN_TIME = PROTO_SYN_BEGIN,	// 同步服务器时间
	PROTO_SYN_BASE_INFO,				// 同步基础信息
	PROTO_SYN_LOGIN_OVER,				// 登陆信息同步完毕
	PROTO_SYN_MAX,

	// 逻辑消息
	PROTO_LOGIC_BEGIN = 0x0900,
	PROTO_LOGIC_TEST = PROTO_LOGIC_BEGIN,	// 
    PROTO_LOGIC_BATTLE_TEST,
	PROTO_LOGIC_MAX,
};

BEGIN_PROTO(proto_register_server, PROTO_REGIST)
	uint32_t id;
	char name[21];
END_PROTO

BEGIN_PROTO(proto_register_server_r, PROTO_REGIST)
	enum RegisterResult {
		R_REGISTEROK,
		R_DUPLICATEID,
	};
	RegisterResult result;
END_PROTO

#endif
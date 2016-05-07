#ifndef __PROTOCOL_LOGIC_H__
#define __PROTOCOL_LOGIC_H__

#include "base.h"
#include "player/playerdata.h"

//------------------------- syn
BEGIN_PROTO(proto_syn_time, PROTO_SYN_TIME)
	uint32_t time;
END_PROTO

BEGIN_PROTO(proto_syn_base_info, PROTO_SYN_BASE_INFO)
	uint32_t m_uid;				// 角色ID
char m_name[MAX_NAME_LEN];	// 角色名
uint8_t m_level;			// 等级
uint32_t m_exp;				// 经验
uint32_t m_gold;			// 钻石
uint32_t m_money;			// 金币
uint8_t m_energy;			// 剩余体力
uint8_t m_energyEx;			// 额外体力值上限
uint32_t m_energyTime;		// 上次体力值维护时间
uint32_t m_fightStrength;	// 战斗力
float m_recharge;			// 充值金额
uint8_t m_vipLevel;			// 当前vip等级
END_PROTO
	BEGIN_PROTO(proto_syn_base_info_r, PROTO_SYN_BASE_INFO)
	enum TestResult {
		R_LOGINOK,
		R_FAILED,
};
TestResult result;
END_PROTO

	BEGIN_PROTO(proto_syn_login_over, PROTO_SYN_LOGIN_OVER)
	END_PROTO

	//------------------------- logic
BEGIN_PROTO(proto_logic_test, PROTO_LOGIC_TEST)
	uint32_t test;
END_PROTO
BEGIN_PROTO(proto_logic_test_r, PROTO_LOGIC_TEST)
	enum TestResult {
		R_LOGINOK,
		R_FAILED,
};
TestResult result;
END_PROTO

BEGIN_PROTO(proto_logic_battle_test, PROTO_LOGIC_BATTLE_TEST)
    uint32_t test;
END_PROTO

#endif
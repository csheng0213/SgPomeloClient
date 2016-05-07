#ifndef __PROTOCOL_LOGIC_H__
#define __PROTOCOL_LOGIC_H__

#include "base.h"
#include "player/playerdata.h"

//------------------------- syn
BEGIN_PROTO(proto_syn_time, PROTO_SYN_TIME)
	uint32_t time;
END_PROTO

BEGIN_PROTO(proto_syn_base_info, PROTO_SYN_BASE_INFO)
	uint32_t m_uid;				// ��ɫID
char m_name[MAX_NAME_LEN];	// ��ɫ��
uint8_t m_level;			// �ȼ�
uint32_t m_exp;				// ����
uint32_t m_gold;			// ��ʯ
uint32_t m_money;			// ���
uint8_t m_energy;			// ʣ������
uint8_t m_energyEx;			// ��������ֵ����
uint32_t m_energyTime;		// �ϴ�����ֵά��ʱ��
uint32_t m_fightStrength;	// ս����
float m_recharge;			// ��ֵ���
uint8_t m_vipLevel;			// ��ǰvip�ȼ�
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
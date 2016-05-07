#ifndef __PVPDATA_H__
#define __PVPDATA_H__

#include "playerdata.h"

#pragma pack(push, 1)

#define MAX_PVP_RECORD 10
#define MAX_ST_RECORD 50

struct SPVPEquip {
	uint16_t m_id;
	uint16_t m_level;
	SPVPEquip () {
		memset(this, 0, sizeof(*this));
	}
};

struct SPVPSkill {
	uint16_t m_id;
	uint16_t m_level;
	SPVPSkill() {
		memset(this, 0, sizeof(*this));
	}
};

struct SPVPCard {
	uint16_t m_id;
	uint16_t m_level;
	uint16_t m_attack;
	uint16_t m_defense;
	uint16_t m_evolution;
	SPVPEquip m_equip[MAX_ROLE_EQUIP_COUNT];
	SPVPSkill m_skill[MAX_ROLE_SKILL_COUNT];
	SPVPCard() {
		memset(this, 0, sizeof(*this));
	}
};

struct SPVPData {
	uint32_t m_id;
	char m_name[MAX_NAME_LEN];
	SPVPCard m_card[MAX_FIGHT_ROLE_COUNT];
	SPVPData() {
		memset(this, 0, sizeof(*this));
	}
};

struct SPVPRecord {
	uint32_t m_id;
	char m_name[MAX_NAME_LEN];
	uint8_t m_isChallenger:1;
	uint8_t m_isWinner:1;
	uint8_t m_isAvenged:1;
	uint8_t m_pvpType:3;
	uint8_t __dummy:2;
	uint32_t m_time;
	SPVPRecord() {
		memset(this, 0, sizeof(*this));
	}
};

struct SPVPHistory {
	SPVPRecord m_records[MAX_PVP_RECORD];
	SPVPHistory() {
		memset(this, 0, sizeof(*this));
	}
};


struct SSTRecord {
	uint32_t m_id;
	char m_name[MAX_NAME_LEN];
	uint8_t m_isWinner:1;
	uint8_t __dummy:7;
	uint32_t m_time;
	SSTRecord() {
		memset(this, 0, sizeof(*this));
	}
};

struct SSTHistory {
	SSTRecord m_records[MAX_ST_RECORD];
	SSTHistory() {
		memset(this, 0, sizeof(*this));
	}
};


#pragma pack(pop)

#endif
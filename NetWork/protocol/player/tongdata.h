#ifndef __TONGDATA_H__
#define __TONGDATA_H__

#pragma pack(push, 1)

#define MAX_TONG_MEMBER 200

struct SMemberInfo {
	uint32_t m_id;
	char m_name[MAX_NAME_LEN];
	SMemberInfo() {
		memset(this, 0, sizeof(*this));
	}
};

struct STongData {
	uint32_t m_id;
	char m_name[MAX_NAME_LEN];
	uint32_t m_creatorID;
	char m_creatorName[MAX_NAME_LEN];
	uint32_t m_leaderID;
	char m_leaderName[MAX_NAME_LEN];
	SMemberInfo m_members[MAX_TONG_MEMBER];

	STongData() {
		memset(this, 0, sizeof(*this));
	}
};

#pragma pack(pop)

#endif
#ifndef __FRDDATA_H__
#define __FRDDATA_H__

#include "playerdata.h"

#pragma pack(push, 1)

struct SFRDData {
	uint32_t m_frdFriend[MAX_FRIEND_COUNT];									//朋友id列表
	uint32_t m_frdRequest[MAX_FRIEND_REQUEST_COUNT];						//请求加朋友的id列表
																			//好友相关事件列表
	
	SFRDData() {
		memset(this, 0, sizeof(*this));
	}
};

struct SFRDPlayer {
	uint32_t id;
	char name[MAX_NAME_LEN];
	SFRDData data;
	SFRDPlayer(){
		memset(this, 0, sizeof(*this));
	}
};

#pragma pack(pop)

#endif
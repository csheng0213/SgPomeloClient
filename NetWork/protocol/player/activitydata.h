#ifndef __ACTIVITYDATA_H__
#define __ACTIVITYDATA_H__

#include "playerdata.h"

#pragma pack(push, 1)

enum EActivity
{
	MaterialBM = 1,
};

enum EActivityParam
{
	MBMQualityToEquip = 1,
};


struct SMaterial {
	uint32_t totalCount[MAX_ACT_MATERIALBM_COUNT];
	uint32_t sellCount[MAX_ACT_MATERIALBM_COUNT];
	uint32_t equipIdx[MAX_ACT_MATERIALBM_COUNT];

	SMaterial() {
		memset(this, 0, sizeof(*this));
	}
};

struct SActMaterial {
	uint32_t id;				//活动id
	SMaterial data;				//活动数据
	SActMaterial(){
		memset(this, 0, sizeof(*this));
	}
};

#pragma pack(pop)

#endif
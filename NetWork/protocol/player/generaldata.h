#ifndef __GENERALDATA_H__
#define __GENERALDATA_H__

#define MAX_GOLD_CHANGE_COUNT 500				//最大兑换元宝记录数

#pragma pack(push, 1)

enum EDataType{
	LoginAward = 1,
	GoldChange = 2,
};

struct SBaseGeneralData {
	uint32_t id;								//数据类型,EDataType
	SGeneralData(){
		memset(this, 0, sizeof(*this));
	}
};

//登陆领奖信息

//元宝兑换信息
struct SGoldChange : SBaseGeneralData {
	
	SGoldChange(){
		memset(this, 0, sizeof(*this));
	}
};


#pragma pack(pop)

#endif
#ifndef __GENERALDATA_H__
#define __GENERALDATA_H__

#define MAX_GOLD_CHANGE_COUNT 500				//���һ�Ԫ����¼��

#pragma pack(push, 1)

enum EDataType{
	LoginAward = 1,
	GoldChange = 2,
};

struct SBaseGeneralData {
	uint32_t id;								//��������,EDataType
	SGeneralData(){
		memset(this, 0, sizeof(*this));
	}
};

//��½�콱��Ϣ

//Ԫ���һ���Ϣ
struct SGoldChange : SBaseGeneralData {
	
	SGoldChange(){
		memset(this, 0, sizeof(*this));
	}
};


#pragma pack(pop)

#endif
#ifndef __PLAYERDATA_H__
#define __PLAYERDATA_H__

#define MAX_NAME_LEN 31							//
#define MAX_PROPERTY_COUNT 5					//
#define MAX_BAG_COUNT 250						//��������
#define MAX_APPRAISE_RECORD_COUNT 5				//��������¼��
#define MAX_LARGE_PASS_COUNT 100				//����ؿ���
#define MAX_SMALL_PASS_COUNT 10					//���С�ؿ���
#define MAX_FIGHT_ROLE_COUNT 6					//����ս������
#define MAX_ROLE_EQUIP_COUNT 4					//��ɫ������װ��װ����
#define MAX_ROLE_SKILL_COUNT 2					//��ɫ������װ��������
#define MAX_CONSTRUCT_COUNT 20					//���ӵ�н�����
#define MAX_STOCK_COUNT 5						//�����������
#define MAX_VIP_COUNT 20						//���vip�ȼ�
#define MAX_SECRET_STORE_COUNT 50				//��������̵���Ʒ��
#define MAX_TRAIN_ROLE_COUNT 3					//���������Ƭ����
#define MAX_TRAIN_VALID_DIA_COUNT 20			//������Ч�Ի�����
#define MAX_TRAIN_INVALID_DIA_COUNT 10			//������Ч�Ի�����
#define MAX_FRIEND_COUNT 100					//���������
#define MAX_FRIEND_REQUEST_COUNT 50				//������������
#define MAX_MARKET_ITEM_COUNT 20				//����г�������Ŀ��
#define MAX_ROLE_BOOK_COUNT 500					//����ɫ��ͼ����
#define MAX_EQUIP_BOOK_COUNT 500				//����ɫ��ͼ����
#define MAX_SKILL_BOOK_COUNT 500				//����ɫ��ͼ����
#define MAX_ROLE_FRAGMENT_COUNT 250				//����ɫ����Ƭ��
#define MAX_EQUIP_FRAGMENT_COUNT 150			//���װ������Ƭ��
#define MAX_SKILL_FRAGMENT_COUNT 150			//����ܿ���Ƭ��
#define MAX_ITEM_FRAGMENT_COUNT 250				//��������Ƭ��
#define MAX_ARTIFICE_QUALITY_COUNT 4			//�������Ʒ����
#define MAX_ITEM_DAILY_LIMIT 100				//��Ҫ����ÿ��ʹ�ô����ĵ��ߵĸ���
#define MAX_ACTIVITYEX_COUNT 32					//����������(ָ��ʱ��δﵽ�����콱��)
#define MAX_ACT_LV_COUNT 10						//����ȼ���vip���콱��¼
#define MAX_PASS_DORP_COUNTER_COUNT 3			//���ؿ��������������
#define MAX_TRAIN_RECORD_COUNT 4				//���������¼����������
#define MAX_ACT_MATERIALBM_COUNT 5				//���������˻������
#define MAX_ACT_RECHARGEGIFT_COUNT 7			//��ѧ���������
#define MAX_TREASURE_COUNT	8					//���ƥ��Ѱ���������

enum FightType {
	FIGHTTYPE_NONE,
	FIGHTTYPE_PVE,
	FIGHTTYPE_PVP,
	FIGHTTYPE_ST,
};
#define MAX_FIGHT_ROUND 3		// ���ս���غ���

#pragma pack(push)
#pragma pack(1)

//�׿�
struct SBlankCard {
	uint8_t m_step;		//�����Ĳ��裺0-���Կ�ʼ����1-ѡ���ɫ2-ѡ��������ʽ3-������4-����
	uint8_t m_trainedTimes;									//����������
	uint8_t m_singleTrainedTimes[MAX_PROPERTY_COUNT];		//������������������
	float m_attr[MAX_PROPERTY_COUNT];						//����ֵ
	uint8_t m_freeTimes;									//�����ˢ�´���
	uint32_t m_freeCD;										//���ˢ��CD
	uint8_t m_payTimes;										//����ˢ�´���
	uint8_t m_occupation;									//�׿����ְҵ����
	uint32_t m_freeCDTime;									//�ϴμ�����CD��ʱ��
	uint8_t m_lastTrainIdx;									//�ϴ�����ѡ��Ŀγ�����
	SBlankCard () {
		memset(this, 0, sizeof(*this));
	}
};
//�ɿ�(��ɫ��)
struct SRoleCard {
	uint16_t m_id;											//���ڵ��߱�ID��0Ϊ��Ч
	uint16_t m_attr[MAX_PROPERTY_COUNT];					//����ֵ
	uint16_t m_attack;										//������
	uint16_t m_defense;										//������
	uint16_t m_evolution;									//�����ȼ�
	uint16_t m_level;										//�ȼ�
	uint32_t m_exp;											//����
	uint8_t m_skillID;					// ����ID
	uint8_t m_skillLevel;				// ���ܵȼ�
	uint16_t m_skillExp;				// ���ܾ���
	void Reset() {
		memset(this, 0, sizeof(*this));
	}
	SRoleCard () {
		memset(this, 0, sizeof(*this));
	}
};
//װ����
struct SEquipCard {
	uint16_t m_id;											//ID
	uint8_t m_level;										//�ȼ�
	SEquipCard () {
		memset(this, 0, sizeof(*this));
	}
};
//���ܿ�
struct SSkillCard {
	uint16_t m_id;											//���ڵ��߱�ID��0Ϊ��Ч
	uint8_t m_level;										//�ȼ�
	SSkillCard () {
		memset(this, 0, sizeof(*this));
	}
};
//����
struct SItem {
	uint16_t m_id;											//���ڵ��߱�ID��0Ϊ��Ч
	SItem () {
		memset(this, 0, sizeof(*this));
	}
};
//��ɫ������
struct SRoleCardBag {
	SRoleCard m_roleCard;									//��Ƭ
	uint8_t m_useFlag;										//ʹ�ñ�ʾ��0-δʹ�� 1~6-��Ӧm_fighterList�е�����
	uint8_t m_seeFlag;										//�๤��ʾ��0-δʹ�� 2~3-��Ӧm_construct������
	void Reset() {
		memset(this, 0, sizeof(*this));
	}
	SRoleCardBag () {
		memset(this, 0, sizeof(*this));
	}
};
//װ��������
struct SEquipCardBag {
	SEquipCard m_equipCard;									//��Ƭ
	uint8_t m_useFlag;										//ʹ�ñ�ʾ��0-δʹ�� 1~6-��Ӧm_equipList�е�����
	SEquipCardBag () {
		memset(this, 0, sizeof(*this));
	}
};
//���ܿ�����
struct SSkillCardBag {
	SSkillCard m_skillCard;									//��Ƭ
	uint8_t m_useFlag;										//ʹ�ñ�ʾ��0-δʹ�� 1~6-��Ӧm_skillList�е�����
	SSkillCardBag () {
		memset(this, 0, sizeof(*this));
	}
};
//���߱���
struct SItemBag {
	SItem m_item;											//����
	uint16_t m_count;										//���ߵ�����
	SItemBag () {
		memset(this, 0, sizeof(*this));
	}
};

//����������¼
struct SAppraiseRecord {
	uint8_t m_first;										//�Ƿ��״�
	uint16_t m_count[4];									//1-4�Ǽ���Ӧ�ıȲ�ϵ��
	SAppraiseRecord () {
		memset(this, 0, sizeof(*this));
	}
};

//С�ؿ���̬��Ϣ
struct SPassInfo {
	uint8_t m_passStar[MAX_SMALL_PASS_COUNT];				//С�ؿ�����(0��ʾδͨ��)
	uint16_t m_passDailyLimit[MAX_SMALL_PASS_COUNT];		//�ؿ�ÿ������
	SPassInfo () {
		memset(this, 0, sizeof(*this));
	}
};

//ս���б��н�ɫ��װ�����б�
struct SRoleEquipList {
	uint8_t m_equip[MAX_ROLE_EQUIP_COUNT];					//����//����//��Ʒ//����
	SRoleEquipList () {
		memset(this, 0, sizeof(*this));
	}
};

//ս���б��н�ɫ�ļ��ܿ��б�
struct SRoleSkillList {
	uint8_t m_skill[MAX_ROLE_SKILL_COUNT];					//
	SRoleSkillList () {
		memset(this, 0, sizeof(*this));
	}
};

//������Դ
struct SConstructRes {
	uint32_t m_id;											//�ȼ�
	uint32_t m_amount;										//����
	SConstructRes () {
		memset(this, 0, sizeof(*this));
	}
};
//����
struct SConstruct {
	uint8_t m_level;										//�ȼ�
	uint32_t m_levelUpTime;									//�������ʱ��
	SConstructRes m_stock[MAX_STOCK_COUNT];					//��汳��
	uint8_t m_stockCount;									//���ÿ��������̬����
	uint32_t m_res;											//��Դ��
	uint32_t m_time;										//�ϴβ������ʱ��
	uint32_t m_CD;											//����CD
	bool m_canOutput;										//��ǰ�Ƿ���ò���
	bool m_canLevelup;										//���ڲ����Ŀ��Ƿ��������
	bool m_canTrain;										//��ɫ���Ƿ������������ɫ������Ч��
	bool m_canRes;											//�ɷ����Դ����ɫ������Ч��
	uint8_t m_crit;											//�ɱ�����
	uint8_t m_select;										//����ѡ����
	uint8_t m_overseeSlot;									//�๤ѧ��������λ��slot
	uint8_t m_request;										//����๤��������
	uint32_t m_requestCD;									//����๤���cd
	uint32_t m_requestTime;									//�ϴ�������ʱ��
	SConstruct () {
		memset(this, 0, sizeof(*this));
	}
};

//�����̵���Ʒ��Ϣ
struct SSStoreGoods {
	uint16_t index;											//����������Ʒ����
	uint16_t count;											//�Ѿ��������
	SSStoreGoods () {
		memset(this, 0, sizeof(*this));
	}
};

//�����̵���Ʒ��Ϣ
struct SSStoreNextGoods {
	uint16_t index;											//����������Ʒ����
	SSStoreNextGoods () {
		memset(this, 0, sizeof(*this));
	}
};

//����ϵͳ-�����ɫ
struct STRandomRole {
	float m_attr[MAX_PROPERTY_COUNT];					//����ֵ
	uint8_t m_occupation;									//ְҵ����
	STRandomRole () {
		memset(this, 0, sizeof(*this));
	}
};

// PVP��Ϣ
struct SPVPInfo {
	uint8_t m_leftTimes;			// ʣ�����
	uint8_t m_totalTimes;			// ������ʤ������
	uint8_t m_unbrokenTimes;		// ������ʤ����
	uint8_t m_maxUnbrokenTimes;		// �����ʤ����
	uint32_t m_refreshTime;			// �ϴ�ˢ��ʱ��
	uint32_t m_challengeTime;		// �ϴ���սʱ��
	SPVPInfo() {
		memset(this, 0, sizeof(*this));
	}
};

// ս����Ϣ
struct SFightPVE {
	uint8_t m_lIndex;
	uint8_t m_sIndex;
};
struct SFightPVP {
	uint32_t m_otherID;
	uint8_t m_pvpType:3;
	uint8_t __dummy:5;
};
struct SFightInfo {
	uint8_t m_fightType;
	uint32_t m_fightSeed;
	uint8_t m_selfOrder[MAX_FIGHT_ROUND];
	uint8_t m_otherOrder[MAX_FIGHT_ROUND];
	union {
		SFightPVE m_PVE;
		SFightPVP m_PVP;
	};
	void Reset() {
		memset(this, 0, sizeof(*this));
	}
	SFightInfo() {
		memset(this, 0, sizeof(*this));
	}
};

//�˲��г�������Ƹ��Ϣ
struct SMarketItem {
	uint8_t m_valid;										//�Ƿ�ʹ��
	uint8_t m_regionIdx;									//��������								
	uint8_t m_industryIdx;									//��ҵ����
	uint8_t m_companyIdx;									//��˾����
	uint8_t m_quality;										//��Ƹ�Ŀ�Ƭ�Ǽ�
	uint8_t m_priceIdx;										//�Ǽ���Ӧ�۸���������¼�����Ҫ��Ϊ��֧������
	uint32_t m_price;										//������ļ۸�
	uint8_t m_sellCount;									//����Ƹ����
	SMarketItem () {
		memset(this, 0, sizeof(*this));
	};
};
//�˲��г���Ϣ
struct SMarket {
	uint8_t m_sellCount;									//�����ѳ�����Ʒ��
	uint32_t m_freeRefreshTime;								//�ϴ����ˢ��ʱ��
	uint32_t m_freeRefreshCD;								//���ˢ��cd
	uint8_t m_freeRefreshCount;								//���������ˢ�´���
	SMarketItem m_item[MAX_MARKET_ITEM_COUNT];				//������Ƹ��Ŀ��Ϣ
	uint8_t m_sellFlag;										//������Ƹ���Ƿ����
	uint8_t m_refresh;										//ϵͳˢ������
	SMarket () {
		memset(this, 0, sizeof(*this));
	}
};

//�������˵�����Ϣ
struct SMaterialBMItem {
	uint8_t m_valid;										//�Ƿ�ʹ��
	uint32_t m_quality;										//��������ʣ��������
	uint32_t m_equipIdx;									//��Ӧ�Ǽ��¾����װ������
	uint32_t m_sellCount;									//�ѳ��۴���
	SMaterialBMItem () {
		memset(this, 0, sizeof(*this));
	};
};
//��������
struct SMaterialBM {
	uint8_t m_sellCount;									//�����ѳ�����Ʒ��
	SMaterialBMItem m_item[MAX_ACT_MATERIALBM_COUNT];		//������Ŀ��Ϣ
	uint8_t m_sellFlag;										//�����Ƿ����
	uint8_t m_refresh;										//ϵͳˢ������
	SMaterialBM () {
		memset(this, 0, sizeof(*this));
	}
};

//У԰Ѱ��
struct STreasure {
	uint32_t m_pids[MAX_TREASURE_COUNT];					//����Ѱ��ƥ�����id
	uint32_t m_searchTime;									//Ѱ��ʣ��ʱ��
	uint32_t m_robRes;										//��ٳɹ���õ���Դ�ܺ�
	uint8_t m_searchCount;									//����������
	uint8_t m_robCount;										//�����ٴ���
	uint8_t m_reward;										//�Ƿ�����콱
	uint8_t m_type;											//����
	STreasure () {
		memset(this, 0, sizeof(*this));
	}
};

//��Ƭ
struct SFragment {
	uint16_t m_id;								//��Ƭ��Ӧ��Ʒid
	uint16_t m_count;							//��Ƭ��
	SFragment () {
		memset(this, 0, sizeof(*this));
	}
};

// ����
struct SArtifice {
	uint16_t m_fragID[MAX_ARTIFICE_QUALITY_COUNT];
	uint32_t m_refreshTime;
	uint8_t m_refreshCount;
	SArtifice() {
		memset(this, 0, sizeof(*this));
	}
};

template<uint32_t size>
struct BoolArray {
	bool GetK(uint32_t k) {
		if (k >= size) return false;
		return ((m_data[k >> 5]) >> (k & 0x1f)) & 1;
	}
	void SetK(uint32_t k, bool val) {
		if (k >= size) return;
		if (val)
			(m_data[k >> 5]) |= (1 << (k & 0x1f));
		else
			(m_data[k >> 5]) &= ~(1 << (k & 0x1f));
	}
	uint32_t m_data[(size >> 5) + ((size & 0x1f) ? 1 : 0)];
};

struct SDataVersion {
	uint32_t m_versionID:8;
	uint32_t m_dataSize:24;
};

struct SPlayerData {
	SDataVersion m_Version;		// ���ݽṹ�汾��Ϣ
	bool m_bInited;				// ��ʼ�����
	uint32_t m_uid;				// ��ɫID
	char m_name[MAX_NAME_LEN];	// ��ɫ��
	uint8_t m_level;			// �ȼ�
	uint32_t m_exp;				// ����
	uint32_t m_gold;			// ��ʯ
	uint32_t m_money;			// ���
	uint8_t m_energy;			// ʣ������
	uint8_t m_energyEx;			// ��������ֵ����
	uint32_t m_energyTime;		// �ϴ�����ֵά��ʱ��
	uint32_t m_tongID;			// ����ID
	uint32_t m_tongName;		// ������
	uint32_t m_dayLimitTime;	// �ս���ʱ��
	uint32_t m_weekLimitTime;	// �ܽ���ʱ��
	uint32_t m_lastLoginTime;	// �ϴε�¼ʱ��

	//------------�ؿ�ϵͳ���
	SPassInfo m_passInfo[MAX_LARGE_PASS_COUNT];					// �ؿ���Ϣ
	uint16_t m_passAwardStep;									// �ؿ������콱
	uint16_t m_passDropCounter[MAX_PASS_DORP_COUNTER_COUNT];	// �ؿ��������
	//------------

	//------------����ϵͳ���
	SBlankCard m_blankCard;										//�׿�
	STRandomRole m_tRandomRole[MAX_TRAIN_ROLE_COUNT];			//����ֵ
	uint8_t m_trainBasePT;										//����ѡ�����ӻ�������ֵ
	uint8_t m_trainExPT;										//����ѡ��������������ֵ
	uint8_t m_dailyCanTrainCount;								//ÿ�տ���������
	uint8_t m_dailyTrainedCount;								//ÿ������������
	uint32_t m_trainCD;											//����CD
	uint32_t m_tTrainCDTime;									//�ϴμ������CD��ʱ��
	uint8_t m_freeChance;										//���ˢ�µĸ��Ӹ���
	uint8_t m_payChance;										//����ˢ�µĸ��Ӹ���
	uint16_t m_trainRoleID;										//���������Ľ�ɫ��ID
	uint16_t m_trainRole4ID;									//�������Ƽ����ǽ�ɫ��ID
	uint16_t m_trainRole5ID;									//�������Ƽ����ǽ�ɫ��ID
	uint8_t m_trainValidIdx[MAX_TRAIN_VALID_DIA_COUNT];			//������Ч�Ի�����
	uint8_t m_trainInvalidIdx[MAX_TRAIN_INVALID_DIA_COUNT];		//������Ч�Ի�����
	uint8_t m_trainValidPt[MAX_TRAIN_VALID_DIA_COUNT];			//��Ч�Ի��Ի�����ĵ���
	uint8_t m_trainInvalidPt[MAX_TRAIN_INVALID_DIA_COUNT];		//��Ч�Ի��Ի�����ĵ���
	SAppraiseRecord m_appraiseRecord[MAX_APPRAISE_RECORD_COUNT];//��������ͬƷ�ʳɿ��ļ�¼
	uint16_t m_trainRoleCount[MAX_TRAIN_RECORD_COUNT];			//���������ÿ���Ǽ�������
	//------------

	//------------����ϵͳ
	SConstruct m_construct[MAX_CONSTRUCT_COUNT];									//����
	//------------

	//------------����
	SRoleCardBag m_pRoleCardBag[MAX_BAG_COUNT];			//��ɫ������
	SEquipCardBag m_pEquipCardBag[MAX_BAG_COUNT];		//װ��������
	SSkillCardBag m_pSkillCardBag[MAX_BAG_COUNT];		//���ܿ�����
	SItemBag m_itemBag[MAX_BAG_COUNT];					//��������
	uint8_t m_RCBagCount;								//��ɫ������ʵ��ռ������
	uint8_t m_ECBagCount;								//װ��������ʵ��ռ������
	uint8_t m_SCBagCount;								//���ܿ�����ʵ��ռ������
	uint8_t m_ItemBagCount;								//���߱�����ʵ��ռ������
	uint8_t m_RCBagSizeExt;								//��ɫ������������չ
	uint8_t m_ECBagSizeExt;								//װ��������������չ
	uint8_t m_SCBagSizeExt;								//���ܿ�����������չ
	uint8_t m_ItemBagSizeExt;							//���߱���������չ
	uint8_t m_itemDailyLimit[MAX_ITEM_DAILY_LIMIT];		//����ʹ������
	//------------

	//------------��Ƭ
	SFragment m_roleFragment[MAX_ROLE_FRAGMENT_COUNT];
	SFragment m_equipFragment[MAX_EQUIP_FRAGMENT_COUNT];
	SFragment m_skillFragment[MAX_SKILL_FRAGMENT_COUNT];
	SFragment m_itemFragment[MAX_ITEM_FRAGMENT_COUNT];
	//------------

	//------------ͼ��ϵͳ
	uint16_t m_roleBook[MAX_ROLE_BOOK_COUNT];
	uint16_t m_equipBook[MAX_EQUIP_BOOK_COUNT];
	uint16_t m_skillBook[MAX_SKILL_BOOK_COUNT];
	//------------

	//------------ս��
	uint8_t m_fighterCount;								//�ܹ�װ���Ľ�ɫ������
	uint8_t m_fighterList[MAX_FIGHT_ROLE_COUNT];		//ս����ɫ������:��Ӧ����ɫ����������
	SRoleEquipList m_equipList[MAX_FIGHT_ROLE_COUNT];	//��ɫ����Ӧװ��������:��Ӧ��װ������������
	SRoleSkillList m_skillList[MAX_FIGHT_ROLE_COUNT];	//��ɫ����Ӧ���ܿ�����:��Ӧ�����ܱ���������
	SPVPInfo m_pvpInfo;
	SFightInfo m_fightInfo;
	//------------

	//------------VIPϵͳ
	float m_recharge;							//��ֵ���
	uint8_t m_vipLevel;							//��ǰvip�ȼ�
	BoolArray<MAX_VIP_COUNT> m_vipReward;		//vip��Ӧ�ȼ��Ƿ��콱
	uint8_t m_vipExtraPassCount;				//����ÿ������ս������
	uint8_t m_vipExtraPVPCount;					//����ÿ������սpvp��
	//------------

	//------------�̳�ϵͳ
	uint8_t m_sStoreIndex;													//��ǰ�����̵����ñ��
	uint8_t m_sStoreNextIndex;												//�¼������̵����ñ��
	SSStoreGoods m_sStoreGoods[MAX_SECRET_STORE_COUNT];						//�����̵������Ʒ��Ϣ
	SSStoreGoods m_sStoreNextGoods[MAX_SECRET_STORE_COUNT];					//�����̵������Ʒ��Ϣ,��һ��vip�ȼ�
	uint32_t m_sRefreshTime;												//�����̵��ϴ�ˢ��ʱ��
	uint8_t m_sStoreNextLevel;												//������һ����Ʒ��Ҫ����ҵȼ�
	uint8_t m_sStoreNextVip;												//������һ����Ʒ��Ҫ��vip�ȼ�
	//------------

	//------------�鿨
	uint8_t m_keepLoginDay;												//������½���� 
	uint8_t	m_loginReward;													//�����콱��¼��0-δ�죬1-�ѷ��ͣ�2-����
	uint16_t m_sample5RoleCard;												//��ȡ�õ�5�ǽ�ɫ��������
	uint16_t m_sample3EquipCard;											//��ȡ�õ�3��װ����������
	uint16_t m_sample4EquipCard;											//��ȡ�õ�4��װ����������
	uint16_t m_sample5EquipCard;											//��ȡ�õ�5��װ����������
	//------------

	//------------�˲��г�
	SMarket m_market;
	//------------

	SArtifice m_artifice;									// ����

	uint8_t __dummy1__[7];

	//------------�ﵽ�����콱�
	BoolArray<MAX_ACTIVITYEX_COUNT> m_actMark;				// ��콱���
	uint32_t m_lastMarkActTime;								// �ϴα�ǻ�콱��ʱ��
	//------------

	//------------�����
	uint8_t m_actLVRecord[MAX_ACT_LV_COUNT];				//����ȼ���vip���콱��¼
	uint8_t m_actEnegryFlag;								//���ȡ������¼
	uint32_t m_actLastEnegryGetTime;
	uint8_t m_studyCount;									//ҡһҡ����
	uint8_t m_actEGRefreshCount;							//Ԫ���һ��������Ѿ�ˢ�´���
	uint8_t m_actEMRefreshCount;							//��Ҷһ��������Ѿ�ˢ�´���
	uint32_t m_actEGRefreshCD;								//ˢ��cd
	uint32_t m_actEMRefreshCD;								//
	uint32_t m_actEGRefreshTime;							//ˢ��cd��ʱ
	uint32_t m_actEMRefreshTime;							//
	uint8_t m_actEGCountIndex;								//�һ���������
	uint8_t m_actEGRateIndex;								//�һ���������
	uint8_t m_actEMCountIndex;								//
	uint8_t m_actEMRateIndex;								//
	uint8_t m_actEGExchange;								//�����Ѿ��һ�Ԫ������
	uint8_t m_actEMExchange;								//�����Ѿ��һ���Ҵ���

	SMaterialBM m_materialBM;								//��������

	uint32_t m_actRechargeGift;								//��ֵ��שʱ��
	uint8_t m_actRechargeFlag[MAX_ACT_RECHARGEGIFT_COUNT];	//������ȡ��ʾ
	uint32_t m_actRechargeCount;							//������ȡ��ʾ
	//------------

	uint32_t m_registerTime;								//ע��ʱ��
	//------------
	uint8_t m_actLoginGiftDay;									//��½������
	uint8_t m_actLoginGiftFlag;									//��½�ͱ�ʾ

	STreasure m_treasure;									//У԰Ѱ��

	int8_t __unused__[10225];

	SPlayerData () {
		memset(this, 0, sizeof(*this));
		m_Version.m_versionID = 1;
		m_Version.m_dataSize = sizeof(*this);
	}
};

static_assert(sizeof(SPlayerData) == 0x8000, "Error: the size of struct 'SPlayerData' must be 32k");

#pragma pack(pop)

#endif
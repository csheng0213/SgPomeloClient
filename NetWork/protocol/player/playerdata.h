#ifndef __PLAYERDATA_H__
#define __PLAYERDATA_H__

#define MAX_NAME_LEN 31							//
#define MAX_PROPERTY_COUNT 5					//
#define MAX_BAG_COUNT 250						//最大包裹数
#define MAX_APPRAISE_RECORD_COUNT 5				//最大鉴定记录数
#define MAX_LARGE_PASS_COUNT 100				//最大大关卡数
#define MAX_SMALL_PASS_COUNT 10					//最大小关卡数
#define MAX_FIGHT_ROLE_COUNT 6					//最大可战斗人数
#define MAX_ROLE_EQUIP_COUNT 4					//角色卡最大可装备装备数
#define MAX_ROLE_SKILL_COUNT 2					//角色卡最大可装备技能数
#define MAX_CONSTRUCT_COUNT 20					//玩家拥有建筑数
#define MAX_STOCK_COUNT 5						//建筑最大库存数
#define MAX_VIP_COUNT 20						//最大vip等级
#define MAX_SECRET_STORE_COUNT 50				//最大神秘商店物品数
#define MAX_TRAIN_ROLE_COUNT 3					//随机培养卡片数量
#define MAX_TRAIN_VALID_DIA_COUNT 20			//培养有效对话索引
#define MAX_TRAIN_INVALID_DIA_COUNT 10			//培养无效对话索引
#define MAX_FRIEND_COUNT 100					//最大朋友数
#define MAX_FRIEND_REQUEST_COUNT 50				//最大好友请求数
#define MAX_MARKET_ITEM_COUNT 20				//最大市场出售项目数
#define MAX_ROLE_BOOK_COUNT 500					//最大角色卡图鉴数
#define MAX_EQUIP_BOOK_COUNT 500				//最大角色卡图鉴数
#define MAX_SKILL_BOOK_COUNT 500				//最大角色卡图鉴数
#define MAX_ROLE_FRAGMENT_COUNT 250				//最大角色卡碎片数
#define MAX_EQUIP_FRAGMENT_COUNT 150			//最大装备卡碎片数
#define MAX_SKILL_FRAGMENT_COUNT 150			//最大技能卡碎片数
#define MAX_ITEM_FRAGMENT_COUNT 250				//最大道具碎片数
#define MAX_ARTIFICE_QUALITY_COUNT 4			//最大炼化品质数
#define MAX_ITEM_DAILY_LIMIT 100				//需要限制每日使用次数的道具的个数
#define MAX_ACTIVITYEX_COUNT 32					//最大活动标记数量(指定时间段达到条件领奖类活动)
#define MAX_ACT_LV_COUNT 10						//活动到等级送vip的领奖记录
#define MAX_PASS_DORP_COUNTER_COUNT 3			//最大关卡掉落计数器个数
#define MAX_TRAIN_RECORD_COUNT 4				//最大培养记录计数器个数
#define MAX_ACT_MATERIALBM_COUNT 5				//最大材料商人活动子项数
#define MAX_ACT_RECHARGEGIFT_COUNT 7			//奖学金活动最大天数
#define MAX_TREASURE_COUNT	8					//最多匹配寻宝玩家数量

enum FightType {
	FIGHTTYPE_NONE,
	FIGHTTYPE_PVE,
	FIGHTTYPE_PVP,
	FIGHTTYPE_ST,
};
#define MAX_FIGHT_ROUND 3		// 最大战斗回合数

#pragma pack(push)
#pragma pack(1)

//白卡
struct SBlankCard {
	uint8_t m_step;		//处于哪步骤：0-可以开始培养1-选择角色2-选择培养方式3-培养中4-鉴定
	uint8_t m_trainedTimes;									//已培养次数
	uint8_t m_singleTrainedTimes[MAX_PROPERTY_COUNT];		//单项属性已培养次数
	float m_attr[MAX_PROPERTY_COUNT];						//属性值
	uint8_t m_freeTimes;									//免费已刷新次数
	uint32_t m_freeCD;										//免费刷新CD
	uint8_t m_payTimes;										//付费刷新次数
	uint8_t m_occupation;									//白卡随机职业索引
	uint32_t m_freeCDTime;									//上次检测免费CD的时间
	uint8_t m_lastTrainIdx;									//上次培养选择的课程索引
	SBlankCard () {
		memset(this, 0, sizeof(*this));
	}
};
//成卡(角色卡)
struct SRoleCard {
	uint16_t m_id;											//对于道具表ID，0为无效
	uint16_t m_attr[MAX_PROPERTY_COUNT];					//属性值
	uint16_t m_attack;										//攻击力
	uint16_t m_defense;										//防御力
	uint16_t m_evolution;									//进化等级
	uint16_t m_level;										//等级
	uint32_t m_exp;											//经验
	uint8_t m_skillID;					// 技能ID
	uint8_t m_skillLevel;				// 技能等级
	uint16_t m_skillExp;				// 技能经验
	void Reset() {
		memset(this, 0, sizeof(*this));
	}
	SRoleCard () {
		memset(this, 0, sizeof(*this));
	}
};
//装备卡
struct SEquipCard {
	uint16_t m_id;											//ID
	uint8_t m_level;										//等级
	SEquipCard () {
		memset(this, 0, sizeof(*this));
	}
};
//技能卡
struct SSkillCard {
	uint16_t m_id;											//对于道具表ID，0为无效
	uint8_t m_level;										//等级
	SSkillCard () {
		memset(this, 0, sizeof(*this));
	}
};
//道具
struct SItem {
	uint16_t m_id;											//对于道具表ID，0为无效
	SItem () {
		memset(this, 0, sizeof(*this));
	}
};
//角色卡背包
struct SRoleCardBag {
	SRoleCard m_roleCard;									//卡片
	uint8_t m_useFlag;										//使用标示：0-未使用 1~6-对应m_fighterList中的索引
	uint8_t m_seeFlag;										//监工标示：0-未使用 2~3-对应m_construct的索引
	void Reset() {
		memset(this, 0, sizeof(*this));
	}
	SRoleCardBag () {
		memset(this, 0, sizeof(*this));
	}
};
//装备卡背包
struct SEquipCardBag {
	SEquipCard m_equipCard;									//卡片
	uint8_t m_useFlag;										//使用标示：0-未使用 1~6-对应m_equipList中的索引
	SEquipCardBag () {
		memset(this, 0, sizeof(*this));
	}
};
//技能卡背包
struct SSkillCardBag {
	SSkillCard m_skillCard;									//卡片
	uint8_t m_useFlag;										//使用标示：0-未使用 1~6-对应m_skillList中的索引
	SSkillCardBag () {
		memset(this, 0, sizeof(*this));
	}
};
//道具背包
struct SItemBag {
	SItem m_item;											//道具
	uint16_t m_count;										//道具叠加数
	SItemBag () {
		memset(this, 0, sizeof(*this));
	}
};

//整个鉴定记录
struct SAppraiseRecord {
	uint8_t m_first;										//是否首次
	uint16_t m_count[4];									//1-4星级对应的比产系数
	SAppraiseRecord () {
		memset(this, 0, sizeof(*this));
	}
};

//小关卡动态信息
struct SPassInfo {
	uint8_t m_passStar[MAX_SMALL_PASS_COUNT];				//小关卡得星(0表示未通过)
	uint16_t m_passDailyLimit[MAX_SMALL_PASS_COUNT];		//关卡每日限制
	SPassInfo () {
		memset(this, 0, sizeof(*this));
	}
};

//战斗列表中角色的装备卡列表
struct SRoleEquipList {
	uint8_t m_equip[MAX_ROLE_EQUIP_COUNT];					//武器//防具//饰品//宝物
	SRoleEquipList () {
		memset(this, 0, sizeof(*this));
	}
};

//战斗列表中角色的技能卡列表
struct SRoleSkillList {
	uint8_t m_skill[MAX_ROLE_SKILL_COUNT];					//
	SRoleSkillList () {
		memset(this, 0, sizeof(*this));
	}
};

//建筑资源
struct SConstructRes {
	uint32_t m_id;											//等级
	uint32_t m_amount;										//数量
	SConstructRes () {
		memset(this, 0, sizeof(*this));
	}
};
//建筑
struct SConstruct {
	uint8_t m_level;										//等级
	uint32_t m_levelUpTime;									//升级完成时间
	SConstructRes m_stock[MAX_STOCK_COUNT];					//库存背包
	uint8_t m_stockCount;									//可用库存数，动态增量
	uint32_t m_res;											//资源数
	uint32_t m_time;										//上次产出检测时间
	uint32_t m_CD;											//产出CD
	bool m_canOutput;										//当前是否可用产出
	bool m_canLevelup;										//对于产出的卡是否可以升级
	bool m_canTrain;										//角色卡是否可用培养（角色建筑有效）
	bool m_canRes;											//可否产资源（角色建筑无效）
	uint8_t m_crit;											//可暴击数
	uint8_t m_select;										//培养选项数
	uint8_t m_overseeSlot;									//监工学生，背包位置slot
	uint8_t m_request;										//需求监工属性类型
	uint32_t m_requestCD;									//需求监工变更cd
	uint32_t m_requestTime;									//上次需求变更时间
	SConstruct () {
		memset(this, 0, sizeof(*this));
	}
};

//神秘商店物品信息
struct SSStoreGoods {
	uint16_t index;											//对于配置物品索引
	uint16_t count;											//已经购买次数
	SSStoreGoods () {
		memset(this, 0, sizeof(*this));
	}
};

//神秘商店物品信息
struct SSStoreNextGoods {
	uint16_t index;											//对于配置物品索引
	SSStoreNextGoods () {
		memset(this, 0, sizeof(*this));
	}
};

//培养系统-随机角色
struct STRandomRole {
	float m_attr[MAX_PROPERTY_COUNT];					//属性值
	uint8_t m_occupation;									//职业索引
	STRandomRole () {
		memset(this, 0, sizeof(*this));
	}
};

// PVP信息
struct SPVPInfo {
	uint8_t m_leftTimes;			// 剩余次数
	uint8_t m_totalTimes;			// 今日总胜利次数
	uint8_t m_unbrokenTimes;		// 今日连胜次数
	uint8_t m_maxUnbrokenTimes;		// 最大连胜次数
	uint32_t m_refreshTime;			// 上次刷新时间
	uint32_t m_challengeTime;		// 上次挑战时间
	SPVPInfo() {
		memset(this, 0, sizeof(*this));
	}
};

// 战斗信息
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

//人才市场单项招聘信息
struct SMarketItem {
	uint8_t m_valid;										//是否使用
	uint8_t m_regionIdx;									//地区索引								
	uint8_t m_industryIdx;									//行业索引
	uint8_t m_companyIdx;									//公司索引
	uint8_t m_quality;										//招聘的卡片星级
	uint8_t m_priceIdx;										//星级对应价格索引，记录这个主要是为了支持配置
	uint32_t m_price;										//随机出的价格
	uint8_t m_sellCount;									//已招聘次数
	SMarketItem () {
		memset(this, 0, sizeof(*this));
	};
};
//人才市场信息
struct SMarket {
	uint8_t m_sellCount;									//当天已出售物品数
	uint32_t m_freeRefreshTime;								//上次免费刷新时间
	uint32_t m_freeRefreshCD;								//免费刷新cd
	uint8_t m_freeRefreshCount;								//当天免费已刷新次数
	SMarketItem m_item[MAX_MARKET_ITEM_COUNT];				//单项招聘项目信息
	uint8_t m_sellFlag;										//当次招聘会是否出售
	uint8_t m_refresh;										//系统刷新索引
	SMarket () {
		memset(this, 0, sizeof(*this));
	}
};

//材料商人单项信息
struct SMaterialBMItem {
	uint8_t m_valid;										//是否使用
	uint32_t m_quality;										//服务器还剩出售数量
	uint32_t m_equipIdx;									//对应星级下具体的装备索引
	uint32_t m_sellCount;									//已出售次数
	SMaterialBMItem () {
		memset(this, 0, sizeof(*this));
	};
};
//材料商人
struct SMaterialBM {
	uint8_t m_sellCount;									//当天已出售物品数
	SMaterialBMItem m_item[MAX_ACT_MATERIALBM_COUNT];		//单项项目信息
	uint8_t m_sellFlag;										//当次是否出售
	uint8_t m_refresh;										//系统刷新索引
	SMaterialBM () {
		memset(this, 0, sizeof(*this));
	}
};

//校园寻宝
struct STreasure {
	uint32_t m_pids[MAX_TREASURE_COUNT];					//本次寻宝匹配玩家id
	uint32_t m_searchTime;									//寻宝剩余时间
	uint32_t m_robRes;										//打劫成功获得的资源总和
	uint8_t m_searchCount;									//当天参与次数
	uint8_t m_robCount;										//当天打劫次数
	uint8_t m_reward;										//是否可以领奖
	uint8_t m_type;											//类型
	STreasure () {
		memset(this, 0, sizeof(*this));
	}
};

//碎片
struct SFragment {
	uint16_t m_id;								//碎片对应物品id
	uint16_t m_count;							//碎片数
	SFragment () {
		memset(this, 0, sizeof(*this));
	}
};

// 炼化
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
	SDataVersion m_Version;		// 数据结构版本信息
	bool m_bInited;				// 初始化标记
	uint32_t m_uid;				// 角色ID
	char m_name[MAX_NAME_LEN];	// 角色名
	uint8_t m_level;			// 等级
	uint32_t m_exp;				// 经验
	uint32_t m_gold;			// 钻石
	uint32_t m_money;			// 金币
	uint8_t m_energy;			// 剩余体力
	uint8_t m_energyEx;			// 额外体力值上限
	uint32_t m_energyTime;		// 上次体力值维护时间
	uint32_t m_tongID;			// 帮派ID
	uint32_t m_tongName;		// 帮派名
	uint32_t m_dayLimitTime;	// 日结算时间
	uint32_t m_weekLimitTime;	// 周结算时间
	uint32_t m_lastLoginTime;	// 上次登录时间

	//------------关卡系统相关
	SPassInfo m_passInfo[MAX_LARGE_PASS_COUNT];					// 关卡信息
	uint16_t m_passAwardStep;									// 关卡星星领奖
	uint16_t m_passDropCounter[MAX_PASS_DORP_COUNTER_COUNT];	// 关卡掉落计数
	//------------

	//------------养成系统相关
	SBlankCard m_blankCard;										//白卡
	STRandomRole m_tRandomRole[MAX_TRAIN_ROLE_COUNT];			//属性值
	uint8_t m_trainBasePT;										//当次选择增加基础属性值
	uint8_t m_trainExPT;										//当次选择增加奇遇属性值
	uint8_t m_dailyCanTrainCount;								//每日可培养次数
	uint8_t m_dailyTrainedCount;								//每日已培养次数
	uint32_t m_trainCD;											//培养CD
	uint32_t m_tTrainCDTime;									//上次检测培养CD的时间
	uint8_t m_freeChance;										//免费刷新的附加概率
	uint8_t m_payChance;										//付费刷新的附加概率
	uint16_t m_trainRoleID;										//培养出来的角色卡ID
	uint16_t m_trainRole4ID;									//培养后推荐四星角色卡ID
	uint16_t m_trainRole5ID;									//培养后推荐五星角色卡ID
	uint8_t m_trainValidIdx[MAX_TRAIN_VALID_DIA_COUNT];			//培养有效对话索引
	uint8_t m_trainInvalidIdx[MAX_TRAIN_INVALID_DIA_COUNT];		//培养无效对话索引
	uint8_t m_trainValidPt[MAX_TRAIN_VALID_DIA_COUNT];			//有效对话对话分配的点数
	uint8_t m_trainInvalidPt[MAX_TRAIN_INVALID_DIA_COUNT];		//无效对话对话分配的点数
	SAppraiseRecord m_appraiseRecord[MAX_APPRAISE_RECORD_COUNT];//鉴定出不同品质成卡的记录
	uint16_t m_trainRoleCount[MAX_TRAIN_RECORD_COUNT];			//玩家培养的每个星级的数量
	//------------

	//------------建筑系统
	SConstruct m_construct[MAX_CONSTRUCT_COUNT];									//建筑
	//------------

	//------------背包
	SRoleCardBag m_pRoleCardBag[MAX_BAG_COUNT];			//角色卡数组
	SEquipCardBag m_pEquipCardBag[MAX_BAG_COUNT];		//装备卡数组
	SSkillCardBag m_pSkillCardBag[MAX_BAG_COUNT];		//技能卡数组
	SItemBag m_itemBag[MAX_BAG_COUNT];					//道具数组
	uint8_t m_RCBagCount;								//角色卡背包实际占用数量
	uint8_t m_ECBagCount;								//装备卡背包实际占用数量
	uint8_t m_SCBagCount;								//技能卡背包实际占用数量
	uint8_t m_ItemBagCount;								//道具背包容实际占用数量
	uint8_t m_RCBagSizeExt;								//角色卡背包容量扩展
	uint8_t m_ECBagSizeExt;								//装备卡背包容量扩展
	uint8_t m_SCBagSizeExt;								//技能卡背包容量扩展
	uint8_t m_ItemBagSizeExt;							//道具背包容量扩展
	uint8_t m_itemDailyLimit[MAX_ITEM_DAILY_LIMIT];		//道具使用限制
	//------------

	//------------碎片
	SFragment m_roleFragment[MAX_ROLE_FRAGMENT_COUNT];
	SFragment m_equipFragment[MAX_EQUIP_FRAGMENT_COUNT];
	SFragment m_skillFragment[MAX_SKILL_FRAGMENT_COUNT];
	SFragment m_itemFragment[MAX_ITEM_FRAGMENT_COUNT];
	//------------

	//------------图鉴系统
	uint16_t m_roleBook[MAX_ROLE_BOOK_COUNT];
	uint16_t m_equipBook[MAX_EQUIP_BOOK_COUNT];
	uint16_t m_skillBook[MAX_SKILL_BOOK_COUNT];
	//------------

	//------------战斗
	uint8_t m_fighterCount;								//能够装备的角色卡数量
	uint8_t m_fighterList[MAX_FIGHT_ROLE_COUNT];		//战斗角色卡队列:对应【角色背包】索引
	SRoleEquipList m_equipList[MAX_FIGHT_ROLE_COUNT];	//角色卡对应装备卡队列:对应【装备背包】索引
	SRoleSkillList m_skillList[MAX_FIGHT_ROLE_COUNT];	//角色卡对应技能卡队列:对应【技能背包】索引
	SPVPInfo m_pvpInfo;
	SFightInfo m_fightInfo;
	//------------

	//------------VIP系统
	float m_recharge;							//充值金额
	uint8_t m_vipLevel;							//当前vip等级
	BoolArray<MAX_VIP_COUNT> m_vipReward;		//vip对应等级是否领奖
	uint8_t m_vipExtraPassCount;				//额外每日已挑战副本数
	uint8_t m_vipExtraPVPCount;					//额外每日已挑战pvp数
	//------------

	//------------商城系统
	uint8_t m_sStoreIndex;													//当前神秘商店配置编号
	uint8_t m_sStoreNextIndex;												//下级神秘商店配置编号
	SSStoreGoods m_sStoreGoods[MAX_SECRET_STORE_COUNT];						//神秘商店具体物品信息
	SSStoreGoods m_sStoreNextGoods[MAX_SECRET_STORE_COUNT];					//神秘商店具体物品信息,下一个vip等级
	uint32_t m_sRefreshTime;												//神秘商店上次刷新时间
	uint8_t m_sStoreNextLevel;												//购买下一级物品需要的玩家等级
	uint8_t m_sStoreNextVip;												//购买下一级物品需要的vip等级
	//------------

	//------------抽卡
	uint8_t m_keepLoginDay;												//连续登陆天数 
	uint8_t	m_loginReward;													//当天领奖记录：0-未领，1-已发送，2-已领
	uint16_t m_sample5RoleCard;												//抽取得到5星角色卡的数量
	uint16_t m_sample3EquipCard;											//抽取得到3星装备卡的数量
	uint16_t m_sample4EquipCard;											//抽取得到4星装备卡的数量
	uint16_t m_sample5EquipCard;											//抽取得到5星装备卡的数量
	//------------

	//------------人才市场
	SMarket m_market;
	//------------

	SArtifice m_artifice;									// 炼化

	uint8_t __dummy1__[7];

	//------------达到条件领奖活动
	BoolArray<MAX_ACTIVITYEX_COUNT> m_actMark;				// 活动领奖标记
	uint32_t m_lastMarkActTime;								// 上次标记活动领奖的时间
	//------------

	//------------活动数据
	uint8_t m_actLVRecord[MAX_ACT_LV_COUNT];				//活动到等级送vip的领奖记录
	uint8_t m_actEnegryFlag;								//活动领取体力记录
	uint32_t m_actLastEnegryGetTime;
	uint8_t m_studyCount;									//摇一摇次数
	uint8_t m_actEGRefreshCount;							//元宝兑换，当日已经刷新次数
	uint8_t m_actEMRefreshCount;							//金币兑换，当日已经刷新次数
	uint32_t m_actEGRefreshCD;								//刷新cd
	uint32_t m_actEMRefreshCD;								//
	uint32_t m_actEGRefreshTime;							//刷新cd计时
	uint32_t m_actEMRefreshTime;							//
	uint8_t m_actEGCountIndex;								//兑换数量索引
	uint8_t m_actEGRateIndex;								//兑换汇率索引
	uint8_t m_actEMCountIndex;								//
	uint8_t m_actEMRateIndex;								//
	uint8_t m_actEGExchange;								//当日已经兑换元宝次数
	uint8_t m_actEMExchange;								//当日已经兑换金币次数

	SMaterialBM m_materialBM;								//材料商人

	uint32_t m_actRechargeGift;								//充值送砖时间
	uint8_t m_actRechargeFlag[MAX_ACT_RECHARGEGIFT_COUNT];	//当日领取标示
	uint32_t m_actRechargeCount;							//当日领取标示
	//------------

	uint32_t m_registerTime;								//注册时间
	//------------
	uint8_t m_actLoginGiftDay;									//登陆送天数
	uint8_t m_actLoginGiftFlag;									//登陆送标示

	STreasure m_treasure;									//校园寻宝

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
#ifndef _AUTO_FIGHT_SYS_H_
#define _AUTO_FIGHT_SYS_H_

#include <map>
#include <vector>
#include <utility>

enum AutoFightCondType
{
    AFCT_NONE,
    AFCT_PID_RANGE,
    AFCT_FRAME_LEAST,
    AFCT_FRAME_MOST,
    AFCT_HP_LEAST,
    AFCT_HP_MOST,
    AFCT_HP_MAX_LEAST,
    AFCT_HP_MAX_MOST,
    AFCT_MP_LEAST,
    AFCT_MP_MOST,
    AFCT_MP_MAX_LEAST,
    AFCT_MP_MAX_MOST,
    AFCT_ATK_LEAST,
    AFCT_ATK_MOST,
    AFCT_ATK_RANGE_LEAST,
    AFCT_ATK_RANGE_MOST,
    AFCT_ATK_SPD_LEAST,
    AFCT_ATK_SPD_MOST,
    AFCT_MV_SPD_LEAST,
    AFCT_MV_SPD_MOST
};

enum AutoFightCondLogic
{
    AFCL_ALL,
    AFCL_ANY
};

enum AutoFightHero
{
	AFH_NONE,
	AFH_MAIN_HERO,
	AFH_VICE_HERO1,
	AFH_VICE_HERO2,
};

enum AutoFightCheckHero
{
	AFCH_SELF			= 0,
	AFCH_MAIN_HERO		= 1,
	AFCH_VICE_HERO1		= 2,
	AFCH_VICE_HERO2		= 4,
};

struct AutoFightCond
{
    AutoFightCond()
        : ctype{AFCT_NONE},
          hero{AFCH_SELF},
          frdly{false},
          frame{0},
          val{0.0f},
          val_pct{0.0f}
    {
    }

    AutoFightCondType   ctype;
    AutoFightCheckHero  hero;
    
    int                 frame;
    bool                frdly;

    union {
        float           val;
        int             pid_low;
    };                  
                        
    union {             
        float           val_pct;
        int             pid_high;
    };
};

struct AutoFightCondSet
{
    using CondVec = std::vector<AutoFightCond>;

    AutoFightCondSet()
        : logic{AFCL_ALL},
          triggered{false}
    {
    }

    AutoFightCondLogic  logic;
    CondVec             cond_vec;
    bool                triggered;
};

class Creature;

class AutoFightSys
{
    using CondSetMultiMap = std::multimap<int, AutoFightCondSet>;

    AutoFightSys();

public:
	static AutoFightSys & getInstance()
	{
		static AutoFightSys s;
		return s;
	}

    void update(bool frdly);

    void clear();

    void resetLock();

    void addAutoSpellCond(bool frdly, int prior, AutoFightHero hero, AutoFightCondSet condset);

private:
    bool heroConditionCheck(Creature *c, CondSetMultiMap &condmap);

private:
    CondSetMultiMap _emhero;    // enemy main hero
    CondSetMultiMap _evhero1;
    CondSetMultiMap _evhero2;

    CondSetMultiMap _smhero;    // self main hero
    CondSetMultiMap _svhero1;
    CondSetMultiMap _svhero2;
};

#define sAutoFightSys (AutoFightSys::getInstance())

#endif // !_AUTO_FIGHT_SYS_H_
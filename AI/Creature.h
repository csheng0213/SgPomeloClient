#ifndef _CREATURE_H_
#define _CREATURE_H_

#include <list>
#include <set>
#include <vector>

#include "Vector2D.h"
#include "StateMachine.h"
#include "Message.h"
#include "MessageDispatcher.h"
#include "Spell.h"

enum CreatureType
{
    CT_BEGIN = 0,

    CT_MAIN_HERO,
    CT_VICE_HERO,
    CT_INFANTRY,
    CT_CAVALRY,
    CT_ARCHER,

    CT_END
};

enum CreatureNation
{
    CN_BEGIN = 0,

    CN_SHU,
    CN_WU,
    CN_WEI,
    CN_MASS,

    CN_END
};

typedef std::vector<std::pair<int, std::set<Creature *>>>   SpellTargetVec;

class Creature
{
    typedef std::list<SpellEffectStateHolder> EffectStateList;
    typedef std::vector<Spell *>              PassiveSpellVec;

public:
    Creature(CreatureType type);

    virtual ~Creature();

	const int getId() const { return _id; }

public:
    bool isAlive()    const { return !isGlobalState(S_Dying); }
         
    bool isHero()     const { return isMainHero() || isViceHero(); }
         
    bool isMainHero() const { return ctype == CT_MAIN_HERO; }
         
    bool isViceHero() const { return ctype == CT_VICE_HERO; }
         
    bool isSoldier()  const { return !isHero(); }
         
    bool isType(CreatureType type)       const { return ctype == type; }

    bool isNation(CreatureNation nation) const { return cnation == nation; }
         
    bool isState(State *state)       const { return fsm->isState(state); }
         
    bool isGlobalState(State *state) const { return fsm->isGlobalState(state); }
         
public:
    State * getState()               const { return fsm->getState(); }

    State * getGlobalState()         const { return fsm->getGlobalState(); }

    void changeState(State *state)         { fsm->changeState(state); }
         
    void changeGlobalState(State *state)   { fsm->changeGlobalState(state); }

    void handleMessage(Message *msg) const { fsm->handleMessage(msg); }

public:
    SpellEffectStateHolder & addEffectState(const SpellEffectStateHolder &esh);

    void    removeEffectState(SpellEffectState state);

    bool    hasEffectState(SpellEffectState state);

public:
    void    setSpellTarget(SpellTargetVec &&st);

    void    removeSpellTarget();

    Spell * getSpell();
    
    void    setSpell(Spell *spl);

    bool    isSpellReady() const;

    void    addPassiveSpell(Spell *spl);

    PassiveSpellVec & getPassiveSpells();

public:
    static float hurtHandler(Creature *src, Creature *tar, Message *msg = NULL, SpellType spl_t = ST_PT_DAMAGE, float val_p = 0.0f, float val = 0.0f);

public:
    float onAttack (Creature *tar, int isBaoji);

    float onDefence(Creature *tar, float damage);
    
    int  isBaoji();
    
    void addHp(float addValue);

public:
    virtual void onAttackStart(int level) {}

    virtual void onHurt(Creature *sender, float val, int level) {}

    virtual void onSpellHpChanged(float val, int level) {}

    virtual void onDead() {}

    virtual void onSpellTargetChanged() {}

	virtual void onEffectStart(int id, bool continuou, int msgId) {}

    virtual void onEffectEnd(int id, int msgId) {}
    
public:
    // id start with 1
	static void resetIdHolder()
	{		
        _id_holder = 0;
	}

public:
    int             pid;
    int             num;

	Vector2D		pos;            // position
	Vector2D	    dire;           // dire
    
    // visible properties
    float           round;

    float           wuli;
    float           zhili;
    float           tongyu;
    float           hp;
    float           hp_max;
    float           mp;
    float           mp_max;
    float           atk;
    float           fas;
    float           faf;
    float           baoj;
    float           baos;
    float           atk_range;      // pixels
    float           atk_spd;        // attack times per second
	float		    mv_spd;         // move pixels per second
	float		    vis_field;      // visual field, pixels

    float           raw_hp;
    float           raw_hp_max;
    float           raw_mp;
    float           raw_mp_max;
    float           raw_atk;
    float           raw_fas;
    float           raw_faf;
    float           raw_baoj;
    float           raw_baos;
    float           raw_atk_range;  // pixels
    float           raw_atk_spd;    // attack times per second
	float		    raw_mv_spd;     // move pixels per second
	float		    raw_vis_field;  // visual field, pixels

    bool            frdly;          // friendly

    Creature       *tar;            // target
    int             tar_buf_cnt;    // target buf
    Vector2D        tar_pos;        // target position, not target's position
    bool            charged;        // tag for whether creature has charged
    
    int             zxId;           //阵型id
    int             enemyZxId;      //克制敌方阵型id

    Vector2D        force;
    int             force_buf_cnt;

    std::list<Vector2D>         recent_pos;

    std::vector<CreatureType>   prior_atk_vec;
    
    EffectStateList             eff_stat;

    SpellTargetVec             *spl_tar;

public:
    CreatureType     ctype;
    CreatureNation   cnation;
	StateMachine    *fsm;

private:
    Spell           *_aspl;
    PassiveSpellVec  _pspl;
    
private:
	int		         _id;
    static int       _id_holder;
    int             _isBaoji;
};

#endif // _CREATURE_H_

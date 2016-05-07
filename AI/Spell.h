#ifndef _SPELL_H_
#define _SPELL_H_

#include <map>
#include <vector>

#include "Vector2D.h"
#include "AIConst.h"

enum SpellRange
{
    SR_BEGIN        = 0,

    SR_CIRCLE,
    SR_SECTOR,
    SR_RECTANGLE,

    SR_ALL,
    SR_ALL_EXCEPT_SELF,
    SR_MAIN_HERO,
    SR_VICE_HERO,
    SR_SOLDIER,

    SR_SELF,
    SR_TARGET,
    SR_CUR_TARGET,

    SR_ATK_HIGHEST,
    SR_HP_HIGHEST,
    SR_FAS_HIGHEST,
    SR_FAF_HIGHEST,
    SR_ATK_SPD_HIGHEST,
    SR_MOV_SPD_HIGHEST,
    SR_ATK_RANGE_HIGHEST,
    SR_WULI_HIGHEST,
    SR_ZHILI_HIGHEST,
    SR_TONGYU_HIGHEST,

    SR_ATK_LOWEST,
	SR_HP_LOWEST,
	SR_FAS_LOWEST,
    SR_FAF_LOWEST,
    SR_ATK_SPD_LOWEST,
    SR_MOV_SPD_LOWEST,
    SR_ATK_RANGE_LOWEST,
    SR_WULI_LOWEST,
    SR_ZHILI_LOWEST,
    SR_TONGYU_LOWEST,

    SR_NEAREST_HERO,
    
    SR_INFANTRY,
    SR_CAVALRY,
    SR_ARCHER,
    
    SR_SHU,
    SR_WU,
    SR_WEI,
    SR_MASS,

    SR_ALL_HERO,

    SR_NEAREST_HERO_CIRCLE,
    SR_NEAREST_HERO_SECTOR,
    SR_NEAREST_HERO_RECTANGLE,
    SR_SELE_SOLDIER,

    SR_END,
};

enum SpellPosition
{
    SP_BEGIN        = 0,

    SP_SELF,
    SP_TARGET,
    SP_POSITION,

    SP_END,
};

struct SpellSelectorPiece
{
    SpellSelectorPiece()
        : range(SR_BEGIN)
        , pos(SP_SELF)
        , frdly(0)   // 0->all, 1->friend, 2->enemy
    {
        data.sector.radius   = 0.0f;
        data.sector.angle    = 0.0f;
        data.sector.back_len = 0.0f;
    }

    SpellRange      range;
    SpellPosition   pos;
    int             frdly;

    struct Circle
    {
        float       radius;
    };
    
    struct Sector
    {
        float       radius;
        float       angle;
        float       back_len;
    };

    struct Rectangle
    {
        float       width;
        float       height;
        float       back_len;
    };

    union
    {
        Circle      circle;
        Sector      sector;
        Rectangle   rect;
    } data;
};

enum SpellEffect
{
    SE_BEGIN        = 0,

    SE_PROP_CHANGE,
    SE_PROP_CHANGE_TIME,
    
    

    SE_EFFECT_STATE_ADD,
    SE_EFFECT_STATE_ADD_TIME,
    
    // state change
    SE_STATE_FREEZING_TIME,

    SE_SPELL_HURT,
    SE_SPELL_CURE,
    
    SE_END,
};

enum SpellEffectState {

    SES_BEGIN       = 0,

    SES_SUCK_BLOOD_BY_ATK_PCT,      // 吸血
    
    SES_STRIKE_INFANTRY,
    SES_STRIKE_CAVALRY,
    SES_STRIKE_ARCHER,
    
    SES_DEFENCE_VAL,
    SES_DEFENCE_VAL_PCT,
    
    SES_STRIKE_ZX, //阵型克制

    SES_END,

};

enum SpellPropertyChoose
{
    SPC_BEGIN   = 0,
    
    SPC_ATK,
    SPC_HP,
    SPC_HP_MAX,
    SPC_FAS,
    SPC_FAF,
    SPC_ATK_SPD,
    SPC_MOV_SPD,
    SPC_ATK_RANGE,
    SPC_HP_CUR, // 当前血量
    SPC_ABSORB_ATK,//吸收攻击力

    SPC_END,
};

struct SpellEffectStateHolder
{
    SpellEffectStateHolder(SpellEffectState state = SES_BEGIN)
        : state(state)
        , val(0.0f)
        , val_pct(0.0f)
    {
    }

    bool operator == (const SpellEffectStateHolder &h)
    {
        return state == h.state;
    }

    bool operator == (const SpellEffectState s)
    {
        return state == s;
    }

    SpellEffectState    state;
    float               val;
    float               val_pct;
};

struct SpellEffectPiece
{
    SpellEffectPiece()
        : effect(SE_BEGIN)
        , spc(SPC_BEGIN)
        , state(SES_BEGIN)

        , val(0.0f)
        , val_pct(0.0f)
        , param1(0.0f)
        , param2(0.0f)
        , param3(0.0f)
        , param4(0.0f)
    {
    }

    SpellEffect         effect;
    SpellPropertyChoose spc;
    SpellEffectState    state;

    float               val;
    float               val_pct;
    float               param1;
    float               param2;
    float               param3;
    float               param4;
};

enum SpellValueCond
{
    SVC_BEGIN        = 0,
    
    SVC_HP_VALUE_GREATER,
    SVC_HP_VALUE_LOWER,

    SVC_END,
};

enum SpellValuePctCond
{
    SVPC_BEGIN        = 0,

    SVPC_HP_PCT_GREATER,
    SVPC_HP_PCT_LOWER,

    SVPC_END,
};

struct SpellConditionPiece
{
    SpellConditionPiece()
        : v_cond(SVC_BEGIN)
        , vp_cond(SVPC_BEGIN)
        , prob(100.0f)
        , val(0.0f)
        , val_pct(0.0f)
    {
    }

    SpellValueCond      v_cond;
    SpellValuePctCond   vp_cond;
    float               prob;
    float               val;
    float               val_pct;
};

enum SpellDelayType
{
    SDT_BEGIN        = 0,
    
    SDT_PT,
    SDT_WULI,
    SDT_ZHILI,
    
    SDT_END,
};

enum SpellType
{
    ST_BEGIN        = 0,
    
    ST_PT_DAMAGE,
    ST_FAS_DAMAGE,
    ST_ATK_DAMAGE,
    ST_WULI_RATIO_DAMAGE,
    ST_ZHILI_RATIO_DAMAGE,
    ST_TONGYU_RATIO_DAMAGE,
    ST_SELF_CUR_HP_PERCENT_DAMAGE,
    ST_SELF_LOST_HP_PERCENT_DAMAGE,
    ST_SELF_FAS_COMPARE_DAMAGE,
    ST_SELF_ATK_COMPARE_DAMAGE,
    ST_ENEMY_ATK_SUCK_BLOOD,
    ST_FAS_ADD_ENEMY_FAS_DAMAGE,
    ST_END
};

struct SpellTimePiece
{
    SpellTimePiece()
        : delay(1)
        , delay_t(SDT_BEGIN)
        , interval(1)
        , total_time(0)
    {
    }

    int         delay;
    SpellDelayType delay_t;
    int         interval;
    int         total_time;
};

struct SpellPiece
{
    SpellPiece(int id)
        : _id(id),
        spType(ST_BEGIN),
        isPtsp(false)
    {}

    int getId() const { return _id; }
    
    SpellSelectorPiece      selector;   //选择(范围、指定单位)
    SpellEffectPiece        effect;     //效果（属性、状态、技能效果状态）
    SpellConditionPiece     condition;  //条件（几率、属性值、属性百分比）
    SpellTimePiece          time;
    SpellType               spType;
    bool                    isPtsp;

private:
    int                     _id;
};

class Creature;

class Spell
{
public:
	Spell(SpellType spell_t = ST_BEGIN, int cool_down = 0, /*float mp_cost = 0.0f,*/ int cast_time = 1);

	~Spell();

    void bindCreature(Creature *c);

    void addSpellPiece(SpellPiece *sp)
    {
        if (sp)
            _sp_vec.push_back(sp);
    }
    
    int getCastTime() const
    {
        return _cast_tm;
    }

    //float getMpCost() const
    //{
    //    return _mp_cost;
    //}

    void resetCoolDown()
    {
        _cool_down = _raw_cool_down;
    }

    void reduceCoolDown()
    {
        if (_cool_down > 0)
            _cool_down--;
    }

    bool isSpellReady() const
    {
        return _cool_down == 0;
    }

    int getReadyFrame() const
    {
        return _cool_down;
    }

    int getSpellCoolDown() const
    {
        return _raw_cool_down;
    }

    void updateValue();

    void castSpell(Creature *tar, Vector2D tar_pos);

private:
    SpellType                   _spell_t;
    //float                       _mp_cost;
    int                         _cool_down;
    const int                   _raw_cool_down;
    int                         _cast_tm;   // cast time

    Creature                   *_castor;
    std::vector<SpellPiece *>   _sp_vec;    // spell piece vector
};

template <int N, bool F = true>
class CoolDownHelper
{
public:
    static int getReadyFrame()
    {
        return _cool_down;
    }

    static bool isSpellReady()
    {
        return _cool_down == 0;
    }

    static void resetCoolDown()
    {
        _cool_down = N;
    }

    static void reduceCoolDown()
    {
        if (_cool_down > 0)
            _cool_down--;
    }

private:
    static int _cool_down;
};

template <int N, bool F>
int CoolDownHelper<N, F>::_cool_down = N;

using FriendCommonCoolDown = CoolDownHelper<CommonSpellCoolDown>;
using EnemyCommonCoolDown  = CoolDownHelper<CommonSpellCoolDown, false>;
using FriendStartBattleCoolDown = CoolDownHelper<FriendStartBattleSpellCoolDown>;
using EnemyStartBattleCoolDown  = CoolDownHelper<EnemyStartBattleSpellCoolDown, false>;

#endif // _SPELL_H_
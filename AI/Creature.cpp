#include <algorithm>

#include "Creature.h"

#include "AIConst.h"
#include "AIFactor.h"

int Creature::_id_holder = 0;

Creature::Creature(CreatureType type)
	: _id(_id_holder++)

    , pid(0)
    , num(1)

    , pos(Vector2D())
    , dire(ForwardDirection)

    , round(20.f)
    , wuli(0.0f)
    , zhili(0.0f)
    , tongyu(0.0f)
    , hp(-1.0f)
    , hp_max(-1.0f)
    , mp(0.0f)
    , mp_max(0.0f)
    , atk(0.0f)
    , fas(0.0f)
    , faf(0.0f)
    , baoj(0.0f)
    , baos(0.0f)
    , atk_range(0.0f)
    , atk_spd(0.0f)
    , mv_spd(0.0f)
    , vis_field(0.0f)
    
    , raw_hp(0.0f)
    , raw_hp_max(0.0f)
    , raw_mp(0.0f)
    , raw_mp_max(0.0f)
    , raw_atk(0.0f)
    , raw_fas(0.0f)
    , raw_faf(0.0f)
    , raw_baoj(0.0f)
    , raw_baos(0.0f)
    , raw_atk_range(0.0f)
    , raw_atk_spd(0.0f)
	, raw_mv_spd(0.0f)
	, raw_vis_field(0.0f)

    , frdly(false)

    , tar(nullptr)
    , tar_buf_cnt(0)
    , tar_pos(Vector2D(0.0f, 0.0f))
    , charged(false)

    , force_buf_cnt(0)

    , _aspl(0)

    , spl_tar(0)

    , ctype(type)
    , cnation(CN_BEGIN)
{
    fsm = new StateMachine(this);

    switch (ctype)
    {
    case CT_INFANTRY:
        prior_atk_vec.push_back(CT_CAVALRY);
        prior_atk_vec.push_back(CT_INFANTRY);
        prior_atk_vec.push_back(CT_ARCHER);
        break;
    default:
        prior_atk_vec.push_back(CT_INFANTRY);
        prior_atk_vec.push_back(CT_CAVALRY);
        prior_atk_vec.push_back(CT_ARCHER);
        break;
    }
}

Creature::~Creature()
{
    delete fsm;
    fsm = 0;
    
    delete _aspl;
    _aspl = 0;

    delete spl_tar;
    spl_tar = 0;

    for (auto &i : _pspl)
    {
        delete i;
        i = nullptr;
    }
}

void Creature::setSpellTarget(SpellTargetVec &&st)
{
    if (spl_tar == nullptr)
        spl_tar = new SpellTargetVec;
    *spl_tar = std::move(st);

    onSpellTargetChanged();
}

void Creature::removeSpellTarget()
{
    delete spl_tar;
    spl_tar = 0;
}

Spell * Creature::getSpell()
{
    return _aspl;
}

void Creature::setSpell(Spell *spl)
{
	if (spl == nullptr)
		return;

    delete _aspl;
    _aspl = spl;
    spl->bindCreature(this);
}

bool Creature::isSpellReady() const
{
    return isAlive() && (frdly ? (FriendCommonCoolDown::isSpellReady() && FriendStartBattleCoolDown::isSpellReady())
                  : (EnemyCommonCoolDown::isSpellReady()  && EnemyStartBattleCoolDown::isSpellReady() )
           ) && _aspl && _aspl->isSpellReady();
}

SpellEffectStateHolder & Creature::addEffectState(const SpellEffectStateHolder &esh)
{
//    auto iter = std::find(eff_stat.begin(), eff_stat.end(), esh);
//    if (iter != eff_stat.end())
//        return *iter;

    eff_stat.push_back(esh);
    return eff_stat.back();
}

void Creature::removeEffectState(SpellEffectState state)
{
    if (eff_stat.empty())
        return;

//    遗留错误，未加状态是否存在判断
    if (hasEffectState(state)) {
         eff_stat.erase(std::find(eff_stat.begin(), eff_stat.end(), state));
    }else
        printf("not found effectstate");
}

bool Creature::hasEffectState(SpellEffectState state)
{
    return std::find(eff_stat.begin(), eff_stat.end(), state) != eff_stat.end();
}

int Creature::isBaoji()
{
    auto value = sAIEngine.randf() * 1.0f;
    if(value >= (1 - baoj))
        return 1;
    else
        return 0;
}

float Creature::hurtHandler(Creature *src, Creature *tar, Message *msg, SpellType spl_t, float val_p, float val)
{
    
    if (val_p == 0 ) {
        val_p = 1;
    }
    
    float damage = 0.0f;
    switch (spl_t)
    {
        case ST_PT_DAMAGE:
            damage = -1*((src->atk + src->fas*(src->fas/(src->fas + tar->faf)))*val_p + val);
            break;
        case ST_FAS_DAMAGE:
        {
            float temp_fas = (src->fas + val)*val_p;
            damage = -1*(temp_fas*(temp_fas/(temp_fas + tar->faf)) + (src->fas + val)*0.52*val_p);
//             float temp_fas = (src->fas + val)*val_p;
//             damage = -1*( src->atk + temp_fas*(temp_fas/(temp_fas + tar->faf)));

        }
        break;
        case ST_ATK_DAMAGE:
        {
            float temp_atk = (src->atk + val)*val_p;
            damage = -1*(temp_atk + src->fas*(src->fas/(src->fas + tar->faf)));
        }
        break;
        case ST_WULI_RATIO_DAMAGE:
        {
            damage = -1*(src->atk + src->fas*(src->fas/(src->fas + tar->faf)))*(src->wuli/tar->wuli + val_p);
        }
        break;
        case ST_ZHILI_RATIO_DAMAGE:
        {
            damage = -1*(src->atk + src->fas*(src->fas/(src->fas + tar->faf)))*(src->zhili/tar->zhili + val_p);
        }
        break;
        case ST_TONGYU_RATIO_DAMAGE:
        {
            damage = -1*(src->atk + src->fas*(src->fas/(src->fas + tar->faf)))*(src->tongyu/tar->tongyu + val_p);
        }
        break;
        case ST_SELF_CUR_HP_PERCENT_DAMAGE:
        {
            damage = -1*(src->hp*val_p+val);
        }
        break;
        case ST_SELF_LOST_HP_PERCENT_DAMAGE:
        {
            damage = -1*((src->raw_hp - src->hp)*val_p+val);
        }
        break;
        case ST_SELF_FAS_COMPARE_DAMAGE:
        {
            if (tar->fas < src->fas*msg->spl_eff.param1) {
                damage = -1*src->fas*msg->spl_eff.param2;
            }
        }
        break;
        case ST_SELF_ATK_COMPARE_DAMAGE:
        {
            if (tar->atk < src->atk*msg->spl_eff.param1) {
                damage = -1*src->atk*msg->spl_eff.param2;
            }
        }
        break;
        case ST_ENEMY_ATK_SUCK_BLOOD:
        {
            damage = -1*((src->atk + src->fas*(src->fas/(src->fas + tar->faf)))*msg->spl_eff.param1 + tar->atk*msg->spl_eff.param2);
        }
        break;
        case ST_FAS_ADD_ENEMY_FAS_DAMAGE:
        {
            float temp_fas = (src->fas + val)*2*msg->spl_eff.param1;
            damage = -1*temp_fas*(temp_fas/(temp_fas + tar->faf));
            
            float enemy_fas_temp = tar->fas*msg->spl_eff.param2;
            damage = -1*enemy_fas_temp*(enemy_fas_temp/(enemy_fas_temp + tar->faf)) + damage;
        }
        break;
    }
    
    return damage;
}

float Creature::onAttack(Creature *tar, int isBaoji)
{
    auto hp_restore = 0.0f;
    auto atk_inc    = 0.0f;
    
    float damage = hurtHandler(this, tar);

    for (auto &i : eff_stat)
    {
        switch (i.state)
        {
//            case SES_SUCK_BLOOD_BY_ATK_PCT:
//                hp_restore += atk * i.val_pct;
//                onSpellHpChanged(hp_restore, 0);
//                break;
            case SES_STRIKE_INFANTRY:
                if (tar->isType(CT_INFANTRY))
                    atk_inc += atk * i.val_pct;
                break;
            case SES_STRIKE_CAVALRY:
                if (tar->isType(CT_CAVALRY))
                    atk_inc += atk * i.val_pct;
                break;
            case SES_STRIKE_ARCHER:
                if (tar->isType(CT_ARCHER))
                    atk_inc += atk * i.val_pct;
                break;
        }
    }
    
    if( isBaoji == 1 )
    {
        damage = (damage - atk_inc) * baos;
    }
    else
    {
        damage = damage - atk_inc;
    }
    
    for (auto &i : eff_stat)
    {
        switch (i.state)
        {
            case SES_SUCK_BLOOD_BY_ATK_PCT:
                hp_restore += std::abs(damage) * i.val_pct;
                onSpellHpChanged(hp_restore, 0);
                break;
        }
    }
    
    if (hp < hp_max)
    {
        if (hp + hp_restore < hp_max)
            hp += hp_restore;
        else
            hp = hp_max;
    }

    return damage + hp_restore;
}

float Creature::onDefence(Creature *tar, float damage)
{
    auto atk_dec    = 0.0f;

    for (auto &it : eff_stat)
    {
        switch (it.state)
        {
        case SES_DEFENCE_VAL:
            atk_dec += it.val;
            break;
        case SES_DEFENCE_VAL_PCT:
            atk_dec += std::abs(damage * it.val_pct);
            break;
        case SES_STRIKE_ZX:
                if (enemyZxId == tar->zxId) {
                    atk_dec += std::abs(damage * it.val_pct);
                }
            break;
        }
    }
    
    if (atk_dec >= std::abs(damage)) {
        return 0;
    }else
        return damage + atk_dec;
}

void Creature::addHp(float addValue)
{
    if (addValue < 0.0f)
        hp += addValue;
    else if (hp + addValue > hp_max)
        hp = hp_max;
    else
        hp += addValue;
    
    if(hp < 0)
    {
        hp = 0.0f;
        changeGlobalState(S_Dying);
        changeState(S_Dying);
        
        onDead();
        
        sCrtMgr.checkHeroAllDead(this);
    }
}

void Creature::addPassiveSpell(Spell *spl)
{
    if (spl)
    {
        _pspl.push_back(spl);
        spl->bindCreature(this);
    }
}

Creature::PassiveSpellVec & Creature::getPassiveSpells()
{
    return _pspl;
}

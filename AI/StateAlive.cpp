#include <algorithm>

#include "StateInclude.h"

#include "AIConst.h"
#include "AIFactor.h"
#include "Creature.h"
#include "CreatureManager.h"
#include "Message.h"
#include "MessageHelper.h"
#include "Spell.h"

static Vector2D getCollideForce  (Creature *c, Message *msg);
static Vector2D getNormalForce   (Creature *c, Message *msg);
                                 
static void doPositionUpdate     (Creature *c, Message *msg);
static bool doConditionAuth      (Creature *c, Message *msg);
static void doSpellEffect        (Creature *c, Message *msg);
static void doPropChange         (Creature *c, Message *msg);
static void doPropChangeTime     (Creature *c, Message *msg);
static void doEffectStateAdd     (Creature *c, Message *msg);
static void doEffectStateAddTime (Creature *c, Message *msg);
static void doStateFreezingTime  (Creature *c, Message *msg);
static void doSpellHurt          (Creature *c, Message *msg);
static void doSpellCure          (Creature *c, Message *msg);

void StateAlive::handleMessage(Creature *c, Message *msg)
{
    if (!c)
        return;

    doPositionUpdate(c, msg);

    switch(msg->msg_t)
    {
    case MSG_EVT_TIME_OUT:
        {
			if (c->frdly == msg->sender->frdly)
			{
				//c->changeGlobalState(S_Idle);
				c->changeState(S_Idle);
			}
			else
			{
				c->changeGlobalState(S_Victory);
				c->changeState(S_Victory);
			}
            sAIEngine.setFinished(true);
        }
        break;
    case MSG_EVT_HERO_ALL_DIE:
        {
			if (c->frdly == msg->sender->frdly)
			{
				//c->changeGlobalState(S_Idle);
				c->changeState(S_Idle);
			}
			else
			{
				c->changeGlobalState(S_Victory);
				c->changeState(S_Victory);
            }
            sAIEngine.setFinished(true);
        }
        break;
    case MSG_HURT:
        {
            auto old_val = c->hp;
            auto cval = msg->val;
            c->hp = c->hp + cval;

            /*c->onSpellHpChanged(-cval);*/

            if (   msg->sender->isAlive() 
                && msg->sender != c
                && !msg->sender->isState(S_Attack)
                && !msg->sender->isState(S_Freezing))
            {
                sMsgDisp.sendDelayMessage(sMsgH.DeleteMessage(c, MSG_MOVE_TARGET));
                sMsgDisp.sendDelayMessage(sMsgH.DeleteMessage(c, MSG_CHARGE));
                c->tar = msg->sender;
                sMsgDisp.sendDelayMessage((sMsgH.moveTarget(c, msg->sender)));
                c->changeState(S_Move);
            }

            if (c->hp < 0.0f)
            {
                c->hp = 0.0f;
                c->changeGlobalState(S_Dying);
                c->changeState(S_Dying);

                c->onDead();

                sCrtMgr.checkHeroAllDead(c);
            }

            c->onHurt(msg->sender, cval, msg->isBaoji);
			
			if (c->isHero())
				sCrtMgr.setTotalHp(sCrtMgr.getTotalHp(c->frdly) + (c->hp - old_val), c->frdly);
        }
        break;
    case MSG_SPELL:
        {
            if (msg->total_time < 0)
            {
                if (msg->spl_isDelay)
                {
                    c->onEffectEnd(msg->spl_piece_id, msg->getId());
                }
                
                if (msg->spl_isTimely) {
                    c->onEffectStart(msg->spl_piece_id, msg->spl_isDelay, msg->getId());
                }
            }else
            {
                c->onEffectStart(msg->spl_piece_id, msg->spl_isDelay, msg->getId());
            }
            
            if (!doConditionAuth(c, msg))
                break;
            doSpellEffect(c, msg);
        }
        break;
    }
}

static Vector2D getCollideForce(Creature *c, Message *msg)
{
//    auto &force = c->force;
//    if (c->force_buf_cnt < 30)
//    {
//        c->force_buf_cnt++;
//        return force;
//    }
//    c->force_buf_cnt = 0;
    auto force = Vector2D();
    
    auto collide_pair = sCrtMgr.getCollideCreaturePair(c);

    for (auto &i : collide_pair.first)
    {
        if (c->isHero() && !i->isHero()
            || !c->isHero() && !c->isType(CT_ARCHER) && i->isType(CT_ARCHER))
                continue;

        auto fromCollide = c->pos - i->pos;
        auto len = fromCollide.len();
        //auto para = 1.0f - len / (c->round + i->round);
        // follows curve needed
        auto para = std::pow(1.03f, -210/(c->round + i->round) * len);
        fromCollide.norm();
        force += fromCollide * para;
    }

    for (auto &i : collide_pair.second)
    {
        if (c->isHero() && !i->isHero()
            || !c->isHero() && !c->isType(CT_ARCHER) && i->isType(CT_ARCHER))
                continue;

        auto fromCollide = c->pos - i->pos;
        auto len = fromCollide.len();
        auto para = std::pow(1.03f, -210/(c->round + i->round) * len);
        fromCollide.norm();
        force += fromCollide * para;
    }

    return force * CollideForceParam;
}

static Vector2D getNormalForce(Creature *c, Message *msg)
{
    if (c->tar)
        return (c->tar->pos - c->pos).getNorm();

    return c->frdly ? ForwardDirection : BackwardDirection;
}

//#include <cocos2d.h>
static void doPositionUpdate(Creature *c, Message *msg)
{
    auto spd = c->mv_spd / 60;

    auto force    = getCollideForce(c, msg);
    auto tar_dire = getNormalForce(c, msg);

    //CCLOG("FORCE: %f", force.len());
    //CCLOG("NORM FORCE: %f", tar_dire.len());

    c->recent_pos.emplace_back(c->pos.x, c->pos.y);
    if (c->recent_pos.size() > HistoryPosCheck)
        c->recent_pos.pop_front();

    if (c->isState(S_Move))
        force += spd * tar_dire;
    
    force.norm();

    c->pos = c->pos + force * spd;
    
    auto max_x = BattleFieldLength * BattleFieldEdge;
    auto max_y = BattleFieldWidth  * BattleFieldEdge;
    c->pos.x = c->pos.x < 0.0f ? 0.0f : (c->pos.x > max_x ? max_x - 0.01f : c->pos.x);
    c->pos.y = c->pos.y < 0.0f ? 0.0f : (c->pos.y > max_y ? max_y - 0.01f : c->pos.y);

    c->dire = tar_dire;
}

static bool doConditionAuth(Creature *c, Message *msg)
{
    if (sAIEngine.randf() > msg->spl_cond.prob)
        return false;

    switch (msg->spl_cond.v_cond)
    {
    case SVC_HP_VALUE_GREATER:
        if (msg->spl_cond.val <= c->hp)
            return false;
    case SVC_HP_VALUE_LOWER:
        if (msg->spl_cond.val >= c->hp)
            return false;
    }

    switch (msg->spl_cond.vp_cond)
    {
    case SVPC_HP_PCT_GREATER:
        if (msg->spl_cond.val_pct <= (c->hp / c->hp_max))
            return false;
    case SVPC_HP_PCT_LOWER:
        if (msg->spl_cond.val_pct >= (c->hp / c->hp_max))
            return false;
    }

    return true;
}

static void doSpellEffect(Creature *c, Message *msg)
{
    switch (msg->spl_eff.effect)
    {
        case SE_PROP_CHANGE:
            doPropChange(c, msg);
            break;
        case SE_PROP_CHANGE_TIME:
            doPropChangeTime(c, msg);
            break;
        case SE_EFFECT_STATE_ADD:
            doEffectStateAdd(c, msg);
            break;
        case SE_EFFECT_STATE_ADD_TIME:
            doEffectStateAddTime(c, msg);
            break;
        case SE_STATE_FREEZING_TIME:
            doStateFreezingTime(c, msg);
            break;
        case SE_SPELL_HURT:
            doSpellHurt(c, msg);
            break;
        case SE_SPELL_CURE:
            doSpellCure(c, msg);
            break;
    }
}

static void doPropChange(Creature *c, Message *msg)
{
    auto val = msg->spl_eff.val;
    auto val_pct = msg->spl_eff.val_pct;

//    if (c->isHero() && (msg->spl_eff.spc == SPC_HP_MAX || msg->spl_eff.spc == SPC_HP))
//    {
//        val *= HeroHPChangeFactor;
//        val_pct *= HeroHPChangeFactor;
//    }
//
    auto old_val = 0.0f;
    auto cval    = 0.0f;
    
    if(msg->total_time >= 0 || msg->spl_isTimely )
    {
        switch (msg->spl_eff.spc)
        {
        case SPC_ATK:
            c->atk += val + c->raw_atk * val_pct;
            if (c->atk < 0)
                c->atk = 0.0f;
            break;
        case SPC_HP:
            old_val = c->hp;
            cval    = val + c->raw_hp * val_pct;
            
            if (not msg->spl_isPtsp) {
                c->onSpellHpChanged(cval, 0);
            }
            if (cval < 0.0f)
                c->hp += c->onDefence(msg->sender, cval);
            else if (c->hp > c->hp_max)
                c->hp = c->hp;
            else if (c->hp + cval > c->hp_max)
            {
                if (msg->spl_isPtsp)
                {
                    if(!c->isHero())
                    {
                        c->hp = c->hp + cval;
                    }
                }else
                    c->hp = c->hp_max;
            }
            else
                c->hp += cval;

            if (c->hp < 0.0f)
            {
                c->hp = 0.0f;
                c->changeGlobalState(S_Dying);
                c->changeState(S_Dying);
                
                c->onDead();

                sCrtMgr.checkHeroAllDead(c);
            }

            if (c->isHero() && !msg->spl_isPtsp)
                sCrtMgr.setTotalHp(sCrtMgr.getTotalHp(c->frdly) + (c->hp - old_val), c->frdly);
            break;
        case SPC_HP_MAX:
            c->hp     += val + c->raw_hp_max * val_pct;
            c->hp_max += val + c->raw_hp_max * val_pct;
            
            if (c->hp < 1.0f)
                c->hp = 1.0f;
            if (c->hp_max < 1.0f)
                c->hp_max = 1.0f;
            break;
        case SPC_FAS:
            c->fas += val + c->raw_fas * val_pct;
            if (c->fas < 0)
                c->fas = 0.0f;
            break;
        case SPC_FAF:
            c->faf += val + c->raw_faf * val_pct;
            if (c->faf < 0)
                c->faf = 0.0f;
            break;
        case SPC_ATK_SPD:
            c->atk_spd += val + c->raw_atk_spd * val_pct;
            if (c->atk_spd < 0)
                c->atk_spd = 0.0f;
            break;
        case SPC_MOV_SPD:
            c->mv_spd += val + c->raw_mv_spd * val_pct;
            if (c->mv_spd < 0)
                c->mv_spd = 0.0f;
            break;
        case SPC_ATK_RANGE:
            c->atk_range += val + c->raw_atk_range * val_pct;
            if (c->atk_range < 0)
                c->atk_range = 0.0f;
            break;
        case SPC_HP_CUR:  //按当前英雄血量的百分比减血
            old_val = c->hp;
            cval    = val + c->hp * val_pct;
            
            if (not msg->spl_isPtsp) {
                c->onSpellHpChanged(cval, 0);
            }
            
            if (cval < 0.0f)
            {
                if( c == msg->sender)  //自身给自身当前血量减血时，不考虑自身buff抵挡
                    c->hp += cval;
                else
                    c->hp += c->onDefence(msg->sender, cval);
            }
            else if (c->hp > c->hp_max)
                c->hp = c->hp;
            else if (c->hp + cval > c->hp_max)
                c->hp = c->hp_max;
            else
                c->hp += cval;
            
            if (c->hp < 0.0f)
            {
                c->hp = 0.0f;
                c->changeGlobalState(S_Dying);
                c->changeState(S_Dying);
                
                c->onDead();
                
                sCrtMgr.checkHeroAllDead(c);
            }
            
            if (c->isHero())
                sCrtMgr.setTotalHp(sCrtMgr.getTotalHp(c->frdly) + (c->hp - old_val), c->frdly);
            break;
        }
    }
}

static void doPropChangeTime(Creature *c, Message *msg)
{
    auto val = msg->spl_eff.val;
    auto val_pct = msg->spl_eff.val_pct;

    auto  chgd_val_ptr = msg->getVarValue(c);
    auto& chgd_val     = *chgd_val_ptr; // if throws, means c is not in _creature 

    auto old_val   = 0.0f;
    auto cval      = 0.0f;

    auto ttm = msg->total_time;

    switch (msg->spl_eff.spc)
    {
    case SPC_ATK:
        old_val = c->atk;

        if (ttm < 0)
            c->atk -= chgd_val;
        else
            c->atk += val + c->raw_atk * val_pct;

        if (c->atk < 0)
            c->atk = 0.0f;

        chgd_val += c->atk - old_val;
        break;
    case SPC_HP:
        old_val = c->hp;

        if (ttm < 0)
        {
            c->hp -= chgd_val;

            if (not msg->spl_isPtsp) {
                c->onSpellHpChanged(cval, 0);
            }
        }
        else
        {
            cval = val + c->raw_hp * val_pct;

            if (not msg->spl_isPtsp) {
                c->onSpellHpChanged(cval, 0);
            }

            if (cval < 0.0f)
                c->hp += c->onDefence(msg->sender, cval);
            else if (c->hp > c->hp_max)
                c->hp = c->hp;
            else if (c->hp + cval > c->hp_max)
                c->hp = c->hp_max;
            else
                c->hp += cval;
        }

        if (c->hp <= 0.0f)
        {
            c->hp = 0.0f;
            c->changeGlobalState(S_Dying);
            c->changeState(S_Dying);
            
            c->onDead();

            sCrtMgr.checkHeroAllDead(c);
        }

		if (c->isHero())
			sCrtMgr.setTotalHp(sCrtMgr.getTotalHp(c->frdly) + (c->hp - old_val), c->frdly);
        chgd_val += c->hp - old_val;
        break;
    case SPC_HP_MAX:
        old_val = c->hp_max;
        if (ttm < 0)
            c->hp_max -= chgd_val;
        else
            c->hp_max += val + c->raw_hp_max * val_pct;

        if (c->hp_max < 1.0f)
            c->hp_max = 1.0f;

        chgd_val += c->hp_max - old_val;
        break;
    case SPC_FAS:
        old_val = c->fas;
        
        if (ttm < 0)
            c->fas -= chgd_val;
        else
            c->fas += val + c->raw_fas * val_pct;
        
        if (c->fas < 0)
            c->fas = 0.0f;
        
        chgd_val += c->fas - old_val;
        break;
            
    case SPC_FAF:
        old_val = c->faf;
        
        if (ttm < 0)
            c->faf -= chgd_val;
        else
            c->faf += val + c->raw_faf * val_pct;
        
        if (c->faf < 0)
            c->faf = 0.0f;
        
        chgd_val += c->faf - old_val;
        break;
            
    case SPC_ATK_SPD:
        old_val = c->atk_spd;

        if (ttm < 0)
            c->atk_spd -= chgd_val;
        else
            c->atk_spd += val + c->raw_atk_spd * val_pct;
        
        if (c->atk_spd < 0)
            c->atk_spd = 0.0f;

        chgd_val += c->atk_spd - old_val;
        break;
    case SPC_MOV_SPD:
        old_val = c->mv_spd;

        if (ttm < 0)
            c->mv_spd -= chgd_val;
        else
            c->mv_spd += val + c->raw_mv_spd * val_pct;
        
        if (c->mv_spd < 0)
            c->mv_spd = 0.0f;

        chgd_val += c->mv_spd - old_val;
        break;
    case SPC_ATK_RANGE:
        old_val = c->atk_range;

        if (ttm < 0)
            c->atk_range -= chgd_val;
        else
            c->atk_range += val + c->raw_atk_range * val_pct;
        
        if (c->atk_range < 0)
            c->atk_range = 0.0f;

        chgd_val += c->atk_range - old_val;
        break;
    case SPC_HP_CUR:  //按当前英雄血量的百分比减血
        old_val = c->hp;
        
        if (ttm < 0)
        {
            c->hp -= chgd_val;
            
            if (not msg->spl_isPtsp) {
                c->onSpellHpChanged(cval, 0);
            }
        }
        else
        {
            cval = val + c->hp * val_pct;
            
            if (not msg->spl_isPtsp) {
                c->onSpellHpChanged(cval, 0);
            }
            
            if (cval < 0.0f)
                c->hp += c->onDefence(msg->sender, cval);
            else if (c->hp > c->hp_max)
                c->hp = c->hp;
            else if (c->hp + cval > c->hp_max)
                c->hp = c->hp_max;
            else
                c->hp += cval;
        }
        
        if (c->hp <= 0.0f)
        {
            c->hp = 0.0f;
            c->changeGlobalState(S_Dying);
            c->changeState(S_Dying);
            
            c->onDead();
            
            sCrtMgr.checkHeroAllDead(c);
        }
        
        if (c->isHero())
            sCrtMgr.setTotalHp(sCrtMgr.getTotalHp(c->frdly) + (c->hp - old_val), c->frdly);
        chgd_val += c->hp - old_val;
        break;
    case SPC_ABSORB_ATK:

        old_val = msg->sender->atk;
        
        if (ttm < 0)
            msg->sender->atk -= chgd_val;
        else
        {
            if (msg->receiver.size()>0) {
                if (msg->receiver[0].first->atk*val_pct > 3*msg->sender->atk)
                {
                    msg->sender->atk += 3*msg->sender->atk;
                }else
                    msg->sender->atk += msg->receiver[0].first->atk*val_pct;
            }
        }
        
        if (msg->sender->atk < 0)
            msg->sender->atk = 0.0f;
        
        chgd_val += msg->sender->atk - old_val;
        break;


    }
}

static void doEffectStateAdd(Creature *c, Message *msg)
{
    auto &e = msg->spl_eff;
    auto &i = c->addEffectState(SpellEffectStateHolder(e.state));
    i.val     = e.val;
    i.val_pct = e.val_pct;
}

static void doEffectStateAddTime(Creature *c, Message *msg)
{
    if (msg->total_time < 0)
    {
        c->removeEffectState(msg->spl_eff.state);
    }
    else
        doEffectStateAdd(c, msg);
}

static void doStateFreezingTime(Creature *c, Message *msg)
{
    if (msg->total_time < 0)
	{
        c->tar = 0;
		c->changeState(S_Move);
		sMsgDisp.sendMessage(sMsgH.charge(c));
    }
    else
    {
        c->changeState(S_Freezing);
    }
}

static void doSpellHurt(Creature *c, Message *msg)
{
    if(msg->total_time >= 0 || msg->spl_isTimely)
    {
        float damage = c->onDefence(msg->sender, Creature::hurtHandler(msg->sender, c, msg, msg->spl_type, msg->spl_eff.val_pct, msg->spl_eff.val));
        
        switch (msg->spl_type) {
            case ST_ENEMY_ATK_SUCK_BLOOD:
                
                auto sender = msg->sender;
                float hp_restore = std::abs(damage)*msg->spl_eff.param3;
                sender->onSpellHpChanged(hp_restore, 0);
                
                if (sender->hp < sender->hp_max)
                {
                    if (sender->hp + hp_restore < sender->hp_max)
                        sender->hp += hp_restore;
                    else
                        sender->hp = sender->hp_max;
                }
                break;
        }
        
        if(damage != 0)
        {
            c->onSpellHpChanged(damage, 0);
            c->addHp(damage);
        }
    }
}

static void doSpellCure(Creature *c, Message *msg)
{
    if(msg->total_time >= 0 || msg->spl_isTimely)
    {
        float cureValue = msg->sender->fas*msg->spl_eff.val_pct;
        c->onSpellHpChanged(cureValue, 0);
        c->addHp(cureValue);
    }
}


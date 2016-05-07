#include "Spell.h"

#include "Message.h"
#include "MessageDispatcher.h"
#include "CreatureManager.h"
#include "Creature.h"
#include <math.h>
#include "Luatool.h"

Spell::Spell(SpellType spell_t, int cool_down,/*float mp_cost,*/ int cast_time)
    : _spell_t(spell_t)
    //, _mp_cost(mp_cost)
    , _cool_down(0)
    , _raw_cool_down(cool_down)
    , _cast_tm(cast_time)
    , _castor(0)
{
}

Spell::~Spell()
{
	for (auto &i : _sp_vec)
		delete i;
}

void Spell::castSpell(Creature *tar, Vector2D tar_pos)
{
    Vector2D                sel_pos;
    Vector2D                sel_dire;

    if (std::cos(_castor->dire.getAngle()) > 0.0f)
        sel_dire = ForwardDirection;
    else
        sel_dire = BackwardDirection;

    auto *msg_vec = new std::vector<Message *>;

    std::vector<Creature*> *crt_vec = 0;

    SpellSelectorPiece     *sel_p   = 0;

//    Creature               *spl_tar = 0;

    SpellTargetVec          stVec;

    auto err = false;

    for (auto i : _sp_vec)
    {
        sel_p = &(i->selector);

        switch (sel_p->pos)
        {
        case SP_SELF:
            if (!_castor)
                break;
            sel_pos  = _castor->pos;
            break;
        case SP_TARGET:
            if (!tar)
                break;
            sel_pos  = tar->pos;
            break;
        case SP_POSITION:
            sel_pos = tar_pos;
            break;
        default:
            delete msg_vec;
            return;
        }

        auto msg = new Message;

        switch (sel_p->range)
        {
            case SR_BEGIN:
                    break;
            case SR_CIRCLE:
                crt_vec = sCrtMgr.getCircleUnits(sel_pos,
                                                 sel_p->data.circle.radius,
                                                 sel_p->frdly);
                for (auto &i : *crt_vec)
                    msg->addReceiver(i);
                break;
            case SR_SECTOR:
                crt_vec = sCrtMgr.getSectorUnits(sel_pos, sel_dire,
                                                 sel_p->data.sector.radius,
                                                 sel_p->data.sector.angle,
                                                 sel_p->frdly,
                                                 sel_p->data.sector.back_len);
                for (auto &i : *crt_vec)
                    msg->addReceiver(i);
                break;
            case SR_RECTANGLE:
                crt_vec = sCrtMgr.getRectangleUnits(sel_pos,
                                                    sel_dire,
                                                    sel_p->data.rect.width,
                                                    sel_p->data.rect.height,
                                                    sel_p->frdly,
                                                    sel_p->data.rect.back_len);
                for (auto &i : *crt_vec)
                    msg->addReceiver(i);
                break;
            case SR_ALL:
                {
                    auto &t = sCrtMgr.getCreatureMap();
                    for (auto &it : t)
                    {
                        switch (sel_p->frdly)
                        {
                        case 0:
                            msg->addReceiver(it.second);
                            break;
                        case 1:
                            if (it.second->frdly)
                                msg->addReceiver(it.second);
                            break;
                        case 2:
                            if (!it.second->frdly)
                                msg->addReceiver(it.second);
                            break;
                        }
                    }
                }
                break;
            case SR_ALL_EXCEPT_SELF:
                {
                    auto &t = sCrtMgr.getCreatureMap();
                    for (auto &it : t)
                    {
                        if (it.second == _castor)
                            continue;

                        switch (sel_p->frdly)
                        {
                        case 0:
                            msg->addReceiver(it.second);
                            break;
                        case 1:
                            if (it.second->frdly)
                                msg->addReceiver(it.second);
                            break;
                        case 2:
                            if (!it.second->frdly)
                                msg->addReceiver(it.second);
                            break;
                        }
                    }
                }
                break;
            case SR_MAIN_HERO:
                crt_vec = sCrtMgr.getMainHeros(sel_p->frdly);
                for (auto it : *crt_vec)
                    msg->addReceiver(it);
                break;
            case SR_VICE_HERO:
                crt_vec = sCrtMgr.getViceHeros(sel_p->frdly);
                for (auto it : *crt_vec)
                    msg->addReceiver(it);
                break;
            case SR_SOLDIER:
                crt_vec = sCrtMgr.getSoldiers(sel_p->frdly);
                for (auto it : *crt_vec)
                    msg->addReceiver(it);
                break;

            case SR_SELF:
                msg->addReceiver(_castor);
                break;
            case SR_TARGET:
                msg->addReceiver(tar);
                break;
            case SR_CUR_TARGET:
                msg->addReceiver(_castor->tar);

            case SR_ATK_HIGHEST:
                msg->addReceiver(sCrtMgr.getPropertyHighest(&Creature::atk, sel_p->frdly));
                break;
            case SR_HP_HIGHEST:
                msg->addReceiver(sCrtMgr.getPropertyHighest(&Creature::hp, sel_p->frdly));
                break;
            case SR_FAS_HIGHEST:
                msg->addReceiver(sCrtMgr.getPropertyHighest(&Creature::fas, sel_p->frdly));
                break;
            case SR_FAF_HIGHEST:
                msg->addReceiver(sCrtMgr.getPropertyHighest(&Creature::faf, sel_p->frdly));
                break;
            case SR_ATK_SPD_HIGHEST:
                msg->addReceiver(sCrtMgr.getPropertyHighest(&Creature::atk_spd, sel_p->frdly));
                break;
            case SR_MOV_SPD_HIGHEST:
                msg->addReceiver(sCrtMgr.getPropertyHighest(&Creature::mv_spd, sel_p->frdly));
                break;
            case SR_ATK_RANGE_HIGHEST:
                msg->addReceiver(sCrtMgr.getPropertyHighest(&Creature::atk_range, sel_p->frdly));
                break;
            case SR_WULI_HIGHEST:
                msg->addReceiver(sCrtMgr.getPropertyHighest(&Creature::wuli, sel_p->frdly));
                break;
            case SR_ZHILI_HIGHEST:
                msg->addReceiver(sCrtMgr.getPropertyHighest(&Creature::zhili, sel_p->frdly));
                break;
            case SR_TONGYU_HIGHEST:
                msg->addReceiver(sCrtMgr.getPropertyHighest(&Creature::tongyu, sel_p->frdly));
                break;
            case SR_ATK_LOWEST:
                msg->addReceiver(sCrtMgr.getPropertyLowest(&Creature::atk, sel_p->frdly));
                break;
            case SR_HP_LOWEST:                 
                msg->addReceiver(sCrtMgr.getPropertyLowest(&Creature::hp, sel_p->frdly));
                break;
            case SR_FAS_LOWEST:
                msg->addReceiver(sCrtMgr.getPropertyLowest(&Creature::fas, sel_p->frdly));
                break;
            case SR_FAF_LOWEST:
                msg->addReceiver(sCrtMgr.getPropertyLowest(&Creature::fas, sel_p->frdly));
                break;
            case SR_ATK_SPD_LOWEST:            
                msg->addReceiver(sCrtMgr.getPropertyLowest(&Creature::atk_spd, sel_p->frdly));
                break;
            case SR_MOV_SPD_LOWEST:            
                msg->addReceiver(sCrtMgr.getPropertyLowest(&Creature::mv_spd, sel_p->frdly));
                break;
            case SR_ATK_RANGE_LOWEST:          
                msg->addReceiver(sCrtMgr.getPropertyLowest(&Creature::atk_range, sel_p->frdly));
                break;
            case SR_WULI_LOWEST:
                msg->addReceiver(sCrtMgr.getPropertyLowest(&Creature::wuli, sel_p->frdly));
                break;
            case SR_ZHILI_LOWEST:
                msg->addReceiver(sCrtMgr.getPropertyLowest(&Creature::zhili, sel_p->frdly));
                break;
            case SR_TONGYU_LOWEST:
                msg->addReceiver(sCrtMgr.getPropertyLowest(&Creature::tongyu, sel_p->frdly));
                break;
            case SR_NEAREST_HERO:
                msg->addReceiver(sCrtMgr.getNearestHero(_castor, sel_p->frdly));
                break;
            case SR_INFANTRY:
                crt_vec = sCrtMgr.getSoldiersByType(CT_INFANTRY, sel_p->frdly);
                for (auto it : *crt_vec)
                    msg->addReceiver(it);
                break;
            case SR_CAVALRY:
                crt_vec = sCrtMgr.getSoldiersByType(CT_CAVALRY, sel_p->frdly);
                for (auto it : *crt_vec)
                    msg->addReceiver(it);
                break;
            case SR_ARCHER:
                crt_vec = sCrtMgr.getSoldiersByType(CT_ARCHER, sel_p->frdly);
                for (auto it : *crt_vec)
                    msg->addReceiver(it);
                break;

            case SR_SHU:
                crt_vec = sCrtMgr.getSoldiersByNation(CN_SHU, sel_p->frdly);
                for (auto it : *crt_vec)
                    msg->addReceiver(it);
                break;
            case SR_WU:
                crt_vec = sCrtMgr.getSoldiersByNation(CN_WU, sel_p->frdly);
                for (auto it : *crt_vec)
                    msg->addReceiver(it);
                break;
            case SR_WEI:
                crt_vec = sCrtMgr.getSoldiersByNation(CN_WEI, sel_p->frdly);
                for (auto it : *crt_vec)
                    msg->addReceiver(it);
                break;
            case SR_MASS:
                crt_vec = sCrtMgr.getSoldiersByNation(CN_MASS, sel_p->frdly);
                for (auto it : *crt_vec)
                    msg->addReceiver(it);
                break;

            case SR_ALL_HERO:
                crt_vec = sCrtMgr.getAllHeros(sel_p->frdly);
                for (auto it : *crt_vec)
                    msg->addReceiver(it);
                break;

            case SR_NEAREST_HERO_CIRCLE:
                {
                    auto h = sCrtMgr.getNearestHero(_castor, sel_p->frdly);
                    msg->addReceiver(h);
                    crt_vec = sCrtMgr.getCircleUnits(h->pos,
                                                     sel_p->data.circle.radius,
                                                     sel_p->frdly);
                    for (auto &i : *crt_vec)
                        msg->addReceiver(i);
                }
                break;
            case SR_NEAREST_HERO_SECTOR:
                {
                    auto h = sCrtMgr.getNearestHero(_castor, sel_p->frdly);

                    auto dire = BackwardDirection;
                    if (std::cos(h->dire.getAngle()) > 0.0f)
                        dire = ForwardDirection;

                    msg->addReceiver(h);
                    crt_vec = sCrtMgr.getSectorUnits(h->pos, dire,
                                                     sel_p->data.sector.radius,
                                                     sel_p->data.sector.angle,
                                                     sel_p->frdly,
                                                     sel_p->data.sector.back_len);
                    for (auto &i : *crt_vec)
                        msg->addReceiver(i);
                }
                break;
            case SR_NEAREST_HERO_RECTANGLE:
                {
                    auto h = sCrtMgr.getNearestHero(_castor, sel_p->frdly);

                    auto dire = BackwardDirection;
                    if (std::cos(h->dire.getAngle()) > 0.0f)
                        dire = ForwardDirection;

                    msg->addReceiver(h);
                    crt_vec = sCrtMgr.getRectangleUnits(h->pos, dire,
                                                        sel_p->data.rect.width,
                                                        sel_p->data.rect.height,
                                                        sel_p->frdly,
                                                        sel_p->data.rect.back_len);
                    for (auto &i : *crt_vec)
                        msg->addReceiver(i);
                }
                break;
            case SR_SELE_SOLDIER:
                {
                    msg->addReceiver(_castor);
                    crt_vec = sCrtMgr.getSoldiers(sel_p->frdly);
                    for (auto it : *crt_vec)
                        msg->addReceiver(it);
                }
                break;
            case SR_END:
                break;
        }
        
        msg->msg_t      = MSG_SPELL;
        msg->sender     = _castor;

        msg->spl_type     = i->spType;
        msg->spl_isPtsp   =  i->isPtsp;
        
        msg->spl_cond     = i->condition;
        msg->spl_eff      = i->effect;
        msg->spl_piece_id = i->getId();
        
        float param1 = Luatool::getLuaGlobalDoubleValue("g_delay_param1");
        float param2 = Luatool::getLuaGlobalDoubleValue("g_delay_param2");
        float param3 = Luatool::getLuaGlobalDoubleValue("g_delay_param3");
        
        switch (i->time.delay_t) {
            case SDT_PT:
                msg->delay      = i->time.delay;
                msg->interval   = i->time.interval;
                msg->total_time = i->time.total_time;
                break;
            case SDT_WULI:
            {
                msg->delay      = i->time.delay;
                if (i->time.interval == 60.0f)
                {
                    msg->interval = i->time.interval;
                    msg->total_time = param1*std::pow(param2, param3*_castor->wuli)*60;
                }else
                {
                    msg->interval = msg->total_time = param1*std::pow(param2, param3*_castor->wuli)*60;
                }
            }
            break;
            case SDT_ZHILI:
            {
                msg->delay      = i->time.delay;
                if (i->time.interval == 60.0f)
                {
                    msg->interval = i->time.interval;
                    msg->total_time = param1*std::pow(param2, param3*_castor->zhili)*60;
                }else
                {
                    msg->interval = msg->total_time = param1*std::pow(param2, param3*_castor->zhili)*60;
                }
            }
            break;
            default:
                msg->delay      = i->time.delay;
                msg->interval   = i->time.interval;
                msg->total_time = i->time.total_time;
                break;
        }
        
        if( msg->interval > 0.0f && msg->interval == msg->total_time )
        {
            msg->spl_isDelay = true;
        }else if(msg->total_time == 0.0f )
        {
            msg->spl_isTimely = true;
            if (msg->interval == 0.0f) {
                msg->interval = 1;
            }
        }

        msg_vec->push_back(msg);

        std::pair<int, std::set<Creature *>> item;
        item.first = i->getId();

		for (auto &i : msg->receiver)
			item.second.insert(i.first);

        stVec.push_back(std::move(item));

        delete crt_vec;
        crt_vec = 0;
    }
    
    if (err)
    {
        for (auto it : *msg_vec)
            delete it;
        delete msg_vec;
        return;
    }

    auto smsg = new Message;    // self message
    smsg->msg_t   = MSG_SPELL_END;
    smsg->delay   = _cast_tm;
    smsg->sender  = _castor;
    smsg->addReceiver(_castor);
    sMsgDisp.sendMessage(smsg);
    
    for (auto it : *msg_vec)
    {
        sMsgDisp.sendMessage(it);
    }

    _castor->setSpellTarget(std::move(stVec));
    _castor->changeState(S_Spelling);
}

void Spell::bindCreature(Creature *c)
{
    _castor = c;

    for (auto &i : _sp_vec)
    {
        if (!_castor->frdly)
        {
            if (i->selector.frdly == 1)
                i->selector.frdly = 2;
            else if (i->selector.frdly == 2)
                i->selector.frdly = 1;
        }
    }
}

void Spell::updateValue()
{
}

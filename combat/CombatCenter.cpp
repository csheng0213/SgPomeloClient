//
//  CombatCenter.cpp
//  LuaSanGuo
//
//  Created by waiboziStudio on 15/6/2.
//
//

#include "CombatCenter.h"
#include "AIEngine.h"
#include "LuaFunctor.hpp"
#include "CCLuaEngine.h"
#include <iostream>
#include <string>
#include <iostream>

#include "LuaFunctor.hpp"


static CombatCenter* m_shareCombatCenter = nullptr;

CombatCenter* CombatCenter::getInstance()
{
    if (!m_shareCombatCenter) {
        m_shareCombatCenter = new CombatCenter();
    }
    
    return  m_shareCombatCenter;
}

void CombatCenter::addLuaCreature(LuaReader& reader)
{
    
    CreatureType viewType = (CreatureType)reader.get_int("m_viewType");
    LuaCreature* creature = new LuaCreature(viewType);
    
    creature->pid = reader.get_int("m_pid");
    creature->frdly = (bool)reader.get_int("m_frdly");
    
    reader.enter_table("m_dire");
    creature->dire = Vector2D(reader.get_int(1), reader.get_int(2));
    reader.leave_table();
    
    reader.enter_table("m_pos");
    creature->pos = Vector2D(reader.get_int("x"), reader.get_int("y"));
    reader.leave_table();
    
    creature->wuli = reader.get_double("m_wuli");
    creature->zhili = reader.get_double("m_zhili");
    creature->tongyu = reader.get_double("m_tongyu");

    creature->hp = reader.get_double("m_hp");
    creature->hp_max = reader.get_double("m_hp_max");
    creature->raw_hp = reader.get_double("m_raw_hp");
    creature->raw_hp_max = reader.get_double("m_raw_hp_max");
    
    creature->mp = reader.get_double("m_mp");
    creature->mp_max = reader.get_double("m_mp_max");
    creature->raw_mp = reader.get_double("m_raw_mp");
    creature->raw_mp_max = reader.get_double("m_raw_mp_max");
    
    creature->atk = reader.get_double("m_atk");
    creature->raw_atk = reader.get_double("m_raw_atk");
    
    creature->fas = reader.get_double("m_fas");
    creature->raw_fas = reader.get_double("m_raw_fas");
    
    creature->faf = reader.get_double("m_faf");
    creature->raw_faf = reader.get_double("m_raw_faf");
    
    creature->baoj = reader.get_double("m_baoj");
    creature->raw_baoj = reader.get_double("m_raw_baoj");
    
    creature->baos = reader.get_double("m_baos");
    creature->raw_baos = reader.get_double("m_raw_baos");
    
    creature->num = reader.get_int("m_num");
    creature->round = reader.get_double("m_round");
    
    creature->atk_spd = reader.get_double("m_atk_spd");
    creature->raw_atk_spd = reader.get_double("m_raw_atk_spd");
    
    creature->atk_range = reader.get_double("m_atk_range");
    creature->raw_atk_range = reader.get_double("m_raw_atk_range");
    
    creature->vis_field = reader.get_double("m_vis_field");
    creature->raw_vis_field = reader.get_double("m_raw_vis_field");
    
    creature->mv_spd = reader.get_double("m_mv_spd");
    creature->raw_mv_spd = reader.get_double("m_raw_mv_spd");
    
    creature->cnation = (CreatureNation)reader.get_int("m_cnotion");
    
    creature->zxId = reader.get_int("m_zxId");
    
    creature->enemyZxId = reader.get_int("m_enemyZxId");
    
    creature->setFightNumer(reader.get_int("m_fightNumber"));
    
    Spell* pSpell = nullptr;
    if (reader.enter_table("m_spells") == 0)
    {
        //添加主动技能
        uint32_t skillLen = reader.get_int("m_SkillLen");
        if (skillLen > 0 ) {
            reader.enter_table("skillDatas");
            for (int i = 1; i <= skillLen; i++) {
                
                reader.enter_table(i);
                if (pSpell == nullptr) {
                    pSpell = new Spell((SpellType)reader.get_int("m_SkillDamageType"), reader.get_int("m_SkillCd"));
                }
                
                pSpell->addSpellPiece(creatSpellPiece(reader));
                reader.leave_table();
            }
            reader.leave_table();
        }
        
        creature->setSpell(pSpell);
        
        //添加被动技能
        uint32_t passiveSkillLen = reader.get_int("m_passiveSkillLen");
        if (passiveSkillLen > 0 ) {
            reader.enter_table("passiveSkillDatas");
            for (int i = 1; i <= passiveSkillLen; i++) {
                
                reader.enter_table(i);
                
                Spell* pSpellb = new Spell(ST_BEGIN, 0, 1); // bing zhong ke zhi
                auto proc = [creature, pSpellb](SpellRange r,int skillFrdly, SpellPropertyChoose c, float pct)
                {
                    SpellPiece* piece = new SpellPiece(creature->pid);
                    piece->selector.range = r; // 技能范围
                    piece->selector.frdly = skillFrdly;
                    piece->effect.effect = SE_PROP_CHANGE; // 改变属性
                    piece->effect.spc = c;
                    piece->effect.val_pct = pct; // 加成百分比
                    piece->time.interval = 0xfffffff;
                    piece->time.total_time = 0xfffffff;
                    piece->isPtsp = true;
                    pSpellb->addSpellPiece(piece);
                };
                
                int skillFrdly = reader.get_double("m_skillFrdly");
                float addhp = reader.get_double("m_addHp");
                if (addhp != 0)
                {
                    proc((SpellRange)reader.get_int("m_spellRange"),skillFrdly, SPC_HP, addhp);
                }
                
                float addAttack = reader.get_double("m_addAttack");
                if (addAttack != 0)
                {
                    proc((SpellRange)reader.get_int("m_spellRange"),skillFrdly, SPC_ATK, addAttack);
                }
                
                float addFas = reader.get_double("m_addFas");
                if( addFas != 0 )
                {
                    proc((SpellRange)reader.get_int("m_spellRange"),skillFrdly, SPC_FAS, addFas);
                }
                
                float addFaf = reader.get_double("m_addFaf");
                if( addFaf != 0 )
                {
                    proc((SpellRange)reader.get_int("m_spellRange"),skillFrdly, SPC_FAF, addFaf);
                }
                
                creature->addPassiveSpell(pSpellb);

                
                reader.leave_table();
            }
            reader.leave_table();
        }

        //添加小兵技能
        uint32_t soldierSkillLen = reader.get_int("m_soldierSkillLen");
        if (soldierSkillLen > 0 ) {
            reader.enter_table("soldierSkillDatas");
            for (int i = 1; i <= soldierSkillLen; i++) {
                
                reader.enter_table(i);
                
                Spell* pSpell = new Spell(ST_BEGIN, 0, 1); // bing zhong ke zhi
                SpellPiece* piece = new SpellPiece(reader.get_int("id"));
                piece->selector.range = SR_SELF;
                piece->effect.effect = SE_EFFECT_STATE_ADD;
                piece->effect.state = (SpellEffectState)reader.get_int("state");
                piece->effect.val_pct = reader.get_double("val_pct");
                piece->time.interval = 0xfffffff;
                piece->time.total_time = 0xfffffff;
                
                pSpell->addSpellPiece(piece);
                creature->addPassiveSpell(pSpell);
                
                reader.leave_table();
            }
            reader.leave_table();
        }
        
        //添加其它被动效果技能
        uint32_t otherSkillLen = reader.get_int("m_otherSkillLen");
        if (otherSkillLen > 0)
        {
            reader.enter_table("otherSkillDatas");
            for (int i = 1; i <= otherSkillLen; i++) {
                reader.enter_table(i);
                
                Spell* pSpell = new Spell(ST_BEGIN);
                SpellPiece* piece = new SpellPiece(reader.get_int("id"));
                piece->selector.range = SR_SELF;
                piece->effect.effect = SE_EFFECT_STATE_ADD;
                piece->effect.state = (SpellEffectState)reader.get_int("state");
                piece->effect.val_pct = reader.get_double("val_pct");
                piece->time.interval = 0xfffffff;
                piece->time.total_time = 0xfffffff;
                
                pSpell->addSpellPiece(piece);
                creature->addPassiveSpell(pSpell);
                
                reader.leave_table();

            }
            reader.leave_table();
        }
        
    }
    
    reader.leave_table();
    m_creatures.insert(std::make_pair(creature->getFightNumer(), creature));
    sCrtMgr.pushCreature(creature);
}

void CombatCenter::addAutoFightCon(LuaReader &reader)
{
    bool isFly = reader.get_int("isFly");
    AutoFightHero fightHero = (AutoFightHero)reader.get_int("index");

    reader.enter_table("data");
    
    int skillConLen = reader.get_int("skillConLen");
    for (int i = 1; i <= skillConLen; i++)
    {
        AutoFightCondSet condset;
        reader.enter_table(i);
        condset.logic = (AutoFightCondLogic)reader.get_int("logic");
        int cfgLen = reader.get_int("skillCfgLen");
        for (int i=1; i <= cfgLen; i++)
        {
            AutoFightCond SkillCond;
            reader.enter_table(i);
            SkillCond.ctype = (AutoFightCondType)reader.get_int("condtype");
            SkillCond.hero = (AutoFightCheckHero)reader.get_int("checkhero");
            SkillCond.frdly = isFly;
            SkillCond.frame = reader.get_int("frame");
            
            if (SkillCond.ctype == AFCT_PID_RANGE)
            {
                SkillCond.pid_low = reader.get_int("pid_low");
                SkillCond.pid_high = reader.get_int("pid_high");
            }
            else
            {
                SkillCond.val = reader.get_int("val");
                SkillCond.val_pct = reader.get_int("val_pct");
            }
            reader.leave_table();
            condset.cond_vec.push_back(SkillCond);
        }
        int prior = reader.get_int("prior");
        reader.leave_table();
        
        sAIEngine.addAutoSpellCond(isFly, prior, fightHero, condset);
    }
    reader.leave_table();

}

void CombatCenter::init()
{
    sAIEngine.init(true);
    m_creatures.clear();
}

#include <chrono>

void CombatCenter::update()
{

//        auto start = std::chrono::system_clock::now();
        sAIEngine.update();
//        auto end = std::chrono::system_clock::now();
//        std::chrono::duration<double>  dur = end-start;
//
//        printf("The updateAI used %f ms by time().\n\n",dur.count()) ;
    
//        start = std::chrono::system_clock::now();
        updateCreature();
//        end =  std::chrono::system_clock::now();
//        dur = end-start;
//        printf("The updateLua used %f ms by time().\n\n",dur.count()) ;

}

void CombatCenter::start()
{
    sAIEngine.start();
}

//private
SpellPiece* CombatCenter::creatSpellPiece(LuaReader &reader)
{
    SpellPiece* piece = new SpellPiece(reader.get_int("m_skillId"));
    piece->spType = (SpellType)reader.get_int("m_SkillDamageType");
    
    /*~~~~~~~~~~~~~*选择(范围、指定单位)***************************/
    piece->selector.frdly = reader.get_int("m_SkillFrdly");
    int range = reader.get_int("m_SkillRange");
    piece->selector.range = (SpellRange)range;
    piece->selector.pos = (SpellPosition)reader.get_int("m_SkillPos");
    if (range == SR_CIRCLE || range == SR_NEAREST_HERO_CIRCLE)
    {
        piece->selector.data.circle.radius = reader.get_double("m_SkillRadius");
    }
    else if (range == SR_SECTOR || range == SR_NEAREST_HERO_SECTOR)
    {
        piece->selector.data.circle.radius = reader.get_double("m_SkillRadius");
        piece->selector.data.sector.angle = reader.get_double("m_SkillAngle") / 180 * PI;
        piece->selector.data.sector.back_len = reader.get_double("m_SkillBackdis");
    }
    else if (range == SR_RECTANGLE || range == SR_NEAREST_HERO_RECTANGLE)
    {
        piece->selector.data.rect.width = reader.get_double("m_SkillWidth");
        piece->selector.data.rect.height = reader.get_double("m_SkillHeight");
    }
    /*~~~~~~~~~~~~~~*效果（属性、状态、技能效果状态）**************/
    piece->effect.effect = (SpellEffect)reader.get_int("m_SkillEffect");
    piece->effect.spc = (SpellPropertyChoose)reader.get_int("m_SkillSpc");
    piece->effect.state = (SpellEffectState)reader.get_int("m_SkillState");
    piece->effect.val_pct = reader.get_double("m_SkillEffectVal_pct");
    piece->effect.val = reader.get_double("m_SkillBaseDamage");
    piece->effect.param1 = reader.get_double("m_param1");
    piece->effect.param2 = reader.get_double("m_param2");
    piece->effect.param3 = reader.get_double("m_param3");
    piece->effect.param4 = reader.get_double("m_param4");
    /****************条件（几率、属性值、属性百分比)*****************************/
    piece->condition.v_cond = (SpellValueCond)reader.get_int("m_SkillCond");
    piece->condition.vp_cond = (SpellValuePctCond)reader.get_int("m_SkillPCond");
    piece->condition.prob = reader.get_double("m_SkillProb");
    piece->condition.val = reader.get_double("m_SkillVal");
    piece->condition.val_pct = reader.get_double("m_Skillval_pct");
    /****************时间*****************************/
    piece->time.delay = reader.get_int("m_SkillDelay");
    piece->time.interval = reader.get_int("m_SkillInterval");
    piece->time.total_time = reader.get_int("m_SkillTotal_time");
    piece->time.delay_t = (SpellDelayType)reader.get_int("m_SkillDelayType");
    
    return piece;
}


void CombatCenter::updateCreature()
{
    for ( auto creatureItem : m_creatures )
    {
        CLuaFunctor functor(SHARE_LUA_STATE, "CombatMsgHandler");
        functor.PushParam(ON_CREATURE_UPDATE);
        
        //战斗编号
        functor.PushParam(creatureItem.second->getFightNumer());
        
        //坐标
        functor.PushParam(creatureItem.second->pos.x);
        functor.PushParam(creatureItem.second->pos.y);
        
        //方向
        functor.PushParam(creatureItem.second->dire.x);
        functor.PushParam(creatureItem.second->dire.y);
        
        //hp
        functor.PushParam(creatureItem.second->hp);
        
        functor.PushParam(creatureItem.second->hp_max);
        
        //attack
        functor.PushParam(creatureItem.second->atk);
        
        //faf
        functor.PushParam(creatureItem.second->faf);
        
        //fas
        functor.PushParam(creatureItem.second->fas);
        
        //状态
        auto state = creatureItem.second->getState();
        if (state == S_Idle)
        {
            functor.PushParam(S_IDLE);
        }
        else if (state == S_Victory)
        {
            functor.PushParam(S_VICTORY);
        }
        else if (state == S_Dying)
        {
            functor.PushParam(S_DYING);
        }
        else if (state == S_Move)
        {
            functor.PushParam(S_MOVE);
        }
        else if (state == S_Spelling)
        {
            functor.PushParam(S_SPELLING);
        }
        else if (state == S_Alive)
        {
            functor.PushParam(S_ALIVE);
        }
        else if (state == S_Attack)
        {
            functor.PushParam(S_ATTACK);
        }
        else if (state == S_Freezing)
        {
            functor.PushParam(S_FREEZING);
        }
        
        //技能状态
        auto creatrue = creatureItem.second;
        if( creatrue->frdly && creatrue->getSpell() != 0 )
        {
            
            float startCombatPercent = (float)(FriendStartBattleSpellCoolDown - FriendStartBattleCoolDown::getReadyFrame())*100/FriendStartBattleSpellCoolDown;
            functor.PushParam(startCombatPercent);
            
            float commonSkillPercent = (float)(CommonSpellCoolDown - FriendCommonCoolDown::getReadyFrame())*100/CommonSpellCoolDown;
            functor.PushParam(commonSkillPercent);
            
            auto spell = creatureItem.second->getSpell();
            float spellPercent = (float)(spell->getSpellCoolDown() - spell->getReadyFrame())*100/spell->getSpellCoolDown();
            functor.PushParam(spellPercent);
        }
        
        functor.Execute();
    }
}

const LuaCreature* CombatCenter::getCreatureOfFightId(int id)
{
    auto temp = m_creatures[id];
    return temp;
}

int CombatCenter::getAttackTarOfFightId(int id)
{
    auto tar = (LuaCreature*)m_creatures[id]->tar;
    
    if (tar) {
        return tar->getFightNumer();
    }
    return 0;
}












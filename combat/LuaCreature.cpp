//
//  LuaCreature.cpp
//  LuaSanGuo
//
//  Created by waiboziStudio on 15/6/2.
//
//

#include "LuaCreature.h"
#include "LuaFunctor.hpp"
#include "CCLuaEngine.h"
#include <iostream>

#include "CombatCenter.h"

LuaCreature::LuaCreature(CreatureType type)
:Creature(type),
m_fightNumber(-1)
{
}

void LuaCreature::setFightNumer(int fightNumber)
{
    m_fightNumber = fightNumber;
}

int LuaCreature::getFightNumer()
{
    return m_fightNumber;
}

void LuaCreature::onAttackStart(int level)
{
    CLuaFunctor functor(SHARE_LUA_STATE, "CombatMsgHandler");
    functor.PushParam(ON_ATTACK_START);
    functor.PushParam(m_fightNumber);
    functor.PushParam(level);
    functor.Execute();
}

void LuaCreature::onHurt(Creature *sender, float val, int level)
{
    CLuaFunctor functor(SHARE_LUA_STATE, "CombatMsgHandler");
    functor.PushParam(ON_HURT);
    functor.PushParam(m_fightNumber);
    
    //获取发送者战斗编号
    int senderID = ((LuaCreature*)sender)->getFightNumer();
    functor.PushParam(senderID);
    functor.PushParam(val);
    functor.PushParam(level);
    functor.Execute();
}

void LuaCreature::onSpellHpChanged(float val, int level)
{
    CLuaFunctor functor(SHARE_LUA_STATE, "CombatMsgHandler");
    functor.PushParam(ON_SPELL_HP_CHANGED);
    functor.PushParam(m_fightNumber);
    functor.PushParam(val);
    functor.PushParam(level);
    functor.Execute();

}

void LuaCreature::onDead()
{
    CLuaFunctor functor(SHARE_LUA_STATE, "CombatMsgHandler");
    functor.PushParam(ON_DEAD);
    functor.PushParam(m_fightNumber);
    functor.Execute();
}

void LuaCreature::onSpellTargetChanged()
{
    CLuaFunctor functor(SHARE_LUA_STATE, "CombatMsgHandler");
    functor.PushParam(ON_SPELL_TARGET_CHANGED);
    functor.PushParam(m_fightNumber);
    functor.Execute();
}

void LuaCreature::onEffectStart(int id, bool continuous, int msgId)
{
    CLuaFunctor functor(SHARE_LUA_STATE, "CombatMsgHandler");
    functor.PushParam(ON_EFFECT_START);
    functor.PushParam(m_fightNumber);
    functor.PushParam(id);
    functor.PushParam(continuous);
    functor.PushParam(msgId);
    functor.Execute();
}

void LuaCreature::onEffectEnd(int id, int msgId)
{
    CLuaFunctor functor(SHARE_LUA_STATE, "CombatMsgHandler");
    functor.PushParam(ON_EFFECT_END);
    functor.PushParam(m_fightNumber);
    functor.PushParam(id);
    functor.PushParam(msgId);
    functor.Execute();
}















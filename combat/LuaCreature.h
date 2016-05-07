//
//  LuaCreature.h
//  LuaSanGuo
//
//  Created by waiboziStudio on 15/6/2.
//
//

#ifndef __LuaSanGuo__LuaCreature__
#define __LuaSanGuo__LuaCreature__

#include <stdio.h>
#include "Creature.h"

#define SHARE_LUA_STATE cocos2d::LuaEngine::getInstance()->getLuaStack()->getLuaState()

enum CreatureState
{
    S_NONE = 1,
    S_ALIVE,
    S_ATTACK,
    S_FREEZING,
    S_IDLE,
    S_VICTORY,
    S_DYING,
    S_MOVE,
    S_SPELLING
};

class LuaCreature : public Creature
{
    
public:
    LuaCreature(CreatureType type);
    virtual ~LuaCreature(){};
    
public:
    virtual void onAttackStart(int level);
    
    virtual void onHurt(Creature *sender, float val, int level);
    
    virtual void onSpellHpChanged(float val, int level);
    
    virtual void onDead();
    
    virtual void onSpellTargetChanged();
    
    virtual void onEffectStart(int id, bool continuous, int msgId);
    
    virtual void onEffectEnd(int id, int msgId);
    
public:
    void  setFightNumer(int fightNumber);
    int   getFightNumer();
    
private:
    int   m_fightNumber;
    
};

#endif /* defined(__LuaSanGuo__LuaCreature__) */

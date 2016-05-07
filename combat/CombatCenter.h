//
//  CombatCenter.h
//  LuaSanGuo
//
//  Created by waiboziStudio on 15/6/2.
//
//

#ifndef __LuaSanGuo__CombatCenter__
#define __LuaSanGuo__CombatCenter__

#include <stdio.h>
#include "LuaCreature.h"
#include "LuaReader.h"

enum LuaCallBackType
{
    ON_ATTACK_START = 1,
    ON_HURT,
    ON_SPELL_HP_CHANGED,
    ON_DEAD,
    ON_SPELL_TARGET_CHANGED,
    ON_EFFECT_START,
    ON_EFFECT_END,
    ON_GAMEOVER,
    ON_CREATURE_UPDATE,
};


class CombatCenter
{
    
public:
    static CombatCenter* getInstance();
    
    void addLuaCreature(LuaReader& reader);
    void addAutoFightCon(LuaReader& reader);
    
    void init();
    void start();
    void update();
    
    const LuaCreature * getCreatureOfFightId(int id);
    int getAttackTarOfFightId(int id);
    
private:
    CombatCenter(){};
    virtual ~CombatCenter(){};
    
private:
    SpellPiece* creatSpellPiece(LuaReader& reader);
    void updateCreature();

private:
     std::map<int, LuaCreature *>   m_creatures; // fightNumber, Creature *
};

#endif /* defined(__LuaSanGuo__CombatCenter__) */

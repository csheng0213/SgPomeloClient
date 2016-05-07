#include <functional>

#include "AutoFightSys.h"

#include "CreatureManager.h"
#include "Creature.h"
#include "AIEngine.h"
#include "AIConst.h"

static void raiseAutoSpellEventProc     (Creature *c);
static bool singleConditionProc(Creature *c, AutoFightCond &cond);
static bool multiConditionProc (Creature *c, AutoFightCondSet &condset);

AutoFightSys::AutoFightSys()
{
}

void AutoFightSys::update(bool frdly)
{
    if (frdly)
    {
        heroConditionCheck(sCrtMgr.getMainHero(true), _smhero)
        || heroConditionCheck(sCrtMgr.getViceHero1(true), _svhero1)
        || heroConditionCheck(sCrtMgr.getViceHero2(true), _svhero2);
    }
    else
    {
        heroConditionCheck(sCrtMgr.getMainHero(false), _emhero)
        || heroConditionCheck(sCrtMgr.getViceHero1(false), _evhero1)
        || heroConditionCheck(sCrtMgr.getViceHero2(false), _evhero2);
    }
}

void AutoFightSys::clear()
{
    _smhero.clear();
    _svhero1.clear();
    _svhero2.clear();

    _emhero.clear();
    _evhero1.clear();
    _evhero2.clear();
}

void AutoFightSys::addAutoSpellCond(bool frdly, int prior, AutoFightHero hero, AutoFightCondSet condset)
{
    switch(hero)
    {
    case AFH_MAIN_HERO:
        if (frdly)
            _smhero.insert(std::make_pair(prior, std::move(condset)));
        else
            _emhero.insert (std::make_pair(prior, std::move(condset)));
        break;
    case AFH_VICE_HERO1:
        if (frdly)
            _svhero1.insert(std::make_pair(prior, std::move(condset)));
        else
            _evhero1.insert(std::make_pair(prior, std::move(condset)));
        break;
    case AFH_VICE_HERO2:
        if (frdly)
            _svhero2.insert(std::make_pair(prior, std::move(condset)));
        else
            _evhero2.insert(std::make_pair(prior, std::move(condset)));
        break;
    }
}

bool AutoFightSys::heroConditionCheck(Creature *c, CondSetMultiMap &condmap)
{
    if (!c || !c->isSpellReady() || c->isState(S_Freezing))
        return false;

    for (auto &i : condmap)
    {
        auto &condset   = i.second;
        auto &triggered = condset.triggered;

        if (triggered)
            continue;

        if (multiConditionProc(c, condset))
        {
            raiseAutoSpellEventProc(c);
            triggered = true;
            return true;
        }
    }

    return false;
}

static void raiseAutoSpellEventProc(Creature *c)
{
    AIEvent evt;
    evt.raiser = c->getId();
    evt.etype  = EVT_AUTO_SPELL;
    sAIEngine.raiseEvent(evt);
}

static bool singleConditionProc(Creature *c, AutoFightCond &cond)
{
    switch (cond.ctype)
    {
    case AFCT_PID_RANGE:
        if (c->pid >= cond.pid_low && c->pid <= cond.pid_high)
            return true;
        break;
    case AFCT_FRAME_LEAST:
        if (sAIEngine.getCurrentFrame() > cond.frame)
            return true;
        break;
    case AFCT_FRAME_MOST:
        if (sAIEngine.getCurrentFrame() < cond.frame)
            return true;
        break;
    case AFCT_HP_LEAST:
        if (c->hp > c->raw_hp * cond.val_pct + cond.val)
            return true;
        break;
    case AFCT_HP_MOST:
        if (c->hp < c->raw_hp * cond.val_pct + cond.val)
            return true;
        break;
    case AFCT_HP_MAX_LEAST:
        if (c->hp_max > c->raw_hp_max * cond.val_pct + cond.val)
            return true;
        break;
    case AFCT_HP_MAX_MOST:
        if (c->hp_max < c->raw_hp_max * cond.val_pct + cond.val)
            return true;
        break;
    case AFCT_MP_LEAST:
        if (c->mp > c->raw_mp * cond.val_pct + cond.val)
            return true;
        break;
    case AFCT_MP_MOST:
        if (c->mp < c->raw_mp * cond.val_pct + cond.val)
            return true;
        break;
    case AFCT_MP_MAX_LEAST:
        if (c->mp_max > c->raw_mp_max * cond.val_pct + cond.val)
            return true;
        break;
    case AFCT_MP_MAX_MOST:
        if (c->mp_max < c->raw_mp_max * cond.val_pct + cond.val)
            return true;
        break;
    case AFCT_ATK_LEAST:
        if (c->atk > c->raw_atk * cond.val_pct + cond.val)
            return true;
        break;
    case AFCT_ATK_MOST:
        if (c->atk < c->raw_atk * cond.val_pct + cond.val)
            return true;
        break;
    case AFCT_ATK_RANGE_LEAST:
        if (c->atk_range > c->raw_atk_range * cond.val_pct + cond.val)
            return true;
        break;
    case AFCT_ATK_RANGE_MOST:
        if (c->atk_range < c->raw_atk_range * cond.val_pct + cond.val)
            return true;
        break;
    case AFCT_ATK_SPD_LEAST:
        if (c->atk_spd > c->raw_atk_spd * cond.val_pct + cond.val)
            return true;
        break;
    case AFCT_ATK_SPD_MOST:
        if (c->atk_spd < c->raw_atk_spd * cond.val_pct + cond.val)
            return true;
        break;
    case AFCT_MV_SPD_LEAST:
        if (c->mv_spd > c->raw_mv_spd * cond.val_pct + cond.val)
            return true;
        break;
    case AFCT_MV_SPD_MOST:
        if (c->mv_spd < c->raw_mv_spd * cond.val_pct + cond.val)
            return true;
        break;
    }
    return false;
}

static bool multiConditionProc(Creature *c, AutoFightCondSet &condset)
{
    std::vector<Creature *> tar_vec;
    for (auto &cond : condset.cond_vec)
	{
		if (!cond.hero)
			tar_vec.push_back(c);
        if (cond.hero & 0x01)
        {
            auto h = sCrtMgr.getMainHero(cond.frdly);
            if (h)
                tar_vec.push_back(h);
        }
        if (cond.hero & 0x02)
        {
            auto h = sCrtMgr.getViceHero1(cond.frdly);
            if (h)
                tar_vec.push_back(h);
        }

        if (cond.hero & 0x04)
        {
            auto h = sCrtMgr.getViceHero1(cond.frdly);
            if (h)
                tar_vec.push_back(h);
        }

        if (tar_vec.empty())
            continue;

        if (condset.logic == AFCL_ALL)
        {
            for (auto tar : tar_vec)
            {
                if (!singleConditionProc(tar, cond))
                    return false;
            }
        }
        else if (condset.logic == AFCL_ANY)
        {
            bool tag = true;
            for (auto tar : tar_vec)
            {
                if (!singleConditionProc(tar, cond))
                {
                    tag = false;
                    break;
                }
            }

            if (tag)
                return true;
        }

		tar_vec.clear();
    }

    if (condset.logic == AFCL_ALL)
        return true;

    return false;
}

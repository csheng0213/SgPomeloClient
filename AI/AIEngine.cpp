#include <algorithm>

#include "AIEngine.h"

#include "AIConst.h"
#include "Message.h"
#include "MessageDispatcher.h"
#include "MessageHelper.h"

void AIEngine::init(bool delCreature)
{
    Message::resetCounter();

    if (delCreature)
        sCrtMgr.deleteCreature();
    sCrtMgr.clear();

    _frame          = 0;
    _evt_id_creator = 1;
    _pause      = false;
    _finished   = false;
    _reload     = false;
    _enemy_auto_fight = false;
    _self_auto_fight  = false;

    _evt_seq.clear();

    sAutoFightSys.clear();

	sMsgDisp.clearDelayMessage();
	sMsgDisp.clearMessage();

    delete _spl_c;
    _spl_c = new Creature(CT_BEGIN);

    //FriendCommonCoolDown::resetCoolDown();
    //EnemyCommonCoolDown::resetCoolDown();
    FriendStartBattleCoolDown::resetCoolDown();
    EnemyStartBattleCoolDown::resetCoolDown();
}

void AIEngine::start()
{
    auto fmc = sCrtMgr.getMainHero(true);
    auto emc = sCrtMgr.getMainHero(false);

//    if (!fmc || !emc)
//        return;
    
    Creature *c = nullptr;

    sCrtMgr.pushIter();
    while(c = sCrtMgr.getNext())
    {
        sMsgDisp.sendMessage(sMsgH.charge(c));
        c->changeState(S_Move);

        auto &pspl = c->getPassiveSpells();
        for (auto &i : pspl)
            i->castSpell(nullptr, Vector2D());
    }
    sCrtMgr.popIter();

    auto &pspl = _spl_c->getPassiveSpells();
    for (auto &i : pspl)
        i->castSpell(nullptr, Vector2D());

    _evt_seq.sort([](AIEvent const &e1, AIEvent const &e2){ return e1.id < e2.id; });
    
    sMsgDisp.update();
    sMsgDisp.update();
    
    auto ft_hp = 0.0f;
    auto ft_mp = 0.0f;
    auto et_hp = 0.0f;
    auto et_mp = 0.0f;
    sCrtMgr.pushIter();
    while(c = sCrtMgr.getNext())
    {
		if (c->isSoldier())
			continue;

        if (c->frdly)
        {
            ft_hp += c->hp_max;
            ft_mp += c->mp_max;
        }
        else
        {
            et_hp += c->hp_max;
            et_mp += c->mp_max;
        }
    }
    sCrtMgr.popIter();

    sCrtMgr.setTotalHp(ft_hp, true );
    sCrtMgr.setTotalHp(et_hp, false);
    sCrtMgr.setTotalMp(ft_mp, true );
    sCrtMgr.setTotalMp(et_mp, false);
}

void AIEngine::end()
{

}

void AIEngine::update()
{
    if (_pause || _finished)
        return;

    sMsgDisp.update();

    if (_self_auto_fight)
        sAutoFightSys.update(true);
    
    if (_enemy_auto_fight)
        sAutoFightSys.update(false);

    if (_frame == TimeLimit)
    {
        auto cv = sCrtMgr.getAllHeros(CreatureManager::S_ALL);
        if (cv->size() != 0)
        {
            AIEvent evt;
            evt.etype = EVT_TIME_OUT;
            evt.raiser = (*cv->begin())->getId();
            raiseEvent(evt);
        }
    }
    else if (_reload && _evt_seq.size())
    {
        auto it = _evt_seq.begin();
        while (true)
        {
            it = std::find_if(it, _evt_seq.end(),
                             [this](AIEvent const &evt) { 
                                 return evt.frame == _frame;
                             }
                             );
            if (it != _evt_seq.end())
            {
                raiseEvent(*it);
                _evt_seq.erase(it++);
            }
            else
            {
                break;
            }
        }
	}

    _frame++;

    FriendCommonCoolDown::reduceCoolDown();
    EnemyCommonCoolDown::reduceCoolDown();
    FriendStartBattleCoolDown::reduceCoolDown();
    EnemyStartBattleCoolDown::reduceCoolDown();
    sCrtMgr.update();
}

void AIEngine::setReload(bool reload /*= true*/)
{
    _reload = reload;
}

void AIEngine::forceShutDown()
{
    //sMsgDisp.clearDelayMessage();
    //sMsgDisp.clearMessage();
    _finished = true;
}

void AIEngine::raiseEvent(AIEvent evt)
{
    if (_finished)
        return;

    auto c = sCrtMgr.getTarget(evt.raiser);

    if (!c)
        c = _spl_c;

    evt.id = _evt_id_creator++;
    evt.frame = _frame;
    auto tar = sCrtMgr.getTarget(evt.tar);

    switch (evt.etype)
    {
    case EVT_TIME_OUT:
        {
            sMsgDisp.sendDelayMessage(sMsgH.timeOut(c));
            //setFinished(true);
        }
        break;
    case EVT_HERO_ALL_DIE:
        {
            sMsgDisp.sendDelayMessage(sMsgH.heroAllDie(c));
            //setFinished(true);
        }
        break;
    case EVT_SPELL:
        {
            if (!c->isSpellReady())
                break;

			if (!_reload)
				_evt_seq.push_back(evt);
		} // go through
	case EVT_AUTO_SPELL:
		{
            if (!c->isSpellReady())
                break;

			sMsgDisp.clearMessage(c, MSG_ATTACK);
			sMsgDisp.clearMessage(c, MSG_ATTACK_END);
			sMsgDisp.clearMessage(c, MSG_CHARGE);
			sMsgDisp.clearMessage(c, MSG_MOVE_TARGET);

			sMsgH.spell(c, tar, evt.tar_pos);

            c->getSpell()->resetCoolDown();
            if (c->frdly)
                FriendCommonCoolDown::resetCoolDown();
            else
                EnemyCommonCoolDown::resetCoolDown();
		}
		break;
    }
}

void AIEngine::addEvent(const AIEvent &evt)
{
    if (_reload && evt.id != 0)
        _evt_seq.push_back(evt);
}

void AIEngine::setGlobalSpell(Spell *spl)
{
    _spl_c->setSpell(spl);
}

Spell * AIEngine::getGlobalSpell()
{
    return _spl_c->getSpell();
}

void AIEngine::addGlobalPassiveSpell(Spell *spl)
{
    _spl_c->addPassiveSpell(spl);
}

std::vector<Spell *> & AIEngine::getGlobalPassiveSpell()
{
    return _spl_c->getPassiveSpells();
}

void AIEngine::raiseGlobalSpell()
{
    AIEvent evt;
    evt.raiser = _spl_c->getId();
    evt.etype  = EVT_SPELL;
    raiseEvent(evt);
}

void AIEngine::addAutoSpellCond(bool frdly, int prior, AutoFightHero hero, AutoFightCondSet condset)
{
    sAutoFightSys.addAutoSpellCond(frdly, prior, hero, std::move(condset));
}

void AIEngine::setEnemyAutoFight(bool f /*= true*/)
{
    _enemy_auto_fight = f;
}

void AIEngine::setSelfAutoFight(bool f /*= true*/)
{
    _self_auto_fight = f;
}

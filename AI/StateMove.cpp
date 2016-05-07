#include "StateInclude.h"

#include "AIConst.h"
#include "AIEngine.h"
#include "Creature.h"
#include "CreatureManager.h"
#include "Message.h"
#include "MessageHelper.h"

//#include <chrono>
//#include "cocos2d.h"
//using namespace std;

static bool isTrafficJam(Creature *c, Message *msg);

void StateMove::onEnter(Creature *c)
{
    c->recent_pos.clear();
}

void StateMove::onExit(Creature *c)
{

}

void StateMove::handleMessage(Creature *c, Message *msg)
{
    //auto start = chrono::system_clock::now();

    if (!c)
        return;

    if (c->tar && isTrafficJam(c, msg))
    {
        c->changeState(S_Idle);
        sMsgDisp.sendMessage(sMsgH.charge(c, IdleToMoveTime));
        return;
    }

    switch(msg->msg_t)
    {
    case MSG_CHARGE:
        {
            auto t = sCrtMgr.getAttackableTarget(c, c->charged);

            if (!t)
            {
                if (c->charged)
                {
                    if (sAIEngine.isFinished())
                        c->changeState(S_Victory);
                    else
                    {
                        t = sCrtMgr.getNearestTarget(c, c->frdly ? CreatureManager::S_ENEMY : CreatureManager::S_FRIEND);
                        c->tar = t;
                        sMsgDisp.sendMessage(sMsgH.moveTarget(c, t));
                    }
                }
                else
                {
                    auto hero = sCrtMgr.getMainHero(!c->frdly); // 包含主英雄不在的情况
                    if ( hero && hero->isAlive()
                         && (  (c->frdly  && c->pos.x < hero->pos.x)
                            || (!c->frdly && c->pos.x > hero->pos.x) ) )
                    {
                        sMsgDisp.sendMessage(sMsgH.charge(c));
                    }
                    else
                    {
                        c->charged = true;
                        t = sCrtMgr.getNearestTarget(c, c->frdly ? CreatureManager::S_ENEMY : CreatureManager::S_FRIEND);

                        if (t)
                        {
                            c->tar = t;
                            sMsgDisp.sendMessage(sMsgH.moveTarget(c, t));
                        }
                        else
                        {
                            if (sAIEngine.isFinished())
                                c->changeState(S_Victory);
                        }
                    }
                }
            }
            else
            {
                c->tar = t;
                sMsgDisp.sendMessage(sMsgH.moveTarget(c, t));
            }
        }
        break;
    case MSG_MOVE_TARGET:
        {
            auto t = msg->tar;

            if (!t || !t->isAlive())
            {
                c->tar = 0;
                sMsgDisp.sendMessage(sMsgH.charge(c));
            }
            else if ((t->pos - c->pos).lenSq() < c->atk_range * c->atk_range)
            {
                c->tar = t;
                sMsgDisp.sendMessage(sMsgH.attack(c, t));
                c->changeState(S_Attack);
            }
            else
            {
                Creature *tar = sCrtMgr.getAttackableTarget(c, c->charged);
                if (!tar)
                    tar = t;
                sMsgDisp.sendMessage(sMsgH.moveTarget(c, tar));
            }
        }
        break;
    }
    //auto handle_move_state_end = chrono::system_clock::now();
    //CCLOG("handle_move_state_end cost time: %ldms", chrono::duration_cast<chrono::milliseconds>(handle_move_state_end-start).count());
}

static bool isTrafficJam(Creature *c, Message *msg)
{
    if (c->recent_pos.size() < HistoryPosCheck)
        return false;
    
    auto spd = c->mv_spd / 60;
    auto prev = c->recent_pos.front();
    for (auto &i : c->recent_pos)
    {
        if (prev.distSq(i) > spd * spd * TraficJamParam)
            return false;
    }

    return true;
}
#include "StateInclude.h"

#include "AIConst.h"
#include "AIFactor.h"
#include "Creature.h"
#include "CreatureManager.h"
#include "Message.h"
#include "MessageHelper.h"

void StateAttack::handleMessage(Creature *c, Message *msg)
{
    if (!c)
        return;

    switch(msg->msg_t)
    {
    case MSG_ATTACK:
        {
            auto t = msg->tar;
            if (!t || !t->isAlive())
            {
                c->tar = nullptr;
                sMsgDisp.sendMessage(sMsgH.charge(c));
                c->changeState(S_Move);
            }
            else if ((t->pos - c->pos).lenSq() >= c->atk_range * c->atk_range)
            {
                sMsgDisp.sendMessage((sMsgH.moveTarget(c, t)));
                c->changeState(S_Move);
            }
            else
            {
                c->dire = (t->pos - c->pos).getNorm();
				c->tar_pos = t->pos;

                auto baoj = c->isBaoji();
				c->onAttackStart(baoj);

				sMsgDisp.sendMessage(sMsgH.attackEnd(c, t, baoj, int(c->atk_spd * 60)));
            }
        }
        break;
    case MSG_ATTACK_END:
        {
            auto t = msg->tar;
            if (!t || !t->isAlive())
            {
                c->tar = 0;
                sMsgDisp.sendMessage(sMsgH.charge(c));
                c->changeState(S_Move);
            }
            else
            {
                sMsgDisp.sendMessage(sMsgH.hurt(c, t, t->onDefence(c, c->onAttack(t, msg->isBaoji)), msg->isBaoji));

                sMsgDisp.sendMessage(sMsgH.attack(c, t));
            }
        }
        break;
    }
}

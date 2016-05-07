#include "MessageHelper.h"

#include "Creature.h"
#include "CreatureManager.h"
#include "Spell.h"

Message * MessageHelper::charge(Creature *c, int delay)
{
    auto msg = new Message();
    msg->msg_t  = MSG_CHARGE;
    msg->sender = c;
    msg->addReceiver(c);
    msg->delay = delay;
    return msg;
}

Message * MessageHelper::attack(Creature *c, Creature *tar)
{
    auto msg = new Message();
    msg->msg_t  = MSG_ATTACK;
    msg->sender = c;
    msg->addReceiver(c);
    msg->tar = tar;
    return msg;
}

Message * MessageHelper::attackEnd(Creature *c, Creature *tar, float baoj, int delay)
{
    if (delay <= 0)
        delay = 1;
	auto msg = new Message();
	msg->msg_t = MSG_ATTACK_END;
    msg->isBaoji = baoj;
    msg->delay = delay;
    msg->sender = c;
    msg->addReceiver(c);
    msg->tar = tar;
    return msg;
}

Message * MessageHelper::moveTarget(Creature *c, Creature *tar, int delay)
{
    auto msg = new Message();
    msg->msg_t  = MSG_MOVE_TARGET;
    msg->sender = c;
    msg->addReceiver(c);
    msg->tar = tar;
    msg->delay = delay;
    return msg;
}

Message * MessageHelper::hurt(Creature *c, Creature *tar, float hurtValue, float baoj)
{
	auto msg = new Message();
	msg->msg_t = MSG_HURT;
	msg->isBaoji = baoj;
    msg->sender = c;
    msg->addReceiver(tar);
    msg->val = hurtValue;
    return msg;
}

Message * MessageHelper::heroAllDie(Creature *c)
{
    auto msg = new Message();
    auto &cm = sCrtMgr.getCreatureMap();
    for (auto &i : cm)
        if (i.second->isAlive())
            msg->addReceiver(i.second);
    msg->msg_t  = MSG_EVT_HERO_ALL_DIE;
    msg->sender = c;
    return msg;
}

Message * MessageHelper::timeOut(Creature *c)
{
    auto msg = new Message();
    auto &cm = sCrtMgr.getCreatureMap();
    for (auto &i : cm)
        if (i.second->isAlive())
            msg->addReceiver(i.second);
    msg->msg_t  = MSG_EVT_TIME_OUT;
    msg->sender = c;
    return msg;
}

Message * MessageHelper::DeleteMessage(Creature *c, MessageType type)
{
    auto msg = new Message();
    msg->msg_t = MSG_CLEAR_MSG;
    msg->sender = c;
    msg->tmsg_t = type;
    return msg;
}

void MessageHelper::spell(Creature *c, Creature *tar, Vector2D tar_pos)
{
    auto spl = c->getSpell();
    spl->updateValue();
    spl->castSpell(tar, tar_pos);
}

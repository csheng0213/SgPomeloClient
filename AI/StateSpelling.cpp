#include "StateInclude.h"

#include "Creature.h"
#include "CreatureManager.h"
#include "Message.h"
#include "MessageHelper.h"

void StateSpelling::handleMessage(Creature *c, Message *msg)
{
    if (!c)
        return;

    switch(msg->msg_t)
    {
    case MSG_SPELL_END:
        //c->removeSpellTarget();
        c->tar = 0;
        sMsgDisp.sendMessage(sMsgH.charge(c));
        c->changeState(S_Move);
        break;
    }
}

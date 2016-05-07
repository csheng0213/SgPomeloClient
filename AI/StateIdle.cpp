#include "StateInclude.h"

#include "Creature.h"
#include "CreatureManager.h"
#include "MessageHelper.h"
#include "MessageDispatcher.h"

void StateIdle::handleMessage(Creature *c, Message *msg)
{
    if (!c)
        return;

    switch (msg->msg_t)
    {
        case MSG_CHARGE:
            c->changeState(S_Move);
            sMsgDisp.sendMessage(sMsgH.charge(c));
            break;
    }
}

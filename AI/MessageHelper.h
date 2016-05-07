#ifndef _MESSAGE_HELPER_H_
#define _MESSAGE_HELPER_H_

#include "Vector2D.h"
#include "Message.h"

class Creature;

class MessageHelper
{
    MessageHelper() {}

public:
	static MessageHelper & getInstance()
	{
		static MessageHelper s;
		return s;
	}
    
    Message * charge(Creature *c, int delay = 1);
    Message * attack(Creature *c, Creature *tar);
    Message * attackEnd(Creature *c, Creature *tar, float factor, int delay);
    Message * moveTarget(Creature *c, Creature *tar, int delay = 1);
    Message * hurt(Creature *c, Creature *tar, float hurtValue, float factor);
              
    Message * heroAllDie(Creature *c);
    Message * timeOut(Creature *c);
    Message * DeleteMessage(Creature *c, MessageType type);

    void spell(Creature *c, Creature *tar, Vector2D tar_pos);
};

#define sMsgH (MessageHelper::getInstance())

#endif // _MESSAGE_HELPER_H_
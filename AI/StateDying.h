#ifndef _STATE_DYING_H_
#define _STATE_DYING_H_

#include "State.h"

class StateDying : public State
{
public:
    virtual void handleMessage(Creature *c, Message *msg) override;

	static StateDying * getInstance()
	{
		static StateDying s;
		return &s;
	}
};

#define S_Dying (StateDying::getInstance())

#endif // _STATE_DYING_H_
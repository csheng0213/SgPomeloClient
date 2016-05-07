#ifndef _STATE_ALIVE_H_
#define _STATE_ALIVE_H_

#include "State.h"

class StateAlive : public State
{
public:
    virtual void handleMessage(Creature *c, Message *msg) override;

	static StateAlive * getInstance()
	{
		static StateAlive s;
		return &s;
	}
};

#define S_Alive (StateAlive::getInstance())

#endif // _STATE_ALIVE_H_
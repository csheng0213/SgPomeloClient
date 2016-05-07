#ifndef _STATE_IDLE_H_
#define _STATE_IDLE_H_

#include "State.h"

class StateIdle : public State
{
public:
    virtual void handleMessage(Creature *c, Message *msg) override;

	static StateIdle * getInstance()
	{
		static StateIdle s;
		return &s;
	}
};

#define S_Idle (StateIdle::getInstance())

#endif // _STATE_IDLE_H_
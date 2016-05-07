#ifndef _STATE_MOVE_H_
#define _STATE_MOVE_H_

#include "State.h"

class StateMove : public State
{
public:
    virtual void onEnter(Creature *c) override;

    virtual void onExit(Creature *c) override;

    virtual void handleMessage(Creature *c, Message *msg) override;

	static StateMove * getInstance()
	{
		static StateMove s;
		return &s;
	}
};

#define S_Move (StateMove::getInstance())

#endif // _STATE_MOVE_H_
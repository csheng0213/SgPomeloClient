#ifndef _STATE_VICTORY_H_
#define _STATE_VICTORY_H_

#include "State.h"

class StateVictory : public State
{
public:
    virtual void handleMessage(Creature *c, Message *msg) override;

	static StateVictory * getInstance()
	{
		static StateVictory s;
		return &s;
	}
};

#define S_Victory (StateVictory::getInstance())

#endif // _STATE_VICTORY_H_
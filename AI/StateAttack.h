#ifndef _STATE_ATTACK_H_
#define _STATE_ATTACK_H_

#include "State.h"

class StateAttack : public State
{
public:
    virtual void handleMessage(Creature *c, Message *msg) override;

	static StateAttack * getInstance()
	{
		static StateAttack s;
		return &s;
	}
};

#define S_Attack (StateAttack::getInstance())

#endif // _STATE_ATTACK_H_
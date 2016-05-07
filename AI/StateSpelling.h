#ifndef _STATE_SPELLING_H_
#define _STATE_SPELLING_H_

#include "State.h"

class StateSpelling : public State
{
public:
    virtual void handleMessage(Creature *c, Message *msg) override;

	static StateSpelling * getInstance()
	{
		static StateSpelling s;
		return &s;
	}
};

#define S_Spelling (StateSpelling::getInstance())

#endif // _STATE_SPELLING_H_
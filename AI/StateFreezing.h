#ifndef _STATE_FREEZING_H_
#define _STATE_FREEZING_H_

#include "State.h"

class StateFreezing : public State
{
public:
    virtual void handleMessage(Creature *c, Message *msg) override;

	static StateFreezing * getInstance()
	{
		static StateFreezing s;
		return &s;
	}
};

#define S_Freezing (StateFreezing::getInstance())

#endif // _STATE_FREEZING_H_
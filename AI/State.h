#ifndef _STATE_H_
#define _STATE_H_

class Creature;
class Message;

class State
{
public:
    virtual void onEnter(Creature *c) {}

    virtual void onExit(Creature *c) {}

    virtual void handleMessage(Creature *c, Message *msg) = 0;

	virtual ~State() {}
};

#endif // _STATE_H_
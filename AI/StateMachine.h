#ifndef _STATE_MACHINE_H_
#define _STATE_MACHINE_H_

#include "StateInclude.h"
#include <stack>

class Creature;
class Message;

class StateMachine
{
public:
	StateMachine(Creature *owner)
		: _owner(owner)
        , _gstate(S_Alive)
        , _state(S_Idle)
	{
    }

    void handleMessage(Message *msg) const
    {
        if (_gstate == S_Alive)
            _gstate->handleMessage(_owner, msg);
        if (_gstate == S_Alive)
            _state->handleMessage(_owner, msg);
    }
    
    void changeState(State *state)
    {
        _state->onExit(_owner);
        _state = state;
        _state->onEnter(_owner);
    }

	State * getState() const
	{
        return _state;
	}

	bool isState(const State *state) const
	{
		return _state == state;
	}

    State * getGlobalState() const
    {
        return _gstate;
    }

    void changeGlobalState(State *state)
    {
        _gstate->onExit(_owner);
        _gstate = state;
        _gstate->onEnter(_owner);
    }

    bool isGlobalState(const State *state) const
    {
        return _gstate == state;
    }

private:
	Creature			   *_owner;
    State                  *_gstate;    // global state
	State                  *_state;
};

#endif // _STATE_MACHINE_H_
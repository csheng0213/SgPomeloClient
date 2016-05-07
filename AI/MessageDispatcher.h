#ifndef _MESSAGE_DISPATCHER_H_
#define _MESSAGE_DISPATCHER_H_

#include <map>

#include "Message.h"

class Message;
class Creature;

class MessageDispatcher
{
public:
	MessageDispatcher()
	{}

	static MessageDispatcher & getInstance()
	{
		static MessageDispatcher msgd;
		return msgd;
	}

	void update();

	void sendMessage(Message *msg);

    void sendDelayMessage(Message *msg);

    void handleDelayMessage(Message *msg);

    void clearDelayMessage();
    
    void clearMessage();
    
    void clearMessage(Creature *c);

    void clearMessage(Creature *c, MessageType msg_t);

private:
	std::map<int, Message *>	 _mmap;     // id, Message * | Message Map
	std::map<int, Message *>	 _tmmap;    // tmp message map
    std::map<int, Message *>     _dlymmap;  // event message map
};

#define sMsgDisp (MessageDispatcher::getInstance())

#endif // _MESSAGE_DISPATCHER_H_
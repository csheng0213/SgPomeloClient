#include <functional>
#include <vector>

#include "MessageDispatcher.h"

#include "MessageHelper.h"
#include "Creature.h"

//#include <chrono>
//#include "cocos2d.h"
//using namespace std;

void MessageDispatcher::update()
{
    //CCLOG("_evtmmap: %d", _evtmmap.size());
    //CCLOG("_mmap: %d", _mmap.size());
    //CCLOG("_tmmap: %d", _tmmap.size());
    
    for (auto &i : _dlymmap)
        handleDelayMessage(i.second);
    _dlymmap.clear();

    _mmap.insert(_tmmap.begin(), _tmmap.end());
    _tmmap.clear();

    Message          *msg;
    std::vector<int>  to_remove;
    bool              willDel;
    
    //auto start = chrono::system_clock::now();
	for (auto &it : _mmap)
	{
        msg = it.second;
		msg->delay -= 1;

        if (msg->delay != 0)
            continue;

        if ((msg->total_time -= msg->interval) < 0)
        {
            willDel = true;
            to_remove.push_back(it.first);
        }
        else
        {
            willDel = false;
            msg->delay = msg->interval;
        }

        for (auto &i : msg->receiver)
        {
            if (i.first)
                i.first->handleMessage(msg);
        }

        if (willDel)
        {
            delete msg;
            it.second = 0;
        }
	}
    //auto handle_disp_msg_end = chrono::system_clock::now();
    //CCLOG("handle_disp_msg_end cost time: %ldms", chrono::duration_cast<chrono::milliseconds>(handle_disp_msg_end-start).count());

    for (auto it : to_remove)
        _mmap.erase(it);
}

void MessageDispatcher::sendMessage(Message *msg)
{
    if (msg)
	    _tmmap.insert(std::make_pair(msg->getId(), msg));
}

void MessageDispatcher::sendDelayMessage(Message *msg)
{
    if (msg)
	    _dlymmap.insert(std::make_pair(msg->getId(), msg));
}

void MessageDispatcher::clearDelayMessage()
{
    for (auto &it : _dlymmap)
        delete it.second;
    _dlymmap.clear();
}

void MessageDispatcher::clearMessage()
{
    for (auto &it : _tmmap)
        delete it.second;

    _tmmap.clear();

    for (auto &it : _mmap)
        delete it.second;

    _mmap.clear();
}

void MessageDispatcher::clearMessage(Creature *c)
{
    std::vector<int> to_remove;

    for (auto &it : _tmmap)
    {
        if (it.second->sender == c)
            delete it.second;
        to_remove.push_back(it.first);
    }

    for (auto it : to_remove)
        _tmmap.erase(it);

    to_remove.clear();

    for (auto &it : _mmap)
    {
        if (it.second->sender == c)
            delete it.second;
        to_remove.push_back(it.first);
    }

    for (auto it : to_remove)
        _mmap.erase(it);
}

void MessageDispatcher::clearMessage(Creature *c, MessageType msg_t)
{
    std::vector<int> to_remove;

    for (auto &it : _tmmap)
    {
        if (it.second->sender == c && it.second->msg_t == msg_t)
        {
            delete it.second;
            to_remove.push_back(it.first);
        }
    }

    for (auto it : to_remove)
        _tmmap.erase(it);

    to_remove.clear();

    for (auto &it : _mmap)
    {
        if (it.second->sender == c && it.second->msg_t == msg_t)
        {
            delete it.second;
            to_remove.push_back(it.first);
        }
    }

    for (auto it : to_remove)
        _mmap.erase(it);
}

void MessageDispatcher::handleDelayMessage(Message *msg)
{
    switch(msg->msg_t)
    {
    case MSG_EVT_TIME_OUT:
        clearMessage();
        sendMessage(msg);
        break;
	case MSG_EVT_HERO_ALL_DIE:
		clearMessage();
        sendMessage(msg);
        break;
    case MSG_CLEAR_MSG:
        clearMessage(msg->sender, msg->tmsg_t);
        break;
    default:
        sendMessage(msg);
        break;
    }
}

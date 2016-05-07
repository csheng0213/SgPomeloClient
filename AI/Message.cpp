#include "Message.h"

int Message::COUNTER = 0;

Message::Message()
    : sender(0)

    , id(++COUNTER)

    , delay(1)
    , interval(1)
    , total_time(0)

    , msg_t(MSG_BEGIN)
    , tmsg_t(MSG_BEGIN)

    , tar(0)
    , tar_pos(Vector2D(0.0f, 0.0f))
    , val(0.0f)

    , rand(0.0f)

    , spl_eff_tag(false)
    , spl_piece_id(0)
    , spl_isPtsp(false)
    , spl_isDelay(false)
    , spl_isTimely(false)

    , isBaoji(0)
{
}

void Message::resetCounter()
{
    COUNTER = 0;
}

int Message::getId()
{
    return id;
}

void Message::addReceiver(Creature *c)
{
    receiver.push_back(std::make_pair(c, 0.0f));
}

std::unique_ptr<float, Message::NonDeleter> Message::getVarValue(Creature *c)
{
    for (auto &i : receiver)
    {
        if (i.first == c)
            return std::unique_ptr<float, NonDeleter>(&i.second, NonDeleter());
    }
    
    return std::unique_ptr<float, NonDeleter>(nullptr);
}

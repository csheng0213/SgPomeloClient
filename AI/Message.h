#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <vector>
#include <memory>

#include "Vector2D.h"
#include "Spell.h"

enum MessageType
{
	MSG_BEGIN   = 0,

    MSG_CHARGE,
    MSG_MOVE_TARGET,

    MSG_ATTACK,
    MSG_ATTACK_END,

    MSG_HURT,

    MSG_SPELL,
    MSG_SPELL_END,

    // event
    MSG_EVT_HERO_ALL_DIE,
    MSG_EVT_TIME_OUT,

    // clear msg
    MSG_CLEAR_MSG,

	MSG_END
};

class Message
{
    struct NonDeleter
    {
        void operator () (float *) {}
    };
public:
    Message();
    static void resetCounter();
    int getId();

    void addReceiver(Creature *c);

    std::unique_ptr<float, NonDeleter> getVarValue(Creature *c);

public:
	MessageType			msg_t;

    MessageType         tmsg_t;

	Creature *          sender;

    //                               var_val
    std::vector<std::pair<Creature*, float>>  receiver;

	int			        delay;      // frame times
    int                 interval;
    int                 total_time;
    
    Creature           *tar;
    Vector2D            tar_pos;    // target position, not target's position
    float               val;        // value for numeric use

    float               rand;

    // spell
    bool                spl_eff_tag;
    SpellEffectPiece    spl_eff;
    int                 spl_piece_id;
    SpellConditionPiece spl_cond;
    SpellType           spl_type;
    bool                spl_isPtsp;
    bool                spl_isDelay;
    bool                spl_isTimely;  //及时技能
    
    //creature
    int                 isBaoji;

private:
    int                 id;
    static int          COUNTER;
};

#endif // _MESSAGE_H_
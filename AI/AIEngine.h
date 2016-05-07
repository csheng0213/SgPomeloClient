#ifndef _AI_ENGINE_H_
#define _AI_ENGINE_H_

#include <list>
#include <utility>

#include "Random.h"
#include "Vector2D.h"
#include "Creature.h"
#include "CreatureManager.h"
#include "AutoFightSys.h"

enum AIEventType
{
    EVT_BEGIN   = 0,
    
    EVT_TIME_OUT,
    EVT_HERO_ALL_DIE,
    EVT_SPELL,
	EVT_AUTO_SPELL,

    EVT_END
};

struct AIEvent
{
    AIEvent()
        : id(0)
        , etype(EVT_BEGIN)
        , frame(0)
        , raiser(0)
        , tar(0)
        , tar_pos(Vector2D())
    {}

    unsigned int id;
    AIEventType  etype;
    int          frame;
    int          raiser;
    int          tar;// 目标
    Vector2D     tar_pos;
};

//             frame, AIEvent
typedef std::list<AIEvent> AIEventSequence;

class Creature;
class CreatureManager;

class AIEngine
{
    AIEngine()
        : _spl_c(nullptr)
    {}
    
    ~AIEngine()
    {
        delete _spl_c;
        _spl_c = nullptr;
    }
public:
	static AIEngine & getInstance()
	{
		static AIEngine s;
		return s;
	}

    void init(bool delCreature = false);    // reset ai engine data, use before AI APIs
         
    void start();                           // use before update, after creature construction
         
    void end();                             // for release memory needlessly
         
    void update();                          // update in each frame
         
    void setReload(bool reload = true);
         
    void forceShutDown();

public:
    void pause()  { _pause = true;  }

    void resume() { _pause = false; }
         
public:  
    bool isFinished() const
    {
        return _finished;
    }

    void setFinished(bool f = true)
    {
        _finished = f;
    }

    void setEnemyAutoFight(bool f = true);

    void setSelfAutoFight(bool f = true);

    float randf()
    {
        return _rand.randf();
    }

    void srand(unsigned int seed = 0xffff)
    {
        _rand.srand(seed);
    }

public:
    void pushCreature(Creature *c)
    {
        sCrtMgr.pushCreature(c);
    }

    CreatureSequence * getCreatureSequence(bool frdly = true)
    {
        return sCrtMgr.getCreatureSequence(frdly);
    }

    AIEventSequence *  getAIEventSequence()
    {
        auto seq = new AIEventSequence();
        *seq = _evt_seq;
        return seq; 
    }

    std::vector<float> getHerosHpLost()
    {
        return sCrtMgr.getHerosHpLost();
    }

    int getCurrentFrame()
    {
        return _frame;
    }
    
public:
    void    raiseEvent(AIEvent evt);

    void    addEvent(const AIEvent &evt);

    void    addAutoSpellCond(bool frdly, int prior, AutoFightHero hero, AutoFightCondSet condset);
   
public:
    void    setGlobalSpell(Spell *spl);

    Spell * getGlobalSpell();

    void    raiseGlobalSpell();

    void    addGlobalPassiveSpell(Spell *spl);

    std::vector<Spell *> & getGlobalPassiveSpell();

private:
    static const int TimeLimit = 10800;

private:
    Random              _rand;
    AIEventSequence     _evt_seq;

    Creature           *_spl_c;

    int                 _frame;
    int                 _evt_id_creator;
    bool                _pause;
    bool                _finished;
    bool                _reload;
    bool                _enemy_auto_fight;
    bool                _self_auto_fight;
};

#define sAIEngine (AIEngine::getInstance())

#endif // _AI_ENGINE_H_
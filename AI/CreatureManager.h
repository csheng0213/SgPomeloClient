#ifndef _CREATURE_MANAGER_H_
#define _CREATURE_MANAGER_H_

#include <map>
#include <vector>
#include <stack>

#include "Vector2D.h"
#include "Creature.h"

class ActiveObject;

struct CreatureProtoInfo
{
    CreatureProtoInfo(unsigned int id, CreatureType ctype, bool frdly, int pid, int num, Vector2D pos)
        : id(id)
        , ctype(ctype)
        , frdly(frdly)
        , pid(pid)
        , num(num)
        , pos(pos)
    {}

    unsigned int    id;
    CreatureType    ctype;
    bool            frdly;
    int             pid;    // use for creature's proto
    int             num;
    Vector2D        pos;
};

using CreatureSequence = std::list<CreatureProtoInfo>;

class CreatureManager
{
    using CollideVec = std::vector<Creature*>;
    using CollidePair = std::pair<CollideVec, CollideVec>;

    CreatureManager()
        : _fh(0), _eh(0), _fhp(0.0f), _ehp(0.0f), _fmp(0.0f), _emp(0.0f), _fcnt(0), _ecnt(0)
    {}
    CreatureManager(const CreatureManager &);
public:
    enum Selector
    {
        S_ALL,
        S_FRIEND,
        S_ENEMY,
    };

public:
	static CreatureManager & getInstance()
	{
		static CreatureManager s;
		return s;
	}

    void deleteCreature();

    void clear();

    void update();

    void pushIter() { _iter.push(_creature.begin()); }

    void popIter() { _iter.pop(); }

    template <typename Pred>
    Creature * getNext(Pred pred)
    {
        Creature *et = nullptr;
        for (; _iter.top() != _creature.end(); ++_iter.top())
        {
            if (pred(_iter.top()->second))
            {
                et = _iter.top()->second;
                ++_iter.top();
                break;
            }
        }
        return et;
    }

    Creature * getNext()
    {
        Creature *et = nullptr;

        if (_iter.top() != _creature.end())
        {
            et = _iter.top()->second;
            ++_iter.top();
        }

        return et;
    }

    Creature * getTarget(const int id);

    void       pushCreature(Creature *creature);

	CreatureSequence *          getCreatureSequence(bool frdly);

    std::map<int, Creature *> & getCreatureMap() { return _creature; }

    CollidePair                 getCollideCreaturePair(Creature *c);

    std::vector<float>          getHerosHpLost();

    Creature * getAttackableTarget(Creature *c, bool charged = false);

    bool       hasHeroAlive(bool friendly);

    Creature * getMainHero(bool friendly);

    Creature * getViceHero1(bool friendly);

    Creature * getViceHero2(bool friendly);
    
    float getTotalHp(bool friendly);

    void  setTotalHp(float v, bool friendly);
    
    float getTotalMp(bool friendly);

    void  setTotalMp(float v, bool friendly);

public:
    std::vector<Creature*> * getCircleUnits(Vector2D pos, float radius, int type); // type: 0 -> all, 1 -> friend, 2 -> enemy
    
    // backRange means distance between creature and its back distance
    std::vector<Creature*> * getSectorUnits(Vector2D pos, Vector2D direction, float radius, float angle, int type, float backLen = 0.0f);

    std::vector<Creature*> * getRectangleUnits(Vector2D pos, Vector2D direction, float width, float height, int type, float backLen = 0.0f);

    std::vector<Creature*> * getMainHeros(int type); // type: 0 -> all, 1 -> friend, 2 -> enemy

    std::vector<Creature*> * getViceHeros(int type);

    std::vector<Creature*> * getAllHeros(int type);

    std::vector<Creature*> * getSoldiers(int type);

    std::vector<Creature*> * getSoldiersByType(CreatureType ct, int type);

    std::vector<Creature*> * getSoldiersByNation(CreatureNation cn, int type);

    Creature * getPropertyHighest(float Creature::* prop, int type);

    Creature * getPropertyLowest (float Creature::* prop, int type);
    
    Creature * getNearestTarget(Creature *c, int type); // 0: all, 1: friend, 2: enemy

    Creature * getNearestHero(Creature *c, int type);

public:
    void checkHeroAllDead(Creature *c);

    bool isFriendTypeEqual(Creature *c, int type);

private:
    std::map<int, Creature *>                       _creature; // id, Creature *
    std::stack<std::map<int, Creature *>::iterator> _iter;
    
    Creature               *_fh;    // friend hero
    Creature               *_eh;    // enemy hero
    
    std::vector<Creature *> _fvh;   // friend vice hero
    std::vector<Creature *> _evh;   // enemy vice hero

    float                   _fhp;   // friend total hp
    float                   _ehp;   // enemy total hp

    float                   _fmp;   // friend total mp
    float                   _emp;   // enemy total mp
    
    int                     _fcnt;  // friend creature count
    int                     _ecnt;  // friend creature count
};

#define sCrtMgr (CreatureManager::getInstance())

#endif // _CREATURE_MANAGER_H_
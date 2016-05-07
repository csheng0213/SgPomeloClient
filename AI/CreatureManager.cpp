#include <functional>

#include "CreatureManager.h"

#include "AIConst.h"
#include "AIEngine.h"
#include "Vector2D.h"
#include "StateInclude.h"

//#include "cocos2d.h"
//USING_NS_CC;

void CreatureManager::deleteCreature()
{
    for (auto &it : _creature)
        delete it.second;
}

void CreatureManager::clear()
{
    Creature::resetIdHolder();

    _creature.clear();

    _fvh.clear();
    _evh.clear();

    _fh = 0;
    _eh = 0;
    
    _fhp = 0.0f;
    _fmp = 0.0f;

    _ehp = 0.0f;
    _emp = 0.0f;

    _fcnt = 0;
    _ecnt = 0;
}

void CreatureManager::update()
{
    auto proc = [](Creature *h) {
        if (h)
        {
            auto spell = h->getSpell();
            if (spell)
                spell->reduceCoolDown();
        }
    };

    proc(_fh);
    if (_fvh.size() > 0)
        proc(_fvh[0]);
    if (_fvh.size() > 1)
        proc(_fvh[1]);

    proc(_eh);
    if (_evh.size() > 0)
        proc(_evh[0]);
    if (_evh.size() > 1)
        proc(_evh[1]);
}

Creature * CreatureManager::getTarget(const int id)
{
    auto it = _creature.find(id);
    return it == _creature.end() ? 0 : it->second;
}

void CreatureManager::pushCreature(Creature *c)
{
    if (!c)
        return;
    
    _creature.insert(std::make_pair(c->getId(), c));

    if (c->frdly)
    {
        _fcnt++;
        if (c->isMainHero())
            _fh = c;
        else if (c->isViceHero())
            _fvh.push_back(c);
    }
    else
    {
        _ecnt++;
        if (c->isMainHero())
            _eh = c;
        else if (c->isViceHero())
            _evh.push_back(c);
    }
}

CreatureSequence * CreatureManager::getCreatureSequence(bool frdly)
{
    auto seq = new CreatureSequence;
    
    Creature *tmp = nullptr;
    int id = 1;
    for (auto &it : _creature)
    {
        tmp = it.second;
        if (tmp->isAlive() && tmp->frdly == frdly)
        {
            auto last_count = (int)std::floor(tmp->hp / (tmp->hp_max / tmp->num)) + 1;
            seq->push_back(CreatureProtoInfo(id++, tmp->ctype, tmp->frdly, tmp->pid, tmp->num, tmp->pos));
        }
    }
    return seq;
}

CreatureManager::CollidePair CreatureManager::getCollideCreaturePair(Creature *c)
{
    CollidePair collide_pair;

    for (auto &i : _creature)
    {
        auto t = i.second;
        if (!t->isAlive())
            continue;

        if (c != t)
        {
            auto len = (t->pos - c->pos).lenSq();
            if ((c->frdly == t->frdly && len < (c->round + t->round) * (c->round + t->round))
             || (c->frdly != t->frdly && len < (c->round + t->round) * (c->round + t->round) * 0.5))
            {
                auto lb = c->dire.len();
                auto lc = (t->pos - c->pos).len();
                auto la = (c->dire - (t->pos - c->pos)).len();

                auto agl = std::acos((lb*lb+lc*lc-la*la)/(2*lb*lc));

                //CCLOG("b: %f, c: %f, a: %f, agl: %f, cos_in: %f", lb, lc, la, agl, (lb*lb+lc*lc-la*la)/(2*lb*lc));

                if (agl < PI / 5)
                    collide_pair.first.push_back(i.second);
                else
                    collide_pair.second.push_back(i.second);
            }
        }
    }
    return collide_pair;
}

std::vector<float> CreatureManager::getHerosHpLost()
{
    return std::vector<float> {
        _fh == NULL ? 0 : _fh->raw_hp - _fh->hp,
        _fvh.size() > 0 ? _fvh[0]->raw_hp - _fvh[0]->hp : 0,
        _fvh.size() > 1 ? _fvh[1]->raw_hp - _fvh[1]->hp : 0,
        _eh == NULL ? 0 : _eh->raw_hp - _eh->hp,
        _evh.size() > 0 ? _evh[0]->raw_hp - _evh[0]->hp : 0,
        _evh.size() > 1 ? _evh[1]->raw_hp - _evh[1]->hp : 0,
    };
}

Creature * CreatureManager::getAttackableTarget(Creature *c, bool charged)
{
    if (c->tar_buf_cnt < 30)
    {
        c->tar_buf_cnt++;
        return c->tar;
    }

    c->tar_buf_cnt = 0;

    //                    Creature *, distSq
    std::vector<std::pair<Creature *, float> > recorder(
        c->prior_atk_vec.size() + 1,
        std::make_pair(
            (Creature *)0,
            MaxCreatureDistanceSeq
        )
    );

    auto const & vec = c->prior_atk_vec;
    auto & rcd_lst   = recorder[recorder.size() - 1];
    auto distSq      = 0.0f;

    for (auto &it : _creature)
    {
        if (it.second->getGlobalState() != S_Alive)
            continue;

        if (it.second->frdly == c->frdly)
            continue;

        distSq = (it.second->pos - c->pos).lenSq();

        for (size_t i = 0; i < vec.size(); i++)
        {
            if (it.second->ctype != vec[i])
                continue;

            if (recorder[i].first == 0 || distSq < recorder[i].second)
            {
                recorder[i].first = it.second;
                recorder[i].second = distSq;
            }
        }

        if (rcd_lst.first == 0 || distSq < rcd_lst.second)
        {
            rcd_lst.first = it.second;
            rcd_lst.second = distSq;
        }
    }

    for (size_t i = 0; i < recorder.size(); ++i)
        if (recorder[i].first != 0 && recorder[i].second < c->vis_field * c->vis_field)
            return recorder[i].first;

    // if creature has charged and cannot get an enemy in its visual field, then return the nearest enemy
    if (charged && rcd_lst.first != 0)
            return rcd_lst.first;

    return 0;
}

bool CreatureManager::hasHeroAlive(bool friendly)
{
    if (friendly)
    {
        if (_fh && _fh->isAlive()) //排除主将不在的情况
            return true;

        if (_fvh.size() > 0 && _fvh[0]->isAlive())
            return true;

        if (_fvh.size() > 1 && _fvh[1]->isAlive())
            return true;
    }
    else
    {
        if (_eh && _eh->isAlive()) //排除主将不在的情况
            return true;
        
        if (_evh.size() > 0 && _evh[0]->isAlive())
            return true;

        if (_evh.size() > 1 && _evh[1]->isAlive())
            return true;
    }

    return false;
}

Creature * CreatureManager::getMainHero(bool friendly)
{
    if (friendly)
        return _fh;
    else
        return _eh;
}

Creature * CreatureManager::getViceHero1(bool friendly)
{
    if (friendly)
    {
        if (_fvh.size() < 1)
            return 0;
        return _fvh[0];
    }
    else
    {
        if (_evh.size() < 1)
            return 0;
        return _evh[0];
    }
}

Creature * CreatureManager::getViceHero2(bool friendly)
{
    if (friendly)
    {
        if (_fvh.size() < 2)
            return 0;
        return _fvh[1];
    }
    else
    {
        if (_evh.size() < 2)
            return 0;
        return _evh[1];
    }
}

float CreatureManager::getTotalHp(bool friendly)
{
    if (friendly)
        return _fhp;
    else
        return _ehp;
}

void CreatureManager::setTotalHp(float v, bool friendly)
{
    if (friendly)
        _fhp = v;
    else
        _ehp = v;
}

float CreatureManager::getTotalMp(bool friendly)
{
    if (friendly)
        return _fmp;
    else
        return _emp;
}

void CreatureManager::setTotalMp(float v, bool friendly)
{
    if (friendly)
        _fmp = v;
    else
        _emp = v;
}

std::vector<Creature*> * CreatureManager::getCircleUnits(Vector2D pos, float radius, int type)
{
    auto r = new std::vector<Creature *>;

    Creature *c = nullptr;

    pushIter();
    while ((c = getNext()))
    {
        if (c->isAlive() && c->pos.distSq(pos) < radius * radius && isFriendTypeEqual(c, type))
            r->push_back(c);
    }
    popIter();

    return r;
}

std::vector<Creature*> * CreatureManager::getSectorUnits(Vector2D pos, Vector2D direction, float radius, float angle, int type, float backLen)
{
    auto r = new std::vector<Creature *>;
    
    Creature *c = nullptr;

    pos += direction.getReverse().getNorm() * backLen;
    auto agl = direction.getAngle();

    pushIter();
    while ((c = getNext()))
    {
        auto tar_agl = (c->pos - pos).getAngle();
        if (c->isAlive()
            && c->pos.distSq(pos) < radius * radius
            && std::cos(agl - tar_agl) > std::cos(angle / 2)
            && isFriendTypeEqual(c, type)
           )
        {
            r->push_back(c);
        }
    }
    popIter();

    return r;
}

std::vector<Creature*> * CreatureManager::getRectangleUnits(Vector2D pos, Vector2D direction, float width, float height, int type, float backLen)
{
    auto r = new std::vector<Creature *>;

    Creature *c = nullptr;
    
    pos += direction.getReverse().getNorm() * backLen;
    auto agl = direction.getAngle();

    auto angle = 0.0f;
    auto dist  = 0.0f;
    auto x     = 0.0f;
    auto y     = 0.0f;

    pushIter();
    while ((c = getNext()))
    {
        angle = (c->pos - pos).getAngle() - agl;
        dist = c->pos.dist(pos);
        x = dist * std::cos(angle);
        y = std::abs(dist * std::sin(angle));

        if (c->isAlive()
            && x > 0.0f
            && x <= height
            && y < width / 2
            && isFriendTypeEqual(c, type)
           )
        {
            r->push_back(c);
        }
    }
    popIter();

    return r;
}

std::vector<Creature*> * CreatureManager::getViceHeros(int type)
{
    auto r = new std::vector<Creature *>;
    
    Creature *c = nullptr;
    if (type == 0)
    {
        c = getViceHero1(true);
        if (c && c->isAlive()) r->push_back(c);
        c = getViceHero2(true);
        if (c && c->isAlive()) r->push_back(c);

        c = getViceHero1(false);
        if (c && c->isAlive()) r->push_back(c);
        c = getViceHero2(false);
        if (c && c->isAlive()) r->push_back(c);
    }
    else if (type == 1)
    {
        c = getViceHero1(true);
        if (c && c->isAlive()) r->push_back(c);
        c = getViceHero2(true);
        if (c && c->isAlive()) r->push_back(c);
    }
    else if (type == 2)
    {
        c = getViceHero1(false);
        if (c && c->isAlive()) r->push_back(c);
        c = getViceHero2(false);
        if (c && c->isAlive()) r->push_back(c);
    }

    return r;
}

std::vector<Creature*> * CreatureManager::getMainHeros(int type)
{
    auto r = new std::vector<Creature *>;

    if (type == 0)
    {
        if (_fh && _fh->isAlive())
            r->push_back(_fh);
        if (_eh && _eh->isAlive())
            r->push_back(_eh);
    }
    else if (type == 1)
    {
        if (_fh && _fh->isAlive())
            r->push_back(_fh);
    }
    else if (type == 2)
    {
        if (_eh && _eh->isAlive())
            r->push_back(_eh);
    }

    return r;
}

std::vector<Creature*> * CreatureManager::getAllHeros(int type)
{
    auto r = new std::vector<Creature *>;
    
    Creature *c = nullptr;
    if (type == 0)
    {
        c = getMainHero(true);
        if (c && c->isAlive()) r->push_back(c);
        c = getViceHero1(true);
        if (c && c->isAlive()) r->push_back(c);
        c = getViceHero2(true);
        if (c && c->isAlive()) r->push_back(c);

        c = getMainHero(false);
        if (c && c->isAlive()) r->push_back(c);
        c = getViceHero1(false);
        if (c && c->isAlive()) r->push_back(c);
        c = getViceHero2(false);
        if (c && c->isAlive()) r->push_back(c);
    }
    else if (type == 1)
    {
        c = getMainHero(true);
        if (c && c->isAlive()) r->push_back(c);
        c = getViceHero1(true);
        if (c && c->isAlive()) r->push_back(c);
        c = getViceHero2(true);
        if (c && c->isAlive()) r->push_back(c);
    }
    else if (type == 2)
    {
        c = getMainHero(false);
        if (c && c->isAlive()) r->push_back(c);
        c = getViceHero1(false);
        if (c && c->isAlive()) r->push_back(c);
        c = getViceHero2(false);
        if (c && c->isAlive()) r->push_back(c);
    }

    return r;
}

std::vector<Creature*> * CreatureManager::getSoldiers(int type)
{
    auto r = new std::vector<Creature *>;
    Creature *c = nullptr;

    pushIter();
    while ((c = getNext()))
    {
        if (c->isAlive() && c->isSoldier() && isFriendTypeEqual(c, type))
            r->push_back(c);
    }
    popIter();

    return r;
}

std::vector<Creature*> * CreatureManager::getSoldiersByType(CreatureType ct, int type)
{
    auto r = new std::vector<Creature *>;
    Creature *c = nullptr;

    pushIter();
    while ((c = getNext()))
    {
        if (c->isAlive() && c->isType(ct) && isFriendTypeEqual(c, type))
            r->push_back(c);
    }
    popIter();

    return r;
}

std::vector<Creature*> * CreatureManager::getSoldiersByNation(CreatureNation cn, int type)
{
    auto r = new std::vector<Creature *>;
    Creature *c = nullptr;

    pushIter();
    while ((c = getNext()))
    {
        if (c->isAlive() && c->isNation(cn) && isFriendTypeEqual(c, type))
            r->push_back(c);
    }
    popIter();

    return r;
}

Creature * CreatureManager::getNearestTarget(Creature *c, int type)
{
    auto distSq     = MaxCreatureDistanceSeq;
    auto distSqTemp = 0.0f;
    int  id         = 0;

    for (auto &it : _creature)
    {
        if (!it.second->isAlive())
            continue;

        if (c != it.second && isFriendTypeEqual(it.second, type))
        {
            distSqTemp = (it.second->pos - c->pos).lenSq();

            if (distSq > distSqTemp)
            {
                distSq = distSqTemp;
                id = it.first;
            }
        }
    }

    if (id > 0)
        return _creature[id];

    return 0;
}

Creature * CreatureManager::getNearestHero(Creature *c, int type)
{
    auto min = MaxCreatureDistanceSeq;
    
    Creature *t  = nullptr;
    Creature *rt = nullptr;

    auto proc_min = [&] {
        if (t && t->isAlive() && t->pos.distSq(c->pos) < min * min)
        {
            rt = t;
            min = t->pos.distSq(c->pos);
        }
    };

    if (type == 0)
    {
        t = getMainHero(true);
        proc_min();
        t = getViceHero1(true);
        proc_min();
        t = getViceHero2(true);
        proc_min();
        
        t = getMainHero(false);
        proc_min();
        t = getViceHero1(false);
        proc_min();
        t = getViceHero2(false);
        proc_min();
    }
    else if (type == 1)
    {
        t = getMainHero(true);
        proc_min();
        t = getViceHero1(true);
        proc_min();
        t = getViceHero2(true);
        proc_min();
    }
    else if (type == 2)
    {
        t = getMainHero(false);
        proc_min();
        t = getViceHero1(false);
        proc_min();
        t = getViceHero2(false);
        proc_min();
    }

    return rt;
}

void CreatureManager::checkHeroAllDead(Creature *c)
{
    if (c->isHero() && !sCrtMgr.hasHeroAlive(c->frdly))
    {
        AIEvent evt;
        evt.etype = EVT_HERO_ALL_DIE;
        evt.raiser = c->getId();
        sAIEngine.raiseEvent(evt);
    }
}

bool CreatureManager::isFriendTypeEqual(Creature *c, int type)
{
    switch (type)
    {
    case 0:
        return true;
    case 1:
        return c->frdly;
    case 2:
        return !c->frdly;
    default:
        return false;
    }
}

Creature * CreatureManager::getPropertyHighest(float Creature::* prop, int type)
{
    auto max = 0.0f;

    Creature *c  = nullptr;
    Creature *rc = nullptr;
    
    pushIter();
    while ((c = getNext()))
    {
        if (c->isAlive() && c->isHero() && c->*prop > max && isFriendTypeEqual(c, type))
        {
            max = c->*prop;
            rc = c;
        }
    }
    popIter();

    return rc;
}

Creature * CreatureManager::getPropertyLowest (float Creature::* prop, int type)
{
    auto min = std::numeric_limits<float>::max();

    Creature *c  = 0;
    Creature *rc = 0;

    pushIter();
    while ((c = getNext()))
    {
        if (c->isAlive() && c->isHero() && c->*prop < min && isFriendTypeEqual(c, type))
        {
            min = c->*prop;
            rc = c;
        }
    }
    popIter();

    return rc;
}

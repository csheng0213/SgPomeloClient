#ifndef _AI_CONST_H_
#define _AI_CONST_H_

#include "Vector2D.h"

const int   BattleFieldWidth    = 10;
const int   BattleFieldLength   = 26;

const float BattleFieldEdge     = 100.0f;

const float MaxCreatureDistanceSeq = (BattleFieldWidth * BattleFieldWidth + BattleFieldLength * BattleFieldLength)
                                      * BattleFieldEdge * BattleFieldEdge;
const float MaxCreatureDistance    = std::sqrt(MaxCreatureDistanceSeq);

const int   HistoryPosCheck     = 30;
const float CollideForceParam   = 18.0f;
const float TraficJamParam      = 500.0f;

const int   IdleToMoveTime      = 60;   // frame

const int   AutoSpellTimeInterval = 180;

const int   CommonSpellCoolDown             = 30;
const int   FriendStartBattleSpellCoolDown  = 480;
const int   EnemyStartBattleSpellCoolDown   = 600;

const Vector2D  ForwardDirection    = Vector2D(1.0f, 0.0f);

const Vector2D  BackwardDirection   = Vector2D(-1.0f, 0.0f);

const float HeroHPChangeFactor = 2.5f;

#endif // !_AI_CONST_H_
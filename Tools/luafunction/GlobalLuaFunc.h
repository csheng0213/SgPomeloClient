#ifndef __GLOBALLUAFUNC_H__
#define __GLOBALLUAFUNC_H__

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "luaex.h"
};

int L_dofile(lua_State *L);
int L_os_clock(lua_State *L);
int L_GetFileData(lua_State *L);
int L_SerializeTable(lua_State *L);
int L_UnserializeTable(lua_State *L);

//debug
int L_DebugHoldTime(lua_State *L);
int L_DebugDelayTime(lua_State *L);

//http
int L_HttpClientUpdate(lua_State *L);
int L_SendHttpRequest(lua_State *L);

//server
int L_SetServerIp(lua_State *L);
int L_SendToServer(lua_State *L);
int L_ServerMainLoop(lua_State *L);
int L_Login(lua_State *L);
int L_Logout(lua_State *L);
int L_SendReconnect(lua_State*L);

//update
int L_UpdateGame(lua_State *L);
int L_GetUpdateState(lua_State *L);
int L_GetUpdateCurInfo(lua_State *L);

int L_GetUIRootLayer(lua_State *L);
int L_GetLoginPanel(lua_State *L);
int L_FastRegAccount(lua_State *L);
int L_RegisterAccount(lua_State *L);
int L_ValidateAccount(lua_State *L);
int L_BindAccount(lua_State *L);
int L_AddLog(lua_State *L);
int L_GetPathIos(lua_State *L);

//xml paser
int L_PaserFBXml(lua_State *L);

//combat
int L_AddCreatureToAIengin(lua_State *L);
int L_InitCombatCenter(lua_State *L);
int L_StartCombatCenter(lua_State *L);
int L_UpdateCombatCenter(lua_State *L);

int L_SetEnemyAutoFight(lua_State *L);
int L_SetSelfAutoFight(lua_State *L);
int L_AddAutoFightCon(lua_State *L);
int L_GetAIEventList(lua_State *L);
int L_GetAICreatureList(lua_State *L);
int L_AiSetRand(lua_State *L);
int L_AiSetReload(lua_State *L);
int L_AiAddEventList(lua_State *L);
int L_AiForceShutDown(lua_State *L);
int L_AiPause(lua_State *L);
int L_AiResume(lua_State *L);
int L_AiGetCurFrame(lua_State *L);
int L_AiRaiseEvent(lua_State *L);
int L_GetAttackTarOfFightId(lua_State *L);
int L_AiIsFinish(lua_State *L);
int L_AiGetHpList(lua_State *L);

//ui tool
int L_GraySprite(lua_State *L);

#endif
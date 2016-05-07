#include "GlobalLuaFunc.h"
#include "LuaCustom.h"
USING_NS_CC;

#ifdef __cplusplus
extern "C" {
#include "lua_cjson.h"
#include "lfs.h"
}
#endif


static CLuaCustom s_LuaCustom;
CLuaCustom *CLuaCustom::getInstance()
{
	return &s_LuaCustom;
}

CLuaCustom::CLuaCustom()
{
}

CLuaCustom::~CLuaCustom()
{

}

void CLuaCustom::Init()
{
	m_State = LuaEngine::getInstance()->getLuaStack()->getLuaState();
	RegisterAllLuaFunc();
}

void CLuaCustom::RegisterAllLuaFunc()
{
    
    //register cjson
    luaopen_cjson(m_State);
    
    //register lfs
    luaopen_lfs(m_State);

    
#define REG_FUNC(name) lua_register(m_State, #name, L_##name)
	REG_FUNC(dofile);
	REG_FUNC(AddLog);
	REG_FUNC(GetFileData);
	REG_FUNC(SerializeTable);
	REG_FUNC(UnserializeTable);
    
    REG_FUNC(DebugHoldTime);
    REG_FUNC(DebugDelayTime);
    
    REG_FUNC(HttpClientUpdate);
    REG_FUNC(SendHttpRequest);
	REG_FUNC(SendToServer);
    REG_FUNC(SetServerIp);
    REG_FUNC(ServerMainLoop);
    
    REG_FUNC(UpdateGame);
    REG_FUNC(GetUpdateState);
    REG_FUNC(GetUpdateCurInfo);
    
    REG_FUNC(PaserFBXml);
    
    REG_FUNC(AddCreatureToAIengin);
    REG_FUNC(InitCombatCenter);
    REG_FUNC(StartCombatCenter);
    REG_FUNC(UpdateCombatCenter);
    REG_FUNC(SetEnemyAutoFight);
    REG_FUNC(SetSelfAutoFight);
    REG_FUNC(AddAutoFightCon);
    
    REG_FUNC(GetAIEventList);
    REG_FUNC(GetAICreatureList);
    REG_FUNC(AiSetRand);
    REG_FUNC(AiSetReload);
    REG_FUNC(AiAddEventList);
    REG_FUNC(AiForceShutDown);
    REG_FUNC(AiPause);
    REG_FUNC(AiResume);
    REG_FUNC(AiGetCurFrame);
    REG_FUNC(AiRaiseEvent);
    REG_FUNC(GetAttackTarOfFightId);
    REG_FUNC(AiIsFinish);
    REG_FUNC(AiGetHpList);
    
    REG_FUNC(GraySprite);
    
	REG_FUNC(GetUIRootLayer);
	REG_FUNC(GetLoginPanel);
	REG_FUNC(FastRegAccount);
	REG_FUNC(RegisterAccount);
	REG_FUNC(ValidateAccount);
	REG_FUNC(Login);
	REG_FUNC(Logout);
    REG_FUNC(SendReconnect);
	REG_FUNC(BindAccount);
	REG_FUNC(GetPathIos);
#undef REG_FUNC

	lua_getglobal(m_State, "os");
	lua_pushliteral(m_State, "clock");
	lua_pushcfunction(m_State, L_os_clock);
	lua_settable(m_State, -3);
	lua_pop(m_State, 1);
}
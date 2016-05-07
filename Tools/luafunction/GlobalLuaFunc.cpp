#include "OnlineComponent.h"
#include "system.h"
#include "cocos2d.h"
#include "CCLuaEngine.h"
#include "tolua_fix.h"
#include "GlobalLuaFunc.h"
#include "ExtensionMacros.h"

#include "GlobalConfig.h"
#include "ResManager.h"
#include "LuaMessage.h"
#include "tinyxml.h"
#include "LuaReader.h"

#include "MainPlayer.h"
#include "CombatCenter.h"
#include "AIEngine.h"

#include "DisplayTool.h"
#include "debugTool.h"

#include "CHttpClinet.hpp"

USING_NS_CC;

static char tempBuffer[0x200000];
int L_dofile(lua_State *L)
{
//	const char *fname = luaL_optstring(L, 1, NULL);
//	unsigned long nSize;
//	unsigned char *pBuff = CCFileUtils::sharedFileUtils()->getFileData(fname, "r", &nSize);
//	if (pBuff == NULL) {
//		std::string path = CCFileUtils::sharedFileUtils()->fullPathForFilename(fname);
//		pBuff = CCFileUtils::sharedFileUtils()->getFileData(path.c_str(), "r", &nSize);
//	}
//	if (pBuff == NULL) {
//		std::string fullpath = CCFileUtils::sharedFileUtils()->getWritablePath();
//		std::string path = fullpath + fname;
//		pBuff = CCFileUtils::sharedFileUtils()->getFileData(path.c_str(), "r", &nSize);
//	}
//
//	if (pBuff == NULL) {
//		lua_pushfstring(L, "cannot open %s: No such file or directory", fname);
//        //CCLOG("cannot open %s: No such file or directory", fname);
//		lua_error(L);
//	}
//
//	int n = lua_gettop(L), result;
//	size_t realSize = pub::LzwDecode(pBuff, nSize, tempBuffer, sizeof(tempBuffer));
//	if (realSize != 0) {
//		result = luaL_loadbuffer(L, (char *)tempBuffer, realSize, fname);
//	}
//	else {
//		result = luaL_loadbuffer(L, (char *)pBuff, nSize, fname);
//	}
//	CC_SAFE_DELETE_ARRAY(pBuff);
//	if (result != 0) {
//		lua_error(L);
//	}
//	int err = lua_pcall(L, 0, LUA_MULTRET,NULL);
//	if (err != 0)
//	{
//		std::string str = (lua_tostring(L, -1));
//		//CCLOG(str.c_str());
//	}
	return 0;
}

int L_GetPathIos(lua_State *L)
{
	const char *fname = luaL_optstring(L, 1, NULL);
	int num = lua_tonumber(L, 2);
	std::string path = CCFileUtils::getInstance()->fullPathForFilename(fname);
	if (num == 1) {
		std::string fullpath = CCFileUtils::getInstance()->getWritablePath();
		path = fullpath + fname;
	}
	lua_pushstring(L, path.c_str());
	return 1;
}

int L_os_clock(lua_State *L)
{
//	cc_timeval now;
//	CCTime::gettimeofdayCocos2d(&now, NULL);
//	lua_pushnumber(L, lua_Number(now.tv_sec) + lua_Number(now.tv_usec) / 1000000);
	return 1;
}

int L_GetFileData(lua_State *L)
{
//	const char *fname = luaL_optstring(L, 1, NULL);
//	unsigned long nSize;
//	unsigned char *pBuff = CCFileUtils::sharedFileUtils()->getFileData(fname, "rb", &nSize);
//	if (pBuff == NULL) {
//		std::string path = "../Server/";
//		path.append(fname);
//		pBuff = CCFileUtils::sharedFileUtils()->getFileData(path.c_str(), "rb", &nSize);
//	}
//
//	if (pBuff == NULL) {
//		return 0;
//	}
//	lua_pushlstring(L, (char *)pBuff, nSize);
//	CC_SAFE_DELETE_ARRAY(pBuff);
	return 1;
}

int L_SerializeTable(lua_State *L)
{
	size_t size = sizeof(tempBuffer);
	if (serializetable(L, 1, tempBuffer, &size) != 0)
		return 0;

	lua_pushlstring(L, tempBuffer, size);
	return 1;
}

int L_UnserializeTable(lua_State *L)
{
	size_t size;
	const char *str = luaL_optlstring(L, 1, NULL, &size);
	if (str == NULL)
		return 0;

	if (unserializetable(L, str, &size) != 0)
		return 0;

	return 1;
}

int L_DebugHoldTime(lua_State *L)
{
    if (!lua_isstring(L, 1)) {
        return 0;
    }
    
    std::string str = lua_tostring(L, 1);
    DebugTool::holdTime(str);
    return 0;

}

int L_DebugDelayTime(lua_State *L)
{
    if (!lua_isstring(L, 1)) {
        return 0;
    }
    
    std::string str = lua_tostring(L, 1);
    DebugTool::holdDelay(str);
    return 0;
}

int L_HttpClientUpdate(lua_State *L)
{
    CHttpClient::getInstance()->update();
    return 0;
}

int L_SendHttpRequest(lua_State *L)
{
    if (!lua_isstring(L, 1)) {
        return 0;
    }
    if (!lua_isstring(L, 2)) {
        return 0;
    }
    std::string url = lua_tostring(L, 1);
    std::string param = lua_tostring(L, 2);
    CHttpClient::getInstance()->sendHttpRequest(url.c_str(), param);
    return  0;
}

int L_SetServerIp(lua_State *L)
{
    if (!lua_isstring(L, 1)) {
        return 0;
    }
    if (!lua_isstring(L, 2)) {
        return 0;
    }
    
    std::string ServerIp = lua_tostring(L, 1);
    std::string ServerId = lua_tostring(L, 2);
    
    OnlineComponent::instance()->SetServerIp(ServerIp, ServerId);
    return 0;
    
}

int L_Login(lua_State *L)
{
    if (!lua_isstring(L, 1))
        return 0;
    if (!lua_isstring(L, 2))
        return 0;
    if (!lua_isnumber(L, 3))
        return 0;
    std::string v_accountid = lua_tostring(L, 1);
    std::string v_token = lua_tostring(L, 2);
    int subareaId = lua_tonumber(L, 3);
    
    g_pPlayer->Login(v_accountid, v_token, subareaId);
    
    return 0;
}

int L_Logout(lua_State *L)
{
    g_pPlayer->Logout();
    return 0;
}

int L_SendReconnect(lua_State *L)
{
    OnlineComponent::instance()->SendReconnect();
    return 0;
}

int L_SendToServer(lua_State *L)
{
	/*if (!g_pPlayer->IsLogin())
	return 0;*/

	if (!lua_istable(L, 1))
		return 0;

	proto_lua_custom proto;
	size_t len = sizeof(proto.data);
	if (serializetable(L, 1, proto.data, &len) != SLT_OK)
		return 0;

	bool ret = OnlineComponent::instance()->Send(&proto, offsetof(proto_lua_custom, data) + len);
    lua_pushboolean(L, ret);
	return 1;
}

int L_ServerMainLoop(lua_State *L)
{
    OnlineComponent::instance()->mainLoop();
    return 0;
}

//update
int L_UpdateGame(lua_State *L)
{
    if (!lua_isstring(L, 1)) {
        return 0;
    }
    
    if (!lua_isnumber(L, 2)) {
        return 0;
    }
    
    if (!lua_isnumber(L, 3)) {
        return 0;
    }
    
    std::string url = lua_tostring(L, 1);
    
    auto midVersion = (int)lua_tonumber(L, 2);
    auto minVersion = (int)lua_tonumber(L, 3);
    
    GlobalConfig::InitConfig(url, midVersion, minVersion);
    CResManager::getInstance()->UpdateAndLoadResources();
    
    return 0;
}

int L_GetUpdateState(lua_State *L)
{
    int stateCode = CResManager::getInstance()->GetResLoadState();
    lua_pushinteger(L, stateCode);
    return 1;
}

//xml paser
int L_PaserFBXml(lua_State *L)
{
    if (!lua_isstring(L, 1)) {\
        return 0;
    }
    
    std::string xmlPath = lua_tostring(L, 1);
    
    Data xmlData = FileUtils::getInstance()->getDataFromFile(xmlPath);
    
    if (xmlData.getSize() == 0) {
        return 0;
    }
    std::unique_ptr<TiXmlDocument> doc(new TiXmlDocument());
    const char * buffer = (const char *)xmlData.getBytes();
    doc->Parse(buffer);
    if (doc->Error())
    {
        return false;
    }
    
    auto rootElement = doc->RootElement();
    if (!rootElement)
    {
        return NULL;
    }
    auto groupElement = rootElement->FirstChildElement("objectgroup");
    auto objectElement = groupElement->FirstChildElement("object");
    
    LuaMessage msg;
    msg.set_lua_state(L);
    
    while (objectElement)
    {
        msg.push_table();
        msg.push_key_value("name", objectElement->Attribute("name"));
        msg.push_key_value("type", objectElement->Attribute("type"));
        msg.push_key_value("x", objectElement->Attribute("x"));
        msg.push_key_value("y", objectElement->Attribute("y"));
        msg.push_key_value("width", objectElement->Attribute("width"));
        msg.push_key_value("height", objectElement->Attribute("height"));
        msg.push_table_end();
        
        objectElement = objectElement->NextSiblingElement();
    }
    
    msg.clear_lua_state();
    doc.reset();
    return 1;
}

//combat
int L_AddCreatureToAIengin(lua_State *L)
{
    if(!lua_istable(L, 1))
        return 0;
    
    LuaReader reader;
    reader.set_lua_state(L);
    
    CombatCenter::getInstance()->addLuaCreature(reader);
    
    reader.clear_lua_state();
    return 0;
}

int L_InitCombatCenter(lua_State *L)
{
    CombatCenter::getInstance()->init();
    return 0;
}

int L_StartCombatCenter(lua_State *L)
{
    CombatCenter::getInstance()->start();
    return 0;
}

int L_UpdateCombatCenter(lua_State *L)
{
    CombatCenter::getInstance()->update();
    return 0;
}

int L_SetEnemyAutoFight(lua_State *L)
{
    if (!lua_isboolean(L, 1)) {
        return 0;
    }
    
    bool isAuto = lua_toboolean(L, 1);
    sAIEngine.setEnemyAutoFight(isAuto);
    return 0;
}

int L_SetSelfAutoFight(lua_State *L)
{
    if (!lua_isboolean(L, 1)) {
        return 0;
    }
    
    bool isAuto = lua_toboolean(L, 1);
    sAIEngine.setSelfAutoFight(isAuto);
    return 0;
}

int L_AddAutoFightCon(lua_State *L)
{
    if(!lua_istable(L, 1))
        return 0;
    
    LuaReader reader;
    reader.set_lua_state(L);
    
    CombatCenter::getInstance()->addAutoFightCon(reader);
    
    reader.clear_lua_state();
    return 0;
}

int L_GetAIEventList(lua_State *L)
{
    LuaMessage msg;
    msg.set_lua_state(L);
    
    std::unique_ptr<AIEventSequence> eventList(sAIEngine.getAIEventSequence());
    
    msg.push_table("elist");
    for (auto &data : *eventList)
    {
        msg.push_table();
        msg.push_key_value("m_id", (lua_Integer)data.id);
        msg.push_key_value("m_frame", (lua_Integer)data.frame);
        msg.push_key_value("m_evt", (lua_Integer)data.etype);
        msg.push_key_value("m_raiser", (lua_Integer)data.raiser);
        msg.push_key_value("m_tar", (lua_Integer)data.tar);
        msg.push_key_value("m_tar_x", data.tar_pos.x);
        msg.push_key_value("m_tar_y", data.tar_pos.y);
        msg.push_table_end();
    }
    msg.push_table_end();
    
    msg.clear_lua_state();

    return 1;
}

int L_GetAICreatureList(lua_State *L)
{
    LuaMessage msg;
    msg.set_lua_state(L);
    
    std::unique_ptr<CreatureSequence> creatureList(sAIEngine.getCreatureSequence());
    
    msg.push_table("clist");
    for (auto &data : *creatureList)
    {
        msg.push_table();
        msg.push_key_value("id", (lua_Integer)data.id);
        msg.push_key_value("ctype", (lua_Integer)data.ctype);
        msg.push_key_value("frdly", (lua_Integer)data.frdly);
        msg.push_key_value("pid", (lua_Integer)data.pid);
        msg.push_key_value("num", (lua_Integer)data.num);
        msg.push_key_value("pos_x", data.pos.x);
        msg.push_key_value("pos_y", data.pos.y);
        msg.push_table_end();
    }
    msg.push_table_end();
    
    msg.clear_lua_state();
    
    return 1;

}

int L_AiSetRand(lua_State *L)
{
    if (!lua_isnumber(L, 1)) {
        return 0;
    }
    
    unsigned int seed = lua_tonumber(L, 1);
    sAIEngine.srand(seed);
    return 0;

}

int L_AiSetReload(lua_State *L)
{
    sAIEngine.setReload();
    return 0;
}

int L_AiAddEventList(lua_State *L)
{
    if(!lua_istable(L, 1))
        return 0;
    
    LuaReader reader;
    reader.set_lua_state(L);
    
    int elistLen = reader.get_int("elistLen");
    
    reader.enter_table("elist");
    
    for (int i = 1; i <= elistLen; i++)
    {
        reader.enter_table(i);
        
        AIEvent eve;
        int x = 0;
        int y = 0;

        eve.id = reader.get_int("id");
        eve.frame = reader.get_int("frame");
        eve.etype = (AIEventType)reader.get_int("evt");
        eve.raiser = reader.get_int("raiser");
        eve.tar = reader.get_int("tar");
        
        x = reader.get_int("tar_x");
        y = reader.get_int("tar_y");
        
        eve.tar_pos = Vector2D(x, y);
        
        sAIEngine.addEvent(eve);
        
        reader.leave_table();
    }
    reader.leave_table();

    
    reader.clear_lua_state();
    return 0;

}

int L_AiForceShutDown(lua_State *L)
{
    sAIEngine.forceShutDown();
    return 0;
}

int L_AiPause(lua_State *L)
{
//    sAIEngine.pause();
    return 0;
}

int L_AiResume(lua_State *L)
{
//    sAIEngine.resume();
    return 0;
}

int L_AiGetCurFrame(lua_State *L)
{
    unsigned int frame = sAIEngine.getCurrentFrame();
    lua_pushinteger(L, frame);
    return 1;
}

int L_AiRaiseEvent(lua_State *L)
{
    if (!lua_istable(L, 1))
    {
        return 0;
    }
    
    LuaReader reader;
    reader.set_lua_state(L);
    
    AIEvent eve;
    eve.etype = (AIEventType)reader.get_int("etype");
    
    auto creature = CombatCenter::getInstance()->getCreatureOfFightId(reader.get_int("raiser"));
    eve.raiser = creature->getId();
    sAIEngine.raiseEvent(eve);
    
    reader.clear_lua_state();
    return 0;
    
}

int L_GetAttackTarOfFightId(lua_State *L)
{
    if(!lua_isnumber(L, 1))
        return 0;
    
    int srcId = lua_tonumber(L, 1);
    int tarFightId = CombatCenter::getInstance()->getAttackTarOfFightId(srcId);
    
    lua_pushinteger(L, tarFightId);
    return 1;
}

int L_AiIsFinish(lua_State *L)
{
    bool isFinish = sAIEngine.isFinished();
    lua_pushboolean(L, isFinish);
    return 1;
}

int L_AiGetHpList(lua_State *L)
{
    LuaMessage msg;
    msg.set_lua_state(L);
    
    auto hplist = sAIEngine.getHerosHpLost();
    
    for (auto &data : hplist)
    {
        msg.push_value(data);
    }
    
    msg.clear_lua_state();
    
    return 1;

}


//ui

int L_GraySprite(lua_State *L)
{
    cocos2d::Node* cobj = nullptr;
    cobj = (cocos2d::Node*)tolua_tousertype(L,1,0);
#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(L,"invalid 'cobj' in function 'lua_cocos2dx_Sprite_setSpriteFrame'", nullptr);
        return 0;
    }
#endif
    
    bool isGray = false;
    
    if (!lua_isboolean(L, 2))
        return 0;
    isGray = lua_toboolean(L, 2);
    
    
    DisplayTool::graySprite(cobj, isGray);
    
    return 0;

}



int L_GetUpdateCurInfo(lua_State *L)
{
    ResourceUpdateInfo curInfo = CResManager::getInstance()->GetResourceUpdateInfo();
    lua_pushinteger(L, curInfo.curDownloadFileIndex);
    lua_pushnumber(L, curInfo.curDownloadFileProgress);
    lua_pushnumber(L, curInfo.totalToDownload);
    lua_pushnumber(L, curInfo.nowDownloaded);
    return 4;
}

int L_GetUIRootLayer(lua_State *L)
{
	/*CUIRootLayer *uiRoot = CUIRootLayer::GetUIRootLayer();
	int nID = (uiRoot) ? (int)uiRoot->m_uID : -1;
	int *pLuaID = (uiRoot) ? &uiRoot->m_nLuaID : NULL;
	toluafix_pushusertype_ccobject(L, nID, pLuaID, (void *)uiRoot, "UILayer");*/
	return 1;
}

int L_GetLoginPanel(lua_State *L)
{
	/*CUIRootLayer *uiRoot = CUIRootLayer::GetUIRootLayer();
	if (uiRoot) {
	UIWidget *loginPanel = uiRoot->GetLoginPanel();
	int nID = (loginPanel) ? (int)loginPanel->m_uID : -1;
	int *pLuaID = (loginPanel) ? &loginPanel->m_nLuaID : NULL;
	toluafix_pushusertype_ccobject(L, nID, pLuaID, (void *)loginPanel, loginPanel->getClassName());
	return 1;
	}*/
	return 0;
}

int L_FastRegAccount(lua_State *L)
{
	int nBegin = 0, nEnd = 0;
	if (lua_isfunction(L, 1)) {
		lua_pushvalue(L, 1);
		nBegin = lua_ref(L, true);
	}
	if (lua_isfunction(L, 2)) {
		lua_pushvalue(L, 2);
		nEnd = lua_ref(L, true);
	}

//	g_pPlayer->FastRegAccount(nBegin, nEnd);
	return 0;
}

int L_RegisterAccount(lua_State *L)
{
	if (!lua_isstring(L, 1))
		return 0;
	if (!lua_isstring(L, 2))
		return 0;

	int nBegin = 0, nEnd = 0;
	if (lua_isfunction(L, 3)) {
		lua_pushvalue(L, 3);
		nBegin = lua_ref(L, true);
	}
	if (lua_isfunction(L, 4)) {
		lua_pushvalue(L, 4);
		nEnd = lua_ref(L, true);
	}

//	const char *account = lua_tostring(L, 1);
//	const char *password = lua_tostring(L, 2);
	//g_pPlayer->RegisterAccount(account, password, nBegin, nEnd);

	return 0;
}

int L_ValidateAccount(lua_State *L)
{
	if (!lua_isstring(L, 1))
		return 0;
	if (!lua_isstring(L, 2))
		return 0;

	int nBegin = 0, nEnd = 0;
	if (lua_isfunction(L, 3)) {
		lua_pushvalue(L, 3);
		nBegin = lua_ref(L, true);
	}
	if (lua_isfunction(L, 4)) {
		lua_pushvalue(L, 4);
		nEnd = lua_ref(L, true);
	}

//	const char *account = lua_tostring(L, 1);
//	const char *password = lua_tostring(L, 2);
//	g_pPlayer->ValidateAccount(account, password, nBegin, nEnd);

	return 0;
}

int L_BindAccount(lua_State *L)
{
	if (!lua_isnumber(L, 1))
		return 0;
	if (!lua_isstring(L, 2))
		return 0;

//	uint32_t accountid = (uint32_t)lua_tonumber(L, 1);
//	size_t len;
//	const char *token = lua_tolstring(L, 2, &len);
//	g_pPlayer->BindAccount(accountid, std::string(token, len));

	return 0;
}

int L_AddLog(lua_State *L)
{
	if (!lua_isstring(L, 1))
		return 0;

//	const char *str = lua_tostring(L, 1);
	//CCLOG(str);

	return 0;
}
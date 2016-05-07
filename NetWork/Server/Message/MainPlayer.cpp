#include "cocos2d.h"

#include "pub.h"
#include "MainPlayer.h"
#include "player.h"
#include "LuaFunctor.hpp"
#include "../Common/OnlineComponent.h"
#include "CCLuaEngine.h"

USING_NS_CC;

static CMainPlayer* s_MainPlayer = nullptr;
CMainPlayer* CMainPlayer::getInstance()
{
    if (s_MainPlayer == nullptr) {
        s_MainPlayer = new CMainPlayer();
    }
    return s_MainPlayer;
}


CMainPlayer::CMainPlayer()
:m_isLogin(false),
m_uid(0),
m_token(0)
{
}

CMainPlayer::~CMainPlayer()
{
}

void CMainPlayer::Login(std::string& accountid, std::string& m_token,int subareaId)
{
	proto_player_login tmp;
	tmp.serverID = subareaId;
	sprintf_safe(tmp.accountid,accountid.c_str());
	sprintf_safe(tmp.token,m_token.c_str());
	sprintf_safe(tmp.version, m_token.c_str());
	OnlineComponent::instance()->Send(&tmp, sizeof(tmp));

}

void CMainPlayer::Logout()
{
	OnlineComponent::instance()->Disconnect();
	m_isLogin = false;
	m_uid = 0;
	m_token = 0;
    
}

void CMainPlayer::OnLogin(proto_player_login_r *msg)
{
	if (msg->result == proto_player_login_r::R_LOGINOK) {
		m_isLogin = true;
		m_uid = msg->uid;
		m_token = msg->token;
	}
    
	lua_State *L = LuaEngine::getInstance()->getLuaStack()->getLuaState();
	CLuaFunctor functor(L, "OnLogin");
	functor.PushParam(msg->result);
	functor.Execute();
}

void CMainPlayer::OnLogout(proto_player_logout_r *msg)
{
	m_isLogin = false;
    lua_State *L = LuaEngine::getInstance()->getLuaStack()->getLuaState();
	CLuaFunctor functor(L, "OnLogout");
	functor.PushParam(msg->reason);
	functor.Execute();
}


void CMainPlayer::OnLuaCustom(proto_lua_custom *msg, size_t size)
{
    lua_State *L = LuaEngine::getInstance()->getLuaStack()->getLuaState();
	CLuaFunctor functor(L, "DispatchLuaMsg");
	functor.PushParam();
	if (unserializetable(L, msg->data, &size) != SLT_OK)
		return;
	lua_replace(L, -2);
	functor.Execute();
}

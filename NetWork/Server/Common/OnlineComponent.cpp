
#include "pub.h"
#include "platform/CCCommon.h"
#include "SocketOpt.h"
#include "../Message/MainPlayer.h"
#include "system.h"
#include "player.h"
#include "logic.h"
#include "OnlineComponent.h"
#include "LuaCustom.h"
#include "CCLuaEngine.h"
#include "tolua_fix.h"
#include "GlobalLuaFunc.h"
#include "LuaFunctor.hpp"
#include "tinyxml.h"
#include "LuaReader.h"
#include "cocos2d.h"
#include <time.h>
#include "PomeloClient.h"

USING_NS_CC;

static OnlineComponent m_Component;

OnlineComponent *OnlineComponent::instance()
{
	return &m_Component;
}


OnlineComponent::OnlineComponent()
{
	m_serverAddr = "";
    m_public_url = "";
    m_token = "";
	m_serverPort = 5550;
}

OnlineComponent::~OnlineComponent()
{
}

bool OnlineComponent::Init()
{
	//加载脚本
	int ret = LuaEngine::getInstance()->executeString("dofile 'script/init.lua'");
    if (ret != 0)
    {
        CCLOG("loading init lua isSucseess = %d", ret);
        return false;
    }

	return true;
}

struct DataHandle {
	void operator()(const void *data, size_t len) {
		m_Component.DispatchMsg(data, len);
	}
} netMsgHaneler;

void OnlineComponent::mainLoop()
{
	m_NetClientEx.DispatchMsg(netMsgHaneler);
//    PomeloClient::getInstance()->loop();
}


void OnlineComponent::SendReconnect()
{
	if (m_NetClientEx.Connect(m_serverAddr.c_str(), m_serverPort))
	{
		if (g_pPlayer->IsLogin() && g_pPlayer->GetUID() != 0) {
			proto_player_reconnect rec;
			rec.uid = g_pPlayer->GetUID();
			rec.token = g_pPlayer->GetToken();
			m_NetClientEx.PushSendMsg(&rec, sizeof(rec));
		}
		return;
	}
}

void OnlineComponent::RequstReconnect(bool isSuccess)
{
//	if (m_ReconnectTag && m_ReconnectBack)
//	{
//		(m_ReconnectTag->*m_ReconnectBack)(isSuccess);
//	}

}

bool OnlineComponent::judgeReconnect()
{
	if (m_NetClientEx.IsConnected())
	{
//		ReconnectionWindow::closeReconnectionWindow();

		return true;
	}

//	ReconnectionWindow::create();

	return false;
}

//当m_NetClientEx.Connect向某个ip发送了链接请求的时候，会做一段时间的链接尝试，在这期间如果再次链接其他ip，就会直接返回false。
bool OnlineComponent::Send(const void *data, size_t size)
{
	if (!m_NetClientEx.IsConnected()) {

		//以下为一个简单的断线重连，用于完善用户体验以及处理漏掉的断线重连
		if (!m_NetClientEx.Connect(m_serverAddr.c_str(), m_serverPort)) {
			return false;
		}

		if (g_pPlayer->IsLogin() && g_pPlayer->GetUID() != 0) {
			proto_player_reconnect rec;
			rec.uid = g_pPlayer->GetUID();
			rec.token = g_pPlayer->GetToken();
			m_NetClientEx.PushSendMsg(&rec, sizeof(rec));
		}
		m_NetClientEx.PushSendMsg(data, size);
		return true;
	}

	return m_NetClientEx.Send(data, size);
}

void OnlineComponent::DispatchMsg(const void *data, size_t size)
{
	proto_base_s *proto = (proto_base_s *)data;
	switch (proto->_proto) {

	case PROTO_PLAYER_LOGIN:
	{
		proto_player_login_r *msg = (proto_player_login_r *)data;
		g_pPlayer->OnLogin(msg);
	}
		break;
	case PROTO_PLAYER_LOGOUT:
	{
		//一定是必要信息需要玩家退出后重新登录（服务器崩溃了，玩家被T了，玩家在其他地方登录等等）。
		proto_player_logout_r *msg = (proto_player_logout_r *)data;
		g_pPlayer->OnLogout(msg);
		//退回登录界面，给出提示
		g_pPlayer->Logout();
	}
		break;
	case PROTO_PLAYER_BINDACCOUNT:
	{
//		proto_player_bindaccount_r *msg = (proto_player_bindaccount_r *)data;
		//g_pPlayer->OnBindAccount(msg);
	}
		break;
	case PROTO_SYSTEM_LUACUSTOM:
	{
		proto_lua_custom *luamsg = (proto_lua_custom *)data;
		size -= offsetof(proto_lua_custom, data);
        g_pPlayer->OnLuaCustom(luamsg,size);
    }
		break;
		//////////////////////////////////////////////////////////////////////////
		// 逻辑消息测试
		//////////////////////////////////////////////////////////////////////////
	case PROTO_LOGIC_TEST:
	{
		proto_logic_test_r *msg = (proto_logic_test_r *)data;
		//
		msg->result = proto_logic_test_r::R_LOGINOK;
	}
		break;

	default:
		break;
	}
}

void OnlineComponent::SetServerIp(std::string ServerIp, std::string ServerId)
{
	m_serverAddr = ServerIp;
    m_serverId = ServerId; 
}
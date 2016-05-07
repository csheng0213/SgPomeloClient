//#include "CCLuaEngine.h"
#include "NetClientEx.h"
#include "cocos2d.h"
#include "../Message/MainPlayer.h"
#include "OnlineComponent.h"
#include "CCLuaEngine.h"

USING_NS_CC;

CNetClientEx::CNetClientEx()
{

}

CNetClientEx::~CNetClientEx()
{
	RemoveAllMsgInSendQueue();
	while (!m_MsgQueue.Empty()) {
		SNetMsg *pMsg = m_MsgQueue.Front();
		delete pMsg->data;
		delete pMsg;
		m_MsgQueue.Pop();
	}
}

void CNetClientEx::OnConnected()
{
	SendAllMsgInSendQueue();

	//关闭断线重连的显示
//	OnlineComponent::instance()->RequstReconnect(true);
	LuaEngine::getInstance()->executeGlobalFunction("OnConnected");
}

void CNetClientEx::OnConnectFailed()
{
	RemoveAllMsgInSendQueue();

	//请求断线重连
//	OnlineComponent::instance()->RequstReconnect(false);
	LuaEngine::getInstance()->executeGlobalFunction("OnConnectFailed");

}

void CNetClientEx::OnDisconnected()
{
	LuaEngine::getInstance()->executeGlobalFunction("OnDisconnected");
	
	//断线重连
//	if (g_pPlayer->m_bNeddReconnet)
//	{
//		g_pPlayer->ShowMsg();
//	}
//	else
//	{
//		//退回登录界面
//	}
//	g_pPlayer->m_bNeddReconnet = true;
//	
//	cc_timeval now;
//	CCTime::gettimeofdayCocos2d(&now, NULL);
//	CCLog("player OnDisconnected （time:%ld）", now.tv_sec);

	//当服务器崩溃的时候，所有玩家收到OnDisconnected，这里可以进入断线重连，服务器会返回让玩家退出重新登录的消息(R_NOTINSERVER)
	//当服务器网络出现问题的时候，所有玩家一旦进行消息通信的时候就会收到OnDisconnected，这里可以循环进入断线重连，直到连上服务器。
	//当客户端崩溃的时候，重新登录
	//当客户端网络出现问题的时候，玩家收到OnDisconnected，这里可以循环进入断线重连，直到连上服务器。
	//当苹果客户端进入后台，再回到前台的时候，首先检查下链接是否断开，断开了进入断线重连。
}

void CNetClientEx::OnReceived(const void *data, size_t size)
{
	SNetMsg *pMsg = new SNetMsg;
	pMsg->len = size;
	if (data != NULL) {
		pMsg->data = new char[size];
		memcpy_unsafe(pMsg->data, size, data, size);
	}
	else {
		pMsg->data = NULL;
	}
	m_MsgQueue.Push(pMsg);
}

void CNetClientEx::PushSendMsg(const void *data, size_t size)
{
	SNetMsg *pMsg = new SNetMsg;
	pMsg->len = size;
	pMsg->data = new char[size];
	memcpy_unsafe(pMsg->data, size, data, size);
	m_SendMsgQueue.push(pMsg);
}

void CNetClientEx::SendAllMsgInSendQueue()
{
	while (!m_SendMsgQueue.empty()) {
		SNetMsg *pMsg = m_SendMsgQueue.front();
		Send(pMsg->data, pMsg->len);
		delete [] pMsg->data;
		delete pMsg;
		m_SendMsgQueue.pop();
	}
}

void CNetClientEx::RemoveAllMsgInSendQueue()
{
	while (!m_SendMsgQueue.empty()) {
		SNetMsg *pMsg = m_SendMsgQueue.front();
		delete pMsg->data;
		delete pMsg;
		m_SendMsgQueue.pop();
	}
}
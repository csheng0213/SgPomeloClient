#ifndef __MAINWORLD_H__
#define __MAINWORLD_H__

#include "NetClientEx.h"

#include "cocos2d.h"

USING_NS_CC;

#define reconnect_func(_func)  (OnlineComponent::ReconnectBack)(&_func)

class OnlineComponent
{
public:

	using ReconnectBack = void(CCObject::*)(bool isSuccess);

	OnlineComponent();
	~OnlineComponent();

	bool Init();
	void mainLoop();
	bool Send(const void *data, size_t size);

	
	void Disconnect() { m_NetClientEx.Disconnect(); }

	void DispatchMsg(const void *data, size_t size);

	void SetServerIp(std::string ServerIp, std::string ServerId);
	////////////////////////////////////////////////////////////////////////// 断线重连

	void SendReconnect();

	bool judgeReconnect();

	void RequstReconnect(bool isSuccess);
	//////////////////////////////////////////////////////////////////////////

//	void RegisterObserver(OnlineObserver * _observer);
//	
//	void unRegisterObserver();
//	
//	void OnLogout(){ m_ReconnectTag = NULL; };

	//////////////////////////////////////////////////////////////////////////
//	void LoadStaticConfig();
	
	//////////////////////////////////////////////////////////////////////////

	static OnlineComponent* instance();

	ReconnectBack m_ReconnectBack;
    std::string		m_serverId;
    std::string		m_public_url;
    std::string     m_token;
private:
	CNetClientEx	m_NetClientEx;
	std::string		m_serverAddr;
	
	int serverPort;
	int	m_serverPort;
	
//////////////////////////////////////////////////////////////////////////
	//全局配置数据变量区
public:
	int m_tax_price;							//税收元宝价格
	std::string m_test;
	//...
//////////////////////////////////////////////////////////////////////////

};

#endif
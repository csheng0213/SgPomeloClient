#ifndef __MAINPLAYER_H__
#define __MAINPLAYER_H__

#include "system.h"
#include "player.h"
#include "logic.h"
#include "mts_queue.h"

class CMainPlayer {
public:
    static CMainPlayer* getInstance();
    
public:
    void Login(std::string& accountid, std::string& m_token, int subareaId);
    
    void Logout();
    
    void OnLogin(proto_player_login_r *msg);
    
    void OnLogout(proto_player_logout_r *msg);
    
    void OnLuaCustom(proto_lua_custom *msg, size_t size);
    
    bool IsLogin() { return m_isLogin; }
    
    /*-------------------geter-----------------------*/
    uint32_t GetUID() { return m_uid; }
    uint64_t GetToken() { return m_token; }
    
private:
    CMainPlayer();
    ~CMainPlayer();
    

    bool m_isLogin;
    
    uint64_t m_token;
    uint32_t m_uid;
    
    std::string	m_serverId;
    std::string	m_serverIp;
    int	m_serverPort;
    
private:

};

#define g_pPlayer CMainPlayer::getInstance()

#endif
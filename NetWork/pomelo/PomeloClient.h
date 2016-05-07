//
//  PomeloClient.hpp
//  LuaSanGuo
//
//  Created by cs on 16/5/6.
//
//

#ifndef PomeloClient_hpp
#define PomeloClient_hpp

#include <stdio.h>
#include "string"
#include <functional>
#include <stdlib.h>
#include "pomelo.h"

typedef std::function<void(bool)> BackCall_b;
typedef std::function<void(std::string)> BackCall_s;
typedef std::function<void(const pc_request_t* req, int rc, const char* resp)> PomeloRequestCB;
typedef std::function<void(int eventCode, std::string arg1, std::string arg2)> PomeloEventCB;

class PomeloClient
{
public:
    static PomeloClient* getInstance();
    static PomeloRequestCB s_request_cb;
    
public:
    PomeloClient();
    virtual ~PomeloClient();
    
    bool Connection(const std::string& host, int port);
    void DisConnection();
    void loop();
    
    //逻辑
    void request(const std::string& route, const std::string& jsonData, PomeloRequestCB cb);
    void notify(const std::string& route, const std::string& jsonData);
   
    
    //事件监听
    PomeloEventCB getEventCB()
    {
        return m_pomeloEventCB;
    };
    
    void setEventCB(PomeloEventCB eventCB)
    {
        m_pomeloEventCB = eventCB;
    }
    
public:
    pc_client_t* client;
    int handler_id;
    PomeloEventCB m_pomeloEventCB;
};

#endif /* PomeloClient_hpp */

//
//  CHttpClinet.hpp
//  LuaSanGuo
//
//  Created by cs on 16/1/9.
//
//

#ifndef CHttpClinet_hpp
#define CHttpClinet_hpp

#include <stdio.h>
#include <string>
#include <thread>

class CHttpClient
{
public:
    CHttpClient();
    virtual ~CHttpClient(){};
    
    static CHttpClient* getInstance();
    void update();
    
    void sendHttpRequest(const char *url, std::string &str);
public:
    std::string m_data;
    std::string m_curUrl;
    std::string m_param;
    std::thread m_thread;
    int m_errorCode;
    bool m_isHaveNewData;
    bool m_sendingMsg;
};

#endif /* CHttpClinet_hpp */

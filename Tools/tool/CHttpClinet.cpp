//
//  CHttpClinet.cpp
//  LuaSanGuo
//
//  Created by cs on 16/1/9.
//
//

#include "CHttpClinet.hpp"
#include "curl.h"
#include "LuaFunctor.hpp"
#include "CCLuaEngine.h"

static size_t loadFileDataWriteData(void *ptr, size_t size, size_t nmemb, void *ud)
{
    size_t total_size = size * nmemb;
    ((std::string *)ud)->append((char *)ptr, total_size);
    return total_size;
}

int UrlPostRequire(const char *url, std::string &data, std::string param)
{
    if (url == NULL || url[0] == '\0')
        return false;
    CURL *_curl = curl_easy_init();
    
    curl_easy_setopt(_curl, CURLOPT_URL, url); //url地址
    curl_easy_setopt(_curl, CURLOPT_POSTFIELDS, param.c_str()); //post参数
    
    curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, loadFileDataWriteData);
    curl_easy_setopt(_curl, CURLOPT_WRITEDATA, &data);
    curl_easy_setopt(_curl, CURLOPT_NOPROGRESS, true);
    curl_easy_setopt(_curl, CURLOPT_TIMEOUT, 30);   //只需要设置一个秒的数量就可以
    CURLcode res = curl_easy_perform(_curl);
    curl_easy_cleanup(_curl);
    
    return res;
    
}


static CHttpClient * m_chttpClient = nullptr;

CHttpClient::CHttpClient()
{
    m_isHaveNewData = false;
    m_sendingMsg = false;
    
    m_thread = std::thread([this](){
        while (true) {
            if (m_sendingMsg) {
                m_errorCode = UrlPostRequire(m_curUrl.c_str(), m_data, m_param);
                m_isHaveNewData = true;
                m_sendingMsg = false;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

    });

}

CHttpClient* CHttpClient::getInstance()
{
    if (m_chttpClient == nullptr) {
        m_chttpClient = new CHttpClient();
    }
    return  m_chttpClient;
}


void CHttpClient::sendHttpRequest(const char *url, std::string &param)
{
    if (not m_sendingMsg && not m_isHaveNewData) {
        m_sendingMsg = true;
        m_curUrl = url;
        m_param = param;
    }
}

void CHttpClient::update()
{
    if (m_isHaveNewData)
    {
        m_isHaveNewData = false;
        CLuaFunctor functor(cocos2d::LuaEngine::getInstance()->getLuaStack()->getLuaState(), "HttpBackCall");
        functor.PushParam(m_curUrl.c_str());
        functor.PushParam(m_errorCode);
        functor.PushParam(m_data.c_str());
        functor.Execute();
        m_curUrl = "";
        m_data = "";
    }
}

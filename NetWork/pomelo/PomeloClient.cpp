//
//  PomeloClient.cpp
//  LuaSanGuo
//
//  Created by cs on 16/5/6.
//
//

#include "PomeloClient.h"
/**
 * Copyright (c) 2014,2015 NetEase, Inc. and other Pomelo contributors
 * MIT Licensed.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pomelo.h>
#include <pomelo_trans.h>

#include "test.h"

PomeloRequestCB PomeloClient::s_request_cb = nullptr;

#define REQ_ROUTE "connector.entryHandler.entry"
#define REQ_MSG "{\"name\": \"test\"}"
#define REQ_EX ((void*)0x22)
#define REQ_TIMEOUT 10

#define NOTI_ROUTE "test.testHandler.notify"
#define NOTI_MSG "{\"content\": \"test content\"}"
#define NOTI_EX ((void*)0x33)
#define NOTI_TIMEOUT 30

#define EV_HANDLER_EX ((void*)0x44)
#define SERVER_PUSH "onPush"

#ifdef _WIN32
#include <windows.h>
#define SLEEP(x) Sleep(x * 1000);
#else
#include <unistd.h>
#define SLEEP(x) sleep(x)
#endif

static int local_storage_cb(pc_local_storage_op_t op, char* data, size_t* len, void* ex_data)
{
    // 0 - success, -1 - fail
    char buf[1024];
    size_t length;
    size_t offset;
    FILE* f;
    
    if (op == PC_LOCAL_STORAGE_OP_WRITE) {
        f = fopen("/Users/cs/student/pomelo/sanguo/pomelo.dat", "w");
        if (!f) {
            return -1;
        }
        fwrite(data, 1, *len, f);
        fclose(f);
        return 0;
        
    } else {
        f = fopen("/Users/cs/student/pomelo/sanguo/pomelo.dat", "r");
        if (!f) {
            *len = 0;
            return -1;
        }
        *len = 0;
        offset = 0;
        
        while((length = fread(buf, 1, 1024, f))) {
            *len += length;
            if (data) {
                memcpy(data + offset, buf, length);
            }
            offset += length;
        }
        
        fclose(f);
        
        return 0;
    }
}


static void event_cb(pc_client_t* client, int ev_type, void* ex_data, const char* arg1, const char* arg2)
{
    
    PC_TEST_ASSERT(ex_data == EV_HANDLER_EX);
    printf("test: get event %s, arg1: %s, arg2: %s\n", pc_client_ev_str(ev_type),
           arg1 ? arg1 : "", arg2 ? arg2 : "");
    
    if(PomeloClient::getInstance()->getEventCB())
        PomeloClient::getInstance()->getEventCB()(ev_type, arg1, arg2);
}

static void request_cb(const pc_request_t* req, int rc, const char* resp)
{
    PC_TEST_ASSERT(rc == PC_RC_OK);
    PC_TEST_ASSERT(resp);
    PC_TEST_ASSERT(pc_request_client(req) == PomeloClient::getInstance()->client);
    PC_TEST_ASSERT(!strcmp(pc_request_route(req), REQ_ROUTE));
    PC_TEST_ASSERT(!strcmp(pc_request_msg(req), REQ_MSG));
    PC_TEST_ASSERT(pc_request_ex_data(req) == REQ_EX);
    PC_TEST_ASSERT(pc_request_timeout(req) == REQ_TIMEOUT);
    
    if (PomeloClient::s_request_cb) {
        PomeloClient::s_request_cb(req, rc, resp);
    }
}

static void notify_cb(const pc_notify_t* noti, int rc)
{
    PC_TEST_ASSERT(rc == PC_RC_OK);
    
    PC_TEST_ASSERT(pc_notify_client(noti) == PomeloClient::getInstance()->client);
    PC_TEST_ASSERT(!strcmp(pc_notify_route(noti), NOTI_ROUTE));
    PC_TEST_ASSERT(!strcmp(pc_notify_msg(noti), NOTI_MSG));
    PC_TEST_ASSERT(pc_notify_ex_data(noti) == NOTI_EX);
    PC_TEST_ASSERT(pc_notify_timeout(noti) == NOTI_TIMEOUT);
}

PomeloClient* PomeloClient::getInstance()
{
    static PomeloClient m_pomeloClinet;
    return &m_pomeloClinet;
}


PomeloClient::PomeloClient()
:m_pomeloEventCB(nullptr)
{
    pc_lib_init(NULL, NULL, NULL, NULL);
    client = (pc_client_t*)malloc(pc_client_size());
    
    pc_client_config_t config = PC_CLIENT_CONFIG_DEFAULT;
    config.local_storage_cb = local_storage_cb;
    config.enable_polling = 1;
    pc_client_init(client,  (void*)0x11, &config);
    
    PC_TEST_ASSERT(pc_client_ex_data(client) == (void*)0x11);
    PC_TEST_ASSERT(pc_client_state(client) == PC_ST_INITED);
    
    handler_id = pc_client_add_ev_handler(client, event_cb, EV_HANDLER_EX, NULL);
}

PomeloClient::~PomeloClient()
{
    pc_client_disconnect(client);
    pc_client_rm_ev_handler(client, handler_id);
    pc_client_cleanup(client);
    PC_TEST_ASSERT(pc_client_state(client) == PC_ST_NOT_INITED);
    free(client);
    pc_lib_cleanup();
}

/**
 * POMELO连接
 * 网关 8002 （GAT）
 * 账号 8001 （AUTH）
 * 192.168.16.200:8080
 */
bool PomeloClient::Connection(const std::string &host, int port)
{
    const char *h = host.c_str();
    int code = pc_client_connect(client, h, port, NULL);
    return code == 0;
};


//断开连接
void PomeloClient::DisConnection()
{
    //移除监听
    pc_client_rm_ev_handler(client, handler_id);
    //断开连接
    pc_client_disconnect(client);
}


// Pomelo 请求方法
// @param route 路由 "connector.entryHandler.longconn"
// @param JSONStr 参数
void PomeloClient::request(const std::string &route, const std::string &jsonData, PomeloRequestCB cb)
{
    s_request_cb = cb;
    if (client==NULL || route == "" || jsonData == "") {
        pc_lib_cleanup();
        pc_lib_init(NULL, NULL, NULL, NULL);
        
    }else{
        pc_request_with_timeout(client, REQ_ROUTE, REQ_MSG, REQ_EX, REQ_TIMEOUT, request_cb);
//        pc_request_with_timeout(client, route.c_str(), jsonData.c_str(), NULL, REQ_TIMEOUT, request_cb);
//        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 0.3 * NSEC_PER_SEC), dispatch_get_main_queue(), ^{
//            Requested(respData);
//        });
    }

}

void PomeloClient::notify(const std::string &route, const std::string &jsonData)
{
    if (client == NULL) {
        pc_lib_cleanup();
        pc_lib_init(NULL, NULL, NULL, NULL);
    }else
    {
        pc_notify_with_timeout(client, NOTI_ROUTE, NOTI_MSG, NOTI_EX, NOTI_TIMEOUT, notify_cb);
    }
}

void PomeloClient::loop()
{
    pc_client_poll(client);
}


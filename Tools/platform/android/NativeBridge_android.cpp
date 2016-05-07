//
//  NativeBridge_ios.m
//  SimpleGame
//
//  Created by csheng on 15/1/20.
//
#include "cocos2d.h"
#include "LuaFunctor.hpp"
#include "CCLuaEngine.h"

//jni

extern "C"
{
    void Java_org_cocos2dx_lua_NativeUtils_JniBackCall(JNIEnv*  env, jobject thiz, jstring msg )
    {
        const char* msg_hc = env->GetStringUTFChars(msg, NULL);
        CCLOG("jni  call is finishdf: %s %s", msg_hc, "-----------------------------------------------------");
        
        //将数据传入lua环境
        CLuaFunctor functor(cocos2d::LuaEngine::getInstance()->getLuaStack()->getLuaState(), "JniBackCall");
        functor.PushParam(msg_hc);
        functor.Execute();
        
    }
}



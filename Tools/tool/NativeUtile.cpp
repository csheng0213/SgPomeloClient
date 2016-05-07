//
//  NativeUtile.cpp
//  LuaSanGuo
//
//  Created by cs on 16/5/5.
//
//

#include "NativeUtile.hpp"
#include "LuaFunctor.hpp"
#include "cocos2d.h"
#include "CCLuaEngine.h"


void NativeUtile::callLua(const std::string &msg)
{
    CLuaFunctor functor(cocos2d::LuaEngine::getInstance()->getLuaStack()->getLuaState(), "JniBackCall");
    functor.PushParam(msg.c_str());
    functor.Execute();
}
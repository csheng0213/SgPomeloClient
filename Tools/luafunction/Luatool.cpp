//
//  luatool.cpp
//  LuaSanGuo
//
//  Created by waiboziStudio on 15/5/14.
//
//

#include "CCLuaEngine.h"
#include "cocos2d.h"

#include "Luatool.h"

USING_NS_CC;

bool Luatool::getLuaGlobalBoolValue(const char *globalName)
{
    bool value = false;
    lua_State *L = LuaEngine::getInstance()->getLuaStack()->getLuaState();
    lua_getglobal(L, globalName);
    if (lua_isboolean(L, -1)) {
        value = lua_toboolean(L, -1);
    }
    lua_pop(L, 1);
    return value;
}

float Luatool::getLuaGlobalDoubleValue(const char *globalName)
{
    float value = 0.0;
    lua_State *L = LuaEngine::getInstance()->getLuaStack()->getLuaState();
    lua_getglobal(L, globalName);
    if (lua_isnumber(L, -1)) {
        value = lua_tonumber(L, -1);
    }
    lua_pop(L, 1);
    return value;
}
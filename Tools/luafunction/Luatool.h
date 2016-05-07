//
//  luatool.h
//  LuaSanGuo
//
//  Created by waiboziStudio on 15/5/14.
//
//

#ifndef __LuaSanGuo__luatool__
#define __LuaSanGuo__luatool__

#include <stdio.h>

class Luatool {

public:
    static bool getLuaGlobalBoolValue(const char* globalName);
    static float getLuaGlobalDoubleValue(const char* globalName);
};



#endif /* defined(__LuaSanGuo__luatool__) */

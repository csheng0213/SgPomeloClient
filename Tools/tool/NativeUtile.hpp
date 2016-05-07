//
//  NativeUtile.hpp
//  LuaSanGuo
//
//  Created by cs on 16/5/5.
//
//

#ifndef NativeUtile_hpp
#define NativeUtile_hpp

#include <stdio.h>
#include <string>

class NativeUtile
{
public:
    static void callLua(const std::string& msg);
};

#endif /* NativeUtile_hpp */

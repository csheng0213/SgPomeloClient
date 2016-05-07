#ifndef __LUACUSTOM_H__
#define __LUACUSTOM_H__

#include "CCLuaEngine.h"

class CLuaCustom {
public:
	CLuaCustom();
	~CLuaCustom();

	static CLuaCustom *getInstance();

	void Init();

	void RegisterAllLuaFunc();

private:
	lua_State *m_State;
};

#endif
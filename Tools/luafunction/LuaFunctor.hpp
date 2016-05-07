#ifndef __LUAFUNCTOR_H__
#define __LUAFUNCTOR_H__

extern "C" {
#	include "lua.h"
#	include "lualib.h"
#	include "lauxlib.h"
#	include "luaex.h"
};

#ifndef LOG
#	define LOG printf
#endif

#include <array>

class CLuaFunctor {
public:
	CLuaFunctor(lua_State *L, const char *func)
		: m_ParamCnt(0)
		, m_isValid(true)
	{
		m_State = L;
		m_StackTop = lua_gettop(L);
		lua_getglobal(L, func);
		if (!lua_isfunction(L , -1)) {
			lua_settop(L, m_StackTop);
			m_isValid = false;
		}
	}

	CLuaFunctor(lua_State *L, int nRef)
		: m_ParamCnt(0)
		, m_isValid(true)
	{
		m_State = L;
		m_StackTop = lua_gettop(L);
		lua_getref(L, nRef);
		if (!lua_isfunction(L , -1)) {
			lua_settop(L, m_StackTop);
			m_isValid = false;
		}
	}

	~CLuaFunctor() {
		lua_settop(m_State, m_StackTop);
	}

	void PushParam() {
		lua_pushnil(m_State);
		m_ParamCnt++;
	}

	void PushParam(lua_Number val) {
		lua_pushnumber(m_State, val);
		m_ParamCnt++;
	}

	void PushParam(const char *str) {
		lua_pushstring(m_State, str);
		m_ParamCnt++;
	}

	void PushParam(const void *data, size_t len) {
		lua_pushlstring(m_State, (char *)data, len);
		m_ParamCnt++;
	}

	bool Execute() {
		if (!m_isValid) {
			lua_settop(m_State, m_StackTop);
			return false;
		}
		if (lua_pcall(m_State, m_ParamCnt, 0, NULL) != 0) {
			//LOG(lua_tostring(m_State, -1));
			lua_settop(m_State, m_StackTop);
			return false;
		}
		lua_settop(m_State, m_StackTop);
		return true;
	}

	int ExecuteReturnOne(double &ret) {
		if (!m_isValid) {
			lua_settop(m_State, m_StackTop);
			return false;
		}
		//只返回一个参数
		int err = lua_pcall(m_State, m_ParamCnt, 1, NULL);
		if (err != 0) {
            //LOG(str.c_str());
			lua_settop(m_State, m_StackTop);
			return false;
		}
		
		if (!lua_isnumber(m_State, -1))
		{
			lua_settop(m_State, m_StackTop);
			return false;
		}
		ret = (double)lua_tonumber(m_State, -1);
		lua_pop(m_State, 1);//弹出结果

		lua_settop(m_State, m_StackTop);

		return true;
	}

	int ExecuteReturnOne(std::string &ret) {
		if (!m_isValid) {
			lua_settop(m_State, m_StackTop);
			return false;
		}
		//只返回一个参数
		int err = lua_pcall(m_State, m_ParamCnt, 1, NULL);
		if (err != 0) {
			if (lua_isstring(m_State, -1))
			{
				//LOG(str.c_str());
			}
			lua_settop(m_State, m_StackTop);
			return false;
		}

		if (!lua_isstring(m_State, -1))
		{
			lua_settop(m_State, m_StackTop);
			return false;
		}
		size_t len;
		const char* str = (lua_tolstring(m_State, -1, &len));
	

		lua_pop(m_State, 1);//弹出结果

		lua_settop(m_State, m_StackTop);

		return true;
	}
    
    template <typename T, int N>
    bool ExcuteReturnMultiDouble(std::array<T, N> &rets) {
		if (!m_isValid) {
			lua_settop(m_State, m_StackTop);
			return false;
		}

        
		//返回N个参数
		int err = lua_pcall(m_State, m_ParamCnt, N, NULL);
		if (err != 0) {
            //LOG(str.c_str());
			lua_settop(m_State, m_StackTop);
			return false;
		}
        
        for (int i = 0; i < N; i++)
        {
		    if (!lua_isnumber(m_State, -(i+1)))
		    {
			    lua_settop(m_State, m_StackTop);
			    return false;
		    }

		    rets[N-i-1] = (T)lua_tonumber(m_State, -(i+1));
        }

		lua_pop(m_State, N);//弹出结果

        return true;
    }

private:
	lua_State *m_State;
	uint32_t m_ParamCnt;
	int32_t m_StackTop;
	bool m_isValid;
};

#endif
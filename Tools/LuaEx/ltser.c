#define ltser_c
#define LUA_CORE

#include "ltser.h"
#include <string.h>


enum LuaValueType {
	LVT_BOOLEAN,
	LVT_NUMBER,
	LVT_STRING,
	LVT_TABLE,
	LVT_USERDATA,
	LVT_UINT32,
};

typedef struct _KVType {
	unsigned char kt:1;
	unsigned char kl:2;
	unsigned char vt:3;
	unsigned char vl:2;
} KVType;

int IsIEEEPositiveInteger4B(double value)
{
	unsigned long long temp = *(unsigned long long *)&value;
	int exp = ((int)(temp >> 52) & 0x7ff) - 1023;

	if (temp == 0 || temp == 0x8000000000000000)	// zero
		return 1;

	if (temp & 0x8000000000000000)	// negative number
		return 0;

	if (exp < 0 || exp > 31)	// decimal or more than 4 bytes
		return 0;

	if (temp & (( ((unsigned long long)1) << (52 - exp) ) - 1))	// decimal
		return 0;

	return 1;
}

// serialize a lua table to the destination buffer at the given index
LUA_API int serializetable(lua_State *L, int n, void *dst, size_t *size)
{
	int top = lua_gettop(L);

	KVType *kvt;
	int kt, vt;
	size_t kl, vl;
	unsigned int nkey, nval;
	const char *skey, *sval;
	lua_Number number;
	size_t i, j;
	int ret;

	char *cur = (char *)dst;
	char *end = cur + *size;

	if (n < 0) {	// convert to positive
		n = n + top + 1;
	}

	if (!lua_istable(L, n))
		return SLT_NeedTable;

#define ERROR_RETURN(code) do { *size = cur - (char *)dst; lua_settop(L, top); return code; } while(0);
#define CHECK_BUFFER_SIZE(size) do { if (cur + size > end) ERROR_RETURN(SLT_BufferTooSmall); } while(0);
#define GET_UINT32_BYTES(ret, val) do { \
	if (!(val & 0xffffff00)) \
		ret = 0; \
	else if (!(val & 0xffff0000)) \
		ret = 1; \
	else if (!(val & 0xff000000)) \
		ret = 2; \
	else \
		ret = 3; } while (0);

	lua_pushnil(L);
	while (lua_next(L, n)) {
		// kv header
		CHECK_BUFFER_SIZE(1);
		kvt = (KVType *)cur;
		*cur++ = 0;

		// serializing key
		kt = lua_type(L, -2);
		if (kt == LUA_TNUMBER) {
			kvt->kt = 0;
			number = lua_tonumber(L, -2);
			if (!IsIEEEPositiveInteger4B(number))
				ERROR_RETURN(SLT_KeyOutOfRange);
			nkey = (unsigned int)number;
			GET_UINT32_BYTES(kvt->kl, nkey);

			CHECK_BUFFER_SIZE(kvt->kl + 1);
			memcpy(cur, &nkey, kvt->kl + 1);
			cur += (kvt->kl + 1);
		}
		else if (kt == LUA_TSTRING) {
			kvt->kt = 1;
			skey = lua_tolstring(L, -2, &kl);
			if (kl > 0xffffffff)
				ERROR_RETURN(SLT_KeyTooLong);
			GET_UINT32_BYTES(kvt->kl, kl);

			CHECK_BUFFER_SIZE(kvt->kl + 1 + kl);
			memcpy(cur, &kl, kvt->kl + 1);
			cur += (kvt->kl + 1);
			memcpy(cur, skey, kl);
			cur += kl;
		}
		else {
			ERROR_RETURN(SLT_KeyTypeError);
		}

		// serializing value
		vt = lua_type(L, -1);
		if (vt == LUA_TBOOLEAN) {
			kvt->vt = LVT_BOOLEAN;
			kvt->vl = lua_toboolean(L, -1) ? 1 : 0;	// vl is the real boolean value
		}
		else if (vt == LUA_TNUMBER) {
			lua_Number number = lua_tonumber(L, -1);
			if (IsIEEEPositiveInteger4B(number)) {
				kvt->vt = LVT_UINT32;
				nval = (unsigned int)number;
				GET_UINT32_BYTES(kvt->vl, nval);

				CHECK_BUFFER_SIZE(kvt->vl + 1);
				memcpy(cur, &nval, kvt->vl + 1);
				cur += (kvt->vl + 1);
			}
			else {
				kvt->vt = LVT_NUMBER;
				kvt->vl = 0;
				sval = (char *)&number;
				vl = sizeof(number);
				CHECK_BUFFER_SIZE(vl);
				for (i = 0, j = sizeof(number) - 1; i < vl; i++)
					*cur++ = sval[j--];

				if (vl < sizeof(number)) {
					CHECK_BUFFER_SIZE(1);
					*cur++ = 0;
				}
			}
		}
		else if (vt == LUA_TSTRING || vt == LUA_TUSERDATA) {
			size_t vl = 0;
			const void *value = NULL;
			if (vt == LUA_TSTRING) {
				kvt->vt = LVT_STRING;
				value = lua_tolstring(L, -1, &vl);
			}
			else {
				kvt->vt = LVT_USERDATA;
				vl = lua_objlen(L, -1);
				value = lua_touserdata(L, -1);
			}
			if (vl > 0xffffffff)
				ERROR_RETURN(SLT_ValueTooLong);
			GET_UINT32_BYTES(kvt->vl, vl);

			CHECK_BUFFER_SIZE(kvt->vl + 1 + vl);
			memcpy(cur, &vl, kvt->vl + 1);
			cur += (kvt->vl + 1);
			memcpy(cur, value, vl);
			cur += vl;
		}
		else if (vt == LUA_TTABLE) {
			kvt->vt = LVT_TABLE;
			kvt->vl = 0;
			vl = end - cur;
			ret = serializetable(L, -1, cur, &vl);
			cur += vl;
			if (ret != SLT_OK)
				ERROR_RETURN(ret);
		}
		else {
			ERROR_RETURN(SLT_ValueTypeError);
		}

		lua_pop(L, 1);
	}

	CHECK_BUFFER_SIZE(1);
	kvt = (KVType *)cur++;		// end of table
	kvt->kt = 0;
	kvt->kl = 0;
	kvt->vt = LVT_TABLE;
	kvt->vl = 1;

#undef GET_UINT32_BYTES
#undef CHECK_BUFFER_SIZE
#undef ERROR_RETURN

	*size = cur - (char *)dst;
	return SLT_OK;
}

// unserialize the source buffer to a lua table and pushes it onto the stack
LUA_API int unserializetable(lua_State *L, const void *src, size_t *size)
{
	int top = lua_gettop(L);

	KVType *kvt;
	size_t kl, vl;
	unsigned int nkey, nval;
	char *temp;
	void *uval;
	lua_Number number;
	size_t i;
	int ret;

	const char *cur = (char *)src;
	const char *end = cur + *size;

#define ERROR_RETURN(code) do { *size = cur - (const char *)src; lua_settop(L, top); return code; } while(0);
#define CHECK_BUFFER_SIZE(size) do { if (cur + size > end) ERROR_RETURN(SLT_BufferError); } while(0);

	lua_newtable(L);	// new table t
	while (1) {
		CHECK_BUFFER_SIZE(1);
		kvt = (KVType *)cur++;
		if (kvt->vt == LVT_TABLE && kvt->vl == 1) {
			*size = cur - (const char *)src;
			return SLT_OK;
		}

		// get key
		switch (kvt->kt) {
		case 0:	// number key
			{
				CHECK_BUFFER_SIZE(kvt->kl + 1);
				nkey = 0;
				memcpy(&nkey, cur, kvt->kl + 1);
				cur += (kvt->kl + 1);
				lua_pushnumber(L, (lua_Number)nkey);
			}
			break;
		case 1:	// string key
			{
				CHECK_BUFFER_SIZE(kvt->kl + 1);
				kl = 0;
				memcpy(&kl, cur, kvt->kl + 1);
				cur += (kvt->kl + 1);
				CHECK_BUFFER_SIZE(kl);
				lua_pushlstring(L, cur, kl);
				cur += kl;
			}
			break;
		default:
			ERROR_RETURN(SLT_KeyTypeError);
		}

		// get value
		switch (kvt->vt) {
		case LVT_BOOLEAN:
			{
				lua_pushboolean(L, kvt->vl);
			}
			break;
		case LVT_UINT32:
			{
				CHECK_BUFFER_SIZE(kvt->vl + 1);
				nval = 0;
				memcpy(&nval, cur, kvt->vl + 1);
				cur += (kvt->vl + 1);
				lua_pushnumber(L, (lua_Number)nval);
			}
			break;
		case LVT_NUMBER:
			{
				number = 0;
				temp = (char *)&number;
				temp += sizeof(number);
				for (i = 0; i < sizeof(number); i++) {
					CHECK_BUFFER_SIZE(1);
					*--temp = *cur++;
				}
				lua_pushnumber(L, number);
			}
			break;
		case LVT_STRING:
			{
				CHECK_BUFFER_SIZE(kvt->vl + 1);
				vl = 0;
				memcpy(&vl, cur, kvt->vl + 1);
				cur += (kvt->vl + 1);
				CHECK_BUFFER_SIZE(vl);
				lua_pushlstring(L, cur, vl);
				cur += vl;
			}
			break;
		case LVT_USERDATA:
			{
				CHECK_BUFFER_SIZE(kvt->vl + 1);
				vl = 0;
				memcpy(&vl, cur, kvt->vl + 1);
				cur += (kvt->vl + 1);
				CHECK_BUFFER_SIZE(vl);
				uval = lua_newuserdata(L, vl);
				memcpy(uval, cur, vl);
				cur += vl;
			}
			break;
		case LVT_TABLE:
			{
				if (kvt->vl == 0) {
					vl = end - cur;
					ret = unserializetable(L, cur, &vl);
					cur += vl;
					if (ret != SLT_OK)
						ERROR_RETURN(ret);
				}
				else {
					ERROR_RETURN(SLT_DataError);
				}
			}
			break;
		default:
			ERROR_RETURN(SLT_ValueTypeError);
		}

		lua_settable(L, -3);
	}

#undef CHECK_BUFFER_SIZE
#undef ERROR_RETURN

	*size = cur - (const char *)src;
	return SLT_OK;
}
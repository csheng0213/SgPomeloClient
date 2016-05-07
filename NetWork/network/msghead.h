#ifndef __MSGHEAD_H__
#define __MSGHEAD_H__

const size_t MAX_MSG_LEN = 0xffff;

struct SMsgHead {
	uint32_t length : 16;
	uint32_t compressed : 1;
	uint32_t encrypted : 1;
	uint32_t ping : 2;
	uint32_t __dummy : 12;
};



#define EXPIRED_TIME_CS 16000
#define PING_INTERVAL_CS 5000

#define PROTO_BREAK_CS 0
#define PROTO_PING_CS 1
#define PROTO_PONG_CS 2


#define IS_CS_MSG(name, proto) \
	inline bool Is##name##Msg(struct SMsgHead *msg) { \
		if (msg == NULL) \
			return false; \
		if (msg->length != 0) \
			return false; \
		if (msg->ping != (uint32_t)proto) \
			return false; \
		return true; \
	}

IS_CS_MSG(Break, PROTO_BREAK_CS)
IS_CS_MSG(Ping, PROTO_PING_CS)
IS_CS_MSG(Pong, PROTO_PONG_CS)

#endif
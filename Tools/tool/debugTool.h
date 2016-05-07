#ifndef _DEBUG_TOOL_H_
#define _DEBUG_TOOL_H_

#include<chrono>
#include<map>

typedef std::map<std::string, std::chrono::time_point<std::chrono::system_clock>> StrTimeMap;

class DebugTool
{
public:
	static std::chrono::time_point<std::chrono::system_clock> now;
	static StrTimeMap nowMap;

	static void printCurTime();
	static void printCurTime(std::string str);
	static void printCurDelay(std::string str);
	static void holdTime(std::string str);
	static void holdDelay(std::string str);

	static void printPlayerMsg(int uid, std::string str);
};


#endif
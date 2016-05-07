#include "debugTool.h"

#include <time.h>
#include <ctime>
#include <string>
#include <sstream>


std::chrono::time_point<std::chrono::system_clock> DebugTool::now;
StrTimeMap DebugTool::nowMap;

std::string TimeToString(std::time_t time)
{

	char tmp[64];
//	struct tm timeTm;
//	localtime_s(&timeTm, &time);
//     std::time_t end_time = std::chrono::system_clock::to_time_t(end);
	strftime(tmp, sizeof(tmp), "%Y-%m-%d %X", std::localtime(&time));
	return tmp;
}

void DebugTool::printCurTime()
{
	now = std::chrono::system_clock::now();
	std::string timeStr = TimeToString(std::chrono::system_clock::to_time_t(now));
	printf(" CurTime is %s\n", timeStr.c_str());
}

void DebugTool::printCurTime(std::string str)
{
	now = std::chrono::system_clock::now();
	std::string timeStr = TimeToString(std::chrono::system_clock::to_time_t(now));
	printf("%s : CurTime is %s\n", str.c_str(), timeStr.c_str());
}


void DebugTool::printCurDelay(std::string str)
{
	StrTimeMap::iterator it = nowMap.find(str);
	if (it == nowMap.end())
	{
		nowMap[str] = std::chrono::system_clock::now();

		printf("%s : begin Count \n", str.c_str());
		return;
	}

	std::chrono::time_point<std::chrono::system_clock> curTime;
	curTime = std::chrono::system_clock::now();

	std::chrono::duration<double> delay = curTime - it->second;
	it->second = curTime;

	std::stringstream sstr;
	//--------intתstring-----------
	std::string  str1;
	sstr << delay.count();
	sstr >> str1;

	printf("%s : lastTimeDelay is %s\n", str.c_str(), str1.c_str());
}

void DebugTool::holdTime(std::string str)
{
	StrTimeMap::iterator it = nowMap.find(str);
	if (it == nowMap.end())
	{
		nowMap[str] = std::chrono::system_clock::now();
		return;
	}

	it->second = std::chrono::system_clock::now();
}

void DebugTool::holdDelay(std::string str)
{
	StrTimeMap::iterator it = nowMap.find(str);
	if (it != nowMap.end())
	{
		std::chrono::time_point<std::chrono::system_clock> curTime;
		curTime = std::chrono::system_clock::now();

		std::chrono::duration<double> delay = curTime - it->second;

		std::string delayStr = "";
		if (delay.count() > 3)
		{
			delayStr = "DelayMax";
		}
		std::stringstream sstr;
		//--------intתstring-----------
		std::string  str1;
		sstr << delay.count();
		sstr >> str1;

		printf("%s : lastHoldDelay is %s Time: %s\n", str.c_str(), str1.c_str(), delayStr.c_str());
	}
}

void DebugTool::printPlayerMsg(int uid, std::string msg)
{
	printf("uid: %d : msgstream is %s \n", uid, msg.c_str());
}

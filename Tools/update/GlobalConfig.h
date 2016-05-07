#ifndef __GLOBALCONFIG_H__
#define __GLOBALCONFIG_H__

#define BIG_VERSION "3"
#define VERSION_COUNT 9

class GlobalConfig {
public:
	static bool InitConfig(std::string& serverUrl, int midVersion, int minVerison);
    static bool initData();
    static bool writeVersion(const char* version);
    static void writeVersionHandler(int midVersion, int minVersion);
    
	static std::string resourceUrl;
	static uint32_t localResVer;
    static uint32_t localBigResVer;
    static uint32_t localMidResVer;

	static std::string accountIP;
	static uint32_t accountPort;

	static std::string serverIP;
	static uint32_t serverPort;
	static uint32_t serverID;
	static bool useLocalID;

	static uint32_t sendPing;
};

#endif
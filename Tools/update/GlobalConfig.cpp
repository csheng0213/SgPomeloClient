
#include "cocos2d.h"
#include "GlobalConfig.h"
#include "../tinyXml/tinyxml.h"
#include <memory>

USING_NS_CC;

std::string GlobalConfig::resourceUrl;
uint32_t GlobalConfig::localResVer;
uint32_t GlobalConfig::localBigResVer;
uint32_t GlobalConfig::localMidResVer;

std::string GlobalConfig::accountIP;
uint32_t GlobalConfig::accountPort = 0;

std::string GlobalConfig::serverIP;
uint32_t GlobalConfig::serverPort = 0;
uint32_t GlobalConfig::serverID = 0;
bool GlobalConfig::useLocalID = false;

uint32_t GlobalConfig::sendPing = 1;

bool GlobalConfig::writeVersion(const char *version)
{
    std::string writePath = FileUtils::getInstance()->getWritablePath();
    
    std::string path = writePath + "version.ini";
    
    FILE *f = fopen(path.c_str(), "wb");
    if (f == NULL)
        return false;
    fwrite(version, std::strlen(version), 1, f);
    fflush(f);
    fclose(f);
    
    return true;
}

bool GlobalConfig::InitConfig(std::string &serverUrl, int midVersion, int minVersion)
{
	resourceUrl = serverUrl;
    localMidResVer = midVersion;
    localResVer = minVersion;
    return true;
}

void GlobalConfig::writeVersionHandler(int midVersion, int minVersion)
{
    std::string str = std::string(BIG_VERSION);
    
    std::stringstream stream;
    std::string  version_temp;
    
    stream << midVersion;
    stream >> version_temp;
    
    str = str + "." + version_temp;
    
    std::stringstream stream1;
    std::string  minVersion_temp;
    stream1 << minVersion;
    stream1 >> minVersion_temp;
    
    str = str + "." + minVersion_temp;
    
    writeVersion(str.c_str());

}

bool GlobalConfig::initData()
{
    localBigResVer = std::atoi(BIG_VERSION);
//
//    std::string fullpath = "";
//    fullpath = FileUtils::getInstance()->getWritablePath();
//    CCLOG("GlobalConfig writablepath = %s", fullpath.c_str());
//    
//    std::string path = fullpath + "version.ini";
//    const char *pBuffpBuff = FileUtils::getInstance()->getStringFromFile(path).c_str();
//    CCLOG("GlobalConfig version data = %s", pBuffpBuff);
//    if(pBuffpBuff != NULL && std::strlen((const char*)pBuffpBuff) < 30 && std::strlen((const char*)pBuffpBuff) >= 1)
//    {
//        __String str((const char*)pBuffpBuff);
//        auto versionList = str.componentsSeparatedByString(".");
//        
//        __String * bigVersionStr = (__String*)versionList->getObjectAtIndex(0);
//        __String * midVersionStr = (__String*)versionList->getObjectAtIndex(1);
//        __String * versionStr = (__String*)versionList->getObjectAtIndex(2);
//        
//        if( localBigResVer > bigVersionStr->intValue())
//        {
//            GlobalConfig::writeVersionHandler( localMidResVer, localResVer);
//        }
//        else if( localBigResVer == bigVersionStr->intValue())
//        {
//            if( localMidResVer > midVersionStr->intValue())
//            {
//                GlobalConfig::writeVersionHandler( localMidResVer, localResVer);
//            }
//            else if( localMidResVer == midVersionStr->intValue() )
//            {
//                if( localResVer >= versionStr->intValue() )
//                {
//                    GlobalConfig::writeVersionHandler( localMidResVer, localResVer);
//                }else
//                {
//                    localBigResVer = bigVersionStr->intValue();
//                    localMidResVer = midVersionStr->intValue();
//                    localResVer = versionStr->intValue();
//                }
//            }else
//            {
//                localBigResVer = bigVersionStr->intValue();
//                localMidResVer = midVersionStr->intValue();
//                localResVer = versionStr->intValue();
//            }
//        }else
//        {
//            localBigResVer = bigVersionStr->intValue();
//            localMidResVer = midVersionStr->intValue();
//            localResVer = versionStr->intValue();
//        }
//    }
//    else
//    {
//        GlobalConfig::writeVersionHandler( localMidResVer, localResVer);
//    }
    
    return true;
}


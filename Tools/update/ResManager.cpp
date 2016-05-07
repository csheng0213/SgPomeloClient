
#include "pub.h"
#include "CCLuaEngine.h"
#include "ResManager.h"
#include "../tinyXml/tinyxml.h"
#include "GlobalConfig.h"
#include "cocos2d.h"
#include "Luatool.h"

#ifdef MINIZIP_FROM_SYSTEM
#include <minizip/unzip.h>
#else // from our embedded sources
#include "unzip.h"
#endif

#define BUFFER_SIZE    8192
#define MAX_FILENAME   512

USING_NS_CC;
using namespace std;

//字符串分割函数
std::vector<std::string> split(std::string str,std::string pattern)
{
    std::string::size_type pos;
    std::vector<std::string> result;
    str+=pattern;//扩展字符串以方便操作
    int size=str.size();
    
    for(int i=0; i<size; i++)
    {
        pos=str.find(pattern,i);
        if(pos<size)
        {
            std::string s=str.substr(i,pos-i);
            result.push_back(s);
            i=pos+pattern.size()-1;
        }
    }
    return result;
}

static char __optPathChar(char c) {
	if (c >= 'A' && c <= 'Z')
		return c + ('a' - 'A');
	if (c == '\\')
		return '/';
	return c;
}

// 以二进制写方式打开文(自动创建不存在的目录)
FILE *openfile(std::string path)
{
	if (path == "")
		return NULL;
    
	if (path[path.size() - 1] == '/' || path[path.size() - 1] == '\\')
		return NULL;
    std::string fullpath = "";
    fullpath = FileUtils::getInstance()->getWritablePath();
    CCLOG("openfile writablepath = %s", fullpath.c_str());
    path = CResManager::getInstance()->getUpdateFilePath() + path;
	size_t pos = fullpath.size() + 1;
	while (true) {
		pos = path.find('/', pos);
		if (pos != std::string::npos) {
			std::string _temp = path.substr(0, pos);
			MKDIR(path.substr(0, pos).c_str());
			pos++;
		}
		else
			break;
	}
    
	return fopen(path.c_str(), "wb");
}

void *updateResources(void *data)
{
	ThreadHelper thread;
	thread.createAutoreleasePool();
	CResManager *pResManager = (CResManager *)data;
	pResManager->CheckVersion();
	return NULL;
}

CResManager *CResManager::getInstance()
{
	static CResManager s_ResManager;
	return &s_ResManager;
}

bool CResManager::isEnableUpdate()
{
    auto curMidVersion = Luatool::getLuaGlobalDoubleValue("CurrentMidVersion");
    auto curMinVersion = Luatool::getLuaGlobalDoubleValue("CurrentMinVersion");
    
    auto curBigResVer = std::atoi(BIG_VERSION);
    
    std::string fullpath = "";
    fullpath = FileUtils::getInstance()->getWritablePath();
    CCLOG("GlobalConfig writablepath = %s", fullpath.c_str());
    
    std::string path = fullpath + "version.ini";
    const char *pBuffpBuff = FileUtils::getInstance()->getStringFromFile(path).c_str();
    
    CCLOG("isEnableUpdate data = %s", pBuffpBuff);
    
    if(pBuffpBuff != NULL && std::strlen((const char*)pBuffpBuff) < 30 && std::strlen((const char*)pBuffpBuff) >= 3)
    {
        __String str((const char*)pBuffpBuff);
        auto versionList = str.componentsSeparatedByString(".");
        
        __String * bigVersionStr = (__String*)versionList->getObjectAtIndex(0);
        __String * midVersionStr = (__String*)versionList->getObjectAtIndex(1);
        __String * versionStr = (__String*)versionList->getObjectAtIndex(2);
        
        if( curBigResVer > bigVersionStr->intValue())
        {
            return false;
        }
        else if( curBigResVer == bigVersionStr->intValue())
        {
            if( curMidVersion > midVersionStr->intValue())
            {
                return false;
            }
            else if( curMidVersion == midVersionStr->intValue() )
            {
                if( curMinVersion >= versionStr->intValue() )
                {
                    return false;
                }else
                {
                    return true;
                }
            }else
            {
                return true;
            }
        }else
        {
            return true;
        }
    }
    else
    {
        return false;
    }
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CResManager::CResManager()
: m_ResLoadState(RLS_NONE)
, m_LocalVersion(0)
,m_localBigVersion(0)
,m_restartclient(false)
, m_NewestVersion(0)
,m_updatePath("")
,m_isUpdateSeachPath(false)
{
	memset(&m_tid, 0, sizeof(m_tid));
}

CResManager::~CResManager()
{
}

bool CResManager::init()
{
    if(GlobalConfig::initData())
    {
        m_ResLoadState = RLS_NONE;
        m_LocalVersion = GlobalConfig::localResVer;
        m_localMidVersion = GlobalConfig::localMidResVer;
        m_localBigVersion = GlobalConfig::localBigResVer;
        m_restartclient = false;
        m_NewestVersion = 0;
        m_UpdateInfo.curVersion = m_LocalVersion;
        
        m_updatePath = FileUtils::getInstance()->getWritablePath() + "up/";
        
        return true;
    }
    return  false;
}

void CResManager::SetResLoadState(ResLoadState us)
{
	m_ResLoadState = us;
}

void CResManager::UpdateAndLoadResources()
{
    if(init())
        pthread_create(&m_tid, NULL, updateResources, this);
}

// load file data from http server
static size_t loadFileDataWriteData(void *ptr, size_t size, size_t nmemb, void *ud)
{
	size_t total_size = size * nmemb;
	((std::string *)ud)->append((char *)ptr, total_size);
	return total_size;
}
bool CResManager::LoadFileData(const char *url, std::string &str)
{
	if (url == NULL || url[0] == '\0')
		return false;
    
	CURL *_curl = curl_easy_init();
	curl_easy_setopt(_curl, CURLOPT_URL, url);
	curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, loadFileDataWriteData);
	curl_easy_setopt(_curl, CURLOPT_WRITEDATA, &str);
	curl_easy_setopt(_curl, CURLOPT_NOPROGRESS, true);
    curl_easy_setopt(_curl, CURLOPT_TIMEOUT,30);   //只需要设置一个秒的数量就可以
	CURLcode res = curl_easy_perform(_curl);
	curl_easy_cleanup(_curl);
    
	return res == 0;
}

// download file from http server
static size_t downLoadFileWriteData(void *ptr, size_t size, size_t nmemb, void *ud)
{
	return fwrite(ptr, size, nmemb, (FILE *)ud);
}

int downLoadFileProgress(void *ptr, double totalToDownload, double nowDownloaded, double totalToUpLoad, double nowUpLoaded)
{
	CResManager *pResManager = (CResManager *)ptr;
	pResManager->m_UpdateInfo.curDownloadFileProgress = (float)(nowDownloaded / totalToDownload);
    pResManager->m_UpdateInfo.totalToDownload = totalToDownload;
    pResManager->m_UpdateInfo.nowDownloaded = nowDownloaded;
	return 0;
}

bool CResManager::DownLoadFile(const char *url, const char *file)
{
    
	if (url == NULL || url[0] == '\0')
		return false;
	if (file == NULL || file[0] == '\0')
		return false;
    
	FILE *fp = openfile(file);
	if (fp == NULL) {
        
		return false;
	}
    
	CURL *_curl = curl_easy_init();
	curl_easy_setopt(_curl, CURLOPT_URL, url);
	curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, downLoadFileWriteData);
	curl_easy_setopt(_curl, CURLOPT_WRITEDATA, fp);
	curl_easy_setopt(_curl, CURLOPT_NOPROGRESS, false);
	curl_easy_setopt(_curl, CURLOPT_PROGRESSFUNCTION, downLoadFileProgress);
	curl_easy_setopt(_curl, CURLOPT_PROGRESSDATA, this);
	CURLcode res = curl_easy_perform(_curl);
	curl_easy_cleanup(_curl);
    
	if (res != 0) {
		fclose(fp);
		return false;
	}
	fclose(fp);
    
    if(!uncompress(file))
    {
        SetResLoadState(RLS_UNCOMPRESS_FAILED);
        return false;
    }
    
    storeVersion();
	return true;
}


void CResManager::storeVersion()
{
    __String out;
    out.initWithFormat("%d.%d.%d", m_localBigVersion, m_localMidVersion, m_LocalVersion);
    std::string fullpath = "";
    fullpath = FileUtils::getInstance()->getWritablePath();
    CCLOG("CResManager writablepath = %s ", fullpath.c_str());
    std::string path = fullpath + "version.ini";
    FILE *f = fopen(FileUtils::getInstance()->getSuitableFOpen(path).c_str(), "wb");
    if (f == NULL)
        return;
    
    std::string writeData = out.getCString();
    writeData = writeData + " ";
    int count = (int)fwrite(writeData.c_str(), 1, writeData.length(), f);
    CCLOG("CResManager data = %s length = %d writeLength = %d ",writeData.c_str(), (int)writeData.length(), count);
    fflush(f);
    fclose(f);
    
    if(not m_isUpdateSeachPath)
    {
        std::string searchRootPath = CResManager::getInstance()->getUpdateFilePath();
        std::string resPath1 = searchRootPath + "res/";
        std::string srcPath1 = searchRootPath + "src/";
        FileUtils::getInstance()->addSearchPath(resPath1, true);
        FileUtils::getInstance()->addSearchPath(srcPath1, true);

        FileUtils::getInstance()->purgeCachedEntries();
        m_isUpdateSeachPath = true;
    }

}

// 版本检查
void CResManager::CheckVersion()
{
	if (m_ResLoadState != RLS_NONE)
		return;
    
	SetResLoadState(RLS_CHECKVERSION);
    
	std::string url =/* m_resourceUrl;*/GlobalConfig::resourceUrl;
    CCLOG("check version get update.txt begin url : %s", GlobalConfig::resourceUrl.c_str());
	url.append("update.txt");
	std::string data;
	if (!LoadFileData(url.c_str(), data)) {
		SetResLoadState(RLS_CHECKVERSIONFAILED);
		return;
	}
    
	/*pub::CINIParser ini;
     ini.parsemem(data.c_str(), data.length());
     
     if (!ini.get("RES", "VER", (int32_t *)&m_NewestVersion)) {
     SetResLoadState(RLS_CHECKVERSIONFAILED);
     return;
     }*/
    
    CCLOG("check version get update.txt");
    
    auto versionList = split(data, ".");
    
     CCLOG("check version get update.txt data : %s", data.c_str());
    if( versionList.size() != 3 )
    {
        SetResLoadState(RLS_CHECKVERSIONFAILED);
        return;
    }
    auto bigVersionStr = std::atoi(versionList.at(0).c_str());
    auto midVersionStr = std::atoi(versionList.at(1).c_str());
    auto versionStr = std::atoi(versionList.at(2).c_str());
    
    if ( bigVersionStr == 0 )
    {
        LoadResources();
        return;
    }
    
    if (bigVersionStr != m_localBigVersion) {
        CCLOG("大版本更新，请下载新版客户端");
        SetResLoadState(RLS_BIG_UPDATE);
        return;
    }
    
	m_NewestVersion = versionStr;
    m_NewestMidVersion = midVersionStr;
	m_UpdateInfo.newestVersion = m_NewestVersion;
    
    while (m_localMidVersion <= m_NewestMidVersion)
    {
        if( m_localMidVersion < m_NewestMidVersion)
        {
            while (m_LocalVersion < VERSION_COUNT) {
                UpdateVersionTo(m_localMidVersion, ++m_LocalVersion);
                
                if (m_ResLoadState == RLS_GETFILELISTFAILED || m_ResLoadState == RLS_GETFILELISTFAILED || m_ResLoadState == RLS_DOWNLOADFILEFAILED ||
                    m_ResLoadState == RLS_UNCOMPRESS_FAILED) {
                    return;
                }
                
                m_UpdateInfo.curVersion = m_LocalVersion;
            }
            m_LocalVersion = 0;
        }else
        {
            while (m_LocalVersion < m_NewestVersion) {
                UpdateVersionTo(m_localMidVersion, ++m_LocalVersion);
                
                if (m_ResLoadState == RLS_GETFILELISTFAILED || m_ResLoadState == RLS_GETFILELISTFAILED || m_ResLoadState == RLS_DOWNLOADFILEFAILED ||
                    m_ResLoadState == RLS_UNCOMPRESS_FAILED) {
                    return;
                }
                m_UpdateInfo.curVersion = m_LocalVersion;
            }
        }
        
        m_localMidVersion++;
    }
    LoadResources();
}

// 取更新文件列表
void CResManager::UpdateVersionTo(uint32_t midVersion, uint32_t version)
{
	SetResLoadState(RLS_GETFILELIST);
	char url[256] = { 0 };
	sprintf_safe(url, "%sversion_%u/minVersion_%u/update_%u/resInfo.xml", GlobalConfig::resourceUrl.c_str(), m_localBigVersion, midVersion, version);
	std::string data;
	if (!LoadFileData(url, data)) {
		SetResLoadState(RLS_GETFILELISTFAILED);
		return;
	}
    
	xml::XmlDocument doc;
	doc.loadFromMemory(data.c_str(), data.length());
    
	xml::NodeIterator iter;
	xml::XmlNode *root = doc.getFirstChild(iter);
	if (root == NULL) {
		SetResLoadState(RLS_GETFILELISTFAILED);
		return;
	}
    
	m_UpdateInfo.totalFileCount = root->getChildrenCount();
	xml::XmlNode *node = root->getFirstChild(iter);
	while (node != NULL) {
		xml::XmlAttribute *attr = node->findAttribute(T("Path"));
        xml::XmlAttribute *isRestart = node->findAttribute("IsRestart");
        
		if (attr != NULL) {
			UpdateOneFile(attr->getString());
		}
        
        if(!m_restartclient)
        {
            if (isRestart !=NULL && isRestart->getBool())
            {
                m_restartclient = true;
            }
        }
		node = root->getNextChild(iter);
	}
	m_UpdateInfo.curDownloadFile = "";
	m_UpdateInfo.curDownloadFileIndex = 0;
	m_UpdateInfo.curDownloadFileProgress = 0.0f;
}

// 更新文件
void CResManager::UpdateOneFile(const char *path)
{
	std::string temp = path;
	std::transform(temp.begin(), temp.end(), temp.begin(), __optPathChar);
	path = temp.c_str();
    
	m_UpdateInfo.curDownloadFile = path;
	m_UpdateInfo.curDownloadFileIndex++;
	m_UpdateInfo.curDownloadFileProgress = 0.0f;
	SetResLoadState(RLS_DOWNLOADFILE);
    
    char url[256] = { 0 };
    sprintf_safe(url, "%sversion_%u/minVersion_%u/update_%u/%s", GlobalConfig::resourceUrl.c_str(), m_localBigVersion, m_localMidVersion, m_LocalVersion, path);
	if (!DownLoadFile(url, path))
		SetResLoadState(RLS_DOWNLOADFILEFAILED);
}

void CResManager::LoadResources()
{
	SetResLoadState(RLS_LOADING);
    
    if(m_restartclient)
        SetResLoadState(RLS_RESTART_CLIENT);
    else
        SetResLoadState(RLS_COMPLETED);
}

bool CResManager::uncompress(const char* outFileName)
{
    SetResLoadState(RLS_UNCOMPRESS);
    // Open the zip file
    string fullFilepath = m_updatePath + outFileName;
    
    unzFile zipfile = unzOpen(fullFilepath.c_str());
    if (! zipfile)
    {
        CCLOG("can not open downloaded zip file %s", fullFilepath.c_str());
        return false;
    }
    
    // Get info about the zip file
    unz_global_info global_info;
    if (unzGetGlobalInfo(zipfile, &global_info) != UNZ_OK)
    {
        CCLOG("can not read file global info of %s", fullFilepath.c_str());
        unzClose(zipfile);
        return false;
    }
    
    // Buffer to hold data read from the zip file
    char readBuffer[BUFFER_SIZE];
    
    CCLOG("start uncompressing");
    
    // Loop to extract all files.
    uLong i;
    for (i = 0; i < global_info.number_entry; ++i)
    {
        // Get info about current file.
        unz_file_info fileInfo;
        char fileName[MAX_FILENAME];
        if (unzGetCurrentFileInfo(zipfile,
                                  &fileInfo,
                                  fileName,
                                  MAX_FILENAME,
                                  nullptr,
                                  0,
                                  nullptr,
                                  0) != UNZ_OK)
        {
            CCLOG("can not read file info");
            unzClose(zipfile);
            return false;
        }
        
        //debug
        std::string fileNameStr = fileName;
        if (fileNameStr == "src/app/ui/combatSystem/model/CombatCenter.lua") {
            printf("sf");
        }
        const std::string fullPath = m_updatePath + fileName;
        
        // Check if this entry is a directory or a file.
        const size_t filenameLength = strlen(fileName);
        if (fileName[filenameLength-1] == '/')
        {
            // Entry is a direcotry, so create it.
            // If the directory exists, it will failed scilently.
            if (!createDirectory(fullPath.c_str()))
            {
                CCLOG("can not create directory %s", fullPath.c_str());
                unzClose(zipfile);
                return false;
            }
        }
        else
        {
            //There are not directory entry in some case.
            //So we need to test whether the file directory exists when uncompressing file entry
            //, if does not exist then create directory
            const std::string fileNameStr(fileName);
            
            size_t startIndex=0;
            
            size_t index=fileNameStr.find("/",startIndex);
            
            while(index != std::string::npos)
            {
                const std::string dir = m_updatePath +fileNameStr.substr(0,index);
                
                FILE *out = fopen(FileUtils::getInstance()->getSuitableFOpen(dir).c_str(), "r");
                
                if(!out)
                {
                    if (!createDirectory(dir.c_str()))
                    {
                        CCLOG("can not create directory %s", dir.c_str());
                        unzClose(zipfile);
                        return false;
                    }
                    else
                    {
                        CCLOG("create directory %s",dir.c_str());
                    }
                }
                else
                {
                    fclose(out);
                }
                
                startIndex=index+1;
                
                index=fileNameStr.find("/",startIndex);
                
            }
            
            
            
            // Entry is a file, so extract it.
            
            // Open current file.
            if (unzOpenCurrentFile(zipfile) != UNZ_OK)
            {
                CCLOG("can not open file %s", fileName);
                unzClose(zipfile);
                return false;
            }
            
            // Create a file to store current file.
            FILE *out = fopen(FileUtils::getInstance()->getSuitableFOpen(fullPath).c_str(), "wb");
            if (! out)
            {
                CCLOG("can not open destination file %s", fullPath.c_str());
                unzCloseCurrentFile(zipfile);
                unzClose(zipfile);
                return false;
            }
            
            // Write current file content to destinate file.
            int error = UNZ_OK;
            do
            {
                error = unzReadCurrentFile(zipfile, readBuffer, BUFFER_SIZE);
                if (error < 0)
                {
                    CCLOG("can not read zip file %s, error code is %d", fileName, error);
                    unzCloseCurrentFile(zipfile);
                    unzClose(zipfile);
                    return false;
                }
                
                if (error > 0)
                {
                    fwrite(readBuffer, error, 1, out);
                }
            } while(error > 0);
            
            fclose(out);
        }
        
        unzCloseCurrentFile(zipfile);
        
        // Goto next entry listed in the zip file.
        if ((i+1) < global_info.number_entry)
        {
            if (unzGoToNextFile(zipfile) != UNZ_OK)
            {
                CCLOG("can not read next file");
                unzClose(zipfile);
                return false;
            }
        }
    }
    
    CCLOG("end uncompressing");
    unzClose(zipfile);
    
    return true;
}

/*
 * Create a direcotry is platform depended.
 */
bool CResManager::createDirectory(const char *path)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
    return FileUtils::getInstance()->createDirectory(_storagePath.c_str());
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    BOOL ret = CreateDirectoryA(path, nullptr);
    if (!ret && ERROR_ALREADY_EXISTS != GetLastError())
    {
        return false;
    }
    return true;
#else
    mode_t processMask = umask(0);
    int ret = mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO);
    umask(processMask);
    if (ret != 0 && (errno != EEXIST))
    {
        return false;
    }
    
    return true;
#endif
    
    
}

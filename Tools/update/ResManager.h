#ifndef __RES_MANAGER_H__
#define __RES_MANAGER_H__

#include <pthread.h>
#include "curl.h"
#include "cocos2d.h"

enum ResLoadState {
	RLS_NONE,

	RLS_CHECKVERSION,
	RLS_CHECKVERSIONFAILED,
	RLS_GETFILELIST,
	RLS_GETFILELISTFAILED,
	RLS_DOWNLOADFILE,
	RLS_DOWNLOADFILEFAILED,
	RLS_LOADING,
	RLS_COMPLETED,
    RLS_UNCOMPRESS,
    RLS_UNCOMPRESS_FAILED,
    RLS_BIG_UPDATE,
    RLS_RESTART_CLIENT,

	RLS_MAX,
};

struct ResourceUpdateInfo {
	uint32_t curVersion;
	uint32_t newestVersion;
	uint32_t totalFileCount;
	std::string curDownloadFile;
	uint32_t curDownloadFileIndex;
	float curDownloadFileProgress;
    float totalToDownload;
    float nowDownloaded;
	ResourceUpdateInfo() {
		curVersion = 0;
		newestVersion = 0;
		totalFileCount = 0;
		curDownloadFileIndex = 0;
		curDownloadFileProgress = 0.0f;
        totalToDownload = 0.0f;
        nowDownloaded = 0.0f;
	}
};

class CResManager {
	friend void *updateResources(void *);
	friend int downLoadFileProgress(void *, double, double, double, double);
public:
	CResManager();
	~CResManager();

	static CResManager *getInstance();
    static bool isEnableUpdate();
    bool init();
	ResLoadState GetResLoadState() const { return m_ResLoadState; }
	const ResourceUpdateInfo &GetResourceUpdateInfo() const { return m_UpdateInfo; }

	void UpdateAndLoadResources();
	bool LoadFileData(const char *url, std::string &str);
	bool DownLoadFile(const char *url, const char *file);
    
    std::string& getUpdateFilePath(){ return m_updatePath; }

private:

	void CheckVersion();
	void UpdateVersionTo(uint32_t midVersion, uint32_t version);
	void UpdateOneFile(const char *path);
	void LoadResources();
    void storeVersion();
    
    bool uncompress(const char* outFileName);
    bool createDirectory(const char *path);
    
    

private:
	//uint32_t &m_LocalVersion;
    uint32_t m_LocalVersion;
    uint32_t m_localMidVersion;
    uint32_t m_localBigVersion;
    
    bool m_restartclient;
    bool m_isUpdateSeachPath;
    
	uint32_t m_NewestVersion;
    uint32_t m_NewestMidVersion;
    uint32_t m_NewestBigVersion;
    
	std::string m_resourceUrl;

	void SetResLoadState(ResLoadState us);
	ResLoadState m_ResLoadState;

	pthread_t m_tid;
	ResourceUpdateInfo m_UpdateInfo;
    
    //path
    std::string m_updatePath;
};

#endif
#include "AppDelegate.h"
#include "CCLuaEngine.h"
#include "SimpleAudioEngine.h"
#include "cocos2d.h"
#include "lua_module_register.h"
#include "LuaCustom.h"
#include "Luatool.h"
#include "PomeloClient.h"

//test
#include "GlobalConfig.h"
#include "ResManager.h"



#if (CC_TARGET_PLATFORM != CC_PLATFORM_LINUX)
#include "ide-support/CodeIDESupport.h"
#endif

#if (COCOS2D_DEBUG > 0) && (CC_CODE_IDE_DEBUG_SUPPORT > 0)
#include "runtime/Runtime.h"
#include "ide-support/RuntimeLuaImpl.h"
#endif

using namespace CocosDenshion;

USING_NS_CC;
using namespace std;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
    SimpleAudioEngine::end();

#if (COCOS2D_DEBUG > 0) && (CC_CODE_IDE_DEBUG_SUPPORT > 0)
    // NOTE:Please don't remove this call if you want to debug with Cocos Code IDE
    RuntimeEngine::getInstance()->end();
#endif

}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages,
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching()
{
    
    // set default FPS
    Director::getInstance()->setAnimationInterval(1.0 / 60.0f);

    // register lua module
    auto engine = LuaEngine::getInstance();
    ScriptEngineManager::getInstance()->setScriptEngine(engine);
    lua_State* L = engine->getLuaStack()->getLuaState();
    lua_module_register(L);
    
    
    //register self lua module
    CLuaCustom::getInstance()->Init();
    
    FileUtils::getInstance()->addSearchPath("res/");
    FileUtils::getInstance()->addSearchPath("src/");

    engine->executeScriptFile("config.lua");
    
    register_all_packages();
    
    LuaStack* stack = engine->getLuaStack();
    stack->setXXTEAKeyAndSign("2dxLua", strlen("2dxLua"), "XXTEA", strlen("XXTEA"));
    
    //更新功能判断
    bool isUpdate = Luatool::getLuaGlobalBoolValue("G_IsUpdate");
    
    if (isUpdate && CResManager::getInstance()->isEnableUpdate()) {
        std::string searchRootPath = CResManager::getInstance()->getUpdateFilePath();
        std::string resPath1 = searchRootPath + "res/";
        std::string srcPath1 = searchRootPath + "src/";
        std::string resPath2 = searchRootPath + "data/res/";
        std::string srcPath2 = searchRootPath + "data/src/";
        FileUtils::getInstance()->addSearchPath(resPath1, true);
        FileUtils::getInstance()->addSearchPath(srcPath1, true);
//        FileUtils::getInstance()->addSearchPath(resPath2, true);
//        FileUtils::getInstance()->addSearchPath(srcPath2, true);
    }

#if (COCOS2D_DEBUG > 0) && (CC_CODE_IDE_DEBUG_SUPPORT > 0)
    // NOTE:Please don't remove this call if you want to debug with Cocos Code IDE
    auto runtimeEngine = RuntimeEngine::getInstance();
    runtimeEngine->addRuntime(RuntimeLuaImpl::create(), kRuntimeEngineLua);
    runtimeEngine->start();
#else
    if (engine->executeScriptFile("main.lua"))
    {
        return false;
    }
#endif
    
//
//    std::string url = "http://127.0.0.1:8080/updateServer/";
//    GlobalConfig::InitConfig(url);
//    CResManager::getInstance()->UpdateAndLoadResources();
    
    if (!PomeloClient::getInstance()->Connection("127.0.0.1", 3010))
    {
        printf("not connect to server!");
    }
    PomeloClient::getInstance()->request("connector.entryHandler.entry", "{\"name\": \"test\"}", [](const pc_request_t* req, int rc, const char* resp)
                                         {
                                             printf("msg: %s", resp);
                                         });
    PomeloClient::getInstance()->notify("test.testHandler.notify", "{\"content\": \"test content\"}");
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();

    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->startAnimation();

    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

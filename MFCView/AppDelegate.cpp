#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "MyEGLView.h"
USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(960, 640);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);
AppDelegate::AppDelegate() :m_hwnd(NULL)
{

}

AppDelegate::~AppDelegate() 
{ 
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	// Director should still do a cleanup if the window was closed manually.
	if (glview)
	{
		if (glview->isOpenGLReady())
		{
			director->end();
			director->mainLoop();
			director = nullptr;
		}
		glview->release();
	}
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


void AppDelegate::CreateWnd(HWND hwnd)
{
	if (!m_hwnd)
	{
		m_hwnd = hwnd;

		initGLContextAttrs();
		// Initialize instance and cocos2d.
		if (!applicationDidFinishLaunching())
		{
			return;
		}

		auto director = Director::getInstance();
		auto glview = director->getOpenGLView();
		if (glview)
		{
			// Retain glview to avoid glview being released in the while loop
			glview->retain();
		}
	}
}

LRESULT AppDelegate::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	if (CMyEGLView* view = dynamic_cast<CMyEGLView*>(glview)) {
		return view->WindowProc(message, wParam, lParam);
	}
	return 0;
}
 

void AppDelegate::Run2()
{
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	if (!glview) {
		return;
	}
	//
	director->mainLoop();
}
 
void AppDelegate::Resize(int width, int height)
{
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	if (glview)
	{ 
		glview->setFrameSize((float)width, (float)height); 
		// Set the design resolution
		glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::FIXED_WIDTH);
	}
}


bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
		cocos2d::CMyEGLView* pView = CMyEGLView::createWithRect("test", Rect(0, 0, designResolutionSize.width, designResolutionSize.height), m_hwnd);
		director->setOpenGLView(pView);
		glview = pView;
    }

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
	 
    register_all_packages();

    // create a scene. it's an autorelease object
    auto scene = HelloWorld::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

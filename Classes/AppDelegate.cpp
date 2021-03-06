#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "BaseLayer.h"
#include "SystemHeader.h"

USING_NS_CC;
using namespace CocosDenshion;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
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

bool AppDelegate::applicationDidFinishLaunching() 
{
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) 
	{
        glview = GLViewImpl::create("My Game");
		//glview->setFrameSize(320, 480);
		glview->setFrameSize(640, 960);
        director->setOpenGLView(glview);
    }

	auto designSize = Size(320, 480);
	auto resourceSize = Size(640, 960);

	vector<string> seachPaths;
	seachPaths.push_back("hd");

	FileUtils::getInstance()->setSearchPaths(seachPaths);

	director->setContentScaleFactor(resourceSize.width / designSize.width);
	glview->setDesignResolutionSize(designSize.width, designSize.height, ResolutionPolicy::FIXED_WIDTH);

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
	auto scene = HelloWorld::createScene();

    // run
    director->runWithScene(scene);

	SimpleAudioEngine::getInstance()->preloadEffect(sound_1);
	SimpleAudioEngine::getInstance()->preloadEffect(sound_2);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() 
{
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

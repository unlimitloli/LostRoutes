#include "HelloWorldScene.h"
#include "HomeMenu.h"
#include "SystemHeader.h"

USING_NS_CC;
using namespace std;
using namespace CocosDenshion;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto bg = TMXTiledMap::create("map/red_bg.tmx");
	addChild(bg);

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("texture/loading_texture.plist");

	auto logo = Sprite::createWithSpriteFrameName("logo.png");
	logo->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	addChild(logo);

	loadSprite = Sprite::createWithSpriteFrameName("loding4.png");
	loadSprite->setPosition(logo->getPosition() - Vec2(0, logo->getContentSize().height / 2 + 30));
	addChild(loadSprite);
	Loading();

	m_nNumberOfLoaded = 0;
	Director::getInstance()->getTextureCache()->addImageAsync("texture/home_texture.png", 
		CC_CALLBACK_1(HelloWorld::LoadingTextureCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("texture/setting_texture.png", 
		CC_CALLBACK_1(HelloWorld::LoadingTextureCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("texture/gameplay_texture.png", 
		CC_CALLBACK_1(HelloWorld::LoadingTextureCallBack, this));

	m_loadAudioThread = new thread(&HelloWorld::LoadingAudio, this);
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HelloWorld::onExit()
{
	Layer::onExit();
	m_loadAudioThread->join();
	CC_SAFE_DELETE(m_loadAudioThread);

	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("texture/loading_texture.plist");
	Director::getInstance()->getTextureCache()->removeTextureForKey("texture/loading_texture.png");
	unschedule(schedule_selector(HelloWorld::delayCall));
}

void HelloWorld::Loading()
{
	Animation *animation = Animation::create();
	for (int i=1; i <= 4; i++)
	{
		__String *frameName = __String::createWithFormat("loding%d.png", i);
		log("frameName = %s", frameName->getCString());
		SpriteFrame *spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName->getCString());
		animation->addSpriteFrame(spriteFrame);
	}

	animation->setDelayPerUnit(0.5f);
	animation->setRestoreOriginalFrame(true);

	Animate *action = Animate::create(animation);
	loadSprite->runAction(RepeatForever::create(action));
}

void HelloWorld::LoadingTextureCallBack(Texture2D *texture)
{
	switch (m_nNumberOfLoaded++)
	{
	case 0:
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("texture/home_texture.plist", texture);
		log("home texture ok");
		break;
	case 1:
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("texture/setting_texture.plist", texture);
		log("setting texture ok");
		break;
	case 2:
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("texture/gameplay_texture.plist", texture);
		log("gameplay texture ok");
		this->schedule(schedule_selector(HelloWorld::delayCall), 1, 1, 3);
		break;
	}
}

void HelloWorld::delayCall(float dt)
{
	auto sc = HomeMenu::createScene();
	Director::getInstance()->replaceScene(sc);
}

void HelloWorld::LoadingAudio()
{
	log("Loading Audio");
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic(bg_music_1);
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic(bg_music_2);
	//SimpleAudioEngine::getInstance()->preloadEffect(sound_1);
	//SimpleAudioEngine::getInstance()->preloadEffect(sound_2);
}
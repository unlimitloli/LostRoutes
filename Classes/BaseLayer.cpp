#include "BaseLayer.h"
#include "MyUtility.h"
#include "SystemHeader.h"

USING_NS_CC;
using namespace CocosDenshion;

Scene* BaseLayer::createScene()
{
	auto scene = Scene::create();
	auto layer = BaseLayer::create();

	scene->addChild(layer);
	return scene;
}

bool BaseLayer::init()
{
	if ( !Layer::init() )
    {
        return false;
    }

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();


	auto bg = TMXTiledMap::create("map/red_bg.tmx");
	addChild(bg);

	auto glassesSprite = Sprite::createWithSpriteFrameName("setting.glasses.png");
	glassesSprite->setPosition(Vec2(visibleSize.width - glassesSprite->getContentSize().width / 2, 160));
	addChild(glassesSprite);

	auto handSprite = Sprite::createWithSpriteFrameName("setting.hand.png");
	handSprite->setPosition(Vec2(handSprite->getContentSize().width / 2, 60));
	addChild(handSprite);

	auto okNormal = Sprite::createWithSpriteFrameName("setting.button.ok.png");
	auto okSelected = Sprite::createWithSpriteFrameName("setting.button.ok-on.png");
	auto okMenuItem = MenuItemSprite::create(okNormal, okSelected, CC_CALLBACK_1(BaseLayer::menuBackCallback, this));
	auto mu = Menu::create(okMenuItem, NULL);
	mu->setPosition(Vec2(visibleSize.width - mu->getContentSize().width / 2 + 60, 60));
	addChild(mu);

	return true;
}

void BaseLayer::menuBackCallback(Ref *pSender)
{
	Director::getInstance()->popScene();
	if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY))
	{
		SimpleAudioEngine::getInstance()->playEffect(sound_1);
	}
}

void BaseLayer::onExit()
{
	Layer::onExit();
}

void BaseLayer::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
	log("BaseLayer OnEnterTransitionDidFinish");

	if (UserDefault::getInstance()->getBoolForKey(MUSIC_KEY))
	{
		SimpleAudioEngine::getInstance()->playBackgroundMusic(bg_music_1, true);
	}
}
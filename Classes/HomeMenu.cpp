#include "HomeMenu.h"
#include "SystemHeader.h"
#include "GameLayer.h"
#include "SettingLayer.h"
#include "HelpLayer.h"

USING_NS_CC;
using namespace CocosDenshion;

Scene *HomeMenu::createScene()
{
	auto scene = Scene::create();
	auto layer = HomeMenu::create();
	scene->addChild(layer);
	return scene;
}

bool HomeMenu::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();

	auto bg = TMXTiledMap::create("map/red_bg.tmx");
	addChild(bg);


	auto top = Sprite::createWithSpriteFrameName("home-top.png");
	top->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - top->getContentSize().height / 2));
	addChild(top);

	auto startNormal = Sprite::createWithSpriteFrameName("home.button.start.png");
	auto startSelected = Sprite::createWithSpriteFrameName("home.button.start-on.png");
	auto startMenuItem = MenuItemSprite::create(startNormal, startSelected,
		CC_CALLBACK_1(HomeMenu::menuItemCallBack, this));
	startMenuItem->setTag(ActionType::MenuItemStart);

	auto settingNormal = Sprite::createWithSpriteFrameName("home.button.setting.png");
	auto settingSelected = Sprite::createWithSpriteFrameName("home.button.setting-on.png");
	auto settingMenuItem  = MenuItemSprite::create(settingNormal, settingSelected, 
		CC_CALLBACK_1(HomeMenu::menuItemCallBack, this));
	settingMenuItem->setTag(ActionType::MenuItemSetting);

	auto helpNormal = Sprite::createWithSpriteFrameName("home.button.help.png");
	auto helpSelected = Sprite::createWithSpriteFrameName("home.button.help-on.png");
	auto helpMenuItem  = MenuItemSprite::create(helpNormal, helpSelected, 
		CC_CALLBACK_1(HomeMenu::menuItemCallBack, this));
	helpMenuItem->setTag(ActionType::MenuItemHelp);

	auto mu = Menu::create(startMenuItem, settingMenuItem, helpMenuItem, NULL);
	mu->alignItemsVerticallyWithPadding(12);
	addChild(mu);


	return true;
}

void HomeMenu::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
	log("HomeMenu OnEnterTransitionDidFinish");

	if (UserDefault::getInstance()->getBoolForKey(MUSIC_KEY))
	{
		SimpleAudioEngine::getInstance()->playBackgroundMusic(bg_music_1, true);
	}
}

void HomeMenu::menuItemCallBack(cocos2d::Ref *pSender)
{
	if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY))
	{
		SimpleAudioEngine::getInstance()->playEffect(sound_1);
	}

	Scene *sc = nullptr;

	MenuItem *menuItem = (MenuItem *)pSender;
	log ("MenuItem tag = %d ", menuItem->getTag());
	switch (menuItem->getTag())
	{
	case ActionType::MenuItemStart:
		sc = TransitionFade::create(1.0f, GameLayer::createScene());
		log("Statr Layer");
		break;
	case ActionType::MenuItemSetting:
		sc = TransitionCrossFade::create(1.0f, SettingLayer::createScene());
		log("Setting Layer");
		break;
	case ActionType::MenuItemHelp:
		sc = TransitionFadeDown::create(1.0f, HelpLayer::createScene());
		log("Help Layer");
		break;
	default:
		break;
	}

	if (sc)
		Director::getInstance()->pushScene(sc);
}
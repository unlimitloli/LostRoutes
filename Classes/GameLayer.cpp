#include "GameLayer.h"
#include "Enemy.h"
#include "SystemHeader.h"
#include "Fighter.h"

USING_NS_CC;
using namespace CocosDenshion;

#define TagBackground	300
#define TagEnemy		400
#define TagFigter		500
#define TagBullet		600
#define TagMenu			999

Scene *GameLayer::createScene()
{
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	auto layer = GameLayer::create();
	scene->addChild(layer);
	return scene;
}

bool GameLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	initBG();

	//auto bg = TMXTiledMap::create("map/red_bg.tmx");
	//addChild(bg);

	//auto enemy1 = Enemy::createWithEnemyTypes(EnemyTypes::EnemyTypeEnemy1);
	//enemy1->setVelocity(Vec2(0, -60));
	////enemy1->setPosition(100, 200);
	//addChild(enemy1);

	//auto enemy2 = Enemy::createWithEnemyTypes(EnemyTypes::EnemyTypeEnemy2);
	//enemy2->setVelocity(Vec2(0, -30));
	////enemy2->setPosition(200, 200);
	//addChild(enemy2);

	//auto fighter = Fighter::createWithSpriteFrameName("gameplay.fighter.png");
	//fighter->setPosition(Vec2(0, 100));
	//addChild(fighter);

	return true;
}

void GameLayer::initBG()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	auto bg = TMXTiledMap::create("map/blue_bg.tmx");
	addChild(bg, 0, TagBackground);

	ParticleSystem *ps = ParticleSystemQuad::create("particle/light.plist");
	ps->setPosition(Vec2(visibleSize.width, visibleSize.height) / 2);
	addChild(ps, 0, TagBackground);

	auto sprite1 = Sprite::createWithSpriteFrameName("gameplay.bg.sprite-1.png");
	sprite1->setPosition(Vec2(-50, -50));
	addChild(sprite1, 0, TagBackground);

	FiniteTimeAction *ac1 = (FiniteTimeAction *)MoveBy::create(20, Vec2(500, 600));
	FiniteTimeAction *ac2 = ((FiniteTimeAction *)ac1)->reverse();
	ActionInterval *as1 = Sequence::create(ac1, ac2, NULL);
	sprite1->runAction(RepeatForever::create(EaseSineInOut::create(as1)));

	auto sprite2 = Sprite::createWithSpriteFrameName("gameplay.bg.sprite-2.png");
	sprite2->setPosition(Vec2(visibleSize.width, 0));
	addChild(sprite2, 0, TagBackground);

	FiniteTimeAction *ac3 = (FiniteTimeAction *)MoveBy::create(10, Vec2(-500, 600));
	FiniteTimeAction *ac4 = ((FiniteTimeAction *)ac3)->reverse();
	ActionInterval *as2 = Sequence::create(ac3, ac4, NULL);
	sprite2->runAction(RepeatForever::create(EaseSineInOut::create(as2)));
}

void GameLayer::onEnter()
{
	Layer::onEnter();

	Size visibleSize = Director::getInstance()->getVisibleSize();

	auto pauseSprite = Sprite::createWithSpriteFrameName("gameplay.button.pause.png");
	auto pauseMenuItem = MenuItemSprite::create(pauseSprite, pauseSprite,
		CC_CALLBACK_1(GameLayer::menuPauseCallBack, this));
	auto pauseMenu = Menu::create(pauseMenuItem, NULL);
	pauseMenu->setPosition(Vec2(30, visibleSize.height - 28));
	addChild(pauseMenu, 20, TagMenu);

	auto stone1 = Enemy::createWithEnemyTypes(EnemyTypeStone);
	stone1->setVelocity(Vec2(0, -100));
	addChild(stone1, 10, TagEnemy);

	auto planet = Enemy::createWithEnemyTypes(EnemyTypePlanet);
	planet->setVelocity(Vec2(0, -50));
	addChild(planet, 10, TagEnemy);

	auto enemy1 = Enemy::createWithEnemyTypes(EnemyTypeEnemy1);
	enemy1->setVelocity(Vec2(0, -80));
	addChild(enemy1, 10, TagEnemy);

	auto enemy2 = Enemy::createWithEnemyTypes(EnemyTypeEnemy2);
	enemy2->setVelocity(Vec2(0, -100));
	addChild(enemy2, 10, TagEnemy);

	fighter = Fighter::createWithSpriteFrameName("gameplay.fighter.png");
	fighter->setHitPoints(5);
	fighter->setPosition(Vec2(visibleSize.width / 2, 70));
	addChild(fighter, 10, TagFigter);

	touchFighter = EventListenerTouchOneByOne::create();
	touchFighter->setSwallowTouches(true);

	touchFighter->onTouchBegan = [](Touch *touch, Event *event)
	{
		return true;
	};

	touchFighter->onTouchMoved = [](Touch *touch, Event *event)
	{
		auto target = event->getCurrentTarget();
		target->setPosition(target->getPosition() + touch->getDelta());
	};

	EventDispatcher *eventDispatcher = Director::getInstance()->getEventDispatcher();
	eventDispatcher->addEventListenerWithSceneGraphPriority(touchFighter, fighter);

}

void GameLayer::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
	log("GameLayer onEntertransitionDidFinish");
	if (UserDefault::getInstance()->getBoolForKey(MUSIC_KEY))
	{
		SimpleAudioEngine::getInstance()->playBackgroundMusic(bg_music_2, true);
	}
}

void GameLayer::onExit()
{
	Layer::onExit();

	//unschedule();

	auto nodes = getChildren();
	for (const auto &node : nodes)
	{
		if (node->getTag() != TagBackground)
			removeChild(node);
	}
}

void GameLayer::menuPauseCallBack(Ref *pSender)
{
	log("menu Pause Callback");
	if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY))
	{
		SimpleAudioEngine::getInstance()->playEffect(sound_1);
	}

	pause();

	for (const auto &node : getChildren())
		node->pause();

	Size visibleSize = Director::getInstance()->getVisibleSize();

	auto backNormal = Sprite::createWithSpriteFrameName("gameplay.button.back.png");
	auto backSelected = Sprite::createWithSpriteFrameName("gameplay.button.back-on.png");
	auto backMenuItem = MenuItemSprite::create(backNormal, backSelected, 
		CC_CALLBACK_1(GameLayer::menuBackCallBack, this));

	auto resumeNormal = Sprite::createWithSpriteFrameName("gameplay.button.resume.png");
	auto resumeSeleted = Sprite::createWithSpriteFrameName("gameplay.button.resume-on.png");
	auto resumeMenuItem = MenuItemSprite::create(resumeNormal, resumeSeleted, 
		CC_CALLBACK_1(GameLayer::menuResumeCallBack, this));

	menu = Menu::create(backMenuItem, resumeMenuItem, NULL);
	menu->alignItemsVertically();
	menu->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

	addChild(menu, 2, TagMenu);
}

void GameLayer::menuBackCallBack(Ref *pSender)
{
	log("menuBackCallBack");
	if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY))
	{
		SimpleAudioEngine::getInstance()->playEffect(sound_1);
	}

	Director::getInstance()->popScene();
}

void GameLayer::menuResumeCallBack(Ref *pSender)
{
	log("menuResumeCallBack");
	if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY))
	{
		SimpleAudioEngine::getInstance()->playEffect(sound_1);
	}

	resume();

	for (const auto &node : getChildren())
		node->resume();

	removeChild(menu);
}
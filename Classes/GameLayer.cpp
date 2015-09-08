#include "GameLayer.h"
#include "Enemy.h"
#include "SystemHeader.h"
#include "Fighter.h"
#include "Bullet.h"

USING_NS_CC;
using namespace CocosDenshion;

#define TagBackground	300
#define TagEnemy		400
#define TagFigter		500
#define TagBullet		600
#define TagExplosion    700
#define TagStatusBar	800		
#define TagStatusLife	900	
#define TagStatusScore	1000

#define TagMenu			999
#define BulletVeloctity 300

Scene *GameLayer::createScene()
{
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
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

	score = 0;
	scorePlaceholder = 0;

	initBG();

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

	updateStatusBarFighter();
	updateStatusBarScore();

	touchFighter = EventListenerTouchOneByOne::create();
	touchFighter->setSwallowTouches(true);

	touchFighter->onTouchBegan = [](Touch *touch, Event *event)
	{
		auto target = event->getCurrentTarget();
		Vec2 localPt = target->convertToNodeSpace(touch->getLocation());
		Size s = target->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);

		if (rect.containsPoint(localPt))
			return true;

		return false;
	};

	touchFighter->onTouchMoved = [](Touch *touch, Event *event)
	{
		auto target = event->getCurrentTarget();
		target->setPosition(target->getPosition() + touch->getDelta());
	};

	EventDispatcher *eventDispatcher = Director::getInstance()->getEventDispatcher();
	eventDispatcher->addEventListenerWithSceneGraphPriority(touchFighter, fighter);

	schedule(schedule_selector(GameLayer::shootButtle), 0.2f);	//发射子弹

	//物理碰撞检测
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = [this](PhysicsContact &contact)
	{
		auto spriteA = contact.getShapeA()->getBody()->getNode();
		auto spriteB = contact.getShapeB()->getBody()->getNode();

		Node *enemy1 = nullptr;

		if ((spriteA->getTag() == TagFigter) && (spriteB->getTag() == TagEnemy))
		{
			enemy1 = spriteB;
		}
		if ((spriteA->getTag() == TagEnemy) && (spriteB->getTag() == TagFigter))
		{
			enemy1 = spriteA;
		}
		if (enemy1 != nullptr)
		{
			fighterHitEnemy((Enemy *)enemy1);
			return false;
		}

		Node *enemy2 = nullptr;

		if ((spriteA->getTag() == TagBullet) && (spriteB->getTag() == TagEnemy))
		{
			if (!spriteA->isVisible())
				return false;
			spriteA->setVisible(false);
			enemy2 = spriteB;
		}
		if ((spriteA->getTag() == TagEnemy) && (spriteB->getTag() == TagBullet))
		{
			if (!spriteB->isVisible())
				return false;
			spriteB->setVisible(false);
			enemy2 = spriteA;
		}
		if (enemy2 != nullptr)
		{
			bulleHitEnemy((Enemy *)enemy2);
			return false;
		}

		return false;
	};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(contactListener, 1);

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

void GameLayer::shootButtle(float dt)
{
	if (fighter && fighter->isVisible())
	{
		Bullet *bullet = Bullet::createWithSpriteFrameName("gameplay.bullet.png");
		bullet->setVelocity(Vec2(0, BulletVeloctity));
		addChild(bullet, 0, TagBullet);
		bullet->shootBulletFromFighter(fighter);
	}
}

void GameLayer::bulleHitEnemy(Enemy *enemy)
{
	enemy->setHitPoints(enemy->getHitPoints() - 1);

	if (enemy->getHitPoints() <= 0)
	{
		Node *node = getChildByTag(TagExplosion);
		if (node)
			removeChild(node);

		ParticleSystem *explosion = ParticleSystemQuad::create("particle/explosion.plist");
		explosion->setPosition(enemy->getPosition());
		addChild(explosion, 2, TagExplosion);
		if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY))
			SimpleAudioEngine::getInstance()->playEffect(sound_2);

		switch (enemy->getEnemyType())
		{
		case EnemyTypeStone:
			score += EnemyStone_Score;
			scorePlaceholder += EnemyStone_Score;
			break;
		case EnemyTypeEnemy1:
			score += Enemy1_Score;
			scorePlaceholder += Enemy1_Score;
			break;
		case EnemyTypeEnemy2:
			score += Enemy2_Score;
			scorePlaceholder += Enemy2_Score;
			break;
		case EnemyTypePlanet:
			score += EnemyPlanet_Score;
			scorePlaceholder += EnemyPlanet_Score;
			break;
		}

		if (scorePlaceholder >= 1000)
		{
			fighter->setHitPoints(fighter->getHitPoints() + 1);
			updateStatusBarFighter();
			scorePlaceholder -= 1000;
		}

		updateStatusBarScore();
		enemy->setVisible(false);
		enemy->spawn();
	}
}

void GameLayer::fighterHitEnemy(Enemy *enemy)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	Node *node = getChildByTag(TagExplosion);
	if (node)
		removeChild(node);

	ParticleSystem *explosion = ParticleSystemQuad::create("particle/explosion.plist");
	explosion->setPosition(fighter->getPosition());
	addChild(explosion, 2, TagExplosion);
	if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY))
		SimpleAudioEngine::getInstance()->playEffect(sound_2);

	enemy->setVisible(false);
	enemy->spawn();

	fighter->setHitPoints(fighter->getHitPoints() - 1);
	updateStatusBarFighter();
	if (fighter->getHitPoints() <= 0)
	{
		log("GameOver");
	}
	else
	{
		fighter->setPosition(Vec2(visibleSize.width / 2, 70));
		auto ac1 = Show::create();
		auto ac2 = FadeIn::create(1.0f);
		auto seq = Sequence::create(ac1, ac2, NULL);
		fighter->runAction(seq);
	}
}

void GameLayer::updateStatusBarFighter()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	Node *n1 = getChildByTag(TagStatusBar);
	if (n1)
		removeChild(n1);

	Sprite *fg = Sprite::createWithSpriteFrameName("gameplay.life.png");
	fg->setPosition(Vec2(visibleSize.width - 60, visibleSize.height - 28));
	addChild(fg, 20, TagStatusBar);

	Node *n2 = getChildByTag(TagStatusLife);
	if (n2)
		removeChild(n2);

	if (fighter->getHitPoints() <= 0)
		fighter->setHitPoints(0);

	__String *life = __String::createWithFormat("x %d ", fighter->getHitPoints());
	auto lbLife = Label::createWithTTF(life->getCString(), "fonts/hanyi.ttf", 18);
	lbLife->setPosition(fg->getPosition() + Vec2(30, 0));
	addChild(lbLife, 20, TagStatusLife);
}

void GameLayer::updateStatusBarScore()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	Node *n = getChildByTag(TagStatusScore);
	if (n)
		removeChild(n);

	if (score <= 0)
		score = 0;

	__String *statusScore = __String::createWithFormat("%d ", score);
	auto lbScore = Label::createWithTTF(statusScore->getCString(), "fonts/hanyi.ttf", 18);
	lbScore->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 28));
	addChild(lbScore, 20, TagStatusScore);
}
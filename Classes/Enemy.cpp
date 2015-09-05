#include "Enemy.h"

USING_NS_CC;

Enemy::Enemy(EnemyTypes enemyType)
{
	this->enemyType = enemyType;
	this->initialHitPoints = 1;
	this->velocity = Vec2::ZERO;
}

Enemy *Enemy::createWithEnemyTypes(EnemyTypes enemyType)
{
	Enemy *enemy = new Enemy(enemyType);

	const char *enemyFrameName = Enemy_Stont;

	switch (enemyType)
	{
	case EnemyTypeStone:
		enemyFrameName = Enemy_Stont;
		enemy->initialHitPoints = 3;
		break;
	case EnemyTypeEnemy1:
		enemyFrameName = Enemy_1;
		enemy->initialHitPoints = 5;
		break;
	case EnemyTypeEnemy2:
		enemyFrameName = Enemy_2;
		enemy->initialHitPoints = 10;
		break;
	case EnemyTypePlanet:
		enemyFrameName = Enemy_Planet;
		enemy->initialHitPoints = 15;
		break;
	default:
		break;
	}

	if (enemy && enemy->initWithSpriteFrameName(enemyFrameName))
	{
		enemy->autorelease();

		auto body = PhysicsBody::create();

		if (enemyType == EnemyTypeStone || enemyType == EnemyTypePlanet)
		{
			body->addShape(PhysicsShapeCircle::create(enemy->getContentSize().width / 2 - 5));
		}
		else if (enemyType == EnemyTypeEnemy1)
		{
			Vec2 verts[] = {
				Vec2(-2.5, -45.75),
				Vec2(-29.5, -27.5),
				Vec2(-53, -0.25),
				Vec2(-34, 43.25),
				Vec2(28, 44.25),
				Vec2(55, -2.25)};
			body->addShape(PhysicsShapePolygon::create(verts, 6));
		}
		else if (enemyType == EnemyTypeEnemy2)
		{
			Vec2 verts[] = {
				Vec2(1.25, 32.25),
				Vec2(36.75, -4.75),
				Vec2(2.75, -31.75),
				Vec2(-35.75, -3.25)};
			body->addShape(PhysicsShapePolygon::create(verts, 4));
		}

		body->setCategoryBitmask(0x01);
		body->setCollisionBitmask(0x02);
		body->setContactTestBitmask(0x01);

		enemy->setPhysicsBody(body);
		enemy->setVisible(false);
		enemy->spawn();
		enemy->unscheduleUpdate();
		enemy->scheduleUpdate();

		return enemy;
	}

	CC_SAFE_DELETE(enemy);

	return nullptr;
}

void Enemy::update(float dt)
{
	switch (enemyType)
	{
	case EnemyTypes::EnemyTypeStone:
		this->setRotation(this->getRotation() - 0.5);
		break;
	case EnemyTypes::EnemyTypePlanet:
		this->setRotation(this->getRotation() + 1.0);
		break;
	}

	Vec2 movelen = velocity * dt;

	setPosition(getPosition() + movelen);
	if (getPosition().y + getContentSize().height / 2 < 0)
	{
		spawn();
	}
}

void Enemy::spawn()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	float yPos = visibleSize.height + getContentSize().height / 2;
	float xPos = CCRANDOM_0_1() * (visibleSize.width - getContentSize().width);

	setPosition(Vec2(xPos, yPos));
	setAnchorPoint(Vec2(0.5f, 0.5f));

	hitPoints = initialHitPoints;
	setVisible(true);
}
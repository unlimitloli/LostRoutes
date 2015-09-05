#include "Bullet.h"
#include "SystemHeader.h"

USING_NS_CC;

Bullet *Bullet::createWithSpriteFrmaeName(const char *spriteFrameName)
{
	Bullet *bullet = new Bullet();

	if (bullet && bullet->initWithSpriteFrameName(spriteFrameName))
	{
		bullet->autorelease();
		bullet->setVisible(false);

		auto body = PhysicsBody::create();

		body->setCategoryBitmask(0x01);
		body->setCollisionBitmask(0x02);
		body->setContactTestBitmask(0x01);

		bullet->setPhysicsBody(body);

		return bullet;
	}

	CC_SAFE_DELETE(bullet);
	return nullptr;
}

void Bullet::shootBulletFromFighter(Fighter *fighter)
{
	setPosition(fighter->getPosition() + Vec2(0, fighter->getContentSize().height / 2));
	setVisible(true);
	unscheduleUpdate();
	scheduleUpdate();
}

void Bullet::update(float dt)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	setPosition(Vec2(getPosition() + velocity * dt));
	if (getPosition().y > visibleSize.height)
	{
		setVisible(false);
		unscheduleUpdate();
		removeFromParent();
	}
}
#include "Fighter.h"
#include "SystemHeader.h"

USING_NS_CC;

Fighter *Fighter::createWithSpriteFrameName(const char *spriteFrmaeName)
{
	Fighter *fighter = new Fighter();

	if (fighter && fighter->initWithSpriteFrameName(spriteFrmaeName))
	{
		fighter->autorelease();

		ParticleSystem *ps = ParticleSystemQuad::create("particle/fire.plist");

		ps->setPosition(Vec2(fighter->getContentSize().width / 2, 0));
		ps->setRotation(180.0f);
		ps->setScale(0.5f);
		fighter->addChild(ps);

		Vec2 verts[] = {
			Vec2(-43.5, 15.5),
			Vec2(-23.5, 33),
			Vec2(28.5, 34),
			Vec2(48, 17.5),
			Vec2(0, -39.5)};

		auto body = PhysicsBody::createPolygon(verts, 5);

		body->setCategoryBitmask(0x01);
		body->setCollisionBitmask(0x02);
		body->setContactTestBitmask(0x01);

		fighter->setPhysicsBody(body);

		return fighter;
	}


	CC_SAFE_DELETE(fighter);
	return nullptr;
}

void Fighter::setPosition(const Vec2 &newPosistion)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	float halfWidth = getContentSize().width / 2;
	float halfHeight = getContentSize().height / 2;
	float pos_x = newPosistion.x;
	float pos_y = newPosistion.y;

	if (pos_x < halfWidth)
		pos_x = halfWidth;
	else if (pos_x > (visibleSize.width - halfWidth))
		pos_x = visibleSize.width - halfWidth;

	if (pos_y < halfHeight)
		pos_y = halfHeight;
	else if (pos_y > (visibleSize.height - halfHeight))
		pos_y = visibleSize.height - halfHeight;

	Sprite::setPosition(Vec2(pos_x, pos_y));
	Sprite::setAnchorPoint(Vec2(0.5f, 0.5f));
}
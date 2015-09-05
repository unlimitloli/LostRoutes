#ifndef __BULLET_H__
#define __BULLET_H__

#include "cocos2d.h"
#include "Fighter.h"

class Bullet : public cocos2d::Sprite
{
	CC_SYNTHESIZE(cocos2d::Vec2, velocity, Velocity);

public:
	void shootBulletFromFighter(Fighter *fighter);

	virtual void update(float dt);
	static Bullet *createWithSpriteFrmaeName(const char *spriteFrameName);
};

#endif